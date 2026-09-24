#include <stdio.h>
#include <stdbool.h>
#include <X11/Xlib.h>

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    Display* display = XOpenDisplay(0);

    Window root = XDefaultRootWindow(display);
    Window window = XCreateSimpleWindow(display, root, 0, 0, WIDTH, HEIGHT, 0, 0, 0x00aade87);

    XSelectInput(display, window, ExposureMask | StructureNotifyMask);

    Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", false);

    XSetWMProtocols(display, window, &wmDeleteMessage, 1);

    XMapWindow(display, window);
    XFlush(display);

    GC gc = XCreateGC(display, window, 0, NULL);

    XEvent event;
    bool running = true;

    while (running) {
        XNextEvent(display, &event);

        switch (event.type) {
            case Expose:
                XSetForeground(display, gc, WhitePixel(display, DefaultScreen(display)));
                XFillRectangle(display, window, gc, 50, 50, 150, 100);

                XFlush(display);
                break;
            case ClientMessage:
                if ((Atom)event.xclient.data.l[0] == wmDeleteMessage) {
                    running = false;
                }
                break;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}

