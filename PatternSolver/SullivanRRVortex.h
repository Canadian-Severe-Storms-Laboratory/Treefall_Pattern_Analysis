#pragma once
#include "RationalPolynomialRRVortex.h"

EXPORT class SullivanRRVortex : public RationalPolynomialRRVortex<1118044e-6, 607023333333e-13, -1e0, 2290069e-6, 223272733333e-11>
{
public:
	SullivanRRVortex(double Vr=-1, double Vt=-1, double Vs=-1) : RationalPolynomialRRVortex(Vr, Vt, Vs) {};
};

