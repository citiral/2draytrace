#pragma once

#include <stdint.h>
#include <vector>
#include "ray.h"
#include "scene.h"

class RayTracer {
public:
    void begin_trace(Scene* scene);
    void trace_scene(int depth);

    void add_rays(Ray* rays, size_t count);

    std::vector<RayList> rayLists;
};