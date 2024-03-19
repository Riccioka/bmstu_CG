#include "sun.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <thread>
#include <chrono>

using namespace std;
#include <unistd.h>
#include <QPixmap>
#include <sys/time.h>

point *sun::get_lights()
{
    return lights;
}

void sun::calc_lights()
{
    int size = get_edges_num();
    lights = new point[size];
    edge *edges = get_edges();
    point *points = get_points();
    point a, b, c, mid;
    for (int i = 0; i < size; i++)
    {
        a = points[edges[i].get_p1()];
        b = points[edges[i].get_p2()];
        c = points[edges[i].get_p3()];

        mid.set_x((a.get_x() + b.get_x() + c.get_x()) / 3);
        mid.set_y((a.get_y() + b.get_y() + c.get_y()) / 3);
        mid.set_z((a.get_z() + b.get_z() + c.get_z()) / 3);

        lights[i] = mid;
//        printf("%f, %f, %f = \n", (a.get_x() + b.get_x() + c.get_x()) / 3, (a.get_y() + b.get_y() + c.get_y()) / 3, (a.get_z() + b.get_z() + c.get_z()) / 3);
    }
}
