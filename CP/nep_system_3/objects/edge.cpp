#include "edge.h"

#include <iostream>

edge::edge(int p1, int p2, int p3)
{
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
}

void edge::append(int p)
{
    switch(i)
    {
        case 0:
            this->p1 = p;
        case 1:
            this->p2 = p;
        case 2:
            this->p3 = p;
    }
    i++;
}

int edge::get_p1()
{
    return p1;
}


int edge::get_p2()
{
    return p2;
}


int edge::get_p3()
{
    return p3;
}

void edge::set_p1(int p1)
{
    this->p1 = p1;
}

void edge::set_p2(int p2)
{
    this->p2 = p2;
}

void edge::set_p3(int p3)
{
    this->p3 = p3;
}
