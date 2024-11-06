#pragma once
#include "LinearPiecewiseVortex.h"
#include "BakerSterlingVortex.h"
#include "CPP_CS_Interop.h"


EXPORT class VortexFactory
{
public:
	
	static BakerSterlingVortex bakerSterlingVortex(double Vr, double Vt, double Vs) {
		return BakerSterlingVortex(Vr, Vt, Vs);
	}

	static LinearPiecewiseVortex BakerSterlingLPVortex(double Vr, double Vt, double Vs) {

		LinearPiecewiseVortex lpv(SplineTable({ { 0.268689, 1.898414, 0.000000, 1.898414, 0.000000 },
												{ 0.491592, 1.303857, 0.159751, 1.303857, 0.159751 },
												{ 0.731630, 0.671810, 0.470460, 0.671810, 0.470460 },
												{ 1.000000, 0.141683, 0.858317, 0.141683, 0.858317 },
												{ 1.409418, -0.117645, 1.117645, -0.117645, 1.117645 },
												{ 2.588731, -0.243813, 1.295469, -0.243813, 1.295469 },
												{ 4.017329, -0.142425, 1.033003, -0.142425, 1.033003 },
												{ 6.320819, -0.069506, 0.740064, -0.069506, 0.740064 },
												{ 10.000000, -0.027916, 0.477176, -0.027916, 0.477176 } }));

		lpv.Vr = Vr;
		lpv.Vt = Vt;
		lpv.Vs = Vs;

		return lpv;
	}

};