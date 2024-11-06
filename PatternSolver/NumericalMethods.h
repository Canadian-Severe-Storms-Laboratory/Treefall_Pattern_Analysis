#pragma once
#include <functional>
#include <array>

double binarySearch(std::function<bool(double)> func, std::array<double, 2> I, double tol = 1e-5) {
    double mid;

	double& left = I[0];
	double& right = I[1];

    while (right - left > tol) {

        mid = (right + left) * 0.5;

        if (func(mid)) {
            left = mid;
        }
        else {
            right = mid;
        }
    }

    return mid;
}

double brentDekkerRoot(std::function<double(double)> func, std::array<double, 2> I, double tol = 1e-5) {
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