#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	void displayBezier(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sPoint) const;
	
};
std::vector<Point> subdivide(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, int sPoint, float u);
Point calculateIntermidate(Point p0, Point p1, float u);


#endif