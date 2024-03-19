#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsScene>
#include <QPen>
#include <iostream>
#include <QPolygonF>
#include <QPointF>
#include <QColor>
#include <QTimer>

#include "objects/point.h"
#include "objects/sphere.h"
#include "objects/sun.h"
#include "objects/astroobject.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void draw_point(point p);
    void draw_line(point p1, point p2);
    void draw_triangle(point p1, point p2, point p3, double k, point c);
    void draw_sun();
    void draw_astroobjects(astroobject &p);
    void draw_stars();

private slots:
    void on_exit_clicked();
    void draw_nep_system();

    void on_rotate_x_actionTriggered();
    void on_rotate_y_actionTriggered();
    void on_rotate_z_actionTriggered();
    void on_minus_clicked();
    void on_plus_clicked();
    void on_move_up_clicked();
    void on_move_down_clicked();
    void on_move_left_clicked();
    void on_move_right_clicked();
    void on_start_2_clicked();
    void rotate_astroobjects();

    void restoreDefaultColors();
    void handleSpinBoxValueChanged(int value);
    void handleButtonClicked();
    void handleSetColorButtonClicked();
//    void rotateOrbit();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QPen pen;
    QBrush brush;
//    std::vector<planet*> planets;
    QTimer *rotationTimer;

//    bool rotationEnabled;
    int n = 7;
    point sun_center = point(350, 700, 0);
    point nep_center = point(600, 400, 0);
    sun Sun = sun(sun_center, 20, n);
    point *lights = Sun.get_lights();

    astroobject Neptune = astroobject(0, 0, 0, nep_center, 25, n, lights, QColor(0,0,255));
    astroobject Triton = astroobject(60, 0, 45, nep_center, 8, n, lights, QColor(0,0,92));
    astroobject Nereid = astroobject(100, 0, 90, nep_center, 7, n, lights, QColor(33,64,150));
    astroobject Naiad = astroobject(140, 0, 135, nep_center, 10, n, lights, QColor(117, 144, 201));
    astroobject Thalassa = astroobject(180, 0, 180, nep_center, 9, n, lights, QColor(54,84,99));
    astroobject Despina = astroobject(220, 0, 225, nep_center, 6, n, lights, QColor(63,76,79));
    astroobject Galatea = astroobject(260, 0, 270, nep_center, 5, n, lights, QColor(0,0,77));
    astroobject Larissa = astroobject(300, 0, 315, nep_center, 7, n, lights, QColor(32,33,79));
    astroobject Proteus = astroobject(340, 0, 0, nep_center, 11, n, lights, QColor(8,37,103));
    astroobject Halimede = astroobject(380, 0, 45, nep_center, 17, n, lights, QColor(53,77,115));
    astroobject Laomedeia = astroobject(420, 0, 90, nep_center, 14, n, lights, QColor(32,21,94));
    astroobject Sao = astroobject(460, 0, 135, nep_center, 8, n, lights, QColor(78,112,186));
    astroobject Psamathe = astroobject(500, 0, 180, nep_center, 8, n, lights, QColor(28,41,99));
    astroobject Sitke = astroobject(540, 0, 225, nep_center, 8, n, lights, QColor(39,58,140));
    astroobject Hydekorion = astroobject(580, 0, 270, nep_center, 12, n, lights, QColor(46, 68, 117));
    astroobject *astroobjects[15] = {&Neptune, &Triton, &Nereid, &Naiad, &Thalassa, &Despina, &Galatea,
                                     &Larissa, &Proteus, &Halimede, &Laomedeia, &Sao, &Psamathe, &Sitke, &Hydekorion};

};
#endif // MAINWINDOW_H
