/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include "reuleauxtriangle.h"
#include "polygon.h"
#include "geom.h"

RegularConvexPolygon::RegularConvexPolygon(std::vector<Point2D> vertices) {
    vertices_ = vertices;
}

std::vector<Point2D> RegularConvexPolygon::vertices() {
    return vertices_;
}

std::vector<Line> RegularConvexPolygon::edges() {
    std::vector<Line> edges;
    for (unsigned int i = 0; i < vertices_.size()-1; i++) 
        edges.push_back(Line(vertices_[i],vertices_[i+1]));
    edges.push_back(Line(vertices_[vertices_.size()-1],vertices_[0]));
    return edges;
}

int RegularConvexPolygon::vertexCount() {
    return vertices_.size();
}

bool RegularConvexPolygon::containedWithin(Circle &circle) {
    for (Point2D const &vertex : vertices())
    {
        if(Geom::distance(vertex, circle.center()) > circle.radius())
        {
            return false;
        }
    }
    return true;
}

bool RegularConvexPolygon::containedWithin(RegularConvexPolygon &polygon) {
    Point2D innerCenter = Geom::center(*this);
    Point2D outerCenter = Geom::center(polygon);
    Line oiEdge =  Line(innerCenter, outerCenter);
    for (Line const &outerEdge: polygon.edges())
    {
        for (Line const &innerEdge: this->edges())
        {
            if (Geom::intersects(innerEdge, outerEdge))
            {
                return false;
            }
        }
        if(Geom::intersects(oiEdge, outerEdge))
        {
            return false;
        }
    }
    return true;
}

bool RegularConvexPolygon::containedWithin(ReuleauxTriangle &rt) {
    Point2D *vertices_ = rt.vertices();
    Point2D one = vertices_[0];
    Point2D two = vertices_[1];
    double radius = Geom::distance(one, two);
    std::vector<Point2D> tempPoints;
    int j;
    for(j = 0; j < 3; j++)
    {
        tempPoints.push_back(vertices_[j]);
    }
    RegularConvexPolygon temp = RegularConvexPolygon( tempPoints );
    int i;
    for(i = 0; i < 3; i++)
    {
        // Line intersects with circle
        Circle outer = Circle(vertices_[i], radius);
        for(Point2D const &vertex : this->vertices())
        {
            if(Geom::distance(vertex, outer.center()) > outer.radius())
            {
                return false;
            }
        }
    }
    return true;
}