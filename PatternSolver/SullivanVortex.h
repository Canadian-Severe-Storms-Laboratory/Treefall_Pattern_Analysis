#pragma once
#include "RationalPolynomialVortex.h"

EXPORT class SullivanVortex : public RationalPolynomialVortex<1.118044, 0.0607023333333, -1.0, 2.290069, 2.23272733333>
{

public:
	SullivanVortex(double Vr = -1, double Vt = -1, double Vs = -1) : RationalPolynomialVortex(Vr, Vt, Vs){};
};

