//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.2.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public partial class LinearPiecewiseVortex : VortexModel {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal LinearPiecewiseVortex(global::System.IntPtr cPtr, bool cMemoryOwn) : base(PatternSolverPINVOKE.LinearPiecewiseVortex_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(LinearPiecewiseVortex obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(LinearPiecewiseVortex obj) {
    if (obj != null) {
      if (!obj.swigCMemOwn)
        throw new global::System.ApplicationException("Cannot release ownership as memory is not owned");
      global::System.Runtime.InteropServices.HandleRef ptr = obj.swigCPtr;
      obj.swigCMemOwn = false;
      obj.Dispose();
      return ptr;
    } else {
      return new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
    }
  }

  protected override void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          PatternSolverPINVOKE.delete_LinearPiecewiseVortex(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public LinearPiecewiseVortex() : this(PatternSolverPINVOKE.new_LinearPiecewiseVortex__SWIG_0(), true) {
  }

  public LinearPiecewiseVortex(SWIGTYPE_p_SplineTable splineTable_, double Vs, double Vr, double Vt) : this(PatternSolverPINVOKE.new_LinearPiecewiseVortex__SWIG_1(SWIGTYPE_p_SplineTable.getCPtr(splineTable_), Vs, Vr, Vt), true) {
    if (PatternSolverPINVOKE.SWIGPendingException.Pending) throw PatternSolverPINVOKE.SWIGPendingException.Retrieve();
  }

  public LinearPiecewiseVortex(DoubleVector vrLineData, DoubleVector vtLineData) : this(PatternSolverPINVOKE.new_LinearPiecewiseVortex__SWIG_2(DoubleVector.getCPtr(vrLineData), DoubleVector.getCPtr(vtLineData)), true) {
    if (PatternSolverPINVOKE.SWIGPendingException.Pending) throw PatternSolverPINVOKE.SWIGPendingException.Retrieve();
  }

  public override SWIGTYPE_p_Vec2 vecAt(double x, double y) {
    SWIGTYPE_p_Vec2 ret = new SWIGTYPE_p_Vec2(PatternSolverPINVOKE.LinearPiecewiseVortex_vecAt(swigCPtr, x, y), true);
    return ret;
  }

  public override double patternLocation(double x) {
    double ret = PatternSolverPINVOKE.LinearPiecewiseVortex_patternLocation(swigCPtr, x);
    return ret;
  }

  public override bool patternLocationExists(double x) {
    bool ret = PatternSolverPINVOKE.LinearPiecewiseVortex_patternLocationExists(swigCPtr, x);
    return ret;
  }

}
