#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <thread>
#include <chrono>

using namespace std;
#include <unistd.h>
#include <QPixmap>
#include <sys/time.h>

//QTimer *rotationTimer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    rotationTimer = new QTimer(this);

    connect(ui->rotateX, SIGNAL(clicked()), this, SLOT(on_rotate_x_actionTriggered()));
    connect(ui->rotateY, SIGNAL(clicked()), this, SLOT(on_rotate_y_actionTriggered()));
    connect(ui->rotateZ, SIGNAL(clicked()), this, SLOT(on_rotate_z_actionTriggered()));

    connect(ui->setColorButton, SIGNAL(clicked()), this, SLOT(handleSetColorButtonClicked()));
    connect(ui->resetColorsButton, SIGNAL(clicked()), this, SLOT(restoreDefaultColors()));

    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1200, 800);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setGeometry(25, 25, 1210, 810);
    move(175,75);
    pen = QPen(QColor(255,140,0), 2);
    brush = QBrush(Qt::yellow);
    draw_nep_system();
}

void MainWindow::draw_nep_system()
{
    scene->clear();
    draw_stars();
    int z[15][2];
    for (int i = 0; i < 15; i++)
    {
        z[i][0] = (*astroobjects[i]).get_center().get_z();
        z[i][1] = i;
    }
    for (int i = 0; i < 15 - 1; i++)
        for (int j = 0; j < 15 - i - 1; j++)
            if (z[j][0] > z[j + 1][0])
            {
                swap(z[j][0], z[j + 1][0]);
                swap(z[j][1], z[j + 1][1]);
            }
    int i = 0;
    while (i < 15 && z[i][0] < Neptune.get_center().get_z())
        draw_astroobjects(*astroobjects[z[i++][1]]);
    draw_sun();
    while (i < 15)
        draw_astroobjects(*astroobjects[z[i++][1]]);
}

void MainWindow::draw_stars()
{
    for (int i = 0; i < 300; ++i)
    {
        int x = QRandomGenerator::global()->bounded(10, 1200);
        int y = QRandomGenerator::global()->bounded(10, 800);

        scene->addLine(x, y, x, y, QPen(Qt::white, 1));
    }
}


void MainWindow::draw_sun()
{
    pen = QPen(QColor(255, 255, 0), 1);
    brush = QBrush(QColor(255, 255, 0));
    edge *net = Sun.get_edges();
    point *points = Sun.get_points();
    int cz = Sun.get_center().get_z();
    point p1, p2, p3;
    double t = Sun.get_center().get_z() / 450;
    for (int i = 0; i < Sun.get_edges_num(); i++)
    {
        p1 = points[net[i].get_p1()];
        p2 = points[net[i].get_p2()];
        p3 = points[net[i].get_p3()];
        if (p1.get_z() >= cz || p2.get_z() >= cz || p3.get_z() >= cz)
            draw_triangle(p1, p2, p3, 1 + t, Sun.get_center());
    }
}

int in_circle(point center, int radius, point p)
{
    double x2 = (center.get_x() - p.get_x()) * (center.get_x() - p.get_x());
    double y2 = (center.get_y() - p.get_y()) * (center.get_y() - p.get_y());
    return (x2 + y2) <= radius * radius;
}

void MainWindow::draw_astroobjects(astroobject &p)
{
    pen = QPen(p.get_surface_color(), 1);
    brush = QBrush(p.get_surface_color());
    edge *net = p.get_edges();
    point *points = p.get_points();
    p.recalculateIntensity(lights);
    double *intenses = p.get_intensities();
    point *orbit = p.get_orbit();

    int n = p.get_net_nodes_param();
    int cz = p.get_center().get_z();
    point c = p.get_center();
    point p1, p2, p3;

    int no = p.get_orbit_nodes_num(), j;
    for (int i = 0; i < no; i++)
    {
        j = (i + 1) % no;
        draw_line(orbit[i], orbit[j]);
    }


    int r, g, b;
    for (int i = 0; i < p.get_edges_num(); i++)
    {
        double t = p.get_center().get_z() / 450;
        p.get_surface_color().getRgb(&r, &g, &b);
        double k = intenses[i] / 1 / ((n * 2 + 1) * 2 * (n + 1) * 4 + 1);
        r += k * 255;
        if (r > 255) r = 255;
        if (r < 0) r = 0;
        g += k * 255;
        if (g > 255) g = 255;
        if (g < 0) g = 0;
        brush = QBrush(QColor(r, g, b));
        pen = QPen(QColor(r, g, b), 1);

        p1 = points[net[i].get_p1()];
        p2 = points[net[i].get_p2()];
        p3 = points[net[i].get_p3()];

        if (p1.get_z() >= cz || p2.get_z() >= cz || p3.get_z() >= cz)
            draw_triangle(p1, p2, p3, 1 + t, c);
    }
}



void MainWindow::draw_point(point p)
{
    this->scene->addLine(p.get_x(), 800 - p.get_y(), p.get_x(), 800 - p.get_y(), this->pen);
}

void MainWindow::draw_line(point p1, point p2)
{
    this->scene->addLine(p1.get_x(), 800 - p1.get_y(), p2.get_x(), 800 - p2.get_y(),this->pen);
}

void MainWindow::draw_triangle(point p1, point p2, point p3, double k, point c)
{
    QPolygonF polygon;

    p1.scale(k, c);
    p2.scale(k, c);
    p3.scale(k, c);

    polygon << QPointF(p1.get_x(), 800 - p1.get_y());
    polygon << QPointF(p2.get_x(), 800 - p2.get_y());
    polygon << QPointF(p3.get_x(), 800 - p3.get_y());

    this->scene->addPolygon(polygon, pen, brush);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exit_clicked()
{
    this->close();
}

void MainWindow::on_rotate_x_actionTriggered()
{
    int angle = 5;
    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).rotate_x(angle, Neptune.get_center());
    draw_nep_system();
}

void MainWindow::on_rotate_y_actionTriggered()
{
    int angle = 5;
    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).rotate_y(angle, Neptune.get_center());
    draw_nep_system();
}

void MainWindow::on_rotate_z_actionTriggered()
{
    int angle = 5;
    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).rotate_z(angle, Neptune.get_center());
    draw_nep_system();
}

void MainWindow::on_minus_clicked()
{
    double k = 0.95;
    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).scale(k, Neptune.get_center());

    draw_nep_system();
}

void MainWindow::on_plus_clicked()
{
    double k = 1.05;

    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).scale(k, Neptune.get_center());

//    (*astroobjects[5]).setRedSurfaceColor();
    draw_nep_system();
}

void MainWindow::on_move_up_clicked()
{
    int step = 4;
    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).move(0, -step, 0);
    draw_nep_system();
}

void MainWindow::on_move_down_clicked()
{
    int step = 4;
    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).move(0, step, 0);
    draw_nep_system();
}

void MainWindow::on_move_left_clicked()
{
    int step = 4;

    for (int i = 0; i < 15; i++)
    {
        (*astroobjects[i]).move(step, 0, 0);
    }
    draw_nep_system();
}

void MainWindow::on_move_right_clicked()
{
    int step = 4;
    for (int i = 0; i < 15; i++)
        (*astroobjects[i]).move(-step, 0, 0);
    draw_nep_system();
}

void MainWindow::on_start_2_clicked()
{
    rotationTimer->start(50);

//    for (int i = 1; i < 14; i++)
//        (*planets[i]).rotate_planet(15);

//    draw_nep_system();
//    rotatePlanets();
}

void MainWindow::rotate_astroobjects()
{
    (*astroobjects[1]).move_astroobject_along_orbit(nep_center);

    for (int i = 1; i < 15; i++)
    {
//        (*astroobjects[i]).rotate_astroobject(15);
    }

    draw_nep_system();
}

void MainWindow::handleSpinBoxValueChanged(int value)
{
    if (value >= 0 && value < 15)
    {
       (*astroobjects[value]).set_surface_color(QColor(255, 0, 0));
    }
}

void MainWindow::handleButtonClicked()
{
    int selectedValue = ui->spinBox->value();
    handleSpinBoxValueChanged(selectedValue);
}

void MainWindow::handleSetColorButtonClicked()
{
    int selectedSatelliteIndex = ui->spinBox->value();
    int r = ui->lineEditR->text().toInt();
    int g = ui->lineEditG->text().toInt();
    int b = ui->lineEditB->text().toInt();

    if (selectedSatelliteIndex >= 0 && selectedSatelliteIndex < 15)
            astroobjects[selectedSatelliteIndex]->setColor(QColor(r, g, b));
    draw_nep_system();
}

void MainWindow::restoreDefaultColors() {
    (*astroobjects[0]).set_surface_color(QColor(0, 0, 255));
    (*astroobjects[1]).set_surface_color(QColor(0, 0, 92));
    (*astroobjects[2]).set_surface_color( QColor(33, 64, 150));
    (*astroobjects[3]).set_surface_color(QColor(117, 144, 201));
    (*astroobjects[4]).set_surface_color(QColor(54, 84, 99));
    (*astroobjects[5]).set_surface_color(QColor(63, 76, 79));
    (*astroobjects[6]).set_surface_color(QColor(0, 0, 77));
    (*astroobjects[7]).set_surface_color(QColor(32, 33, 79));
    (*astroobjects[8]).set_surface_color(QColor(8, 37, 103));
    (*astroobjects[9]).set_surface_color(QColor(53, 77, 115));
    (*astroobjects[10]).set_surface_color(QColor(32, 21, 94));
    (*astroobjects[10]).set_surface_color(QColor(78, 112, 186));
    (*astroobjects[12]).set_surface_color(QColor(28, 41, 99));
    (*astroobjects[13]).set_surface_color(QColor(39, 58, 140));
    (*astroobjects[14]).set_surface_color(QColor(46, 68, 117));

    draw_nep_system();
}

