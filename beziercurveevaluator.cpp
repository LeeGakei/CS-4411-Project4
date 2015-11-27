#include "BezierCurveEvaluator.h"
#include <assert.h>
#include "vec.h"

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();

	std::cout << "BezierCurveEvaluator::evaluateCurve\n";

	//ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	ptvEvaluatedCurvePts.clear();

	if (iCtrlPtCount >= 4){
		for (int i = 0; iCtrlPtCount - (i+1) >=3; i += 3){
			// de Castaljau’s algorithm
			displayBezier(ptvCtrlPts, ptvEvaluatedCurvePts, i);
		}
	}
}


float v0Tov1dist, v1Tov2dist, v2Tov3dist, v0Tov3dist, heightDiff, standardValue;
void BezierCurveEvaluator::displayBezier(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sIndex) const{
	//check for flatness
	//by distance
	 v0Tov1dist = findDistance(ptvCtrlPts[sIndex], ptvCtrlPts[sIndex + 1]);
	 v1Tov2dist = findDistance(ptvCtrlPts[sIndex + 1], ptvCtrlPts[sIndex + 2]);
	 v2Tov3dist = findDistance(ptvCtrlPts[sIndex + 2], ptvCtrlPts[sIndex + 3]);
	 v0Tov3dist = findDistance(ptvCtrlPts[sIndex], ptvCtrlPts[sIndex + 3]);

	 if (v0Tov3dist == 0){
		 return;
	 }

	 float heightDiff = (abs(v0Tov1dist) + abs(v1Tov2dist) + abs(v2Tov3dist)) / abs(v0Tov3dist);

	 

	 std::cout << heightDiff << '\n';

	//std::cout << "heightDiff" << heightDiff << '\n';
	
	//by height diff
	/*float heightDiff = abs(ptvCtrlPts[sIndex].y - ptvCtrlPts[sIndex + 1].y) +
		abs(ptvCtrlPts[sIndex + 1].y - ptvCtrlPts[sIndex + 2].y) +
		abs(ptvCtrlPts[sIndex + 2].y - ptvCtrlPts[sIndex + 3].y) /
		abs(ptvCtrlPts[sIndex].y - ptvCtrlPts[sIndex + 3].y);*/

	////by width diff
	//float widthDiff = abs(ptvCtrlPts[sIndex].x - ptvCtrlPts[sIndex + 1].x) +
	//	abs(ptvCtrlPts[sIndex + 1].x - ptvCtrlPts[sIndex + 2].x) +
	//	abs(ptvCtrlPts[sIndex + 2].x - ptvCtrlPts[sIndex + 3].x) /
	//	abs(ptvCtrlPts[sIndex].x - ptvCtrlPts[sIndex + 3].x);
	//std::cout << heightDiff << '\n';
	//std::cout << "standardValue" <<standardValue << '\n';
	if (heightDiff <= (1 + s_fFlatnessEpsilon)){
		//std::cout << v0Tov1dist << ' ' << v1Tov2dist << ' ' << v2Tov3dist << ' ' << v0Tov3dist << '\n';
		float x, y;
		x = (ptvCtrlPts[sIndex + 1].x + ptvCtrlPts[sIndex + 2].x) / 2;
		y = (ptvCtrlPts[sIndex + 1].y + ptvCtrlPts[sIndex + 2].y) / 2;
		ptvEvaluatedCurvePts.push_back(Point(x,y));
	}
	//do something smart
	else{
		float u = 0.5;	//leave for further declaration of u(ratio)
		std::vector<Point> subPointsVector = subdivide(ptvCtrlPts, ptvEvaluatedCurvePts, sIndex, u);
		//left side
		displayBezier(subPointsVector, ptvEvaluatedCurvePts, 0);
		//right side
		displayBezier(subPointsVector, ptvEvaluatedCurvePts, 3);
	}

}

std::vector<Point> subdivide(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sIndex, float u){
	Point v0 = ptvCtrlPts[sIndex];
	Point v3 = ptvCtrlPts[sIndex+3];
	Point v0prime = calculateIntermidate(ptvCtrlPts[sIndex], ptvCtrlPts[sIndex+1], u);
	Point v1prime = calculateIntermidate(ptvCtrlPts[sIndex + 1], ptvCtrlPts[sIndex + 2], u);
	Point v2prime = calculateIntermidate(ptvCtrlPts[sIndex + 2], ptvCtrlPts[sIndex + 3], u);
	Point v0doublePrime = calculateIntermidate(v0prime, v1prime, u);
	Point v1doublePrime = calculateIntermidate(v1prime, v2prime, u);
	Point q_u = calculateIntermidate(v0doublePrime, v1doublePrime, u);
	ptvEvaluatedCurvePts.push_back(q_u);

	std::vector<Point> subPointsVector;
	subPointsVector.push_back(v0);
	subPointsVector.push_back(v0prime);
	subPointsVector.push_back(v0doublePrime);
	subPointsVector.push_back(q_u);
	subPointsVector.push_back(v1doublePrime);
	subPointsVector.push_back(v2prime);
	subPointsVector.push_back(v3);
	return subPointsVector;
}

Point calculateIntermidate(Point p0, Point p1, float u){
	float x, y;
	x = p0.x*u + p1.x*(1-u);
	y = p0.y*u + p1.y*(1-u);
	return Point(x, y);
}

float findDistance(Point p0, Point p1){
	//std::cout << p0.x << ' ' << p0.y << ' ' << p1.x << ' ' << p1.y << '\n';
	return sqrt((p0.x - p1.x)*(p0.x - p1.x) + (p0.y - p1.y)*(p0.y - p1.y));
}