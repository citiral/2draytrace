#include "rayrenderer.h"
#include <stdio.h>
#include <math.h>


void RayRenderer::init(uint32_t max_rays, int width, int height)
{
    this->width = width;
    this->height = height;

    program = glCreateProgram();

    // Create vertex shader
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertexShaderSource[] =
    {
        "#version 140\n"
        "in vec2 LVertexPos2D;\n"
        "in vec3 Lcolor;\n"
        "out vec3 Fcolor;\n"
        "uniform mat3 camera;\n"
        "void main() {\n"
        "   vec2 vertex_out = (camera * vec3(LVertexPos2D.x, LVertexPos2D.y, 1)).xy;\n"
        "   Fcolor = Lcolor;\n"
        "   gl_Position = vec4( vertex_out.x, vertex_out.y, -1, 1 ); }"
    };
    glShaderSource(vshader, 1, vertexShaderSource, nullptr);
    glCompileShader(vshader);

    //Check vertex shader for errors
    GLint vshaderCompiled = GL_FALSE;
    glGetShaderiv( vshader, GL_COMPILE_STATUS, &vshaderCompiled );
    if(!vshaderCompiled)
    {
        printf("Unable to compile vertex shader!\n");
        return;
    }
    glAttachShader(program, vshader);

    
    // Create fragment shader
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragmentShaderSource[] =
    {
            "#version 140\n"
            "in vec3 Fcolor;\n"
            "out vec4 LFragment; void main() { LFragment =  vec4(Fcolor, 1); }"
    };
    glShaderSource(fshader, 1, fragmentShaderSource, nullptr);
    glCompileShader(fshader);

    //Check vertex shader for errors
    GLint fshaderCompiled = GL_FALSE;
    glGetShaderiv( fshader, GL_COMPILE_STATUS, &fshaderCompiled );
    if(!fshaderCompiled)
    {
        printf("Unable to compile fragment shader!\n");
    }
    glAttachShader(program, fshader);


    attribute_vertex = 0;
    attribute_color = 1;
    glBindAttribLocation(program, attribute_vertex, "LVertexPos2D");
    glBindAttribLocation(program, attribute_color, "Lcolor");

    //Link program
    glLinkProgram(program);
    glUseProgram(program);


    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
    if( programSuccess != GL_TRUE )
    {
        printf( "Error linking program %d!\n",program);
    }

    // Get vertex attribute location

    //VBO data
    GLfloat vertexData[] =
    {
        -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f,  0.5f,
        -0.5f,  0.5f
    };

    //IBO data
    GLuint indexData[] = { 0, 1, 2, 3 };

    // Create vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vbo
    glGenBuffers(1, &vbo_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
    glBufferData(GL_ARRAY_BUFFER, max_rays * 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attribute_vertex, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(attribute_vertex);

    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, max_rays * 6 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(attribute_color, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(attribute_color);
    
    // create renderbuffer
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB16F, width, height);

    // Create fbo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
    
}

void RayRenderer::set_camera(float x, float y, float ratio, float zoom)
{
    GLfloat matrix[3*3] = {0};
    matrix[0] = zoom;
    matrix[4] = ratio*zoom;
    matrix[8] = 1;
    matrix[2] = -x;
    matrix[5] = -y;

    GLint loc = glGetUniformLocation(program, "camera");
    glUniformMatrix3fv(loc, 1, GL_TRUE, matrix);
}

void RayRenderer::begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RayRenderer::render_rays(Ray* rays, int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
    GLfloat* vertex = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER,  GL_WRITE_ONLY);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    GLfloat* color = (GLfloat*) glMapBuffer(GL_ARRAY_BUFFER,  GL_WRITE_ONLY);

    for (int i = 0 ; i < count ; i++) {
        vertex[i*4 + 0] = rays[i].x;
        vertex[i*4 + 1] = rays[i].y;

        float dx = cosf(rays[i].angle);
        float dy = sinf(rays[i].angle);

        vertex[i*4 + 2] = rays[i].x + dx * rays[i].length;
        vertex[i*4 + 3] = rays[i].y + dy * rays[i].length;

        float bias = 1.0f / fmaxf(fabsf(dx), fabsf(dy));

        color[i*6+0] = bias * rays[i].r;
        color[i*6+1] = bias * rays[i].g;
        color[i*6+2] = bias * rays[i].b;
        color[i*6+3] = bias * rays[i].r;
        color[i*6+4] = bias * rays[i].g;
        color[i*6+5] = bias * rays[i].b;
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
    if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
        printf("Error unmapping buffer!\n");
    }


    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    if (!glUnmapBuffer(GL_ARRAY_BUFFER)) {
        printf("Error unmapping buffer!\n");
    }

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

    glDrawArrays(GL_LINES, 0, count*2);
}

void RayRenderer::end()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
