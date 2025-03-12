#pragma once
#include "RationalPolynomialRRVortex.h"

class SullivanRRVortex : public RationalPolynomialRRVortex<1.118044, 0.0607023333333, -1.0, 2.290069, 2.23272733333>
{
public:
	SullivanRRVortex(double Vr=-1, double Vt=-1, double Vs=-1) : RationalPolynomialRRVortex(Vr, Vt, Vs) {};
};

