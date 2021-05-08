#include <X11/Xlib.h>
#include <iostream>
#include <X11/extensions/Xrandr.h>
#include <unistd.h>
#include <cmath>

void diag_move() {
    Display* dpy = XOpenDisplay(0); // can also be 'nullptr' or NULL
    int scr = XDefaultScreen(dpy);
    Window root_window = XRootWindow(dpy, scr);

    int height = DisplayHeight(dpy, scr);
    int width  = DisplayWidth(dpy, scr);
    std::cout << "Screen size : " << width << "x" << height << std::endl;

    float m = (float)height/(float)width;

    int j;
    
    for(int i=0; i<width; i++){
        j = m*i;
        XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, i, j);
        XFlush(dpy);
        usleep(1000);
    }
    //XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, width/2, height/2);
    //XFlush(dpy);
    //usleep(1000);
    std::cout << "Diagonal motion done" << std::endl; 
}

void circular_move(int num_rounds, int min_radius=200) {
    Display* dpy = XOpenDisplay(0); // can also be 'nullptr' or NULL
    int scr = XDefaultScreen(dpy);
    Window root_window = XRootWindow(dpy, scr);

    int height = DisplayHeight(dpy, scr);
    int width  = DisplayWidth(dpy, scr);
    std::cout << "Screen size : " << width << "x" << height << std::endl;
    width = width + (width /2);
    float m = (float)height/(float)width;

    int j;
    double angle_in_rad = 0;
    double rad_converter = (3.14159 / 180);

    for(int round=0; round < num_rounds; round++){
        // move mouse pointer to center
        XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, width/2, height/2);
        
        for(int i=0; i<360; i++){
            double computed_angle = rad_converter * i;
            int x_coord = (int)((width/2) + (min_radius * sin(computed_angle)));
            int y_coord = (int)((height/2) + (min_radius * cos(computed_angle)));
            XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x_coord, y_coord);
            XFlush(dpy);
            usleep(2000);
        }
    }
    //XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, width/2, height/2);
    //XFlush(dpy);
    //usleep(1000);
    std::cout << "Circular motion done" << std::endl; 
}

void printMonitorInfo()
{
    Display * display = XOpenDisplay( nullptr );
    if (!display)
    {
        std::cout << "cannot open X Display" << std::endl;
        return;
    }

    int dummy1, dummy2;
    int major, minor;
    if (!XRRQueryExtension( display, &dummy1, &dummy2 )
     || !XRRQueryVersion( display, &major, &minor ))
    {
        std::cout << "failed to retrieve XRandR version" << std::endl;
        XCloseDisplay( display );
        return;
    }

    if (major <= 1 && minor < 5)
    {
        std::cout << "RandR version at least 1.5 is required" << std::endl;
        XCloseDisplay( display );
        return;
    }

    Window rootWindow = RootWindow( display, DefaultScreen( display ) );

    Bool onlyActive = false;  // if set it returns only active monitors (non-0x0 monitors)
    int monitorCnt;
    XRRMonitorInfo * xMonitors = XRRGetMonitors( display, rootWindow, onlyActive, &monitorCnt );
    if (!xMonitors)
    {
        std::cout << "failed to retrieve monitor info" << std::endl;
        XCloseDisplay( display );
        return;
    }

    for (int monitorIdx = 0; monitorIdx < monitorCnt; monitorIdx++)
    {
        std::cout
            << "name: " << XGetAtomName( display, xMonitors[ monitorIdx ].name )
            << ", width: " << xMonitors[ monitorIdx ].width
            << ", height: " << xMonitors[ monitorIdx ].height
            << ", primary: " << xMonitors[ monitorIdx ].primary
            << std::endl;
    }

    XCloseDisplay( display );
}

int main(){
    printMonitorInfo();
    //diag_move();
    circular_move(10);
    return 0;
}