// Based on the work of Саша Миленковић, https://github.com/sasamil/Quartic/tree/master

#ifndef QUARTIC_H
#define QUARTIC_H

#include <cmath>
#include <limits>
#include <algorithm>

 // solve real root of cubic x^3 + ax^2 + bx + c with maximum absolute value
double solveP3(double a, double b, double c) {
	constexpr double M_2PI = 6.283185307179586;
	constexpr double f1_3 = 1.0 / 3.0, f1_9 = 1.0 / 9.0, f1_54 = 1.0 / 54.0;
	constexpr double eps = 1e-12;

	const double a2 = a * a;
	const double q = (a2 - 3.0 * b) * f1_9;
	const double r = (a * (2.0 * a2 - 9.0 * b) + 27.0 * c) * f1_54;
	const double r2 = r * r;
	const double q3 = q * q * q;

	a *= f1_3;

	// 3 real roots
	if (r2 < q3) {
		const double t = acos(std::clamp(r / sqrt(q3), -1.0, 1.0));
		const double q2 = -2.0 * sqrt(q);

		const double x0 = q2 * cos(t * f1_3) - a;
		const double x1 = q2 * cos((t + M_2PI) * f1_3) - a;
		const double x2 = q2 * cos((t - M_2PI) * f1_3) - a;

		double y = x0;

		if (fabs(x1) > fabs(y)) y = x1;
		if (fabs(x2) > fabs(y)) y = x2;

		return y;
	}
	
	const double R = -pow(fabs(r) + sqrt(r2 - q3), f1_3);
	const double A = r < 0 ? -R : R;
	const double B = (A == 0.0 ? 0.0 : q / A);

	const double x0 = (A + B) - a;
	const double x2 = 0.5 * sqrt(3.0) * (A - B);

	// 1 real root
	if (fabs(x2) >= eps) return x0;

	// 1 unique real root + 1 double real root
	const double x1 = -0.5 * (A + B) - a;

	return fabs(x0) > fabs(x1) ? x0 : x1;
}

double quartic_greatest_root(double a, double b, double c, double d) {
	constexpr double eps = 1e-12;
	double a3 = -b;
	double b3 = a * c - 4.0 * d;
	double c3 = -a * a * d - c * c + 4.0 * b * d;

	// solve cubic resolvent maximal real root
	// y^3 − b*y^2 + (ac−4d)*y − a^2*d−c^2+4*b*d = 0
	const double y = solveP3(a3, b3, c3);

	double q1, q2, p1, p2, D, sqD;

	D = y * y - 4.0 * d;

	if (fabs(D) < eps) {
		q1 = q2 = y * 0.5;

		D = a * a - 4.0 * (b - y);

		if (fabs(D) < eps) {
			p1 = p2 = a * 0.5;
		}
		else {
			sqD = sqrt(D);
			p1 = (a + sqD) * 0.5;
			p2 = (a - sqD) * 0.5;
		}
	}
	else {
		sqD = sqrt(D);
		q1 = (y + sqD) * 0.5;
		q2 = (y - sqD) * 0.5;
		p1 = (a * q1 - c) / (q1 - q2);
		p2 = (c - a * q2) / (q1 - q2);
	}

	double root = std::numeric_limits<double>::lowest();

	//solving the greatest real root of the separated quadratics
	D = p1 * p1 - 4.0 * q1;

	if (D >= 0.0) {
		sqD = sqrt(D);
		root = (sqD - p1) * 0.5;
	}

	D = p2 * p2 - 4.0 * q2;

	if (D >= 0.0) {
		sqD = sqrt(D);
		root = std::max(root, (sqD - p2) * 0.5);
	}

	return root;
}

#endif // QUARTIC_H
