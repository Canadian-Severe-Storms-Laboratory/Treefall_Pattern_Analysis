#pragma once
#include <array>
#include <vector>
#include <algorithm>

struct Vec2 {
	double x;
	double y;

	Vec2() : x(0.0), y(0.0) {};
	Vec2(const double x, const double y) : x(x), y(y) {};
	Vec2(const Vec2& v) : x(v.x), y(v.y) {};
	Vec2(const std::array<double, 2>& v) : x(v[0]), y(v[1]) {};

	Vec2 operator+(const Vec2& v) const {
		return { x + v.x, y + v.y };
	}

	Vec2 operator+(const double s) const {
		return { x + s, y + s };
	}

	Vec2 operator-(const double s) const {
		return { x - s, y - s };
	}

	Vec2 operator-() const {
		return { -x, -y };
	}

	Vec2 operator-(const Vec2& v) const {
		return { x - v.x, y - v.y };
	}

	Vec2 operator*(const double s) const {
		return { x * s, y * s };
	}

	Vec2 operator*(const Vec2& v) const {
		return { x * v.x, y * v.y };
	}

	Vec2 operator/(const double s) const {
		return { x / s, y / s };
	}

	Vec2 operator/(const Vec2& v) const {
		return { x / v.x, y / v.y };
	}

	Vec2& operator+=(const Vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2& operator-=(const Vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vec2& operator*=(const double s) {
		x *= s;
		y *= s;
		return *this;
	}

	Vec2& operator/=(const double s) {
		x /= s;
		y /= s;
		return *this;
	}

	double mag() const {
		return hypot(x, y);
	}

	double magSq() const {
		return x * x + y * y;
	}

	Vec2 unit() const {
		const double m = mag();
		return { x / m, y / m };
	}

	void normalize() {
		const double m = mag();
		x /= m;
		y /= m;
	}

	double dot(const Vec2& v) const {
		return x * v.x + y * v.y;
	}

	double cross(const Vec2& v) const {
		return x * v.y - y * v.x;
	}

	Vec2 rot90CCW() const {
		return { -y, x };
	}

	Vec2 rot90CW() const {
		return { y, -x };
	}

	double angle() const {
		return atan2(y, x);
	}

	double angle2PI() const {
		const double angle = atan2(y, x);

		return angle < 0.0 ? angle + 6.2831853071796 : angle;
	}

	double angle360() const {
		double angle = atan2(y, x);
		angle = angle < 0.0 ? angle + 6.2831853071796 : angle;

		return 180.0 * angle / 3.141592653589793;
	}

	double angleBetween(const Vec2& v) const {
		return acos(std::clamp(dot(v) / (mag() * v.mag()), -1.0, 1.0));
	}

	double angleBetweenUnit(const Vec2& v) const {
		return acos(std::clamp(x * v.x + y * v.y, -1.0, 1.0));
	}

	bool isBetween(const Vec2& a, const Vec2& b) const {
		const double cab = a.cross(b);

		return a.cross(*this) * cab >= 0 && b.cross(*this) * cab <= 0;
	}

	bool operator==(const Vec2& v) const {
		return x == v.x && y == v.y;
	}

	bool operator!=(const Vec2& v) const {
		return x != v.x || y != v.y;
	}

	Vec2& operator=(const Vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	Vec2& operator=(const std::array<double, 2>& v) {
		x = v[0];
		y = v[1];
		return *this;
	}

	Vec2& operator=(const double s) {
		x = s;
		y = s;
		return *this;
	}

	static Vec2 median(std::vector<Vec2>& vecs) {

		const int m = vecs.size() / 2;

		const auto compX = [](const Vec2 a, const Vec2 b) { return a.x < b.x; };
		const auto compY = [](const Vec2 a, const Vec2 b) { return a.y < b.y; };

		std::nth_element(vecs.begin(), vecs.begin() + m, vecs.end(), compX);

		double mx1 = vecs[m].x;

		std::nth_element(vecs.begin(), vecs.begin() + m, vecs.end(), compY);

		double my1 = vecs[m].y;

		if ((vecs.size() & 1) == 1) return Vec2(mx1, my1);

		std::nth_element(vecs.begin(), vecs.begin() + m - 1, vecs.end(), compX);

		double mx2 = vecs[m - 1].x;

		std::nth_element(vecs.begin(), vecs.begin() + m - 1, vecs.end(), compY);

		double my2 = vecs[m - 1].y;

		return Vec2(0.5 * (mx1 + mx2), 0.5 * (my1 + my2));
	}

	static Vec2 averageUnit(std::vector<Vec2>& vecs) {
		double sumX = 0.0, sumY = 0.0;
		for (const Vec2& v : vecs) {
			sumX += v.x;
			sumY += v.y;
		}
		return Vec2(sumX, sumY).unit();
	}

};