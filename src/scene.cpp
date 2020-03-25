#include "scene.h"
#include "math.h"
#include <stdio.h>

PointLight::PointLight(float x, float y, float r, float g, float b, float intensity): x(x), y(y), r(r * intensity), g(g * intensity), b(b * intensity) {
    rayCount = 50003;
    rays = new Ray[rayCount];
}

PointLight::~PointLight() {
    delete rays;
}

RayList PointLight::generate_rays() {
    RayList l;
    l.rays = rays;
    l.count = rayCount;
    
    float step = ((float)1 / (float)rayCount);

    for (size_t i = 0 ; i < rayCount ; i++) {
        rays[i].x = x;
        rays[i].y = y;
        rays[i].length = 10.f;
        rays[i].angle = step * 2.f * M_PI * i;
        rays[i].r = r * step;
        rays[i].g = g * step;
        rays[i].b = b * step;
    }

    return l;
}

Scene::Scene() {

}

Scene::~Scene() {
    for (Light* light : _lights) {
        delete light;
    }
}

PointLight* Scene::add_point_light(float x, float y, float r, float g, float b, float intensity) {
    PointLight* light = new PointLight(x, y, r, g, b, intensity);

    _lights.push_back(light);

    return light;
}

std::vector<RayList> Scene::generate_rays() {
    std::vector<RayList> rays;

    for (Light* light : _lights) {
        rays.push_back(light->generate_rays());
    }

    return rays;
}
