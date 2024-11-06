
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

public partial class SWIGTYPE_p_std__spanT_double_t {

    public static unsafe implicit operator SWIGTYPE_p_std__spanT_double_t(Span<double> inVal)
    {
        SWIGTYPE_p_std__spanT_double_t scpp;

        fixed (double* p = inVal)
        {
            scpp = PatternSolver.createSpan(new SWIGTYPE_p_double(new IntPtr(p), false), inVal.Length);
        }

        return scpp;
    }

    public static unsafe implicit operator SWIGTYPE_p_std__spanT_double_t(List<double> inVal)
    {
        var sx = CollectionsMarshal.AsSpan(inVal);

        SWIGTYPE_p_std__spanT_double_t scpp;

        fixed (double* p = sx)
        {
            scpp = PatternSolver.createSpan(new SWIGTYPE_p_double(new IntPtr(p), false), sx.Length);
        }

        return scpp;
    }

    public static unsafe implicit operator SWIGTYPE_p_std__spanT_double_t(double[] inVal)
    {
        SWIGTYPE_p_std__spanT_double_t scpp;

        fixed (double* p = inVal)
        {
            scpp = PatternSolver.createSpan(new SWIGTYPE_p_double(new IntPtr(p), false), inVal.Length);
        }

        return scpp;
    }

    public static unsafe implicit operator Span<double>(SWIGTYPE_p_std__spanT_double_t inVal)
    {
        IntPtr p = SWIGTYPE_p_double.getCPtr(PatternSolver.getSpanPtr(inVal)).Handle;

        return new Span<double>((void*)p, PatternSolver.getSpanSize(inVal));
    }

}