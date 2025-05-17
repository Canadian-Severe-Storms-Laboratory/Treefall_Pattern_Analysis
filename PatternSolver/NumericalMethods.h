#pragma once
#include <functional>
#include <array>

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
    double a = I[0], b = I[1], c = I[1], d = 0.0, e = 0.0, min1, min2;
    double fa = func(a), fb = func(b), fc, p, q, r, s, tol1, xm;

    fc = fb;

    for (int iter = 0; iter < 1000; iter++) {
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


double localMinima(std::function<double(double)> f, std::array<double, 2> I, const double t = 1e-5, const double minThres = -1e308) {
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

	if (fx < minThres) return x;

    while (true) {
        m = 0.5 * (sa + sb);
        tol = eps * fabs(x) + t;
        t2 = 2.0 * tol;

        // Check the stopping criterion.
        if (fabs(x - m) <= t2 - 0.5 * (sb - sa)) break;

        // Fit a parabola.
        r = 0.0;
        q = r;
        p = q;

        if (tol < fabs(e)) {
            r = (x - w) * (fx - fv);
            q = (x - v) * (fx - fw);
            p = (x - v) * q - (x - w) * r;
            q = 2.0 * (q - r);

            if (q > 0.0) p = -p;

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
                d = x < m ? tol : -tol;
            }
        }
        // A golden-section step.
        else {
            e = x < m ? sb - x : sa - x;
            d = c * e;
        }

        // F must not be evaluated too close to X.
        if (tol <= fabs(d)) {
            u = x + d;
        }
        else if (d > 0.0) {
            u = x + tol;
        }
        else {
            u = x - tol;
        }

        fu = f(u);

        if (fu < minThres) return u;

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

std::array<double, 2> globalMinimum(std::function<double(double)> f, std::array<double, 2> I, size_t iters = 100, double eps = 1E-7) {
    const double dx = (I[1] - I[0]) / (double)(iters - 1);
    double f0 = f(I[0]);
    double f1 = f(I[0] + dx);

    std::array<double, 2> result = { f0, I[0] };

    for (size_t i = 1; i < iters - 1; i++) {
        const double x = I[0] + i * dx;
        const double f2 = f(x + dx);

        if (f0 > f1 && f1 < f2) {
            const double local_min = localMinima(f, { x - dx, x + dx }, eps);
            const double local_min_value = f(local_min);

            if (local_min_value < result[0]) result = { local_min_value, local_min };
        }

        f0 = f1;
        f1 = f2;
    }

    if (f1 < result[0]) result = { f1, I[1] };

    return result;
}

// integration using Simpson's method, N = number of function samples
template<size_t N=10>
double integrate(std::function<double(double)> f, std::array<double, 2> I) {

    static_assert(N % 2 == 0); // N must be even

    const double a = I[0];
    const double b = I[1];
    const double h = (b - a) / N;
    
    double sum_odds = f(a + h) + f(a + (N - 1) * h);
    double sum_evens = f(a + 2 * h);

    for (int i = 3; i < N - 2; i += 2) {
        sum_odds += f(a + i * h);
        sum_evens += f(a + (i + 1) * h);
    }

    return (f(a) + f(b) + 2.0 * sum_evens + 4.0 * sum_odds) * h / 3.0;
}

template<size_t N = 10>
double integrate2D(std::function<double(double, double)> f, std::array<double, 4> I) {

    const auto outerFunc = [&](double y) {
        
        const auto innerFunc = [&](double x) { return f(x, y); };

        return integrate<N>(innerFunc, {I[0], I[1]});
    };

    return integrate<N>(outerFunc, { I[2], I[3] });
}


