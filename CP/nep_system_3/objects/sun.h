#ifndef STAR_H
#define STAR_H

#include "sphere.h"

class sun : public sphere
{
public:
    sun(point center, int radius, int net_nodes_num) : sphere(center, radius, net_nodes_num){calc_lights();}

    point *get_lights();

private:
    point *lights;
    void calc_lights();
};

#endif // STAR_H
