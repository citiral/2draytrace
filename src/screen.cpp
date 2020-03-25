#include "screen.h"

void GLAPIENTRY MessageCallback( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (type == GL_DEBUG_TYPE_OTHER)
        return;
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                type, severity, message );
}

Screen::Screen()
{

}

int Screen::width()
{
    int w;

    SDL_GetWindowSize(window, &w, nullptr);

    return w;
}

int Screen::height()
{
    int h;

    SDL_GetWindowSize(window, nullptr, &h);

    return h;
}

int Screen::mouse_x()
{
    int x;

    SDL_GetMouseState(&x, nullptr);
    
    return x;
}

int Screen::mouse_y()
{
    int y;

    SDL_GetMouseState(nullptr, &y);

    return y;
}


bool Screen::init()
{
    //Initialize SDL
    if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    } else {
        //Create window
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if(window == NULL) {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        } else {
            //Get window surface
            screenSurface = SDL_GetWindowSurface(window);
            //SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF));
        }
    }

    return true;
}

void Screen::close()
{
    //Destroy window
    SDL_DestroyWindow(window);
    window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void Screen::update_surface()
{
    SDL_GL_SwapWindow(window);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {

    }

    //SDL_UpdateWindowSurface(window);
}

bool Screen::init_opengl(int major, int minor)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    //Create context
    context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    if (!context) {
        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    } else {
        //Initialize GLEW
        glewExperimental = GL_TRUE; 
        GLenum glewError = glewInit();
        if(glewError != GLEW_OK) {
            printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
            return false;
        }

        //Use Vsync
        if(SDL_GL_SetSwapInterval( 1 ) < 0) {
            printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
        }
    }

    // During init, enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    return true;
}