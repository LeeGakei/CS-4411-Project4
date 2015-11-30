#include "CatmullRomCurveEvaluator.h"
#include <assert.h>
#include "vec.h"

void CatmullRomCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();

	std::cout << "BezierCurveEvaluator::evaluateCurve\n";

	//ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
	ptvEvaluatedCurvePts.clear();

	if (iCtrlPtCount >= 4){
		if (!bWrap){
			for (int i = 0; i < iCtrlPtCount; i++){
				if (i == 0 || i == iCtrlPtCount - 2 || i == iCtrlPtCount - 1)
					continue;
				// de Castaljau’s algorithm
				ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
				displayCatmullRom(ptvCtrlPts, ptvEvaluatedCurvePts, i, fAniLength, bWrap);
				ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i + 1]);
			}
		}
		else{
			std::vector<Point> tempPts;
			tempPts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());
			//make wrapping vector
			std::vector<Point>::iterator sit;
			sit = tempPts.begin();
			tempPts.push_back(Point(sit->x + fAniLength, sit->y));
			tempPts.push_back(Point(tempPts[1].x + fAniLength, tempPts[1].y));
			tempPts.push_back(Point(tempPts[2].x + fAniLength, tempPts[2].y));

			iCtrlPtCount = tempPts.size();
			for (int i = 0; i < iCtrlPtCount; i++){
				if (i == 0 || i == iCtrlPtCount - 2 || i == iCtrlPtCount - 1)
					continue;
				// de Castaljau’s algorithm
				ptvEvaluatedCurvePts.push_back(tempPts[i]);
				displayCatmullRom(tempPts, ptvEvaluatedCurvePts, i, fAniLength, bWrap);
				ptvEvaluatedCurvePts.push_back(tempPts[i + 1]);

				//if (i == 0)
				//	continue;
				//// de Castaljau’s algorithm
				//displayCatmullRom(tempPts, ptvEvaluatedCurvePts, i, fAniLength, bWrap);
			}
		}

		
	}
}

void CatmullRomCurveEvaluator::displayCatmullRom(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sIndex, const float& fAniLength, const bool& bWrap) const{
	//t is always between 0 and 1 and determines the resolution of the spline
	//0 is always at p1
	for (float t = 0; t < 1; t += 0.1f) {
		if (t == 0) {
			continue;
		}
		//Find the coordinates between the control points with a Catmull-Rom spline
		Point catmullRomPoint = returnCatmullRom(t, ptvCtrlPts[sIndex - 1], ptvCtrlPts[sIndex], ptvCtrlPts[sIndex + 1], ptvCtrlPts[sIndex + 2]);
		Point bPoint;
		if (ptvEvaluatedCurvePts.size() == 1){
			ptvEvaluatedCurvePts.push_back(catmullRomPoint);
			continue;
		}
			bPoint = ptvEvaluatedCurvePts.back();

			if (!bWrap){
				if (catmullRomPoint.x >= bPoint.x){
					ptvEvaluatedCurvePts.push_back(catmullRomPoint);
				}
			}
			else{
				if (catmullRomPoint.x >= bPoint.x){
					if (catmullRomPoint.x > fAniLength){
						ptvEvaluatedCurvePts.push_back(Point(catmullRomPoint.x - fAniLength, catmullRomPoint.y));
					}
					else{
						ptvEvaluatedCurvePts.push_back(catmullRomPoint);
					}
				}
			}
	}

	////Also draw the last line since it is always less than 1, so we will always miss it
	//Gizmos.DrawLine(lastPos, p2);
}

Point CatmullRomCurveEvaluator::returnCatmullRom(float t, Point p1, Point p2, Point p3, Point p4)const{
	float t2 = t*t;
	float t3 = t2*t;
	float transformMatrix[4][4] = {
		{ float(-1) / float(2), float(3) / float(2), float(-3) / float(2), float(1) / float(2) },
		{ float(2) / float(2), float(-5) / float(2), float(4) / float(2), float(-1) / float(2) },
		{ float(-1) / float(2), 0, float(1) / float(2), 0 },
		{ 0, float(2) / float(2), 0, 0 }
	};

	float tMatrix[1][4] = {
		{ t3, t2, t, 1 }
	};

	const float ctrlPtsMatrix[4][2] = {
		p1.x, p1.y,
		p2.x, p2.y,
		p3.x, p3.y,
		p4.x, p4.y,
	};

	float catmullRomMatrix[1][2];
	float intermMatrix[1][4];
	float entry;
	//matrix multiplication
	for (int i = 0; i < 1; i++){
		for (int j = 0; j < 4; j++){
			entry = tMatrix[i][0] * transformMatrix[0][j] +
				tMatrix[i][1] * transformMatrix[1][j] +
				tMatrix[i][2] * transformMatrix[2][j] +
				tMatrix[i][3] * transformMatrix[3][j];
			intermMatrix[i][j] = entry;
		}
	}

	//matrix multiplication
	for (int i = 0; i < 1; i++){
		for (int j = 0; j < 2; j++){
			entry = intermMatrix[i][0] * ctrlPtsMatrix[0][j] +
				intermMatrix[i][1] * ctrlPtsMatrix[1][j] +
				intermMatrix[i][2] * ctrlPtsMatrix[2][j] +
				intermMatrix[i][3] * ctrlPtsMatrix[3][j];
			catmullRomMatrix[i][j] = entry;
		}
	}

	return Point(catmullRomMatrix[0][0], catmullRomMatrix[0][1]);
}