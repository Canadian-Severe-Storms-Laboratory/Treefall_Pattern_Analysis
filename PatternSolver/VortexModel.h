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
		
		constexpr double limit = 100.0;

		//x axis of Vmax
		const double x0 = Vt / hypot(Vr, Vt);

		auto lowerFunc = [&](double x) { return !patternLocationExists(x); }; //there is not a root

		auto upperFunc = [&](double x) { return patternLocationExists(x); }; //there is a root
		
		const double lower = binarySearch(lowerFunc, { -limit, x0 });

		const double upper = binarySearch(upperFunc, { x0, limit });

		auto signFunc = [&](double x) { return vecAt(x, patternLocation(x)).x; };

		const double Xc = localRoot(signFunc, { lower + 1e-5, upper - 1e-5 });

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

	VortexModel(double _Vr=-1, double _Vt=-1, double _Vs=-1) : Vr(_Vr), Vt(_Vt), Vs(_Vs), lower(0.0), upper(0.0), Xc(0.0) {};

	virtual Vec2 vecAt(double x, double y) = 0;
	
	virtual double patternLocation(double x) {
		constexpr double Rmax = 1.0;
		constexpr double Vc2 = 1.0;
		constexpr double limit = 100.0;

		const auto rootFunc = [&](double y) { return Vc2 - vecAt(x, y).magSq(); };

		if (fabs(x) < Rmax) {
			const double ym = sqrt(Rmax * Rmax - x * x);

			if (rootFunc(ym) <= 0.0) return localRoot(rootFunc, { ym, limit });

			if (rootFunc(-ym) <= 0.0) return localRoot(rootFunc, { -ym, ym });
		}

		const double ym = localMinima(rootFunc, { -limit, limit }, 1e-5, 0.0); // upper = 0.0?

		if (rootFunc(ym) <= 0.0) return localRoot(rootFunc, { ym, limit });

		return std::numeric_limits<double>::lowest();
	}

	virtual bool patternLocationExists(double x) {
		constexpr double Rmax = 1.0;
		constexpr double Vc2 = 1.0;
		constexpr double limit = 100.0;

		const auto rootFunc = [&](double y) { return Vc2 - vecAt(x, y).magSq(); };

		if (fabs(x) < Rmax) {
			const double ym = sqrt(Rmax * Rmax - x * x);

			if (rootFunc(ym) <= 0.0) return true;

			if (rootFunc(-ym) <= 0.0) return true;
		}

		const double ym = localMinima(rootFunc, { -limit, limit }, 1e-5, 0.0);

		if (rootFunc(ym) <= 0.0) return true;

		return false;
	}

	double maxVelLocation(double x) {
		constexpr double Rmax = 1.0;
		constexpr double limit = 10.0;

		const auto func = [&](double y) { return -magAt(x, y); };

		/*if (fabs(x) <= Rmax) {
			return globalMinimum(func, { -limit, limit })[1];
		}*/

		return localMinima(func, { -limit, 0.0 }, 1e-5);
	}

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
		return vmax() > 1.01;
	}

	bool isOuterType() {
		return vmax() >= 1.0 + 2.0 * Vs;
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

	double vgust(double Vc, double Rmax) {
		const double x0 = Vt / hypot(Vr, Vt);
		const double y0 = -Vr / hypot(Vr, Vt);

		const double Vs_r = Vc * Vs / Rmax;

		const double s = 1.5 * Vs_r;

		const double i = integrate2D([&](double x, double y) {return magAt(x, y); }, { x0 - s, x0 + s, y0 - s, y0 + s });

		const double area = 9.0 * Vs_r * Vs_r;

		return i / area;
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

		//could be parallized
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

		//could be parallized
		for (double x = lower + 0.001; x <= upper - 0.001; x += dx) {
			curve.xs.push_back(x);
			curve.ys.push_back(patternLocation(x));
		}

		return curve;
	}

	Curve maxVelCurve(double min, double max, int numPoints) {
		Curve curve;
		curve.reserve(numPoints);

		const double dx = (max - min) / (double)(numPoints - 1);

		for (int i = 0; i < numPoints; i++) {
			const double x = min + (double)i * dx;

			curve.xs.push_back(x);
			curve.ys.push_back(maxVelLocation(x));
		}

		return curve;
	}

	Curve maxVelMagCurve(double min, double max, int numPoints) {
		Curve curve;
		curve.reserve(numPoints);

		const double dx = (max - min) / (double)(numPoints - 1);

		for (int i = 0; i < numPoints; i++) {
			const double x = min + (double)i * dx;

			curve.xs.push_back(x);
			curve.ys.push_back(magAt(x, maxVelLocation(x)));
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