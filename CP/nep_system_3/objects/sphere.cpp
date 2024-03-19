#include "sphere.h"
#include <iostream>

sphere::sphere(point center, int radius, int net_nodes_num)
{
    this->center = center;
    this->radius = radius;
    this->net_nodes_param = net_nodes_num;
    create_net();
}

void sphere::create_net()
{
    create_points();
    create_edges();
}

void sphere::create_points()
{
    points = new point[get_points_num()];
    int k = 0;
    int r = radius;
    int cx = center.get_x(); int cy = center.get_y(); int cz = center.get_z();

    points[k++] = point(cx - r, cy, cz);

    int step_degree = 90 / (net_nodes_param + 1);
    double radians, a, b, c;
    for (int i = 1; i <= net_nodes_param; i++)
    {
        radians = (90 - step_degree * i) * M_PI / 180;
        a = sqrt(2 * r * r * (1 - cos(radians)));
        b = sin(radians) * r;
        c = sqrt(a * a - b * b);
        points[k++] = point(cx - b, cy + r - c, cz);
    }
    points[k++] = point(cx, cy + r, cz);

    for (int i = 0; i <= net_nodes_param; i++)
    {
        points[k++] = point(2 * cx - points[net_nodes_param - i].get_x(),
                          points[net_nodes_param - i].get_y(),
                          points[net_nodes_param - i].get_z());
    }
}

void sphere::create_edges()
{
    int n = 3 + net_nodes_param * 2;
    int m = 2 + net_nodes_param * 2 * 2;

    edge cur_edges[m];
    edges = new edge[get_edges_num()];

    int j = 0;
    int top_set[n];
    int bottom_set[n];
    double step_degree = 90 / (net_nodes_param + 1);
    int k = n;
    int cur, right, left;

    for (int i = 0; i < n; i++)
    {
        top_set[i] = i;
    }

    int s = n;

    for (double degree = step_degree; degree <= 360; degree += step_degree)
    {
        for (int i = 1; i < n - 1; i++)
        {
            point temp = points[i];
            points[s++] = temp.rotate_x(degree, center);
        }
        for (int i = 0; i < n - 2; i++)
        {
            bottom_set[i] = k + i;
        }
        for (int i = 0; i < m; i++)
        {
            cur_edges[i] = edge();
        }

        cur_edges[0].append(top_set[0]);
        cur_edges[n - 2].append(top_set[n - 1]);

        for (int i = 1; i < n - 1; i++)
        {
            cur_edges[i - 1].append(top_set[i]);
            cur_edges[i].append(top_set[i]);
        }

        for (int i = 2; i < n - 1; i++)
        {
            cur_edges[n - 1 + i - 2].append(top_set[i]);
        }
        for (int i = 0; i < n - 2; i++)
        {
            cur = (n + i) % (n - 1);
            right = (n - 1) + i;
            left = right - 1;
            if (cur == 1)
            {
                left = 0;
            }
            cur_edges[cur].append(bottom_set[i]);
            cur_edges[left].append(bottom_set[i]);
            if (cur != n - 2)
            {
                cur_edges[right].append(bottom_set[i]);
            }
        }
        for (int i = j; i < j + m; i++)
        {
            edges[i] = cur_edges[i % m];
        }
        j += m;
        top_set[0] = 0;
        top_set[n - 1] = n - 1;
        for (int i = 0; i < n - 2; i++)
        {
            top_set[i + 1] = bottom_set[i];
        }
        k += n - 2;
    }

    for (int i = 0; i < n - 2; i++)
    {
        bottom_set[i] = top_set[i + 1];
    }
    for (int i = 0; i < n; i++)
    {
        top_set[i] = i;
    }
    for (int i = 0; i < m; i++)
    {
        cur_edges[i] = edge();
    }

    cur_edges[0].append(top_set[0]);
    cur_edges[n - 2].append(top_set[n - 1]);

    for (int i = 1; i < n - 1; i++)
    {
        cur_edges[i - 1].append(top_set[i]);
        cur_edges[i].append(top_set[i]);
    }

    for (int i = 2; i < n - 1; i++)
    {
        cur_edges[n - 1 + i - 2].append(top_set[i]);
    }
    for (int i = 0; i < n - 2; i++)
    {
        cur = (n + i) % (n - 1);
        right = (n - 1) + i;
        left = right - 1;
        if (cur == 1)
        {
            left = 0;
        }
        cur_edges[cur].append(bottom_set[i]);
        cur_edges[left].append(bottom_set[i]);
        if (cur != n - 2)
        {
            cur_edges[right].append(bottom_set[i]);
        }
    }
    for (int i = j; i < j + m; i++)
    {
        edges[i] = cur_edges[i % m];
    }

    //std::cout << s << " " << get_points_num() << std::endl;
    //std::cout << j + m << " " << get_edges_num() << std::endl;
}

sphere::~sphere()
{
    delete []edges;
    delete []points;
}

point sphere::get_center()
{
    return this->center;
}

int sphere::get_radius()
{
    return radius;
}

int sphere::get_net_nodes_param()
{
    return net_nodes_param;
}

edge *sphere::get_edges()
{
    return edges;
}

point *sphere::get_points()
{
    return points;
}

int sphere::get_points_num()
{
    int n = net_nodes_param;
    return (3 + n * 2) + (n * 2 + 1) * 2 * (360 / 90 * (n + 1));
}

int sphere::get_edges_num()
{
    int n = net_nodes_param;
    return (n * 2 * 2 + 2) * (360 / 90 * (n + 1) + 2) + (n * 2 * 2 + 2);
}

void sphere::set_center(point c)
{
    center = c;
}

void sphere::rotate_x(int degree, point c)
{
    int n = get_points_num();
    for (int i = 0; i < n; i++)
    {
        points[i].rotate_x(degree, c);
    }
    center.rotate_x(degree, c);
}

void sphere::rotate_y(int degree, point c)
{
    int n = get_points_num();
    for (int i = 0; i < n; i++)
    {
        points[i].rotate_y(degree, c);
    }
    center.rotate_y(degree, c);
}

void sphere::rotate_z(int degree, point c)
{
    int n = get_points_num();
    for (int i = 0; i < n; i++)
    {
        points[i].rotate_z(degree, c);
    }
    center.rotate_z(degree, c);
}

void sphere::scale(double kx, double ky, double kz, point c)
{
    int n = get_points_num();
    for (int i = 0; i < n; i++)
    {
        points[i].scale(kx, ky, kz, c.get_x(), c.get_y(), c.get_z());
    }
    center.scale(kx, ky, kz, c.get_x(), c.get_y(), c.get_z());
}

void sphere::move(double dx, double dy, double dz)
{
    int n = get_points_num();
    for (int i = 0; i < n; i++)
    {
        points[i].move(dx, dy, dz);
    }
    center.move(dx, dy, dz);
}
