#include <QApplication>
#include "mainwindow.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <unistd.h>
#include <stdio.h>

#define _NET_WM_STATE_REMOVE        0
#define _NET_WM_STATE_ADD           1
#define _NET_WM_STATE_TOGGLE        2

using namespace std;

const char* defconfig = "[main]\nupdate_freq=20\nsize=400\n\n# Clock position on screen, -1 means center\nx=-1\ny=-1\n\n[misc]\nfont_size=30 # Font size for time\nmask_window=0 # Shaped window, default set to 0 due to low performance after enabling it.\ntransparent_for_mouse_events=1 # Enabling makes window COMPLETELY unclickable, used as alternative to masking.\nbelow=0 # Defaultly set to 0, on kde window may dissappear after enabling it.\n";

int main(int argc, char *argv[])
{
    char path[4096];
    snprintf(path, 4096, "%s/.config/clock.ini", std::getenv("HOME"));
    if(access(path, F_OK))
    {
        FILE* config = fopen(path, "w");
        fwrite(defconfig, strlen(defconfig), 1, config);
        fclose(config);
        printf("Config file created.\n");
    }

    QApplication a(argc, argv);
    MainWindow w;

    Display* display = XOpenDisplay(NULL);
    Window win = w.winId();

    Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    Atom normal = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", 0);
    XChangeProperty(display, win, type, XA_ATOM, 32, PropModeReplace, (unsigned char*)&normal, 1);
    w.show();


    XEvent e;
    e.type = ClientMessage;
    e.xclient.type = ClientMessage;
    e.xclient.window = win;
    e.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", 0);
    e.xclient.format = 32;
    e.xclient.data.l[0] = _NET_WM_STATE_ADD;
    e.xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", 0);
    if(w.settings->value("misc/below", "0").toBool())
        e.xclient.data.l[2] = XInternAtom(display, "_NET_WM_STATE_BELOW", 0);
    else
        e.xclient.data.l[2] = 0;
    e.xclient.data.l[3] = 0;
    e.xclient.data.l[4] = 0;

    XSendEvent(display, XDefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &e);
    XCloseDisplay(display);

    int ret = a.exec();
    return ret;
}
