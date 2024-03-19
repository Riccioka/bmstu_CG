#ifndef ASTROOBJECT_H
#define ASTROOBJECT_H

#include "vector.h"
#include "sphere.h"
#include <QColor>

class astroobject : public sphere
{
public:
    astroobject(int dist, int a_xy, int a_yz, point nep_center,
           int radius, int net_nodes_num,
           point *lights,
           QColor surface_color);

    QColor get_surface_color();

    void rotate_x(int degree, point center);
    void rotate_y(int degree, point center);
    void rotate_z(int degree, point center);
    void scale(double kx, double ky, double kz, point c);
    void scale(double k, point c);
    void move(double dx, double dy, double dz);

    void setColor(QColor newColor);

    void set_surface_color(const QColor& new_color) { surface_color = new_color; }
    void setRedSurfaceColor() { set_surface_color(QColor(255, 0, 0)); }



    void rotate_planet(double angle, point nep_center);
    void rotate_astroobject(int degrees);
    void move_astroobject_along_orbit(point nep_center);
//    void move_astroobject_along_orbit(planet &p, int o);

    void move_satellite_along_orbit(double angle, point nep_center);

    int get_orbit_nodes_num();
    int getDist() const;

    int getRotationAngleXY()  ;
    void setRotationAngleXY(int angle) ;

    double *get_intensities();

    point *get_orbit();
    double *intensities = new double[get_edges_num()];

    void recalculateIntensity(point *lights);
    point *recalc_orbit(double dist, point sun_center, double a_xy);


private:
    int dist;
    int a_xy; int a_yz;
    point calc_center(int dist, int a_xy, int a_yz, point nep_center);
    void calc_intensities(point *lights);
//    void calc_intensities_neg(point *lights);
    void calc_orbit(double dist, point nep_center, double a_xy);


    point *orbit;

    QColor surface_color;
};

#endif // ASTROOBJECT_H
