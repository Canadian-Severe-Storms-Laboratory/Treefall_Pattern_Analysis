#pragma once
#include <span>
#include <vector>
#include <array>
#include <omp.h>

#include "CPP_CS_Interop.h"
#include "Vec2.h"
#include "NumericalMethods.h"

EXPORT struct Curve {
	std::vector<double> xs{};
	std::vector<double> ys{};

	void reserve(const int N) {
		xs.reserve(N);
		ys.reserve(N);
	}
};

EXPORT struct Pattern {
	std::vector<double> vecs{};
	double dx;
	double lower;
	double upper;
	double Xc;

	Pattern() : dx(0.0), lower(0.0), upper(0.0), Xc(0.0) {}

	Pattern(double dx_, double lower_, double upper_, double Xc_) : dx(dx_), lower(lower_), upper(upper_), Xc(Xc_) {}

	void reserve(size_t n) {
		vecs.reserve(4 * n);
	}

	// x, y, vx, vy
	void insert(std::array<double, 4> p) {
		vecs.push_back(p[0]);
		vecs.push_back(p[1]);
		vecs.push_back(p[2]);
		vecs.push_back(p[3]);
	}
};

EXPORT class VortexModel
{

protected:
	std::array<double, 3> axesOfInterest() {
		
		//x axis of Vmax
		const double x0 = Vt / hypot(Vr, Vt);

		auto lowerFunc = [&](double x) {

			const double y = patternLocation(x);
			return y < -100.0; //there is not a root
		};

		auto upperFunc = [&](double x) {
			return patternLocation(x) > -100.0; //there is a root
		};

		const double lower = binarySearch(lowerFunc, { -100.0, x0 });
		const double upper = binarySearch(upperFunc, { x0, 100.0 });

		auto signFunc = [&](double x) {
			const double y = patternLocation(x);
			return vecAt(x, y).x;
		};

		const double Xc = brentDekkerRoot(signFunc, { lower + 1e-5, upper - 1e-5 });

		return { lower, upper, Xc };
	}

	std::array<double, 4> patternVecArray(double x) {
		const double y = patternLocation(x);
		Vec2 v = unitVecAt(x, y);

		return { x, y, v.x, v.y };
	}

public:
	double Vr;
	double Vt;
	double Vs;

	double lower;
	double upper;
	double Xc;

	VortexModel() : Vr(0.0), Vt(0.0), Vs(0.0), lower(0.0), upper(0.0), Xc(0.0) {};

	VortexModel(double _Vr, double _Vt, double _Vs) : Vr(_Vr), Vt(_Vt), Vs(_Vs), lower(0.0), upper(0.0), Xc(0.0) {};

	virtual Vec2 vecAt(double x, double y) = 0;
	virtual double patternLocation(double x) = 0;

	void solveAxesOfInterest() {
		const std::array<double, 3> axes = axesOfInterest();
		lower = axes[0];
		upper = axes[1];
		Xc = axes[2];
	}

	double length() {
		return upper - lower;
	}

	double lengthAbove() {
		return Xc - lower;
	}

	double lengthBelow() {
		return upper - Xc;
	}

	double vmax() {
		return hypot(Vr, Vt) + Vs;
	}

	bool hasPattern() {
		return vmax() > 1.0;
	}

	Vec2 unitVecAt(double x, double y) {
		return vecAt(x, y).unit();
	}

	Vec2 patternVec(double x) {
		const double y = patternLocation(x);
		
		return unitVecAt(x, y);
	}

	double magAt(double x, double y) {
		return vecAt(x, y).mag();
	}

	double swirlRatio() {
		return Vt / Vr;
	}

	std::vector<double> field(double minX, double maxX, double minY, double maxY, int N) {
		
		double dx = (maxX - minX) / N;
		double dy = (maxY - minY) / N;

		std::vector<double> fieldList;
		fieldList.reserve(2 * N * N);

		for (int i = 0; i < N; i++) {

			double y = maxY - (i + 0.5) * dy;

			//avoid division by zero
			y = fabs(y) < 1e-5 ? 1e-5 : y;

			for (int j = 0; j < N; j++) {

				double x = minX + (j + 0.5) * dx;

				Vec2 v = vecAt(x, y);

				fieldList.push_back(v.x);
				fieldList.push_back(v.y);
			}
		}

		return fieldList;
	}

	std::vector<double> field(double minX, double maxX, double minY, double maxY, double dx) {
		const size_t Nx = (size_t)ceil((maxX - minX) / dx);
		const size_t Ny = (size_t)ceil((maxY - minY) / dx);

		std::vector<double> fieldList;
		fieldList.reserve(2 * Nx * Ny);

		for (int i = 0; i < Ny; i++) {

			//avoid division by zero
			double y = maxY - i * dx + 0.0001;

			for (int j = 0; j < Nx; j++) {

				double x = minX + j * dx;

				Vec2 v = vecAt(x, y);

				fieldList.push_back(v.x);
				fieldList.push_back(v.y);
			}
		}

		return fieldList;
	}
	
	Curve patternCurve(int numPoints) {
		Curve curve;

		if (!hasPattern()) return curve;

		curve.reserve(numPoints);

		const double dx = (upper - lower) / (numPoints - 1);

		for (double x = lower + 0.001; x <= upper - 0.001; x += dx) {
			curve.xs.push_back(x);
			curve.ys.push_back(patternLocation(x));
		}

		return curve;
	}

	Pattern pattern(double dx) {

		if (!hasPattern()) return Pattern();

		Pattern pattern(dx, lower, upper, Xc);

		const double xMin = Xc - floor((Xc - lower) / dx) * dx + 0.001;
		const double xMax = Xc + floor((upper - Xc) / dx) * dx - 0.001;

		for (double x = xMin; x < Xc; x += dx) {
			pattern.insert(patternVecArray(x));
		}

		pattern.insert(patternVecArray(Xc - 0.001));
		pattern.insert(patternVecArray(Xc + 0.001));

		for (double x = Xc + dx; x <= xMax + 0.002; x += dx) {
			pattern.insert(patternVecArray(x));
		}

		return pattern;
	}

	Pattern pattern(int numVecs) {

		const double dx = (upper - lower) / numVecs;

		return pattern(dx);
	}

	~VortexModel() {};
};