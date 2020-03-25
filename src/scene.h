#ifndef SCENE_H
#define SCENE_H

#include <stdint.h>
#include <vector>
#include <list>
#include "ray.h"

class Light {
public:
    Light() {};
    virtual ~Light() {};
    virtual RayList generate_rays() = 0;
};

class PointLight : public Light {
public:
    PointLight(float x, float y, float r, float g, float b, float intensity);
    ~PointLight();
    RayList generate_rays();

    float x;
    float y;
    float r;
    float g;
    float b;
    
private:
    Ray *rays;
    size_t rayCount;
};

class Scene {
public:
    Scene();
    ~Scene();

    PointLight* add_point_light(float x, float y, float r, float g, float b, float intensity);

    std::vector<RayList> generate_rays();

private:
    std::list<Light*> _lights;
};

#endif