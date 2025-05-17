#pragma once
#include "VortexModel.h"
#include <array>


template<double k0, double k1, double k2, double k3, double k4>
class RationalPolynomialRRVortex : public VortexModel
{

public:
	RationalPolynomialRRVortex(double Vr = -1, double Vt = -1, double Vs = -1) : VortexModel(Vr, Vt, Vs) {};

	Vec2 vecAt(double x, double y) {

		constexpr double Rmax = 1.0;

		const double r = hypot(x, y);

		const std::array<double, 2> radial_unit_vec = { -x / r, -y / r };

		const double sf = r <= Rmax ? r / Rmax : Rmax / r;

		const double r2 = r * r;
		const double Rmax2 = Rmax * Rmax;

		const double Vrad = Vr * sf;

		const std::array<double, 2> radial_vec = { radial_unit_vec[0] * Vrad, radial_unit_vec[1] * Vrad };

		const double Vtan = Vt * Rmax * (k1 * Rmax2 + k3 * r2) / (k0 * Rmax2 * Rmax2 + k2 * r2 * Rmax2 + k4 * r2 * r2);

		const std::array<double, 2> tangential_vec = { -y * Vtan, x * Vtan };

		return { tangential_vec[0] + radial_vec[0], tangential_vec[1] + radial_vec[1] + Vs };
	}

};