#include "draw.h"


extern Display *display;
extern Window  window;
extern GC gc;


void drawBrightnessIcon() {
    // get window attributes for width and height
    XWindowAttributes win_attr;
    XGetWindowAttributes(display, window, &win_attr);

    // extract window width and height
    int width = win_attr.width;
    int height = win_attr.height;

    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillRectangle(display, window, gc, 0, 0, width, height);
    XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));
    for (int i=0; i < 12; i++) {
        XFillArc(
                display, window, gc,
                60, 60,
                80, 80,
                30 * i * 64, 5 * 64);
    }


    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            75, 75,
            50, 50,
            0 * 64, 360 * 64);
    XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            85, 85,
            30, 30,
            0 * 64, 360 * 64);
}


void drawVolumeIcon() {
    // get window attributes for width and height
    XWindowAttributes win_attr;
    XGetWindowAttributes(display, window, &win_attr);

    // extract window width and height
    int width = win_attr.width;
    int height = win_attr.height;

    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillRectangle(display, window, gc, 0, 0, width, height);
    XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            50, 50,
            100, 100,
            -45 * 64, 90 * 64);
    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            55, 55,
            90, 90,
            -45 * 64, 90 * 64);
    XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            60, 60,
            80, 80,
            -45 * 64, 90 * 64);
    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            65, 65,
            70, 70,
            -45 * 64, 90 * 64);
    XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            70, 70,
            60, 60,
            -45 * 64, 90 * 64);
    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            75, 75,
            50, 50,
            -45 * 64, 90 * 64);

    XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));
    XFillRectangle(display, window, gc, 55, 75, 50, 50);

    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            50, 60,
            60, 60,
            45 * 64, 135 * 64);
    XFillArc(
            display, window, gc,
            50, 80,
            60, 60,
            180 * 64, 135 * 64);
}


void drawVolumeIconMuted() {
    // get window attributes for width and height
    XWindowAttributes win_attr;
    XGetWindowAttributes(display, window, &win_attr);

    // extract window width and height
    int width = win_attr.width;
    int height = win_attr.height;

    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillRectangle(display, window, gc, 0, 0, width, height);

    XSetForeground(display, gc, strtoul(FG_MUTED, 0, 16));
    XFillRectangle(display, window, gc, 55, 75, 50, 50);

    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillArc(
            display, window, gc,
            50, 60,
            60, 60,
            45 * 64, 135 * 64);
    XFillArc(
            display, window, gc,
            50, 80,
            60, 60,
            180 * 64, 135 * 64);
}
