#pragma once
#include "BurgersRottVortex.h"

class SullivanVortex : public BurgersRottVortex
{

public:
	SullivanVortex(double Vr = -1, double Vt = -1, double Vs = -1) : BurgersRottVortex(Vr, Vt, Vs) {
		k0 = 1.118044, k1 = 0.0607023333333, k2 = -1.0, k3 = 2.290069, k4 = 2.23272733333;
	}
};

