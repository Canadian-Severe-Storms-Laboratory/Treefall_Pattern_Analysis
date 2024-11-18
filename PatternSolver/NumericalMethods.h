#pragma once
#include <functional>
#include <array>
#include <boost/math/tools/toms748_solve.hpp>
#include <boost/math/tools/minima.hpp>

double binarySearch(std::function<bool(double)> func, std::array<double, 2> I, double tol = 1e-5) {
    
	double left = I[0];
	double right = I[1];
    double mid = (right + left) * 0.5;

    while (right - left > tol) {

        if (func(mid)) {
            left = mid;
        }
        else {
            right = mid;
        }

        mid = (right + left) * 0.5;
    }

    return mid;
}

double localRoot(std::function<double(double)> func, std::array<double, 2> I, double tol = 1e-5) {
    int iter;

    double a = I[0], b = I[1], c = I[1], d = 0.0, e = 0.0, min1, min2;
    double fa = func(a), fb = func(b), fc, p, q, r, s, tol1, xm;

    fc = fb;

    for (iter = 1; iter <= 1000; iter++) {
        // if sign(fb) = sign(fc)
        if (fb * fc > 0.0) {
            c = a;
            fc = fa;
            e = d = b - a;
        }
        if (fabs(fc) < fabs(fb)) {
            a = b;
            b = c;
            c = a;
            fa = fb;
            fb = fc;
            fc = fa;
        }

        tol1 = 2.0 * sqrt(2.220446049250313E-016) * fabs(b) + 0.5 * tol;
        xm = 0.5 * (c - b);

        if (fabs(xm) <= tol1 || fb == 0.0) return b;

        if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
            s = fb / fa;

            if (a == c) {
                p = 2.0 * xm * s;
                q = 1.0 - s;
            }
            else {
                q = fa / fc;
                r = fb / fc;
                p = s * (2.0 * xm * q * (q - r) - (b - a) * (r - 1.0));
                q = (q - 1.0) * (r - 1.0) * (s - 1.0);
            }

            if (p > 0.0) q = -q;

            p = fabs(p);

            min1 = 3.0 * xm * q - fabs(tol1 * q);
            min2 = fabs(e * q);

            if (2.0 * p < (min1 < min2 ? min1 : min2)) {
                e = d;
                d = p / q;
            }
            else {
                d = xm;
                e = d;
            }
        }
        else {
            d = xm;
            e = d;

        }
        a = b;
        fa = fb;

        if (fabs(d) > tol1)
            b += d;
        else
            b += copysign(tol1, xm);

        fb = func(b);
    }
    return 0.0;
}


double localMinima(std::function<double(double)> f, std::array<double, 2> I, const double t = 1e-5) {
    double a = I[0];
    double b = I[1];

    double c, d, e, eps, fu, fv, fw, fx, m, p, q, r, sa, sb, t2, tol, u, v, w, x;

    // C is the square of the inverse of the golden ratio.
    c = 0.5 * (3.0 - sqrt(5.0));

    eps = sqrt(2.220446049250313E-016);

    sa = a;
    sb = b;
    x = sa + c * (b - a);
    w = x;
    v = w;
    d = 0.0;
    e = 0.0;
    fx = f(x);
    fw = fx;
    fv = fw;

    while (true) {
        m = 0.5 * (sa + sb);
        tol = eps * fabs(x) + t;
        t2 = 2.0 * tol;

        // Check the stopping criterion.
        if (fabs(x - m) <= t2 - 0.5 * (sb - sa))
        {
            break;
        }

        // Fit a parabola.
        r = 0.0;
        q = r;
        p = q;

        if (tol < fabs(e)) {
            r = (x - w) * (fx - fv);
            q = (x - v) * (fx - fw);
            p = (x - v) * q - (x - w) * r;
            q = 2.0 * (q - r);

            if (0.0 < q) {
                p = -p;
            }

            q = fabs(q);
            r = e;
            e = d;
        }

        if (fabs(p) < fabs(0.5 * q * r) && q * (sa - x) < p && p < q * (sb - x)) {

            // Take the parabolic interpolation step.
            d = p / q;
            u = x + d;

            // F must not be evaluated too close to A or B.
            if ((u - sa) < t2 || (sb - u) < t2) {
                if (x < m) {
                    d = tol;
                }
                else {
                    d = -tol;
                }
            }
        }

        // A golden-section step.
        else {
            if (x < m) {
                e = sb - x;
            }
            else {
                e = sa - x;
            }
            d = c * e;
        }

        // F must not be evaluated too close to X.
        if (tol <= fabs(d)) {
            u = x + d;
        }
        else if (0.0 < d) {
            u = x + tol;
        }
        else {
            u = x - tol;
        }

        fu = f(u);

        // Update A, B, V, W, and X.
        if (fu <= fx) {
            if (u < x) {
                sb = x;
            }
            else {
                sa = x;
            }

            v = w;
            fv = fw;
            w = x;
            fw = fx;
            x = u;
            fx = fu;
        }
        else {
            if (u < x) {
                sa = u;
            }
            else {
                sb = u;
            }

            if (fu <= fw || w == x) {
                v = w;
                fv = fw;
                w = u;
                fw = fu;
            }
            else if (fu <= fv || v == x || v == w) {
                v = u;
                fv = fu;
            }
        }
    }

    return x;
}