#include <iostream>
#include <vector>

#include "polygon.h"
#include "circle.h"
#include "reuleauxtriangle.h"
#include "geom.h"

ReuleauxTriangle::ReuleauxTriangle(Point2D vertices[3])
{
	vertices_ = vertices;
}

Point2D* ReuleauxTriangle::vertices()
{
	return vertices_;
}


bool ReuleauxTriangle::containedWithin(Circle &circle) 
{
    Point2D *innerVertices_ = this->vertices();
    double radius = Geom::distance(innerVertices_[0], innerVertices_[1]);
    Circle one = Circle(innerVertices_[0], radius);
    Circle two = Circle(innerVertices_[1], radius);
    Circle three = Circle(innerVertices_[2], radius);

    if(one.containedWithin(circle) && two.containedWithin(circle) && three.containedWithin(circle))
    {
        return true;
    }
    return false;
}

bool ReuleauxTriangle::containedWithin(RegularConvexPolygon &polygon) 
{
    Point2D *vertices_ = this->vertices();
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
    Point2D centerTri = Geom::center(temp);
    Line line(Geom::center(polygon), centerTri);
    int i;
    for(i = 0; i < 3; i++)
    {
        // Line intersects with circle
        Circle inner = Circle(vertices_[i], radius);
        for(Point2D const &vertex : polygon.vertices())
        {
            if(Geom::distance(vertex, inner.center()) < inner.radius())
            {
                return false;
            }
        }
        // Checks for rt outside poly
        for (Line const &edge: polygon.edges()) 
        {
        	if (Geom::intersects(edge, line))
        	{
            	return false;
        	}
    	}
    }
    return true;
}

bool ReuleauxTriangle::containedWithin(ReuleauxTriangle &rt)
{
	// Circle intersects with circle (Circle and Circle)
    // Otherwise return true
    Point2D *outerVertices_ = rt.vertices();
    Point2D oOne = outerVertices_[0];
    Point2D oTwo = outerVertices_[1];
    double outerRadius = Geom::distance(oOne, oTwo);

    Circle one = Circle(outerVertices_[0], outerRadius);
    Circle two = Circle(outerVertices_[1], outerRadius);
    Circle three = Circle(outerVertices_[2], outerRadius);

    if(this->containedWithin(one) && this->containedWithin(two) && this->containedWithin(three))
    {
        return true;
    }
    return false;
}