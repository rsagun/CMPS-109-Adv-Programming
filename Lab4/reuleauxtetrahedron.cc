#include <iostream>
#include <vector>

#include "cube.h"
#include "sphere.h"
#include "reuleauxtetrahedron.h"
#include "geom.h"

ReuleauxTetrahedron::ReuleauxTetrahedron(Point3D vertices[4])
{
	vertices_ = vertices;
}

Point3D* ReuleauxTetrahedron::vertices()
{
	return vertices_;
}


bool ReuleauxTetrahedron::containedWithin(Sphere &sphere) 
{
    return false;
}

bool ReuleauxTetrahedron::containedWithin(Cube &cube) 
{
    return false;
}

bool ReuleauxTetrahedron::containedWithin(ReuleauxTetrahedron &rTetra)
{
    return false;
}