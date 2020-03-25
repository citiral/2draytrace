#pragma once

#include <gl\glew.h>
#include <gl\glu.h>
#include <stdint.h>
#include "ray.h"

class RayRenderer {
public:
    void init(uint32_t max_rays, int width, int height);

    void set_camera(float x, float y, float ratio, float zoom);

    void begin();
    void render_rays(Ray* rays, int count);
    void end();

private:
    GLuint program;
    GLuint attribute_vertex;
    GLuint attribute_color;
    GLuint vbo_vertex;
    GLuint vbo_color;
    GLuint ibo;
    GLuint vao;

    GLuint fbo;
    GLuint renderbuffer;

    int width;
    int height;
};