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

	
	ptvEvaluatedCurvePts.clear();

	if (!bWrap) {
		std::vector<Point> tempPts;
		tempPts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
		Point fPoint = tempPts.front();
		Point ePoint = tempPts.back();
		ptvEvaluatedCurvePts.push_back(fPoint);
		ptvEvaluatedCurvePts.push_back(Point(0, fPoint.y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ePoint.y));
		ptvEvaluatedCurvePts.push_back(ePoint);
		//not wrapping, liear at the begin and end pts
		if (iCtrlPtCount >= 4){
			for (int i = 0; iCtrlPtCount - (i + 1) >= 3; i += 3){
				// de Castaljau’s algorithm
				displayBezier(ptvCtrlPts, ptvEvaluatedCurvePts, i);
			}
		}
	}
	else{
		//3k ctrl pts
		if (iCtrlPtCount%3==0){
			//make wrapping vector
			std::vector<Point> tempPts;
			tempPts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
			Point fPoint = tempPts.front();
			tempPts.push_back(Point(fPoint.x + fAniLength, fPoint.y));

			iCtrlPtCount = tempPts.size();

			if (iCtrlPtCount >= 4){
				for (int i = 0; iCtrlPtCount - (i + 1) >= 3; i += 3){
					if (i == 3)
						;
					// de Castaljau’s algorithm
					displayBezier(tempPts, ptvEvaluatedCurvePts, i, fAniLength);
				}
			}
		}
		// not 3k ctrl pts
		 else{
			 float y1;
			 //using linear eqtn: (y1-y2)/(x1-x2)=slope => find yi(y1)
			 if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
				 y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
					 ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
					 (ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
			 }
			 else
				 y1 = ptvCtrlPts[0].y;

			 //make wrapping vector
			 ptvEvaluatedCurvePts.push_back(Point(0, y1));
			 ptvEvaluatedCurvePts.push_back(Point(fAniLength, y1));

			 //make wrapping vector
			 std::vector<Point> tempPts;
			 tempPts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
			 Point fPoint = tempPts.front();
			 Point bPoint = tempPts.back();

			 ptvEvaluatedCurvePts.push_back(fPoint);
			 ptvEvaluatedCurvePts.push_back(bPoint);

			 if (iCtrlPtCount >= 4){
				 for (int i = 0; iCtrlPtCount - (i + 1) >= 3; i += 3){
					 // de Castaljau’s algorithm
					 displayBezier(ptvCtrlPts, ptvEvaluatedCurvePts, i, fAniLength);
				 }
			 }
		 }
		
	}
}

float v0Tov1dist, v1Tov2dist, v2Tov3dist, v0Tov3dist, heightDiff, standardValue;
void BezierCurveEvaluator::displayBezier(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sIndex) const{
	//check for flatness
	//by same x or y
	float cx = ptvCtrlPts[sIndex].x;
	float cy = ptvCtrlPts[sIndex].y;

	if ((ptvCtrlPts[sIndex + 1].x == cx&&ptvCtrlPts[sIndex + 2].x == cx&&ptvCtrlPts[sIndex + 3].x == cx) ||
		(ptvCtrlPts[sIndex + 1].y == cy&&ptvCtrlPts[sIndex + 2].y == cy&&ptvCtrlPts[sIndex + 3].y == cy)){
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex]);
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex + 3]);
		return;
	}

	//by distance
	 v0Tov1dist = findDistance(ptvCtrlPts[sIndex], ptvCtrlPts[sIndex + 1]);
	 v1Tov2dist = findDistance(ptvCtrlPts[sIndex + 1], ptvCtrlPts[sIndex + 2]);
	 v2Tov3dist = findDistance(ptvCtrlPts[sIndex + 2], ptvCtrlPts[sIndex + 3]);
	 v0Tov3dist = findDistance(ptvCtrlPts[sIndex], ptvCtrlPts[sIndex + 3]);

	 float heightDiff = (abs(v0Tov1dist) + abs(v1Tov2dist) + abs(v2Tov3dist)) / abs(v0Tov3dist);

	if (heightDiff <= (1 + s_fFlatnessEpsilon)){
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex]);
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex+3]);
		return;
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

void BezierCurveEvaluator::displayBezier(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sIndex, int fAniLength) const{
	//check for flatness
	//by same x or y
	float cx = ptvCtrlPts[sIndex].x;
	float cy = ptvCtrlPts[sIndex].y;

	if ((ptvCtrlPts[sIndex + 1].x == cx&&ptvCtrlPts[sIndex + 2].x == cx&&ptvCtrlPts[sIndex + 3].x == cx) ||
		(ptvCtrlPts[sIndex + 1].y == cy&&ptvCtrlPts[sIndex + 2].y == cy&&ptvCtrlPts[sIndex + 3].y == cy)){
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex]);
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex + 3]);
		return;
	}

	//by distance
	v0Tov1dist = findDistance(ptvCtrlPts[sIndex], ptvCtrlPts[sIndex + 1]);
	v1Tov2dist = findDistance(ptvCtrlPts[sIndex + 1], ptvCtrlPts[sIndex + 2]);
	v2Tov3dist = findDistance(ptvCtrlPts[sIndex + 2], ptvCtrlPts[sIndex + 3]);
	v0Tov3dist = findDistance(ptvCtrlPts[sIndex], ptvCtrlPts[sIndex + 3]);

	float heightDiff = (abs(v0Tov1dist) + abs(v1Tov2dist) + abs(v2Tov3dist)) / abs(v0Tov3dist);

	if (heightDiff <= (1 + s_fFlatnessEpsilon + s_fFlatnessEpsilon)){
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex]);
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[sIndex+3]);
		return;
	}
	//do something smart
	else{
		float u = 0.5;	//leave for further declaration of u(ratio)
		std::vector<Point> subPointsVector = subdivide(ptvCtrlPts, ptvEvaluatedCurvePts, sIndex, u);
			
		if (subPointsVector[3].x > fAniLength){
			//left side
			displayBezier(subPointsVector, ptvEvaluatedCurvePts, 0, fAniLength);
			subPointsVector[3].x -= fAniLength;
			subPointsVector[4].x -= fAniLength;
			subPointsVector[5].x -= fAniLength;
			subPointsVector[6].x -= fAniLength;
			//right side
			displayBezier(subPointsVector, ptvEvaluatedCurvePts, 3, fAniLength);
		}
		else{
			//left side
			displayBezier(subPointsVector, ptvEvaluatedCurvePts, 0, fAniLength);
			//right side
			displayBezier(subPointsVector, ptvEvaluatedCurvePts, 3, fAniLength);
		}
			return;
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
	if (subPointsVector.size() < 7){
		std::cout << subPointsVector[0] << ' ' << subPointsVector[1] << ' ' << subPointsVector[2] << ' '
			<< subPointsVector[3] << ' ' << subPointsVector[4] << ' ' << subPointsVector[5] << '\n';
	}
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