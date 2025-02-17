//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.2.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public partial class VecHashGrid : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal VecHashGrid(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(VecHashGrid obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(VecHashGrid obj) {
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

  ~VecHashGrid() {
    Dispose(false);
  }

  public void Dispose() {
    Dispose(true);
    global::System.GC.SuppressFinalize(this);
  }

  protected virtual void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          PatternSolverPINVOKE.delete_VecHashGrid(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public double xmin {
    set {
      PatternSolverPINVOKE.VecHashGrid_xmin_set(swigCPtr, value);
    } 
    get {
      double ret = PatternSolverPINVOKE.VecHashGrid_xmin_get(swigCPtr);
      return ret;
    } 
  }

  public double xmax {
    set {
      PatternSolverPINVOKE.VecHashGrid_xmax_set(swigCPtr, value);
    } 
    get {
      double ret = PatternSolverPINVOKE.VecHashGrid_xmax_get(swigCPtr);
      return ret;
    } 
  }

  public double ymin {
    set {
      PatternSolverPINVOKE.VecHashGrid_ymin_set(swigCPtr, value);
    } 
    get {
      double ret = PatternSolverPINVOKE.VecHashGrid_ymin_get(swigCPtr);
      return ret;
    } 
  }

  public double ymax {
    set {
      PatternSolverPINVOKE.VecHashGrid_ymax_set(swigCPtr, value);
    } 
    get {
      double ret = PatternSolverPINVOKE.VecHashGrid_ymax_get(swigCPtr);
      return ret;
    } 
  }

  public VecHashGrid(DoubleVector vecs) : this(PatternSolverPINVOKE.new_VecHashGrid__SWIG_0(DoubleVector.getCPtr(vecs)), true) {
    if (PatternSolverPINVOKE.SWIGPendingException.Pending) throw PatternSolverPINVOKE.SWIGPendingException.Retrieve();
  }

  public VecHashGrid(string filePath) : this(PatternSolverPINVOKE.new_VecHashGrid__SWIG_1(filePath), true) {
    if (PatternSolverPINVOKE.SWIGPendingException.Pending) throw PatternSolverPINVOKE.SWIGPendingException.Retrieve();
  }

  public void insert(DoubleVector vecs) {
    PatternSolverPINVOKE.VecHashGrid_insert(swigCPtr, DoubleVector.getCPtr(vecs));
    if (PatternSolverPINVOKE.SWIGPendingException.Pending) throw PatternSolverPINVOKE.SWIGPendingException.Retrieve();
  }

  public DoubleVector query(Rect rect) {
    DoubleVector ret = new DoubleVector(PatternSolverPINVOKE.VecHashGrid_query__SWIG_0(swigCPtr, Rect.getCPtr(rect)), true);
    if (PatternSolverPINVOKE.SWIGPendingException.Pending) throw PatternSolverPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public DoubleVector query(Transect transect) {
    DoubleVector ret = new DoubleVector(PatternSolverPINVOKE.VecHashGrid_query__SWIG_1(swigCPtr, Transect.getCPtr(transect)), true);
    if (PatternSolverPINVOKE.SWIGPendingException.Pending) throw PatternSolverPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public DoubleVector vectors() {
    DoubleVector ret = new DoubleVector(PatternSolverPINVOKE.VecHashGrid_vectors(swigCPtr), true);
    return ret;
  }

  public DoubleVector counts() {
    DoubleVector ret = new DoubleVector(PatternSolverPINVOKE.VecHashGrid_counts(swigCPtr), true);
    return ret;
  }

}
