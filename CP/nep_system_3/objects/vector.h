#ifndef VECTOR_H
#define VECTOR_H

#include "point.h"

class vector : public point
{
public:
    vector() : p(0, 0, 0) {}
    vector(point p1, point p2);

    void set(point p1, point p2);
    point get();
    double get_len();

    void neg();

    vector vect_prod(vector a);
    double scalar_prod(vector v);

private:
    point p;
};

#endif // VECTOR_H
