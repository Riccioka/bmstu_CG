#include "vector.h"
#include <iostream>

vector::vector(point p1, point p2)
{
    p.set_x(p2.get_x() - p1.get_x());
    p.set_y(p2.get_y() - p1.get_y());
    p.set_z(p2.get_z() - p1.get_z());
}

void vector::set(point p1, point p2)
{
    p.set_x(p2.get_x() - p1.get_x());
    p.set_y(p2.get_y() - p1.get_y());
    p.set_z(p2.get_z() - p1.get_z());
}

point vector::get()
{
    return p;
}

double vector::get_len()
{
    double x = p.get_x(), y = p.get_y(), z = p.get_z();
    return sqrt(x * x + y * y + z * z);
}

void vector::neg()
{
    p.set_x(-p.get_x());
    p.set_y(-p.get_y());
    p.set_z(-p.get_z());
}

vector vector::vect_prod(vector a)
{
    vector res;
    res.p.set_x(p.get_y() * a.p.get_z() - p.get_z() * a.p.get_y());
    res.p.set_y(p.get_z() * a.p.get_x() - p.get_x() * a.p.get_z());
    res.p.set_z(p.get_x() * a.p.get_y() - p.get_y() * a.p.get_x());
    return res;
}

double vector::scalar_prod(vector v)
{
    return p.get_x() * v.p.get_x() + p.get_y() * v.p.get_y() + p.get_z() * v.p.get_z();
}
