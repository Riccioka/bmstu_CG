#include "point.h"
#include <iostream>

point point::scale(double kx, double ky, double kz,  double xc, double yc, double zc)
{
    x = (x - xc) * kx + xc;
    y = (y - yc) * ky + yc;
    z = (z - zc) * kz + zc;
    return *this;
}

point point::scale(double kx, double ky, double kz, point center)
{
    x = (x - center.get_x()) * kx + center.get_x();
    y = (y - center.get_y()) * ky + center.get_y();
    z = (z - center.get_z()) * kz + center.get_z();
    return *this;
}

point point::scale(double k, point center)
{
    x = (x - center.get_x()) * k + center.get_x();
    y = (y - center.get_y()) * k + center.get_y();
    z = (z - center.get_z()) * k + center.get_z();
    return *this;
}

point point::move(double dx, double dy, double dz)
{
    this->x += dx;
    this->y += dy;
    this->z += dz;
    return *this;
}

point point::rotate_x(int degree, point center)
{
    double radians = degree * M_PI / 180;
    move(-center.x, -center.y, -center.z);
    double temp = y;
    y = cos(radians) * y - sin(radians) * z;
    z = cos(radians) * z + sin(radians) * temp;
    move(center.x, center.y, center.z);
    return *this;
}

point point::rotate_y(int degree, point center)
{
    double radians = degree * M_PI / 180;
    move(-center.x, -center.y, -center.z);
    double temp = x;
    x = cos(radians) * x - sin(radians) * z;
    z = cos(radians) * z + sin(radians) * temp;
    move(center.x, center.y, center.z);
    return *this;
}

point point::rotate_z(int degree, point center)
{
    double radians = degree * M_PI / 180;
    move(-center.x, -center.y, -center.z);
    double temp = x;
    x = cos(radians) * x - sin(radians) * y;
    y = cos(radians) * y + sin(radians) * temp;
    move(center.x, center.y, center.z);
    return *this;
}

bool point::operator ==(point p) const
{
    return p.get_x() == x && p.get_y() == y && p.get_z() == z;
}

double point::get_x()
{
    return x;
}

double point::get_y()
{
    return y;
}

double point::get_z()
{
    return z;
}

void point::set_x(double x)
{
    this->x = x;
}

void point::set_y(double y)
{
    this->y = y;
}

void point::set_z(double z)
{
    this->z = z;
}

double point::get_dist(point p)
{
    return sqrt((x - p.get_x()) * (x - p.get_x()) + (y - p.get_y()) * (y - p.get_y()) + (z - p.get_z()) * (z - p.get_z()));
}

point point::get()
{
    return *this;
}
