#ifndef POINT_H
#define POINT_H

#include <math.h>
#include <cmath>

class point
{
public:
    point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    double get_x();
    double get_y();
    double get_z();

    void set_x(double x);
    void set_y(double y);
    void set_z(double z);

    double get_dist(point p);

    point get();

    point scale(double kx, double ky, double kz, double xc, double yc, double zc);
    point scale(double kx, double ky, double kz, point center);
    point scale(double k, point center);
    point move(double dx, double dy, double dz);
    point rotate_x(int degree, point center);
    point rotate_y(int degree, point center);
    point rotate_z(int degree, point center);

    bool operator == (point p) const;

private:
    double x;
    double y;
    double z;
};

#endif // TYPES_H
