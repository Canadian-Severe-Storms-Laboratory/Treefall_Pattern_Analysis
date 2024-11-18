#include "pch.h"
#include "BurgersRottAxiVortex.h"

const std::array<const double, 2> BurgersRottAxiVortex::computeVec(const double x, const double y, const double Vr, const double Vt, const double Vs, const double Rmax, const double Phi)
{
	const double r2 = x*x + y*y;

	const double R2 = Rmax * Rmax;

	const double s = Rmax * (k1 * R2 + k3 * r2) / (R2 * (k0 * R2 + k2 * r2) + k4 * r2 * r2);

	return {-s * (x*Vr + y*Vt), s * (x*Vt - y*Vr) + Vs};
}

const std::array<const double, 3> BurgersRottAxiVortex::getAxesOfInterest(const double Vr, const double Vt, const double Vs, const double Vc, const double Rmax, const double Phi)
{
	int idx = ((int)Vr - 20) * VtRange * VsRange * VcRange * 3 + ((int)Vt - 1)  * VsRange * VcRange * 3 + ((int)Vs - 1) * VcRange * 3 + ((int)Vc - 35) * 3;

	if (axesOfInterest != nullptr) {
		return { (double)axesOfInterest[idx] * Rmax, (double)axesOfInterest[idx + 1] * Rmax, (double)axesOfInterest[idx + 2] * Rmax };
	}

	std::ifstream binaryFile(precompFile, std::ios::in | std::ios::binary);

	std::array<float, 3> w{};

	if (binaryFile.is_open()) {
		binaryFile.seekg(idx * sizeof(float), std::ios::cur);
		binaryFile.read(reinterpret_cast<char*>(w.data()), 3 * sizeof(float));
	}

	binaryFile.close();

	return { (double)w[0] * Rmax, (double)w[1] * Rmax, (double)w[2] * Rmax };
}

const double BurgersRottAxiVortex::solvePatternLocation(const double xi, const double Vr, const double Vt, const double Vs, const double Vc, const double Rmax, const double Phi)
{
	constexpr double R = 1.0;
	const double x = xi / Rmax;

	const double C0 = Vc * Vc;
	const double C1 = Vs * Vs;
	const double C2 = C0 - C1;
	const double C3 = R * R;
	const double C4 = C3 * k1;
	const double C5 = x * x;
	const double C6 = C5 * k3;
	const double C7 = C4 + C6;
	const double C8 = R * Vs;
	const double C9 = C8 * Vt;
	const double C10 = C7 * C9;
	const double C11 = C3 * k2;
	const double C12 = C5 * k4;
	const double C13 = C3 * C3;
	const double C14 = C13 * k0;
	const double C15 = 2.0 * C14;
	const double C16 = x * C5;
	const double C17 = k4 * k4;
	const double C18 = C17 * C2;
	const double C19 = k2 * k2;
	const double C20 = C16 * C16;
	const double C21 = 2.0 * k4;
	const double C22 = C16 * C21;
	const double C23 = Vr * Vr;
	const double C24 = Vt * Vt;
	const double C25 = C3 * (C23 + C24);
	const double C26 = C2 * k4;
	const double C27 = 2.0 * C11;
	const double C28 = C27 * x;
	const double C29 = C5 * C5;
	const double C30 = C29 * k4;
	const double C31 = C11 * C5;
	const double C32 = 2.0 * C8 * Vr;
	const double C33 = C1 * C17;
	const double C34 = C13 * C23 * k1;
	const double C35 = k3 * k3;
	const double C36 = 3.0 * C23 * C3 * C35;
	const double C37 = 3.0 * C6;
	const double C38 = C37 + 2.0 * C4;
	const double C39 = C37 + C4;
	const double C40 = C24 * C3;
	const double C41 = C9 * k3;
	const double C42 = C41 * x;
	const double C43 = C11 + 2.0 * C12;
	const double C44 = C39 * k4;

	//---------------------------------------------------------
	const double a0 = C15 * x * (C2 * x * (C11 + C12) - C10) + C2 * C13 * C13 * k0 * k0 - C5 * (C10 * C22 - C13 * C19 * C2 * C5 - C20 * C18 + C25 * C7 * C7 + C28 * (C10 - C16 * C26));
	const double a1 = C32 * C7 * (C14 + C30 + C31);
	const double a2 = 4.0 * C0 * C20 * C17 + 2.0 * C13 * C19 * C2 * C5 - C15 * (-C2 * C43 + C42) - 4.0 * C20 * C33 - C22 * C38 * C9 - C23 * C13 * C3 * k1 * k1 - C29 * C36 + 2.0 * C3 * k2 * x * (3.0 * C2 * C16 * k4 - C9 * (C4 + 2.0 * C6)) - 4.0 * C34 * C6 - C39 * C40 * C7;
	const double a3 = C32 * (C4 * C43 + k3 * (C14 + 3.0 * C30 + 2.0 * C31));
	const double a4 = 6.0 * C0 * C17 * C29 + C13 * C19 * C2 + 2.0 * C13 * C2 * k0 * k4 - C28 * (C41 - 3.0 * C26 * x) - 6.0 * C29 * C33 - 2.0 * C34 * k3 - C36 * C5 - C38 * C40 * k3 - 2.0 * C44 * C9 * x;
	const double a5 = C32 * (C11 * k3 + C44);
	const double a6 = 4.0 * C18 * C5 - C21 * C42 - C25 * C35 + C26 * C27;
	const double a7 = C32 * k3 * k4;
	const double a8 = C18;

	//y -> -y
	std::array<const double, 9> coeffs = { a0, -a1, a2, -a3, a4, -a5, a6, -a7, a8 };

	double root;

	cy::PolynomialFirstRoot<8, double>(root, coeffs.data(), -4.0, 12.0);

	//-y -> y
	return root * -Rmax;

	/*std::array<double, 8> roots{};

	const int rootsFound = cy::PolynomialRoots<8, double>(roots.data(), coeffs.data(), -12.0 * Rmax, 4.0 * Rmax);

	return *std::max_element(roots.data(), roots.data() + rootsFound);*/
}
