#pragma once
#include "VortexModel.h"
#include <array>
#include <vector>

class ModifiedRankineVortex : public VortexModel
{

public:
	double phi;


	ModifiedRankineVortex(double phi_, double Vr=-1, double Vt=-1, double Vs=-1) : phi(phi_), VortexModel(Vr, Vt, Vs){}

	Vec2 vecAt(double x, double y) {

        constexpr double Rmax = 1.0;

        const double r = hypot(x, y);

        const std::array<double, 2> radial_unit_vec = { -x / r, -y / r };

        const std::array<double, 2> tangential_unit_vec = { radial_unit_vec[1], -radial_unit_vec[0] };

        const double scale_factor = r <= Rmax ? pow(r / Rmax, phi) : pow(Rmax / r, phi);

        const double Vtan = Vt * scale_factor;
        const double Vrad = Vr * scale_factor;

        const std::array<double, 2> tangential_vec = { tangential_unit_vec[0] * Vtan, tangential_unit_vec[1] * Vtan };

        const std::array<double, 2> radial_vec = { radial_unit_vec[0] * Vrad, radial_unit_vec[1] * Vrad };

        return { tangential_vec[0] + radial_vec[0], tangential_vec[1] + radial_vec[1] + Vs };
	}

};

