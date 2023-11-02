#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <cstddef>
#include <chrono>

#include "utils.cpp"

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;
static bool globalRunning = false;

static bool grabMouse = false;
float lastX = SCREEN_WIDTH / 2.0;
float lastY = SCREEN_HEIGHT / 2.0;
//static void HandleInputs(Display *display, Window *window, XEvent *event, double deltaTime)
static void HandleInputs(Display *display, Window *window, XEvent *event)
{
    while(XPending(display) > 0) {
        XNextEvent(display, event);

        switch(event->type) {
            case KeyPress: {
                KeyCode keycode = event->xkey.keycode;
                if (keycode == XKeysymToKeycode(display, XK_W)) {
                }
                if (keycode == XKeysymToKeycode(display, XK_S)) {
                }
                if (keycode == XKeysymToKeycode(display, XK_A)) {
                }
                if (keycode == XKeysymToKeycode(display, XK_D)) {
                }
                if (keycode == XKeysymToKeycode(display, XK_Escape)) {
                  globalRunning = false;
                }
            } break;

            // Mouse movement
            case MotionNotify: {

                int x = event->xmotion.x;
                int y = event->xmotion.y;
                // printf("x %d y %d\n", event->xmotion.x, event->xmotion.y);

                int dx = ClampBetweenNegativeOneAndOne(x - lastX);
                int dy = ClampBetweenNegativeOneAndOne(y - lastY);
                printf("mousedelta: %d, %d\n", dx, dy);

                // Process movement

                // Update last known location
                lastX = x;
                lastY = y;

                // If the cursor hits the edge if the window..
                if (x <= 0 || y <= 0 || x >= SCREEN_WIDTH - 1 ||
                    y >= SCREEN_HEIGHT - 1) {
                    if (grabMouse) {
                        // warp the pointer to the middle of the screen
                        XWarpPointer(display, None, *window, 0, 0, 0, 0,
                                    SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

                        lastX = (float)SCREEN_WIDTH / 2;
                        lastY = (float)SCREEN_HEIGHT / 2;
                    }
                }
            } break;
            // Mouse buttons
            case ButtonPress: {
                switch(event->xbutton.button) {
                    case Button1: { // Left
                    } break;
                    case Button2: { // Middle
                        if (grabMouse) {
                            XUngrabPointer(display, CurrentTime);
                        } else {
                            XGrabPointer(display, *window, True,
                                        ButtonPressMask | ButtonReleaseMask |
                                        PointerMotionMask,
                                        GrabModeAsync, GrabModeAsync, *window,
                                        None, CurrentTime);
                        }
                        grabMouse = !grabMouse;
                    } break;
                    case Button3: { // Right
                    } break;
                    case Button4: { // Mouse wheel up
                    } break;
                    case Button5: { // Mouse wheel down
                    } break;
                }
            } break;
            
            case ButtonRelease: {
            } break;

            // Window move/resize/etc
            case ConfigureNotify:
            case ResizeRequest: {
                uint width  = event->xconfigure.width;
                uint height = event->xconfigure.height;
                if (width != 0 || height != 0) {
                    SCREEN_WIDTH = width;
                    SCREEN_HEIGHT = height;
                }
            } break;
        }
    }
}

int main()
{
    Display *display;
    Window window;
    XEvent event;

    // Open connection to the X server
    display = XOpenDisplay(NULL);
    if(!display) {
        fprintf(stderr, "Cannot open display.\n");
        return 1;
    }

    // Create window
    window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                                 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
                                 BlackPixel(display, DefaultScreen(display)),
                                 WhitePixel(display, DefaultScreen(display)));

    // Window title
    XStoreName(display, window, "Window Title");

    // Select events to listen for
    XSelectInput(display, window,
                 ExposureMask|
                 KeyPressMask|
                 StructureNotifyMask|
                 ResizeRequest|
                 PointerMotionMask|
                 ButtonPress|
                 ButtonRelease);

    // Map the window to the screen
    XMapWindow(display, window);

#if 0
    // Create blank cursor
    Pixmap blank;
    XColor dummy;
    char data[1] = {0};
    Cursor cursor;

    blank = XCreateBitmapFromData(display, window, data, 1, 1);
    if(blank == None) {
        fprintf(stderr, "Unable to create bitmap for cursor\n");
        exit(1);
    }

    cursor = XCreatePixmapCursor(display, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(display, blank);

    XDefineCursor(display, window, cursor);
    XFlush(display);
#endif

    //double lastFrame = 0.0;
    globalRunning = true;
    while(globalRunning) {

        //double currentFrame = std::chrono::duration<double>(std::chrono::system_clock::now().time_since_epoch()).count();
        //double deltaTime = currentFrame - lastFrame;
        //double fps = 1 / deltaTime;

        //lastFrame = currentFrame;

        HandleInputs(display, &window, &event); //, deltaTime);
    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
