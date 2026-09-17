#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define MAX_CARS 20
#define STARTING_SPEED 3
#define SPEED_INCREMENT 2
#define MAX_LEVEL 10

typedef struct { float x, y; } Vector2;
typedef struct { Vector2 pos; float speed; unsigned long color; } Car;

// Game State Vars
Display *dpy;
Atom wmDeleteMessage;
Window win;
Vector2 player = { 300, 550 };
Car cars[MAX_CARS];
int car_count = 0;
int current_speed = STARTING_SPEED;
int level = 1;
int high_score = 1; 
bool game_over = false;
bool game_won = false; 

// SOUND TRIGGER FLAGS
bool collision_sound_played = false;
bool win_sound_played = false;


void load_high_score() {
    FILE *file = fopen("highscore.txt", "r");
    if (file) {
        if (fscanf(file, "%d", &high_score) != 1) {
            high_score = 1;
        }
        fclose(file);
    }
}

void save_high_score() {
    FILE *file = fopen("highscore.txt", "w");
    if (file) {
        fprintf(file, "%d", high_score);
        fclose(file);
    }
}


void draw_vector_char(char c, float x, float y, float size) {
    glLineWidth(2.0f);
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBegin(GL_LINES);
    
    switch (c) {
        case '0':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + size, y + size * 2);
            break;
        case '1':
            glVertex2f(x + size/2, y); glVertex2f(x + size/2, y + size * 2);
            glVertex2f(x + size/4, y); glVertex2f(x + size/2, y);
            glVertex2f(x + size/4, y + size * 2); glVertex2f(x + size * 3/4, y + size * 2);
            break;
        case '2':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x, y + size);
            glVertex2f(x, y + size); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            break;
        case '3':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            break;
        case '4':
            glVertex2f(x, y); glVertex2f(x, y + size);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            break;
        case '5':
            glVertex2f(x + size, y); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x, y + size);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x, y + size * 2);
            break;
        case '6':
            glVertex2f(x + size, y); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x, y + size);
            break;
        case '7':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x, y + size * 2);
            break;
        case '8':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            break;
        case '9':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x, y); glVertex2f(x, y + size);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            break;
        case 'G':
            glVertex2f(x + size, y); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x + size/2, y + size);
            break;
        case 'A':
            glVertex2f(x, y + size * 2); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            break;
        case 'M':
            glVertex2f(x, y + size * 2); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + size/2, y + size);
            glVertex2f(x + size/2, y + size); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            break;
        case 'E':
            glVertex2f(x + size, y); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size); glVertex2f(x + size * 3/4, y + size);
            break;
        case 'O':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x, y);
            break;
        case 'V':
            glVertex2f(x, y); glVertex2f(x + size/2, y + size * 2);
            glVertex2f(x + size/2, y + size * 2); glVertex2f(x + size, y);
            break;
        case 'R':
            glVertex2f(x, y + size * 2); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x, y + size);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size * 2);
            break;
        case 'W':
            glVertex2f(x, y); glVertex2f(x + size/4, y + size * 2);
            glVertex2f(x + size/4, y + size * 2); glVertex2f(x + size/2, y + size);
            glVertex2f(x + size/2, y + size); glVertex2f(x + size * 3/4, y + size * 2);
            glVertex2f(x + size * 3/4, y + size * 2); glVertex2f(x + size, y);
            break;
        case 'I':
            glVertex2f(x + size/2, y); glVertex2f(x + size/2, y + size * 2);
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            break;
        case 'N':
            glVertex2f(x, y + size * 2); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x + size, y);
            break;
        case 'S':
            glVertex2f(x + size, y); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x, y + size);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x, y + size * 2);
            break;
        case 'P':
            glVertex2f(x, y + size * 2); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x, y + size);
            break;
        case 'L':
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            break;
        case 'H':
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x + size, y); glVertex2f(x + size, y + size * 2);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            break;
        case 'C':
            glVertex2f(x + size, y); glVertex2f(x, y);
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            break;
        case 'T':
            glVertex2f(x, y); glVertex2f(x + size, y);
            glVertex2f(x + size/2, y); glVertex2f(x + size/2, y + size * 2);
            break;
        case 'U':
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x, y + size * 2); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x + size, y);
            break;
        case 'B':
            glVertex2f(x, y); glVertex2f(x, y + size * 2);
            glVertex2f(x, y); glVertex2f(x + size * 3/4, y);
            glVertex2f(x + size * 3/4, y); glVertex2f(x + size * 3/4, y + size);
            glVertex2f(x + size * 3/4, y + size); glVertex2f(x, y + size);
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            glVertex2f(x + size, y + size); glVertex2f(x + size, y + size * 2);
            glVertex2f(x + size, y + size * 2); glVertex2f(x, y + size * 2);
            break;
        case 'Y':
            glVertex2f(x, y); glVertex2f(x + size/2, y + size);
            glVertex2f(x + size, y); glVertex2f(x + size/2, y + size);
            glVertex2f(x + size/2, y + size); glVertex2f(x + size/2, y + size * 2);
            break;
        case ':':
            glVertex2f(x + size/2, y + size/2); glVertex2f(x + size/2, y + size/2 + 2);
            glVertex2f(x + size/2, y + size * 3/2); glVertex2f(x + size/2, y + size * 3/2 + 2);
            break;
        case '-':
            glVertex2f(x, y + size); glVertex2f(x + size, y + size);
            break;
    }
    glEnd();
}

void draw_vector_string(const char *str, float x, float y, float size, float spacing) {
    while (*str) {
        draw_vector_char(*str, x, y, size);
        x += size + spacing;
        str++;
    }
}



void spawn_car() {
    if (car_count < MAX_CARS && (rand() % 15 == 0)) {
        cars[car_count].pos.x = 620;
        cars[car_count].pos.y = 80 + (rand() % 420);
        cars[car_count].speed = current_speed;
        cars[car_count].color = ((rand()%256)<<16) | ((rand()%256)<<8) | (rand()%256);
        car_count++;
    }
}

void update_game() {
    if (game_over || game_won) return;

    spawn_car();

    for (int i = 0; i < car_count; i++) {
        cars[i].pos.x -= cars[i].speed;

        if (player.x + 15 > cars[i].pos.x && player.x - 15 < cars[i].pos.x + 50 &&
            player.y + 15 > cars[i].pos.y && player.y - 15 < cars[i].pos.y + 25) {
            
            game_over = true;
            if (!collision_sound_played) {
                system("[ -f .music.pid ] && kill $(cat .music.pid) 2>/dev/null; rm -f .music.pid 2>/dev/null");
                system("aplay -q collision.wav 2>/dev/null &");
                collision_sound_played = true;
            }
        }

        if (cars[i].pos.x < -60) {
            for (int j = i; j < car_count - 1; j++) cars[j] = cars[j + 1];
            car_count--;
            i--;
        }
    }

    if (player.y < 40) {
        if (level >= MAX_LEVEL) {
            game_won = true;
            if (!win_sound_played) {
                system("[ -f .music.pid ] && kill $(cat .music.pid) 2>/dev/null; rm -f .music.pid 2>/dev/null");
                system("aplay -q next_level.wav 2>/dev/null &");
                win_sound_played = true;
            }
        } else {
            level++;
            if (level > high_score) {
                high_score = level;
                save_high_score();
            }
            current_speed += SPEED_INCREMENT;
            player.x = 300;
            player.y = 550;
            car_count = 0;
            system("aplay -q next_level.wav 2>/dev/null &");
        }
    }
}


void draw_rect(float x, float y, float w, float h, unsigned long color) {
    float r = ((color >> 16) & 0xFF) / 255.0f;
    float g = ((color >> 8) & 0xFF) / 255.0f;
    float b = (color & 0xFF) / 255.0f;
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();
}

void draw_triangle(float x1, float y1, float x2, float y2, float x3, float y3, unsigned long color) {
    float r = ((color >> 16) & 0xFF) / 255.0f;
    float g = ((color >> 8) & 0xFF) / 255.0f;
    float b = (color & 0xFF) / 255.0f;
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

void reset_game() {
    player.x = 300;
    player.y = 550;
    car_count = 0;
    current_speed = STARTING_SPEED;
    level = 1;
    game_over = false;
    game_won = false;
    collision_sound_played = false;
    win_sound_played = false;
    system("ffplay -nodisp -loop 0 background.mp3 > /dev/null 2>&1 & echo $! > .music.pid");
}


void draw_frame() {
    glViewport(0, 0, 600, 600);
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned long panel_color = 0x222222;
    if (game_over) panel_color = 0x990000;
    if (game_won) panel_color  = 0x006600;
    draw_rect(0, 0, 600, 40, panel_color);

    char lvl_str[16];  snprintf(lvl_str, sizeof(lvl_str), "LEVEL:%02d", level);
    char high_str[16]; snprintf(high_str, sizeof(high_str), "HI:%02d", high_score);
    draw_vector_string(lvl_str, 20, 12, 8, 4);
    draw_vector_string(high_str, 480, 12, 8, 4);

    draw_rect(0, 40, 600, 5, 0x00FF00);  
    draw_rect(0, 535, 600, 5, 0x888888); 

    for (int i = 0; i < car_count; i++) {
        draw_rect(cars[i].pos.x, cars[i].pos.y, 50, 25, cars[i].color);
    }

    unsigned long turtle_color = game_over ? 0xFF0000 : 0x22AA22; 
    unsigned long head_color   = game_over ? 0xAA0000 : 0x66FF66; 
    if (game_won) { turtle_color = 0x00FFFF; head_color = 0x88FFFF; }

    draw_triangle(player.x - 12, player.y - 12, player.x - 18, player.y - 8, player.x - 10, player.y, turtle_color);
    draw_triangle(player.x + 12, player.y - 12, player.x + 18, player.y - 8, player.x + 10, player.y, turtle_color);
    draw_triangle(player.x - 10, player.y + 10, player.x - 16, player.y + 14, player.x - 8, player.y + 6, turtle_color);
    draw_triangle(player.x + 10, player.y + 10, player.x + 16, player.y + 14, player.x + 8, player.y + 6, turtle_color);
    draw_rect(player.x - 10, player.y - 10, 20, 20, turtle_color);
    draw_triangle(player.x, player.y - 18, player.x - 6, player.y - 10, player.x + 6, player.y - 10, head_color);

    if (game_over) {
        draw_rect(100, 250, 400, 100, 0x111111);
        draw_vector_string("GAME OVER", 200, 270, 15, 6);
        draw_vector_string("PRESS R TO RESTART", 175, 310, 8, 4);
    }
    
    if (game_won) {
        draw_rect(100, 250, 400, 100, 0x111111);
        draw_vector_string("YOU WIN", 220, 270, 15, 6);
        draw_vector_string("PRESS R TO PLAY AGAIN", 175, 310, 8, 4);
    }
}


int main() {
    srand(time(NULL));
    load_high_score();

    system("ffplay -nodisp -loop 0 background.mp3 > /dev/null 2>&1 & echo $! > .music.pid");

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        system("[ -f .music.pid ] && kill $(cat .music.pid) 2>/dev/null; rm -f .music.pid 2>/dev/null");
        return printf("Cannot open display\n"), 1;
    }

    Window root = DefaultRootWindow(dpy);
    GLint att[] = { GLX_RGBA, GLX_DOUBLEBUFFER, None };
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    
    XSetWindowAttributes swa = { 
        .colormap = cmap, 
        .event_mask = ExposureMask | KeyPressMask | StructureNotifyMask 
    };
    win = XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "Native C Turtle Crossing");

    wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, win, &wmDeleteMessage, 1);

    GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
    glXMakeCurrent(dpy, win, glc);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 600, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    while (true) {
        while (XPending(dpy)) {
            XEvent xev; XNextEvent(dpy, &xev);
            
            if (xev.type == ConfigureNotify) {
                glViewport(0, 0, xev.xconfigure.width, xev.xconfigure.height);
            }

            if (xev.type == ClientMessage && xev.xclient.data.l[0] == wmDeleteMessage) {
                goto cleanup;
            }
            
            if (xev.type == KeyPress) {
                char buf[10] = {0};
                KeySym key;
                XLookupString(&xev.xkey, buf, sizeof(buf), &key, NULL);
                
                if (key == XK_Escape) goto cleanup;
                
                if ((game_over || game_won) && (key == XK_r || key == XK_R || buf[0] == 'r' || buf[0] == 'R')) {
                    reset_game();
                }

                if (!game_over && !game_won) {
                    if (key == XK_Up)    player.y -= 15;
                    if (key == XK_Down)  player.y += 15;
                    if (key == XK_Left)  player.x -= 15;
                    if (key == XK_Right) player.x += 15;
                }
            }
        }
        update_game();
        draw_frame();
        glXSwapBuffers(dpy, win);
        usleep(16666);
    } 

cleanup:
    system("[ -f .music.pid ] && kill $(cat .music.pid) 2>/dev/null; rm -f .music.pid 2>/dev/null");
    glXMakeCurrent(dpy, None, NULL);
    glXDestroyContext(dpy, glc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
    return 0;
}



