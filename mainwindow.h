#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QBitmap>
#include <QScreen>
#include <QTimer>
#include <QGuiApplication>
#include <QBitmap>
#include <QSettings>
#include <QMouseEvent>
#include <QDebug>

#include <time.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QSettings* settings;
protected:
    void paintEvent(QPaintEvent*) Q_DECL_OVERRIDE;

private:
    QColor ring1_color;
    qreal ring_1_angle_begin;
    qreal ring_1_angle_end;

    QColor ring2_color;
    qreal ring_2_angle_begin;
    qreal ring_2_angle_end;

    QColor ring3_color;

    int font_size;
    bool mask_window;

    QPixmap* pm;
    Display* display;
};

#endif // MAINWINDOW_H
