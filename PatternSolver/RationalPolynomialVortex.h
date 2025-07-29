#pragma once
#include "VortexModel.h"


template <double k0, double k1, double k2, double k3, double k4>
class RationalPolynomialVortex : public VortexModel
{

public:

	RationalPolynomialVortex(double Vr = -1, double Vt = -1, double Vs = -1) : VortexModel(Vr, Vt, Vs) {};

	Vec2 vecAt(double x, double y) {

		constexpr double Rmax = 1.0;

		const double r2 = x * x + y * y;

		const double R2 = Rmax * Rmax;

		const double s = Rmax * (k1 * R2 + k3 * r2) / (R2 * (k0 * R2 + k2 * r2) + k4 * r2 * r2);

		return { -s * (x * Vr + y * Vt), s * (x * Vt - y * Vr) + Vs };
	}
};