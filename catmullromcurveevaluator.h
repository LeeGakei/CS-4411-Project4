#ifndef INCLUDED_CATMULLROM_CURVE_EVALUATOR_H
#define INCLUDED_CATMULLROM_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class CatmullRomCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	virtual void displayCatmullRom(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sPoint, const float& fAniLength, const bool& bWrap) const;
	Point returnCatmullRom(float t, Point p1, Point p2, Point p3, Point p4)const;
private:
	float v0Tov1dist, v1Tov2dist, v2Tov3dist, v0Tov3dist, heightDiff, standardValue;
};
std::vector<Point> subdivide(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sPoint, float u);
Point calculateIntermidate(Point p0, Point p1, float u);
float findDistance(Point p0, Point p1);


#endif