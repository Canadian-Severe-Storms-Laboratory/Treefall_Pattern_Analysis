#pragma once
#include "RationalPolynomialRRVortex.h"


EXPORT class BurgersRottRRVortex : public RationalPolynomialRRVortex<182417933e-8, 311150184667e-11, 151889411e-8, 1e0, 768428406667e-12>
{
public:
	BurgersRottRRVortex(double Vr=-1, double Vt=-1, double Vs=-1) : RationalPolynomialRRVortex(Vr, Vt, Vs) {};

};

