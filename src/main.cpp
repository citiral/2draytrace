//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include "screen.h"
#include "scene.h"
#include "raytracer.h"
#include "rayrenderer.h"
#include <math.h>

int main(int argc, char** argv)
{
    Screen screen;

    Scene scene;
    RayTracer tracer;
    RayRenderer renderer;

    if (!screen.init()) {
        printf("Failed initializing screen\n");
    }
    if (!screen.init_opengl(3, 3)) {
        printf("Failed initializing oepngl\n");
    }

    renderer.init(100000, screen.width(), screen.height());
    scene.add_point_light(0, 0, 0, 1, 0, 200.f);
    scene.add_point_light(0.4, 0, 1, 1, 0, 200.f);
    scene.add_point_light(-0.4, 0.001, 0, 1, 1, 200.f);
    PointLight* light = scene.add_point_light(0, 0, 0, 0, 1, 200.f);

    //glDisable(GL_ALPHA_TEST);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    //glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_MULTISAMPLE);
    //glEnable(GL_SMOOTH);

    glBlendFunc(GL_ONE, GL_ONE);

    while (1) {
        
        tracer.begin_trace(&scene);

        light->x = 2 * ((float)screen.mouse_x() - screen.width()/2) / screen.width() * 1.0 / 0.7;
        light->y = -2 * ((float)screen.mouse_y() - screen.height()/2) / screen.height() * (float)screen.height() / (float)screen.width() * 1.0 / 0.7;

        renderer.set_camera(0, 0, (float)screen.width() / (float)screen.height(), 0.7);

        renderer.begin();
        for (RayList& list : tracer.rayLists) {
            renderer.render_rays(list.rays, list.count);
        }
        renderer.end();

        screen.update_surface();

    }

    screen.close();

    return 0;
}