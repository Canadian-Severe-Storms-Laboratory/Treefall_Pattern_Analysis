#pragma once
#include <string>
#include <vector>
#include <array>
#include "CPP_CS_Interop.h"

constexpr double PI = 3.14159265358979323846;

EXPORT struct Rect {
    double x;
    double y;
    double w;
    double h;
};

EXPORT class Transect
{
public:
	double x, y, theta, lengthAbove, lengthBelow, width, positionOffset, heightOffset, angleOffset, spacing;
    std::string name = "";

    Transect(double x, double y, double lengthAbove, double lengthBelow, double width)
    {
        this->x = x;
        this->y = y;
        this->lengthAbove = lengthAbove;
        this->lengthBelow = lengthBelow;
        this->width = width;
        heightOffset = 0;
        angleOffset = 0;
        positionOffset = 0;
        theta = 0;
        spacing = 0;
    }

    Transect(std::string name)
    {
        this->name = name;
        x = 0;
        y = 0;
        lengthAbove = -1;
        lengthBelow = -1;
        width = -1;
        heightOffset = 0;
        angleOffset = 0;
        positionOffset = 0;
		theta = 0;
        spacing = 0;
    }

    Transect() {
        x = 0;
        y = 0;
        lengthAbove = -1;
        lengthBelow = -1;
        width = -1;
        heightOffset = 0;
        angleOffset = 0;
        positionOffset = 0;
		theta = 0;
        spacing = 0;
    }

    void setPerpendicularAngle(double a)
    {
        //convert to 0 - 2PI
        a = a < 0 ? 2.0 * PI - a : a;

        //rotate by 90 degrees
        a = (3.0 * PI / 2.0 > a && a > PI / 2.0) ? a - PI / 2.0 : a + PI / 2.0;

        a += (PI / 180.0) * angleOffset;

        //clamp to 0 - 2PI
        a = a > 2.0 * PI ? a - 2.0 * PI : a;

        theta = a;
    }

    std::array<double, 2> center() {
        double xc = x + cos(theta) * heightOffset;
		double yc = y + sin(theta) * heightOffset;

        return { xc, yc };
    }

    Rect normRect() {
        return { -width, -lengthBelow, width * 2.0, lengthAbove + lengthBelow };
    }

    std::vector<double> extent()
	{
        std::vector<double> result;

        std::array<double, 2> pv = { cos(theta), sin(theta) };
        std::array<double, 2> tv = { pv[1], -pv[0] };

        std::array<double, 4> xs;
        std::array<double, 4> ys;

        xs[0] = x + pv[0] * (heightOffset + lengthAbove) + tv[0] * width;
        xs[1] = x + pv[0] * (heightOffset + lengthAbove) - tv[0] * width;
        xs[2] = x + pv[0] * (heightOffset - lengthBelow) - tv[0] * width;
        xs[3] = x + pv[0] * (heightOffset - lengthBelow) + tv[0] * width;

        ys[0] = y + pv[1] * (heightOffset + lengthAbove) + tv[1] * width;
        ys[1] = y + pv[1] * (heightOffset + lengthAbove) - tv[1] * width;
        ys[2] = y + pv[1] * (heightOffset - lengthBelow) - tv[1] * width;
        ys[3] = y + pv[1] * (heightOffset - lengthBelow) + tv[1] * width;

        result.push_back(std::min(std::min(std::min(xs[0], xs[1]), xs[2]), xs[3]));
		result.push_back(std::min(std::min(std::min(ys[0], ys[1]), ys[2]), ys[3]));
		result.push_back(std::max(std::max(std::max(xs[0], xs[1]), xs[2]), xs[3]));
		result.push_back(std::max(std::max(std::max(ys[0], ys[1]), ys[2]), ys[3]));

		return result;
	}

private:


};