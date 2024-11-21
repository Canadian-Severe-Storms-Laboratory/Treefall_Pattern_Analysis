#pragma once
#include <algorithm>

#include "VortexModel.h"
#include "cyPolynomial.h"
#include "SplineTable.h"

EXPORT class LinearPiecewiseVortex : public VortexModel {

private:
    SplineTable splineTable;

    std::array<double, 7> solveCoeffs(const double x, const LineNode& line) const {
        //normalized verison
        constexpr double R = 1.0;
        constexpr double Vc = 1.0;

        const auto& [maxR, ar, br, at, bt] = line;

        const double x0 = x * x;
        const double x16 = Vr * Vr;
        const double x8 = Vt * Vt;
        const double x12 = Vs * Vs;
        const double x23 = Vc * Vc;
        const double x22 = R * R;
        const double x20 = ar * ar;
        const double x17 = br * br;
        const double x27 = at * at;
        const double x9 = bt * bt;
        const double x1 = x0 * x0;
        const double x2 = x16 * x16;
        const double x3 = x2 * x20 * x20;
        const double x4 = x1 * x3;
        const double x5 = x8 * x8;
        const double x6 = x5 * x27 * x27;
        const double x7 = x1 * x6;
        const double x10 = x8 * x9;
        const double x11 = x22 * x22;
        const double x13 = 2.0 * x12;
        const double x14 = x11 * x13;
        const double x15 = x10 * x14;
        const double x18 = x16 * x17;
        const double x19 = x14 * x18;
        const double x21 = x16 * x20;
        const double x24 = 2.0 * x23;
        const double x25 = x22 * x24;
        const double x26 = x21 * x25;
        const double x28 = x27 * x8;
        const double x29 = x25 * x28;
        const double x30 = x17 * x2 * x20;
        const double x31 = 2.0 * x0;
        const double x32 = x22 * x31;
        const double x33 = x27 * x5 * x9;
        const double x34 = x21 * x22;
        const double x35 = x12 * x34;
        const double x36 = x21 * x28;
        const double x37 = 2.0 * x36;
        const double x38 = x0 * x;
        const double x39 = R * Vs;
        const double x40 = 4.0 * x39;
        const double x41 = x38 * x40;
        const double x42 = x8 * Vt;
        const double x43 = x42 * x27 * at;
        const double x44 = 6.0 * x12;
        const double x45 = x22 * x28;
        const double x46 = x0 * x45;
        const double x47 = Vt * at;
        const double x48 = x21 * x47;
        const double x49 = 8.0 * x0;
        const double x50 = ar * x22;
        const double x51 = br * bt;
        const double x52 = at * x50 * x51 * x8;
        const double x53 = x16 * x52;
        const double x54 = x11 * x24;
        const double x55 = x22 * R;
        const double x56 = Vt * x55;
        const double x57 = at * x;
        const double x58 = 4.0 * x57;
        const double x59 = x56 * x58;
        const double x60 = Vs * x59;
        const double x61 = 8.0 * x16;
        const double x62 = -Vs * ar * x * x51 * x56 * x61 - Vs * x42 * x55 * x58 * x9 + 2.0 * x10 * x11 * x18 - x10 * x54 + x11 * x2 * x17 * x17 + x11 * x5 * x9 * x9 + x11 * x23 * x23 + x11 * x12 * x12 - x12 * x54 - x18 * x54 + x18 * x60 - x23 * x60 + x59 * x12 * Vs;
        const double x63 = x12 * x50;
        const double x64 = x16 * x20 * ar;
        const double x65 = ar * x39;
        const double x66 = ar * x28;
        const double x67 = x10 * x50;
        const double x68 = Vr * x40;
        const double x69 = 4.0 * x23;
        const double x70 = x0 * x34;
        const double x71 = 4.0 * x0 * x22;
        const double x72 = 6.0 * x36;
        const double x73 = 8.0 * x38 * x39;
        const double x74 = Vt * x57;
        const double x75 = 3.0 * x0;
        const double x76 = 2.0 * x22;
        const double a0 = x0 * (-x0 * x26 - x0 * x29 + x1 * x37 + x10 * x31 * x34 - x15 + x18 * x31 * x45 + x19 - x30 * x32 + x31 * x35 - x32 * x33 + x4 + x41 * x43 + x41 * x48 + x44 * x46 - x49 * x53 + x62 + x7);
        const double a1 = x * x68 * (2.0 * Vs * Vt * br * bt * x55 + ar * x * x16 * x17 * x22 + ar * x * x22 * x23 + 2.0 * at * br * bt * x * x22 * x8 - x * x63 - x * x67 - x31 * x47 * x65 - x38 * x64 - x38 * x66);
        const double a2 = -16.0 * x0 * x53 + x1 * x72 + 4.0 * x10 * x70 + 8.0 * x12 * x46 + x15 + 4.0 * x18 * x46 - x19 - x30 * x71 - x33 * x71 + x35 * x49 + 3.0 * x4 + x43 * x73 - x46 * x69 + x48 * x73 + x62 - x69 * x70 + 3.0 * x7;
        const double a3 = x68 * (ar * x16 * x17 * x22 + ar * x22 * x23 + 2.0 * at * br * bt * x22 * x8 - x31 * x64 - x31 * x66 - x63 - 2.0 * x65 * x74 - x67);
        const double a4 = x * x40 * x43 + x0 * x72 + x10 * x21 * x76 + x13 * x45 + x18 * x28 * x76 + x21 * x40 * x74 - x26 - x29 + x3 * x75 - x30 * x76 - x33 * x76 + x34 * x44 - x52 * x61 + x6 * x75;
        const double a5 = -ar * x68 * (x21 + x28);
        const double a6 = x3 + x37 + x6;

        return { a0, a1, a2, a3, a4, a5, a6 };
    }

    std::array<double, 2> b0Roots(const double x, const LineNode& line) const {

        //normalized verison
        constexpr double R = 1.0;
        constexpr double Vc = 1.0;

        const double ar = line.ar;
        const double at = line.at;

        const double ar2 = ar * ar;
        const double at2 = at * at;
        const double Vr2 = Vr * Vr;
        const double Vt2 = Vt * Vt;
        const double ar2Vr2 = ar2 * Vr2;
        const double at2Vt2 = at2 * Vt2;
        const double s = 2.0 * R * Vs;

        const double A = ar2Vr2 + at2Vt2;
        const double B = -s * ar * Vr;
        const double C = R * R * (Vs * Vs - Vc * Vc) + x * (x * (ar2Vr2 + at2Vt2) + s * at * Vt);

        const double d = B * B - 4.0 * A * C;

        if (d < 0) return { -1.7976931348623157E+308, -1.7976931348623157E+308 };

        const double k = B + (B / fabs(B)) * sqrt(d);

        return { fabs(k) < 1e-10 ? -1.7976931348623157E+308 : -2.0 * C / k, fabs(A) < 1e-10 ? -1.7976931348623157E+308 : -k / (2.0 * A) };
    }

    double mag(const double x, const double y, const LineNode& line) {
        const double r = hypot(x, y);

        const double Pr = Vr * (line.ar * r + line.br);
        const double Pt = Vt * (line.at * r + line.bt);

        const double vx = (Pr * x + Pt * y) / r;
        const double vy = (Pt * x - Pr * y) / r;

        return hypot(vx, vy + Vs);
    }

    bool isValidRoot(double x, double root, double minY, double maxY, const LineNode& line) {
        const double y = fabs(root);

        // root is valid for line interval
        if (maxY < y || y < minY) return false;

        const double m = mag(x, root, line);

        // mag = Vc at solution location
        if (fabs(1.0 - m) > 1e-5) return false;

        return true;
    }

public:

    LinearPiecewiseVortex() : VortexModel(-1, -1, -1) {};

    LinearPiecewiseVortex(SplineTable splineTable_, double Vs, double Vr, double Vt) : splineTable(splineTable_), VortexModel(Vs, Vr, Vt) {};

    LinearPiecewiseVortex(std::vector<double> vrLineData, std::vector<double> vtLineData) : VortexModel(-1, -1, -1) {
        std::vector<std::array<double, 4>> lineData;

        for (int i = 0; i < vrLineData.size(); i += 3) {
            lineData.push_back({ vrLineData[i], vrLineData[i + 1], vrLineData[i + 2], 1.0 });
        }

        for (int i = 0; i < vtLineData.size(); i += 3) {
            lineData.push_back({ vtLineData[i], vtLineData[i + 1], vtLineData[i + 2], -1.0 });
        }

        //sort intervals
        std::sort(lineData.begin(), lineData.end(), [](const std::array<double, 4>& a, const std::array<double, 4>& b) {
            return a[0] < b[0];
        });

        const int n = lineData.size();
        splineTable.reserve(n + 1);

        //construct spline table based on differing intervals between Vr and Vt
        for (int i = 0; i < n; i++) {

            //ignore intervals that are too small / non-existent
            if (i < n - 1 && lineData[i + 1][0] - lineData[i][0] < 1e-3) continue;

            //get current interval's line (Vr or Vt)
            const int type = lineData[i][3];
            const double maxR = lineData[i][0];
            const double a1 = lineData[i][1];
            const double b1 = lineData[i][2];
            double a2 = 0.0;
            double b2 = 0.0;

            //find other line (Vt or Vr) contained in the same interval
            for (int j = 0; j < n; j++) {
                if (lineData[j][3] == type || lineData[j][0] < maxR) continue;

                a2 = lineData[j][1];
                b2 = lineData[j][2];
                break;
            }

            //add line to spline table
            if (type > 0.0) {
                splineTable.emplace_back(maxR, a1, b1, a2, b2);
            }
            else {
                splineTable.emplace_back(maxR, a2, b2, a1, b1);
            }
        }

        splineTable.emplace_back(1e100, 0.0, 0.0, 0.0, 0.0);
    }

    Vec2 vecAt(double x, double y) {
        const double r = hypot(x, y);
        const LineNode& node = splineTable[r];

        const double Pr = Vr * (node.ar * r + node.br);
        const double Pt = Vt * (node.at * r + node.bt);

        const double vx = (Pr * x + Pt * y) / r;
        const double vy = (Pt * x - Pr * y) / r;

        return { -vx, vy + Vs };
    }

    double patternLocation(double x) override {
        constexpr double eps = 1e-5;
        double greatestY = -1.7976931348623157E+308;

        for (int i = 0; i < splineTable.size() - 1; i++) {

            const LineNode& line = splineTable[i];

            if (x > line.maxR) continue;

            const bool b0 = (fabs(line.br) < eps && fabs(line.bt) < eps);

            const double minR = b0 ? 0.0 : splineTable[i - 1].maxR;

            const double dmin = std::max(0.0, minR * minR - x * x);
            const double dmax = line.maxR * line.maxR - x * x;

            const double minY = sqrt(dmin) - eps;
            const double maxY = sqrt(dmax) + eps;

            std::array<double, 6> roots{};
            int numRoots;

            if (b0) {
                const auto qr = b0Roots(x, line);
                roots[0] = qr[0];
                roots[1] = qr[1];
                numRoots = 2;
            }
            else {
                auto coeffs = solveCoeffs(x, line);
                numRoots = cy::PolynomialRoots<6, double>(roots.data(), coeffs.data());
            }

            for (int j = numRoots - 1; j >= 0; j--) {
                const double y = fabs(roots[j]);

                if (y <= maxY && y >= minY) {

                    const double m = mag(x, roots[j], line);

                    if (fabs(1.0 - m) < eps) {
                        greatestY = std::max(greatestY, roots[j]);
                        break;
                    }
                }
            }

            /*for (int j = numRoots - 1; j >= 0; j--) {
                
                if (!isValidRoot(x, roots[j], minY, maxY, line)) continue;

                greatestY = max(greatestY, roots[j]);
                break;
            }*/
        }

        return greatestY;
    }

    bool patternLocationExists(double x) override {
		return patternLocation(x) > -1E300;
    }

};