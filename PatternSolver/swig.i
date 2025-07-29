%module PatternSolver

%{
#include "CPP_CS_Interop.h"
#include "Utils.h"
#include "VortexModel.h"
#include "RationalPolynomialVortex.h"
#include "RationalPolynomialRRVortex.h"
#include "LinearPiecewiseVortex.h"
#include "BakerSterlingVortex.h"
#include "BurgersRottVortex.h"
#include "BurgersRottRRVortex.h"
#include "SullivanVortex.h"
#include "SullivanRRVortex.h"
#include "ModifiedRankineVortex.h"
#include "VecHashGrid.h"
#include "ConvergenceLine.h"
#include "Transect.h"
#include "ObservedPattern.h"
#include "TransectRandomizer.h"
#include "AutoTransectFitter.h"
#include "PatternMatcher.h"
#include "Monitor.h"
%}

%include "typemaps.i"
%include "std_string.i"

/* allow partial c# classes */
%typemap(csclassmodifiers) SWIGTYPE "public partial class"
%typemap(csclassmodifiers) std::span<double>* "public partial class"
%typemap(csclassmodifiers) Vec2* "public partial class"

%include "std_vector.i"
%include "carrays.i"
%template(DoubleVector) std::vector<double>;

%include "arrays_csharp.i"
%apply double FIXED[] {double *sourceArray} 
%apply double* {double *spanArray} 

%csmethodmodifiers createVector "public unsafe";
%csmethodmodifiers createSpan "public unsafe";

%include <windows.i>
%include "CPP_CS_Interop.h"
%include "Utils.h"
%include "VortexModel.h"
%include "LinearPiecewiseVortex.h"
%include "BakerSterlingVortex.h"
%include "ModifiedRankineVortex.h"
%include "RationalPolynomialVortex.h"
%include "RationalPolynomialRRVortex.h"

%template(RPV_BR) RationalPolynomialVortex<182417933e-8, 311150184667e-11, 151889411e-8, 1e0, 768428406667e-12>;
%template(RPV_BR_RR) RationalPolynomialRRVortex<182417933e-8, 311150184667e-11, 151889411e-8, 1e0, 768428406667e-12>;
%template(RPV_S) RationalPolynomialVortex<1118044e-6, 607023333333e-13, -1e0, 2290069e-6, 223272733333e-11>;
%template(RPV_S_RR) RationalPolynomialRRVortex<1118044e-6, 607023333333e-13, -1e0, 2290069e-6, 223272733333e-11>;

%include "BurgersRottVortex.h"
%include "BurgersRottRRVortex.h"
%include "SullivanVortex.h"
%include "SullivanRRVortex.h"
%include "VecHashGrid.h"
%include "ConvergenceLine.h"
%include "Transect.h"
%include "ObservedPattern.h"
%include "TransectRandomizer.h"
%include "AutoTransectFitter.h"
%include "PatternMatcher.h"
%include "Monitor.h"

%begin %{
#ifndef _NOEXPORT
%}

%init %{
#endif
%}