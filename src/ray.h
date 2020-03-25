#ifndef RAY_H
#define RAY_H

class Ray {
public:
    float x;
    float y;
    float angle;
    float length;
    float r;
    float g;
    float b;
};

class RayList {
public:
    size_t count;
    Ray* rays;
};

#endif