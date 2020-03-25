#pragma once

#include <gl\glew.h>
#include <gl\glu.h>
#include <SDL2\SDL.h>
#include <SDL2\SDL_opengl.h>
#include <stdio.h>
#include <string>

class Screen {
public:
    Screen();

    int width();
    int height();

    int mouse_x();
    int mouse_y();

    bool init();
    void close();
    void update_surface();    

    bool init_opengl(int major, int minor);

private:
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_GLContext context;
};


void init_window();