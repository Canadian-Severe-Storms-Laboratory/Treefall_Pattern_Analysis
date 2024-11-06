#pragma once
#include <array>
#include <vector>
#include <iostream>
#include "Transect.h"
#include "CPP_CS_Interop.h"
#include "Utils.h"

constexpr int gridSize = 128;

EXPORT class VecHashGrid
{

private:

	struct GridVec {
		double x1;
		double y1;
		double x2;
		double y2;
	};

	std::vector<GridVec> grid[gridSize][gridSize];

	void computeExtent(std::vector<double>& vecs) {

		xmin = vecs[0];
		xmax = xmin;
		ymin = vecs[1];
		ymax = ymin;

		for (int i = 0; i < vecs.size(); i += 2) {
			xmin = std::min(xmin, vecs[i]);
			xmax = std::max(xmax, vecs[i]);
			ymin = std::min(ymin, vecs[i + 1]);
			ymax = std::max(ymax, vecs[i + 1]);
		}
	}

	std::array<int, 2> gridIndex(double x, double y) {

		int col = (int)((x - xmin) / (xmax - xmin) * gridSize);
		int row = (int)((y - ymin) / (ymax - ymin) * gridSize);
		
		col = std::clamp(col, 0, gridSize - 1);
		row = std::clamp(row, 0, gridSize - 1);

		return { col, row };
	}

	void insertRef(std::vector<double>& vecs) {
		
		for (int i = 0; i < vecs.size(); i += 4) {
			const std::array<double, 2> midPoint = { 0.5 * (vecs[i] + vecs[i + 2]), 0.5 * (vecs[i + 1] + vecs[i + 3]) };

			const auto [col, row] = gridIndex(midPoint[0], midPoint[1]);

			grid[row][col].push_back({ vecs[i], vecs[i + 1], vecs[i + 2], vecs[i + 3] });
		}
	}

	std::array<double, 2> rotatePoint(double x, double y, double a) {
		return { x * cos(a) - y * sin(a), x * sin(a) + y * cos(a) };
	}

public:
	double xmin;
	double xmax;
	double ymin;
	double ymax;

	VecHashGrid(std::vector<double> vecs) {

		Utils::writeBinaryFile(vecs, "C:\\Users\\danie\\Documents\\Treefall Pattern Analysis\\TreefallPatternAnalysisV2\\PatternSolver\\Vecs.bin");

		computeExtent(vecs);

		insertRef(vecs);
	}

	VecHashGrid(std::string filePath) {
		std::vector<double> vecs = Utils::readBinaryFile(filePath);
	
		computeExtent(vecs);

		insertRef(vecs);

		for (int r = 0; r < gridSize; ++r) {
			for (int c = 0; c < gridSize; ++c) {
				grid[r][c].shrink_to_fit(); // Optimize memory usage
			}
		}
	}

	void insert(std::vector<double> vecs) {
		insertRef(vecs);
	}

	std::vector<double> query(Rect rect) {
		const auto [col1, row1] = gridIndex(rect.x, rect.y);
		const auto [col2, row2] = gridIndex(rect.x + rect.w, rect.y + rect.h);

		std::vector<double> ret;

		for (int r = row1; r <= row2; r++) {
			for (int c = col1; c <= col2; c++) {
				for (const auto& vec : grid[r][c]) {

					const std::array<double, 2> midPoint = { 0.5 * (vec.x1 + vec.x2), 0.5 * (vec.y1 + vec.y2) };

					if (midPoint[0] < rect.x || midPoint[0] > rect.x + rect.w || midPoint[1] < rect.y || midPoint[1] > rect.y + rect.h) continue;

					ret.push_back(vec.x1);
					ret.push_back(vec.y1);
					ret.push_back(vec.x2);
					ret.push_back(vec.y2);
				}
			}
		}
		return ret;
	}

	std::vector<double> query(Transect transect) {

		std::vector<double> ret;

		std::array<double, 2> tc = transect.center();
		Rect rect = transect.normRect();
		const auto extent = transect.extent();

		const auto [col1, row1] = gridIndex(extent[0], extent[1]);
		const auto [col2, row2] = gridIndex(extent[2], extent[3]);

		const double cosA = cos(PI / 2.0 - transect.theta);
		const double sinA = sin(PI / 2.0 - transect.theta);

		for (int r = row1; r <= row2; r++) {
			for (int c = col1; c <= col2; c++) {
				for (const auto& vec : grid[r][c]) {

					const double midX = 0.5 * (vec.x1 + vec.x2) - tc[0];
					const double midY = 0.5 * (vec.y1 + vec.y2) - tc[1];

					// Rotated midpoint calculation
					const double rotatedX = midX * cosA - midY * sinA;
					const double rotatedY = midX * sinA + midY * cosA;

					if (rotatedX < rect.x || rotatedX > rect.x + rect.w || rotatedY < rect.y || rotatedY > rect.y + rect.h) continue;

					const std::array<double, 4> temp{
						(vec.x1 - tc[0]) * cosA - (vec.y1 - tc[1]) * sinA,
						(vec.x1 - tc[0]) * sinA + (vec.y1 - tc[1]) * cosA,
						(vec.x2 - tc[0]) * cosA - (vec.y2 - tc[1]) * sinA,
						(vec.x2 - tc[0]) * sinA + (vec.y2 - tc[1]) * cosA
					};

					ret.insert(ret.end(), temp.begin(), temp.end());
				}
			}
		}
		return ret;
	}

	std::vector<double> vectors() {
		std::vector<double> ret;

		for (const auto& row : grid) {
			for (const auto& col : row) {
				for (const auto& vec : col) {

					ret.push_back(vec.x1);
					ret.push_back(vec.y1);
					ret.push_back(vec.x2);
					ret.push_back(vec.y2);
				}
			}
		}

		return ret;
	}

	std::vector<double> counts() {
		std::vector<double> ret;

		for (const auto& row : grid) {
			for (const auto& col : row) {
				ret.push_back((double)col.size());
			}
		}

		return ret;
	}
};