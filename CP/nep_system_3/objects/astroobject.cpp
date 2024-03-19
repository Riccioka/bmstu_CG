#include "astroobject.h"
#include <iostream>

astroobject::astroobject(int dist, int a_xy, int a_yz, point nep_center,
               int radius, int net_nodes_num,
               point *lights,
               QColor surface_color) : sphere(calc_center(dist, a_xy, a_yz, nep_center), radius, net_nodes_num)
{
    this->a_xy = a_xy;
    this->a_yz = a_yz;
    this->surface_color = surface_color;
    calc_orbit(dist, nep_center, a_xy);
    calc_intensities(lights);
}

int astroobject::getDist() const
{
    return dist;
}

int astroobject::getRotationAngleXY()
{
    return a_xy;
}

void astroobject::setRotationAngleXY(int angle)
{
    a_xy +=angle;
}


point astroobject::calc_center(int dist, int a_xy, int a_yz, point sun_center)
{
    point astroobject_c = sun_center;
    astroobject_c.set_x(sun_center.get_x() + dist);

    astroobject_c.rotate_z(a_xy, sun_center);
    astroobject_c.rotate_y(a_yz, sun_center);

    return astroobject_c;
}

void astroobject::calc_orbit(double dist, point nep_center, double a_xy)
{
    int n = get_net_nodes_param();

    orbit = new point[4 + n * 4];
    orbit[0] = point(nep_center.get_x() - dist, nep_center.get_y(), nep_center.get_z());
    double step = 90 / (n + 1), angle = step;
    for (int i = 1; i <= n; i++)
    {
        double z = sin(angle * M_PI / 180) * dist;
        double g = dist * sqrt(2 * (1 - cos(angle * M_PI / 180)));
        double x = sqrt(g * g - z * z);
        orbit[i] = point(nep_center.get_x() - dist + x, nep_center.get_y(), nep_center.get_z() - z);
        angle += step;
    }
    orbit[1 + n] = point(nep_center.get_x(), nep_center.get_y(), nep_center.get_z() - dist);
    for (int i = 1; i <= n; i++)
    {
        orbit[i + n + 1] = orbit[n + 1 - i];
        orbit[i + n + 1].set_x(2 * nep_center.get_x() - orbit[n + 1 - i].get_x());

        orbit[3 + n * 3 - i] = orbit[n + 1 + i];
        orbit[3 + n * 3 - i].set_z(nep_center.get_z() + (nep_center.get_z() - orbit[n + 1 + i].get_z()));

        orbit[3 + n * 3 + i] = orbit[n + 1 - i];
        orbit[3 + n * 3 + i].set_z(nep_center.get_z() + (nep_center.get_z() - orbit[n + 1 - i].get_z()));
    }
    orbit[2 + 2 * n] = point(nep_center.get_x() + dist, nep_center.get_y(), nep_center.get_z());
    orbit[3 + 3 * n] = point(nep_center.get_x(), nep_center.get_y(), nep_center.get_z() + dist);

    for (int i = 0; i < 4 + 4 * n; i++)
    {
        orbit[i].rotate_z(a_xy, nep_center);
//        printf("orbit rotate %d : x = %d, y = %d, z = %d\n", i, orbit[i].get_x(), orbit[i].get_y(), orbit[i].get_z());
    }
}

point* astroobject::recalc_orbit(double dist, point sun_center, double a_xy)
{
//    calc_orbit(dist, sun_center, a_xy);


        int n = get_net_nodes_param();
        point *new_orbit = new point[4 + n * 4];
    //    orbit = new point[4 + n * 4];
        new_orbit[0] = point(sun_center.get_x() - dist, sun_center.get_y(), sun_center.get_z());
        double step = 90 / (n + 1), angle = step;
        for (int i = 1; i <= n; i++)
        {
            double z = sin(angle * M_PI / 180) * dist;
            double g = dist * sqrt(2 * (1 - cos(angle * M_PI / 180)));
            double x = sqrt(g * g - z * z);
            new_orbit[i] = point(sun_center.get_x() - dist + x, sun_center.get_y(), sun_center.get_z() - z);
            angle += step;
        }
        orbit[1 + n] = point(sun_center.get_x(), sun_center.get_y(), sun_center.get_z() - dist);
        for (int i = 1; i <= n; i++)
        {
            new_orbit[i + n + 1] = new_orbit[n + 1 - i];
            new_orbit[i + n + 1].set_x(2 * sun_center.get_x() - new_orbit[n + 1 - i].get_x());

            new_orbit[3 + n * 3 - i] = new_orbit[n + 1 + i];
            new_orbit[3 + n * 3 - i].set_z(sun_center.get_z() + (sun_center.get_z() - new_orbit[n + 1 + i].get_z()));

            new_orbit[3 + n * 3 + i] = new_orbit[n + 1 - i];
            new_orbit[3 + n * 3 + i].set_z(sun_center.get_z() + (sun_center.get_z() - new_orbit[n + 1 - i].get_z()));
        }
        new_orbit[2 + 2 * n] = point(sun_center.get_x() + dist, sun_center.get_y(), sun_center.get_z());
        new_orbit[3 + 3 * n] = point(sun_center.get_x(), sun_center.get_y(), sun_center.get_z() + dist);


        for (int i = 0; i < 4 + 4 * n; i++)
        {
            new_orbit[i].rotate_z(a_xy, sun_center);
    //        printf("orbit rotate %d : x = %f, y = %f, z = %f\n", i, new_orbit[i].get_x(), new_orbit[i].get_y(), new_orbit[i].get_z());
        }
        return new_orbit;
}

void astroobject::rotate_planet(double angle, point nep_center)
{
    int axy = getRotationAngleXY();
    setRotationAngleXY(axy + angle);

        // Если угол стал больше 360, вернуться в начальное положение
    if (a_xy >= 360.0) {
        a_xy -= 360.0;
    }

//    calc_orbit(dist, nep_center, a_xy);
    // Пересчитать орбиту с новым углом поворота
//    point *new_orbit = recalc_orbit(dist, nep_center, a_xy);

    // Переместить планету по новой орбите

    set_center(calc_center(dist, a_xy, a_yz, nep_center));
//    set_center(orbit[get_orbit_nodes_num() / 4]);
}


void astroobject::calc_intensities(point *lights)
{
    edge *edges = get_edges();
    point *points = get_points();

    vector *normals = new vector[get_edges_num()];
    point a, b, c;
    vector ab, ac, ra;
    int I_0 = 1;
    double cos_lamb, cos_r;
    double len_n, len_a, len_r;
    double sn, sr;

    point center = get_center();

    for (int i = 0; i < get_edges_num(); i++)
    {
        intensities[i] = 0;
        for (int j = 0; j < get_edges_num(); j++)
        {
            a = points[edges[i].get_p1()];
            b = points[edges[i].get_p2()];
            c = points[edges[i].get_p3()];

            ab.set(a, b);
            ac.set(a, c);
            normals[i] = ab.vect_prod(ac);

            ra.set(a, center);
            len_r = ra.get_len();
            sr = normals[i].scalar_prod(ra);
            len_n = normals[i].get_len();
            len_a = a.get_dist(lights[j]);

            cos_r = sr / len_r / len_n;

            if (cos_r > 0)
                normals[i].neg();

            sn = normals[i].scalar_prod(vector(a, lights[j]));
            cos_lamb = sn / len_n / len_a;

            intensities[i] += I_0 * cos_lamb;
        }
    }
    delete [] normals;
}

void astroobject::setColor(QColor newColor)
{
    surface_color = newColor;
}

QColor astroobject::get_surface_color()
{
    return surface_color;
}

void astroobject::rotate_x(int degree, point c)
{
    point *points = get_points();
    point center = get_center();
    for (int i = 0; i < get_points_num(); i++)
        points[i].rotate_x(degree, c);

    for (int i = 0; i < get_orbit_nodes_num(); i++)
        orbit[i].rotate_x(degree, c);

    set_center(center.rotate_x(degree, c));
}

void astroobject::rotate_y(int degree, point c)
{
    point *points = get_points();
    point center = get_center();
    for (int i = 0; i < get_points_num(); i++)
        points[i].rotate_y(degree, c);

    for (int i = 0; i < get_orbit_nodes_num(); i++)
        orbit[i].rotate_y(degree, c);

    set_center(center.rotate_y(degree, c));
}

void astroobject::rotate_z(int degree, point c)
{
    point *points = get_points();
    point center = get_center();

    for (int i = 0; i < get_points_num(); i++)
        points[i].rotate_z(degree, c);

    for (int i = 0; i < get_orbit_nodes_num(); i++)
        orbit[i].rotate_z(degree, c);

    set_center(center.rotate_z(degree, c));
}

void astroobject::scale(double kx, double ky, double kz, point c)
{
    point *points = get_points();
    point center = get_center();

    for (int i = 0; i < get_points_num(); i++)
        points[i].scale(kx, ky, kz, c);

    for (int i = 0; i < get_orbit_nodes_num(); i++)
        orbit[i].scale(kx, ky, kz, c);

    set_center(center.scale(kx, ky, kz, c));
}

void astroobject::scale(double k, point c)
{
    point *points = get_points();
    point center = get_center();

    for (int i = 0; i < get_points_num(); i++)
        points[i].scale(k, c);

    for (int i = 0; i < get_orbit_nodes_num(); i++)
        orbit[i].scale(k, c);

    set_center(center.scale(k, c));
}

void astroobject::move(double dx, double dy, double dz)
{
    point *points = get_points();
    point center = get_center();
    for (int i = 0; i < get_points_num(); i++)
        points[i].move(dx, dy, dz);

    for (int i = 0; i < get_orbit_nodes_num(); i++)
        orbit[i].move(dx, dy, dz);

    set_center(center.move(dx, dy, dz));
}


void astroobject::rotate_astroobject(int degrees)
{
    point center = get_center();

    point *points = get_points();
    for (int i = 0; i < get_points_num(); i++)
    {
        points[i].rotate_y(degrees, center);
    }

    set_center(center.rotate_y(degrees, center));
}

//void astroobject::move_planet_along_orbit(double angle)
//{
//    point center = get_center();
//    for (int i = 0; i < get_orbit_nodes_num(); i++)
//    {
//        orbit[i].rotate_z(angle, get_center());
//    }
//    set_center(center);
//}

//void astroobject::move_planet_along_orbit(planet &p, int i)
//{
////    p.get_orbit();
//    // Вычисляем новые координаты центра планеты
//    double new_x = p13_orbit[i].get_x();
////    double new_y = orbit[i].get_y();
//    double new_y = p13_orbit[i].get_y();
//    double new_z = p13_orbit[i].get_z();
//    // Устанавливаем новые координаты центра планеты

//    set_center(point(new_x, new_y, new_z));

//    printf("\n new_x = %d, new_y = %d, new_z = %d\n", new_x, new_y, new_z);
//}

//void astroobject::move_planet_along_orbit(point sun_center)
//{
//    point c = this->get_center();

//    c.rotate_x(-200, sun_center);
//    c.rotate_y(15, sun_center);
//    c.rotate_x(200, sun_center);

//    // Устанавливаем новые координаты центра планеты
//    set_center(c);
//}

int astroobject::get_orbit_nodes_num()
{
    int n = get_net_nodes_param();
    return 4 + 4 * n;
}

double *astroobject::get_intensities()
{
    return intensities;
}

point *astroobject::get_orbit()
{
    return orbit;
}

void astroobject::move_astroobject_along_orbit(point nep_center)
{

    // Получаем текущий угол орбиты (в радианах)
    double current_angle = atan2(get_center().get_y() - nep_center.get_y(), get_center().get_x() - nep_center.get_x());

    // Вычисляем новый угол с учетом перемещения на заданное расстояние
    double new_angle = current_angle + 1 / 6; // dist = 30, radius = 8

    printf("angle = %f\n", new_angle * (180.0 / M_PI));
    printf("old x = %f, y = %f, z = %f\n", get_center().get_x(), get_center().get_y(), get_center().get_z());

    // Вычисляем новые координаты центра планеты


    double new_x = nep_center.get_x() + (get_center().get_x() - nep_center.get_x()) * cos(new_angle) -
            (get_center().get_y() - nep_center.get_y()) * sin(new_angle);

    double new_y = nep_center.get_y() + (get_center().get_x() - nep_center.get_x()) * sin(new_angle) +
            (get_center().get_y() - nep_center.get_y()) * cos(new_angle);

//    double new_x = nep_center.get_x() + 8 * cos(new_angle);
//    double new_y = nep_center.get_y() + 8 * sin(new_angle);

    double new_z = get_center().get_z();

    printf("new x = %f, y = %f, z = %f\n", new_x, new_y, new_z);
    set_center(point(new_x, new_y, new_z));

}

void astroobject::recalculateIntensity(point *lights)
{
    edge *edges = get_edges();
    point *points = get_points();

    vector *normals = new vector[get_edges_num()];
    point a, b, c;
    vector ab, ac, ra;
    int I_0 = 1;
    double cos_lamb, cos_r;
    double len_n, len_a, len_r;
    double sn, sr;

    point center = get_center();

    for (int i = 0; i < get_edges_num(); i++)
    {
        intensities[i] = 0;
        for (int j = 0; j < get_edges_num(); j++)
        {
            a = points[edges[i].get_p1()];
            b = points[edges[i].get_p2()];
            c = points[edges[i].get_p3()];

            ab.set(a, b);
            ac.set(a, c);
            normals[i] = ab.vect_prod(ac);

            ra.set(a, center);
            len_r = ra.get_len();
            sr = normals[i].scalar_prod(ra);
            len_n = normals[i].get_len();
            len_a = a.get_dist(lights[j]);

            cos_r = sr / len_r / len_n;

//            if (cos_r > 0) {
//                // Поменять направление нормали, если угол между нормалью и вектором к источнику света больше 90 градусов
//                double angle = acos(cos_r) * 180.0 / M_PI;
//                if (angle > 90.0) {
//                    normals[i].neg();
//                }
//            }

            if (cos_r > 0)
                normals[i].neg();

            sn = normals[i].scalar_prod(vector(a, lights[j]));
            cos_lamb = sn / len_n / len_a;

            intensities[i] += I_0 * cos_lamb;
        }
    }
    delete [] normals;
}

