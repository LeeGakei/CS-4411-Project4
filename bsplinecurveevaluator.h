#ifndef INCLUDED_BSPLINE_CURVE_EVALUATOR_H
#define INCLUDED_BSPLINE_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

//#include "CurveEvaluator.h"
#include "BezierCurveEvaluator.h"

//using namespace std;

class BSplineCurveEvaluator : public BezierCurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap)const override;
	std::vector<Point> transformToBezier(const std::vector<Point>& ptvCtrlPts, int sPoint)const;
	void displayBSpline(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts) const;
	void displayBSpline(const std::vector<Point>& ptvCtrlPts, std::vector<Point>& ptvEvaluatedCurvePts, const float& fAniLength) const;

};


#endif