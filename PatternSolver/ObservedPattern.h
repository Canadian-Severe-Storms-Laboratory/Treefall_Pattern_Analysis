#pragma once
#include <vector>
#include <array>
#include "Vec2.h"
#include "CPP_CS_Interop.h"

EXPORT class ObservedPattern
{
private:

	void buildPattern(std::vector<double>& obsVecs) {

		const int n = (int)round((lengthAbove + lengthBelow) / spacing) + 1;
		const double sf = n / (lengthAbove + lengthBelow + spacing);

		std::vector<std::vector<Vec2>> bins(n);
		for (int i = 0; i < n; i++) {
			bins[i].reserve(obsVecs.size() / n);
		}

		for (int i = 0; i < obsVecs.size(); i += 4) {

			double my = 0.5 * (obsVecs[i + 1] + obsVecs[i + 3]);

			int idx = n - 1 - (int)floor(sf * (my + lengthBelow) + 0.5);

			bins[idx].emplace_back(obsVecs[i + 2] - obsVecs[i], obsVecs[i + 3] - obsVecs[i + 1]);
		}

		vecs.reserve(n);

		for (auto& bin : bins) {

			if (bin.size() == 0) {
				vecs.emplace_back(0.0, 0.0);
			}
			else {
				vecs.emplace_back(Vec2::averageUnit(bin));
			}
		}
	}

	std::array<bool, 2> consistentRotation(int startIdx, int endIdx) {
		int clockwiseCount = 0, counterclockwiseCount = 0;

		for (int i = startIdx + 1; i < endIdx; i++) {
			const Vec2& v1 = vecs[i - 1];
			const Vec2& v2 = vecs[i];

			if (v1.cross(v2) > 0) {
				counterclockwiseCount++;
			}
			else {
				clockwiseCount++;
			}

			// allow for 1 unconsistent rotation
			if (counterclockwiseCount > 0 && clockwiseCount > 0) {
				return { false, false };
			}
		}
		
		return { true, clockwiseCount > counterclockwiseCount };
	}

	bool isOuterInnerOuter() {
	
		int directionChanges = 0;
		int currentDirection = 0;  // 1 for counterclockwise, -1 for clockwise, 0 for no direction yet

		for (int i = 1; i < vecs.size(); ++i) {
			const Vec2& v1 = vecs[i - 1];
			const Vec2& v2 = vecs[i];

			if (v1.cross(v2) > 0) {
				// Counterclockwise
				if (currentDirection == 0)  return false;

				if (currentDirection == -1) directionChanges++;

				currentDirection = 1;
			}
			else {
				// Clockwise
				if (currentDirection == 1) directionChanges++;

				currentDirection = -1;
			}

			if (directionChanges > 2) {
				return false;
			}
		}

		return directionChanges == 2;
	}


public:
	std::vector<Vec2> vecs;
	double lengthAbove;
	double lengthBelow;
	double spacing;

	ObservedPattern(double lengthAbove_, double lengthBelow_, double spacing_, std::vector<double> obsVecs) {
		this->lengthAbove = floor((lengthAbove_ + 0.5 * spacing_) / spacing_) * spacing_;
		this->lengthBelow = floor((lengthBelow_ + 0.5 * spacing_) / spacing_) * spacing_;
		this->spacing = spacing_;
		
		buildPattern(obsVecs);
	}	

	// length of pattern
	double length() {
		return lengthAbove + lengthBelow;
	}

	// number of vectors in pattern
	int size() {
		return vecs.size();
	}

	int centerIdx() {
		return (int)round(lengthAbove / spacing);
	}

	std::vector<double> vectors() {
		std::vector<double> ret;
		ret.reserve(vecs.size() * 3);

		for (int i = 0; i < vecs.size(); i++) {
			ret.push_back(lengthAbove - i * spacing);
			ret.push_back(vecs[i].x);
			ret.push_back(vecs[i].y);		
		}

		return ret;
	}

	bool isMissingVectors() {
		for (const auto& vec : vecs) {
			if (vec.x == 0.0 && vec.y == 0.0) {
				return true;
			}
		}
		return false;
	}

	bool isConverging() {
		int cIdx = centerIdx();

		for (int i = 0; i < cIdx; i++) {
			if (vecs[i].y > 0) {
				return false;
			}
		}

		for (int i = cIdx + 1; i < vecs.size(); i++) {
			if (vecs[i].y < 0) {
				return false;
			}
		}

		return true;
	}

	bool isUnchanging() {

		for (int i = 1; i < vecs.size(); i++) {
			const Vec2& v1 = vecs[i - 1];
			const Vec2& v2 = vecs[i];

			if (v1.dot(v2) > 0.996) return true;
		}

		return false;
	}

	bool isSimulatableType() {
		int cIdx = centerIdx();

		auto [upperConsistent, upperClockwise] = consistentRotation(0, cIdx);
		auto [lowerConsistent, lowerClockwise] = consistentRotation(cIdx, vecs.size());

		if (!upperConsistent || !lowerConsistent) {
			// check for outer-inner-outer
			if (!isOuterInnerOuter()) return false;

			if (vecs[cIdx].x < 0.98) return false;
		}
		else {
			// outer
			if (upperClockwise && lowerClockwise) {
				if (vecs[cIdx].x > -0.98) return false;
			}
			// inner
			else if (!upperClockwise && !lowerClockwise) {
				if (vecs[cIdx].x < 0.98) return false;
			}
			// outer-inner not possible
			else if (upperClockwise && !lowerClockwise) {
				return false;
			}
		}

		return true;
	}

	bool isValid() {
		if (isMissingVectors()) return false;

		if (!isConverging()) return false;

		if (isUnchanging()) return false;

		if (!isSimulatableType()) return false;

		return true;
	}
};
