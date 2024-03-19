#ifndef EDGE_H
#define EDGE_H

#include "point.h"

class edge
{
public:
    edge(int p1 = 0, int p2 = 0, int p3 = 0);
    void append(int p);
    int get_p1();
    int get_p2();
    int get_p3();

    void set_p1(int p1);
    void set_p2(int p2);
    void set_p3(int p3);

private:
    int p1;
    int p2;
    int p3;

    int i = 0;
};

#endif // EDGES_H
