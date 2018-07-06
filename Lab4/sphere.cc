/*
 * sphere.cc
 */

#include <iostream>
#include <vector>

#include "sphere.h"
#include "cube.h"
#include "reuleauxtetrahedron.h"
#include "geom.h"

Sphere::Sphere(const Point3D center, double radius) 
{
    setCenter(center);
    setRadius(radius);
}

Point3D Sphere::center() 
{
    return center_;
}

void Sphere::setCenter(const Point3D &center) 
{
    center_.x = center.x;
    center_.y = center.y;
    center_.z = center.z;
}

double Sphere::radius() 
{
    return radius_;
}

void Sphere::setRadius(double radius) 
{
    radius_ = radius;
}

bool Sphere::containedWithin(Sphere &sphere) 
{
    return false;
}

bool Sphere::containedWithin(Cube &cube) 
{
    return false;
}

bool Sphere::containedWithin(ReuleauxTetrahedron &rTetra) 
{
    return false;
}