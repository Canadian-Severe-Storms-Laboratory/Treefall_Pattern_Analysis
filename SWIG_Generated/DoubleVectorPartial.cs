
using System;
using System.Runtime.InteropServices;

public partial class DoubleVector : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IList<double>
{
    // cast from C# array to vector
    public static implicit operator DoubleVector(double[] inVal)
    {
        return PatternSolver.createVector(inVal, inVal.Length);
    }

    // cast to C# array from vector
    public static unsafe implicit operator double[](DoubleVector inVal)
    {
        if (inVal.Count == 0) return [];

        var p = PatternSolver.getVector(inVal);

        IntPtr ptr = SWIGTYPE_p_double.getCPtr(p).Handle;

        double[] ret = new double[inVal.Count];
        Marshal.Copy(ptr, ret, 0, ret.Length);

        return ret;
    }

    public static unsafe implicit operator Span<double>(DoubleVector inVal)
    {
        var p = PatternSolver.getVector(inVal);
        double* ptr = (double*)SWIGTYPE_p_double.getCPtr(p).Handle;

        return new Span<double>(ptr, inVal.Count);
    }
}


