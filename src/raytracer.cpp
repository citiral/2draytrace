#include "raytracer.h"
#include <stdio.h>

void RayTracer::begin_trace(Scene* scene)
{
    rayLists = scene->generate_rays();
}

void RayTracer::trace_scene(int depth)
{

}

void RayTracer::add_rays(Ray* rays, size_t count)
{
    RayList list;
    list.count = count;
    list.rays = rays;
    
    rayLists.push_back(list);
}