#pragma once
#include <algorithm>
#include "VortexModel.h"
#include "cyPolynomial.h"
#include "quartic.h"

EXPORT class BakerSterlingVortex : public VortexModel {

private:
	std::array<DComplex, 4> solveRoots(double x) {
		constexpr double Rmax = 1.0;
		constexpr double Vc = 1.0;

		const double x2 = x * x;
		const double Vr2 = Vr * Vr;
		const double Vt2 = Vt * Vt;
		const double Vs2 = Vs * Vs;
		const double Vc2 = Vc * Vc;
		const double Rmax2 = Rmax * Rmax;

		const double q = 2.0 * (Vr2 + Vt2) + Vs2;
		const double d = Rmax2 + x2;
		const double k = Rmax * Vs;
		const double kt = k * Vt;
		const double kr = k * Vr;

		const double b1 = x2 * Vs2;
		const double b2 = d * Vc2;
		const double b3 = 2.0 * q * Rmax2;
		const double b4 = 4.0 * kt * x;

		const double a4 = Vs2 - Vc2;
		const double a3 = -4.0 * kr;
		const double a2 = 2.0 * (b1 - b2) + b3 + b4;
		const double a1 = d * a3;
		const double a0 = x2 * (b3 + b1) + Vs2 * Rmax2 * Rmax2 + d * (b4 - b2);

		return solve_quartic(a3 / a4, a2 / a4, a1 / a4, a0 / a4);
	}

public:

	BakerSterlingVortex(double Vr=-1, double Vt=-1, double Vs=-1) : VortexModel(Vr, Vt, Vs) {};

	Vec2 vecAt(double x, double y) {

		constexpr double Rmax = 1.0;

		const double r2 = x * x + y * y;
		const double k = (2.0 * Rmax) / (r2 + Rmax * Rmax);

		return { -k * (Vr * x + Vt * y), k * (Vt * x - Vr * y) + Vs };
	}

	double patternLocation(double x) override {

		std::array<DComplex, 4> solutions = solveRoots(x);

		double y = std::numeric_limits<double>::lowest();

		y = fabs(imag(solutions[0])) < 1e-10 ? std::max(y, real(solutions[0])) : y;
		y = fabs(imag(solutions[1])) < 1e-10 ? std::max(y, real(solutions[1])) : y;
		y = fabs(imag(solutions[2])) < 1e-10 ? std::max(y, real(solutions[2])) : y;
		y = fabs(imag(solutions[3])) < 1e-10 ? std::max(y, real(solutions[3])) : y;

		return y;
	}

	bool patternLocationExists(double x) override {
		std::array<DComplex, 4> solutions = solveRoots(x);

		return fabs(imag(solutions[0])) < 1e-10 ||
			   fabs(imag(solutions[1])) < 1e-10 ||
			   fabs(imag(solutions[2])) < 1e-10 ||
			   fabs(imag(solutions[3])) < 1e-10;
	}
};