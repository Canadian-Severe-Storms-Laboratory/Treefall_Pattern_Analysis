#pragma once
#include "RationalPolynomialVortex.h"

EXPORT class SullivanVortex : public RationalPolynomialVortex<1118044e-6, 607023333333e-13, -1e0, 2290069e-6, 223272733333e-11>
{

public:
	SullivanVortex(double Vr = -1, double Vt = -1, double Vs = -1) : RationalPolynomialVortex(Vr, Vt, Vs){};
};

