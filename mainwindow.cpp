#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ring1_color.setRgb(213, 178, 140, 200);
    ring_1_angle_begin = 0;
    ring_1_angle_end = 1;

    ring2_color.setRgb(200, 200, 200, 200);
    ring_2_angle_begin = 120;
    ring_2_angle_end = 1;

    ring3_color.setRgb(60, 62, 59, 200);

    char* path =(char*) malloc(4096);
    snprintf(path, 4096, "%s/clock.ini", getenv("HOME"));
    settings = new QSettings(path, QSettings::IniFormat, this);
    free(path);
    int size = settings->value("main/size", "400").toInt();
    int posx = settings->value("main/x", "-1").toInt();
    int posy = settings->value("main/y", "-1").toInt();
    font_size = settings->value("misc/font_size", "30").toInt();
    mask_window = settings->value("misc/mask_window", "0").toBool();
    int update = settings->value("main/update_freq", "30").toInt();

    if(settings->value("misc/transparent_for_mouse_events", "1").toBool())
        setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);

    QScreen* screen = QGuiApplication::primaryScreen();
    if(posx == -1)
        posx = (screen->availableSize().width()/2)-(size/2);
    if(posy == -1)
        posy = (screen->availableSize().height()/2)-(size/2);

    setGeometry(posx, posy, size, size);;
    setFixedSize(size, size);

    pm = new QPixmap(width(), height());
    display = XOpenDisplay(NULL);

    QTimer* updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer->start(update);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete pm;
    XCloseDisplay(display);
}

void MainWindow::paintEvent(QPaintEvent*)
{
    int w = width();
    int h = height();

    time_t time_T = time(NULL);
    tm time = *localtime(&time_T);

    pm->fill(Qt::transparent);
    QPainter painter;
    painter.begin(pm);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);

    if(ring_1_angle_end < 90) ring_1_angle_end+=2;
    if(ring_1_angle_begin < 360) ring_1_angle_begin ++;
    else ring_1_angle_begin = 0;

    QPen pen(ring1_color);
    pen.setWidth(5);
    painter.setPen(pen);

    QPainterPath ring_1;
    ring_1.arcMoveTo(5, 5, w - 10, h - 10, ring_1_angle_begin);
    ring_1.arcTo(5, 5, w - 10, h - 10, ring_1_angle_begin, ring_1_angle_end);
    painter.drawPath(ring_1);

    if(ring_2_angle_end < 45) ring_2_angle_end+=2;
    if(ring_2_angle_begin < 360) ring_2_angle_begin-=1.5;
    else ring_2_angle_begin = 121;

    pen.setColor(ring2_color);
    painter.setPen(pen);
    QPainterPath ring_2;
    ring_2.arcMoveTo(15, 15, w - 30, h - 30, ring_2_angle_begin);
    ring_2.arcTo(15, 15, w - 30, h - 30, ring_2_angle_begin, ring_2_angle_end);
    painter.drawPath(ring_2);


    int i = (360/60)*time.tm_sec;
    i = 0-i;
    pen.setColor(ring3_color);
    painter.setPen(pen);
    QPainterPath ring_3;
    ring_3.arcMoveTo(35, 35, w - 70, h - 70, 90);
    ring_3.arcTo(35, 35, w - 70, h - 70, 90, i);
    painter.drawPath(ring_3);

    QFont font;
    font.setPointSize(font_size);

    char* buf =(char*) malloc(4096);
    if(time.tm_hour < 10 && time.tm_min < 10)
        snprintf(buf, 4096, "0%d:0%d", time.tm_hour, time.tm_min);
    else if(time.tm_min < 10)
        snprintf(buf, 4096, "%d:0%d", time.tm_hour, time.tm_min);
    else if(time.tm_hour < 10)
        snprintf(buf, 4096, "0%d:%d", time.tm_hour, time.tm_min);
    else snprintf(buf, 4096, "%d:%d", time.tm_hour, time.tm_min);
    QFontMetrics fm(font);
    int tw = fm.width(buf);
    int th = fm.height();

    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText((w/2)-(tw/2), (h/2)-(th/2), QString(buf));
    free(buf);

    QFont font2;
    font2.setPointSize(20);
    painter.setFont(font2);
    QFontMetrics fm2(font2);
    char* ddmmyy = (char*) malloc(4096);
    snprintf(ddmmyy, 4096, "%d/%d/%d", time.tm_mday, time.tm_mon+1, time.tm_year+1900);
    int tw2 = fm2.width(ddmmyy);
    int th2 = fm2.height();
    painter.drawText((w/2)-(tw2/2), (h/2)-(th2/2)+th, ddmmyy);

    free(ddmmyy);
    painter.end();
    painter.begin(this);
    if(mask_window)
    {
        setMask(pm->mask());
    }
    painter.drawPixmap(0, 0, w, h, *pm);
    painter.end();
}
