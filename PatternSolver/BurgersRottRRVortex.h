#pragma once
#include "RationalPolynomialRRVortex.h"


class BurgersRottRRVortex : public RationalPolynomialRRVortex<1.82417933, 3.11150184667, 1.51889411, 1.0, 0.768428406667>
{
public:
	BurgersRottRRVortex(double Vr=-1, double Vt=-1, double Vs=-1) : RationalPolynomialRRVortex(Vr, Vt, Vs) {};

};

