/*
 *
 * fn-notify
 *
 * (c) 2017 Daniel Jankowski
 */

#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>

#include <fcntl.h>
#include <errno.h>

#include "draw.h"

#define MAX(a,b)              ((a) > (b) ? (a) : (b))

Display *display;
Window  window;
XSetWindowAttributes attributes;
XGCValues gc_values;
GC gc;
int depth;
int screen;

Colormap colormap;

int num;
struct timespec tstart={0,0}, tend={0,0};
double executionLength = 1;


void drawRoundCorners(void) {
    // get window attributes for width and height
    XWindowAttributes win_attr;
    XGetWindowAttributes(display, window, &win_attr);

    // extract window width and height
    int width = win_attr.width + win_attr.border_width;
    int height = win_attr.height + win_attr.border_width;

    // create a pixmap to draw the shaped window in
    Pixmap mask = XCreatePixmap(display, window, width, height, 1);
    XGCValues xgcv;

    // create a graphics context for the shaped window
    GC shape_gc = XCreateGC(display, mask, 0, &xgcv);

    // set radius und diameter
    int rad = 10;
    int dia = 2 * rad;

    // flush the gc
    XSetForeground(display, shape_gc, 0);
    XFillRectangle(
            display, mask, shape_gc,
            0, 0,
            width, height);

    // draw the rounded corners first
    XSetForeground(display, shape_gc, 1);
    XFillArc(
            display, mask, shape_gc,
             0, 0,
             dia, dia,
             0, 360 * 64);
    XFillArc(
            display, mask, shape_gc,
            width - dia - 1, 0,
            dia, dia,
            0, 360 * 64);
    XFillArc(
            display, mask, shape_gc,
            0, height - dia - 1,
            dia, dia,
            0, 360 * 64);
    XFillArc(
            display, mask, shape_gc,
            width - dia - 1, height - dia - 1,
            dia, dia, 
            0, 360 * 64);

    // draw the center rectangles of the window
    XFillRectangle(
            display, mask, shape_gc,
            rad, 0,
            width-dia, height);
    XFillRectangle(
            display, mask, shape_gc,
            0, rad,
            width, height-dia);

    // combine the shape we created in the pixmap with a mask
    XShapeCombineMask(display, window, ShapeBounding, 0, 0, mask, ShapeSet);


    // free the pixmap
    XFreePixmap(display, mask);

    // apply the shape onto the window
    XShapeSelectInput(display,
            window, ShapeNotifyMask);
}


void* checkExit(void *args) {
    double diff_t;

    // get the start and end time
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    clock_gettime(CLOCK_MONOTONIC, &tend);

    // create the difference between those times
    diff_t = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

    // while the time difference is shorter than the execution time...
    while (diff_t < executionLength) {
        // ...get the actual time...
        clock_gettime(CLOCK_MONOTONIC, &tend);

        // ...calculate the new difference...
        diff_t = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

        // ...and wait 100 milliseconds
        usleep(100);
    }

    // if the execution time is reache, exit gracefully
    exit(0);
}
    

int CheckForAnotherInstance() {
#ifndef _MSC_VER
    int fd;
    struct flock fl;

    // open the file or create it, if its non existant
    fd = open("/home/daniel/.cache/fn-control.lock", O_RDWR | O_CREAT, 0755);

    // if an error occured, return 0
    if (fd == -1) {
        printf("%s\n", strerror(errno));
        return 0;
    }

    // set file lock parameter
    fl.l_type   = F_WRLCK;  // F_RDLCK, F_WRLCK, F_UNLCK   
    fl.l_whence = SEEK_SET; // SEEK_SET, SEEK_CUR, SEEK_END
    fl.l_start  = 0;        // Offset from l_whence        
    fl.l_len    = 0;        // length, 0 = to EOF          
    fl.l_pid    = getpid(); // our PID                     

    // try to create a file lock
    if( fcntl(fd, F_SETLK, &fl) == -1) {
        // we failed to create a file lock, meaning it's already locked
        if( errno == EACCES || errno == EAGAIN) {
            return 1;
        }
    }

#endif
    return 0;
}




void sighandler(int signum) {
    printf("Caught signal %d, coming out...\n", signum);
    // get the new value
    FILE *fp;
    int i;

    double diff_t;

    // get the actual time
    clock_gettime(CLOCK_MONOTONIC, &tend);

    // calculate the recent execution time
    diff_t = ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec);

    // set the new execution length to 1 second in the future
    executionLength = diff_t + 1;

    // get value from save file
    fp = fopen("/home/daniel/.cache/fn-control.save", "r");
    fscanf (fp, "%d", &i); 
    fclose(fp);

    // get the mute status
    int mute = (int) ((i % 100) / 10);

    printf("%d\n", i);
    printf("%d\n", mute);

    int mode = i % 2; 

    // get the volume value
    i = (int) i / 100;

    // calculate slider length
    int sliderLength = (int) (i * 1.6);

    // erase the old slider
    XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));
    XFillRectangle(display, window, gc, 20, 175, 160, 5);

    // change color if muted
    if (mute) {
        drawVolumeIconMuted();

        // draw a new one
        XSetForeground(display, gc, strtoul(FG_MUTED, 0, 16));
        XFillRectangle(display, window, gc, 20, 175, sliderLength, 5);
    } else {
        switch (mode) {
            case 1:
                drawVolumeIcon();
                break;
            case 2:
                drawBrightnessIcon();
                break;
            default:
                break;
        }

        // draw a new one
        XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));
        XFillRectangle(display, window, gc, 20, 175, sliderLength, 5);
    }

    // flush the display to show the changes
    XFlush(display);
}

void init_window() {
    // open the display connection
    display = XOpenDisplay(NULL);

    // get screen properties
    XWindowAttributes xwAttr;
    XGetWindowAttributes(
            display,
            DefaultRootWindow(display),
            &xwAttr
            );
    int screenWidth = xwAttr.width;
    int screenHeight = xwAttr.height;

    int width = 200;
    int height = 200;

    int x = (screenWidth / 2) - (width / 2);
    int y = (screenHeight / 2) - (height / 2);


    // build matching visual
    XVisualInfo vinfo;
    XMatchVisualInfo(
            display,
            DefaultScreen(display),
            32,
            TrueColor,
            &vinfo
            );

    // set window attributes 
    XSetWindowAttributes attr;
    attr.colormap = XCreateColormap(
            display,
            DefaultRootWindow(display),
            vinfo.visual,
            AllocNone
            );
    attr.border_pixel = 0;
    attr.background_pixel = strtoul(BG_COLOR, 0, 16);
    attr.override_redirect = 1;
    attr.event_mask = KeyPressMask | KeyReleaseMask | ExposureMask | SubstructureNotifyMask;

    // create the window
    window = XCreateWindow(
            display,
            DefaultRootWindow(display),
            x,
            y,
            width,
            height,
            0,
            vinfo.depth,
            InputOutput,
            vinfo.visual,
            CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect | CWEventMask,
            &attr
            );


    XSelectInput(display, window, StructureNotifyMask);
    
    // create graphic context
    gc = XCreateGC(display, window, 0, 0);

    // set wm protocols
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    // set drawing color
    XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));

    drawRoundCorners();

    // map the window and display it
    XMapWindow(display, window);
}


int main(int argc, char *argv[]) {
    // check if arguments are provided
    if (argc != 3 && argc != 4) {
      //TODO: multiple arguments and help text
        printf("No arguments provided\n");
        exit(-1);
    }

    int mute = 0;
    int mode = 0;
    if (strncmp(argv[1], "vol", 3) == 0) {
        mode = 1;
    }
    if (strncmp(argv[1], "brig", 4) == 0) {
        mode = 2;
    }

    // check if the mute flag is set
    if (argc == 4) {
        printf("%s\n", argv[3]);
        if (strncmp(argv[3], "--mute", 6) == 0) {
            mute = 1;
        }
    }

    // check if another instance is running
    int isRunning;
    isRunning = CheckForAnotherInstance();

    // initialize variables for the arguments
    char *p;
    num = 0;
    long conv = strtol(argv[2], &p, 10);
    errno = 0;

    // Check for errors: e.g., the string does not represent an integer
    // or the integer is larger than int
    if (errno != 0 || *p != '\0') {
        //TODO: handle wrong values
    } else {
        // No error
        num = conv;    
    }

    if (isRunning == 1) {
        FILE *fp;

        // write value to save file
        fp = fopen("/home/daniel/.cache/fn-control.save", "w+");
        if (mute) {
            fprintf(fp, "%03d1%d", num, mode);
        } else {
            fprintf(fp, "%03d0%d", num, mode);
        }
        fclose(fp);

        // get pid of the running process
        char line[20];
        FILE * command = popen("pidof fn-notify | cut -d' ' -f 2", "r");

        fgets(line, 20, command);

        char number[strlen(line) + 1];
        snprintf(number, strlen(line) + 1,"%s", line);

        pid_t pid = strtoul(number,NULL,10);
        pclose(command);

        // send SIGUSR1 to other process
        kill(pid, 10);

        // exit
        exit(0);
    }

    // initialize window
    init_window();

    // calculate slider length
    int sliderLength = (int) (num * 1.6);

    if (mute == 1) {
        drawVolumeIconMuted();

        // set slider color
        XSetForeground(display, gc, strtoul(BG_COLOR, 0, 16));

        // draw the slider
        XFillRectangle(display, window, gc, 20, 175, sliderLength, 5);
    } else {
        switch (mode) {
            case 1:
                drawVolumeIcon();
                break;
            case 2:
                drawBrightnessIcon();
                break;
            default:
                break;
        }

        // set slider color
        XSetForeground(display, gc, strtoul(FG_COLOR, 0, 16));

        // draw the slider
        XFillRectangle(display, window, gc, 20, 175, sliderLength, 5);
    }

    // flush the display to show the drawn objects
    XFlush(display);

    // register the signal handler
    signal(SIGUSR1, sighandler);
    
    // set the execution time to 1 second
    executionLength = 1;

    // start the main loop to display the window 1 second
    checkExit(NULL);
}
