#ifndef SPHERE_H
#define SPHERE_H

#include "point.h"
#include "edge.h"

class sphere
{
public:
    sphere(point center, int radius, int net_nodes_num = 0);
    ~sphere();
    point get_center();
    int get_radius();
    int get_net_nodes_param();

    edge *get_edges();
    point *get_points();

    int get_points_num();
    int get_edges_num();

    void set_center(point c);

    void rotate_x(int degree, point c);
    void rotate_y(int degree, point c);
    void rotate_z(int degree, point c);

    void scale(double kx, double ky, double kz, point c);
    void move(double dx, double dy, double dz);

private:
    point center;
    int radius;
    int net_nodes_param;
    edge *edges;
    point *points;

    void create_net();
    void create_points();
    void create_edges();
};

#endif // SPHERE_H
