#include "BSplineCurveEvaluator.h"
#include <assert.h>
#include "mat.h"

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap)const
{
	int iCtrlPtCount = ptvCtrlPts.size();

	ptvEvaluatedCurvePts.clear();
	std::vector<Point> bezierPtvCtrlPts;

	if (iCtrlPtCount >= 4){
		std::vector<Point> tempPts;
		if (!bWrap){
			std::vector<Point> tempPts;
			tempPts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
			Point fPoint = tempPts.front();
			Point ePoint = tempPts.back();
			ptvEvaluatedCurvePts.push_back(fPoint);
			ptvEvaluatedCurvePts.push_back(Point(0, fPoint.y));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, ePoint.y));
			ptvEvaluatedCurvePts.push_back(ePoint);
		}
		else{
			tempPts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
			//make wrapping vector
			std::vector<Point>::iterator sit;
			sit = tempPts.begin();
			tempPts.push_back(Point(sit->x + fAniLength, sit->y));
			tempPts.push_back(Point(tempPts[1].x + fAniLength, tempPts[1].y));

			iCtrlPtCount = tempPts.size();
		}
		for (int i = 0; i + 3 < iCtrlPtCount; i++){
			if (!bWrap){
				bezierPtvCtrlPts = transformToBezier(ptvCtrlPts, i);
				displayBSpline(bezierPtvCtrlPts, ptvEvaluatedCurvePts);
			}
			else{
				bezierPtvCtrlPts = transformToBezier(tempPts, i);
				displayBSpline(bezierPtvCtrlPts, ptvEvaluatedCurvePts, fAniLength);
			}
		}
	}
	bezierPtvCtrlPts.clear();
}

std::vector<Point> BSplineCurveEvaluator::transformToBezier(const std::vector<Point>& ptvCtrlPts, int sPoint)const{
	//set up transform matrix
	 float transformMatrix[4][4] = {
		 { float(1) / float(6), float(4) / float(6), float(1) / float(6), 0 },
		 { 0, float(4) / float(6), float(2) / float(6), 0 },
		 { 0, float(2) / float(6), float(4) / float(6), 0 },
		 { 0, float(1) / float(6), float(4) / float(6), float(1) / float(6) }
	};

	//set up B-spline matrix
	const float BSplineMatrix[4][2] = { 
		ptvCtrlPts[sPoint].x, ptvCtrlPts[sPoint].y,
		ptvCtrlPts[sPoint + 1].x, ptvCtrlPts[sPoint + 1].y,
		ptvCtrlPts[sPoint + 2].x, ptvCtrlPts[sPoint + 2].y,
		ptvCtrlPts[sPoint + 3].x, ptvCtrlPts[sPoint + 3].y,
	};

	float bezierMatrix[4][2];
	float entry;
	//matrix multiplication
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 2; j++){
			entry = transformMatrix[i][0] * BSplineMatrix[0][j] +
				transformMatrix[i][1] * BSplineMatrix[1][j] +
				transformMatrix[i][2] * BSplineMatrix[2][j] + 
				transformMatrix[i][3] * BSplineMatrix[3][j];
			bezierMatrix[i][j] = entry;
		}
	}

	std::vector<Point> ctrlPts;
	ctrlPts.push_back(Point(bezierMatrix[0][0], bezierMatrix[0][1]));
	ctrlPts.push_back(Point(bezierMatrix[1][0], bezierMatrix[1][1]));
	ctrlPts.push_back(Point(bezierMatrix[2][0], bezierMatrix[2][1]));
	ctrlPts.push_back(Point(bezierMatrix[3][0], bezierMatrix[3][1]));

	return ctrlPts;

}
void BSplineCurveEvaluator::displayBSpline(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts) const{
	displayBezier(ptvCtrlPts, ptvEvaluatedCurvePts, 0);
	return;
}

void BSplineCurveEvaluator::displayBSpline(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, const float& fAniLength) const{
	displayBezier(ptvCtrlPts, ptvEvaluatedCurvePts, 0, fAniLength);
	return;
}