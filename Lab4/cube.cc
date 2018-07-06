/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include "cube.h"
#include "sphere.h"
#include "reuleauxtetrahedron.h"
#include "geom.h"


Cube::Cube(Point3D upperFace[4], Point3D lowerFace[4]) 
{
    upperFace_ = upperFace;
    lowerFace_ = lowerFace;
}

Point3D* Cube::verticesUpper() 
{
    return upperFace_;
}

Point3D* Cube::verticesLower() 
{
    return lowerFace_;
}

/*std::vector<Line> Cube::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < vertices_.size()-1; i++) 
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[vertices_.size()-1],vertices_[0]));
    return edges;
}*/

bool Cube::containedWithin(Sphere &sphere) 
{
    return false;
}

bool Cube::containedWithin(Cube &cube) 
{
    return false;
}

bool Cube::containedWithin(ReuleauxTetrahedron &rTetra) 
{
    return false;
}