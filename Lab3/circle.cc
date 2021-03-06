/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

#include <iostream>
#include <vector>

#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"
#include "geom.h"

Circle::Circle(const Point2D &center, double radius) 
{
    setCenter(center);
    setRadius(radius);
}

Point2D Circle::center() 
{
    return center_;
}

void Circle::setCenter(const Point2D &center) 
{
    center_.x = center.x;
    center_.y = center.y;
}

double Circle::radius() 
{
    return radius_;
}

void Circle::setRadius(double radius) 
{
    radius_ = radius;
}

/*
 * If distance between centers is less than difference between the radius of containing 
 * circle and the radius of this circle, this circle is not contained
 */
bool Circle::containedWithin(Circle &circle) 
{
    return Geom::distance(center(), circle.center()) <= (circle.radius() - radius());
}

/*
 * If circle intersects with any polygon edge, circle is not contained       
 * If line between centers intersects with any polygon edge, circle is not contained
 * Only other possibility is that the circle is contained
 */
bool Circle::containedWithin(RegularConvexPolygon &polygon) 
{
    for (Line const &edge: polygon.edges()) {
        if (Geom::intersects(edge, *this)) 
            return false;
    }

    Line line(Geom::center(polygon), center_);
    for (Line const &edge: polygon.edges()) {
        if (Geom::intersects(edge, line))
            return false;
    }

    return true;
}

bool Circle::containedWithin(ReuleauxTriangle &rt) 
{
    // Circle intersects with circle (Circle and Circle)
    // Otherwise return true
    Point2D *vertices_ = rt.vertices();
    Point2D one = vertices_[0];
    Point2D two = vertices_[1];
    double radius = Geom::distance(one, two);
    int i;
    for(i = 0; i < 3; i++)
    {
        // Not contained 
        // Intersecting
        // Else true; which is contained by the circle
        Circle outer = Circle(vertices_[i], radius);
        if (Geom::distance(this->center(), outer.center()) > (outer.radius() - this->radius()))
        {
            return false;
        }
    }
    return true;
}