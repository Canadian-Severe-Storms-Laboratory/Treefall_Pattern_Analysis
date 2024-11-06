#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include "CPP_CS_Interop.h"
#include "vec2.h"
#include "Utils.h"

EXPORT class ConvergenceLine
{
private:
	std::vector<Vec2> linePoints;
	std::vector<double> cumulativeLengths;

public:
	ConvergenceLine(std::vector<double> points) {
	
		Utils::writeBinaryFile(points, "C:\\Users\\danie\\Documents\\Treefall Pattern Analysis\\TreefallPatternAnalysisV2\\PatternSolver\\ConvergenceLine.bin");

		linePoints.reserve(points.size() / 2);

		for (int i = 0; i < points.size(); i += 2) {
			linePoints.push_back(Vec2(points[i], points[i + 1]));
		}

		cumulativeLengths.reserve(linePoints.size());
		cumulativeLengths.push_back(0.0);

		for (int i = 1; i < linePoints.size(); i++) {
			cumulativeLengths.push_back(cumulativeLengths[i - 1] + (linePoints[i] - linePoints[i - 1]).mag());
		}
	
	};

	ConvergenceLine(std::string filePath) {

		std::vector<double> points = Utils::readBinaryFile(filePath);

		linePoints.reserve(points.size() / 2);

		for (int i = 0; i < points.size(); i += 2) {
			linePoints.push_back(Vec2(points[i], points[i + 1]));
		}

		cumulativeLengths.reserve(linePoints.size());
		cumulativeLengths.push_back(0.0);

		for (int i = 1; i < linePoints.size(); i++) {
			cumulativeLengths.push_back(cumulativeLengths[i - 1] + (linePoints[i] - linePoints[i - 1]).mag());
		}

	};


	// returns point and angle of polyline at given distance from start
	std::vector<double> query(double dist) {

		dist = std::max(std::min(cumulativeLengths.back(), dist), 0.000001);

		int idx = lower_bound(cumulativeLengths.begin(), cumulativeLengths.end(), dist) - cumulativeLengths.begin();

		Vec2 p1 = linePoints[idx - 1];
		Vec2 p2 = linePoints[idx];

		double n = (dist - cumulativeLengths[idx - 1]) / (cumulativeLengths[idx] - cumulativeLengths[idx - 1]);

		Vec2 p = p1 * (1.0 - n) + p2 * n;

		double angle = (p2 - p1).angle();

		return { p.x, p.y, angle };
	}

	double length() {
		return cumulativeLengths.back();
	}

	std::vector<double> getXs() {
		std::vector<double> xs;
		xs.reserve(linePoints.size());
		for (int i = 0; i < linePoints.size(); i++) {
			xs.push_back(linePoints[i].x);
		}
		return xs;
	}

	std::vector<double> getYs() {
		std::vector<double> ys;
		ys.reserve(linePoints.size());
		for (int i = 0; i < linePoints.size(); i++) {
			ys.push_back(linePoints[i].y);
		}
		return ys;
	}
};
