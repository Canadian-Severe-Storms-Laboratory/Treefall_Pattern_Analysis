%module PatternSolver

%{
#include "CPP_CS_Interop.h"
#include "Utils.h"
#include "VortexModel.h"
#include "LinearPiecewiseVortex.h"
#include "VecHashGrid.h"
#include "ConvergenceLine.h"
#include "Transect.h"
#include "ObservedPattern.h"
#include "AutoTransectFitter.h"
#include "PatternMatcher.h"
#include "VortexFactory.h"
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
%include "VecHashGrid.h"
%include "ConvergenceLine.h"
%include "Transect.h"
%include "ObservedPattern.h"
%include "AutoTransectFitter.h"
%include "PatternMatcher.h"
%include "VortexFactory.h"
%include "Monitor.h"

%begin %{
#ifndef _NOEXPORT
%}

%init %{
#endif
%}