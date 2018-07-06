#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bounds.h"

static Shape *a;

/*
 * Intialise a two or thre dimensional space of shape ARENA containing NUMSHAPES 
 * SHAPES, all of which may or may not be entirely contained within ARENA.
 *
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=785d50a3-c653-46b8-b9f6-0c468e959ac7
 * - For circle in circle/Trivial test
 * https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon?noredirect=1&lq=1
 * - Idea of how to check if point is in polygon (complements my implementation of intersection)
 */
void setup(Shape *arena, Shape *shapes[], int numShapes) {
    a = arena;
}

/*
 * Helper Methods for functions that check for shapes in shapes
 */
static int intersection(Circle *inner, Point one, Point two)
{
	float circX = inner->center.x;
	float circY = inner->center.y;
	//printf("Circle: (%f, %f)\n", circX, circY);

	float x0 = one.x;
	float y0 = one.y;
	float x1 = two.x;
	float y1 = two.y;
	//printf("Points: (%f, %f) & (%f, %f)\n", x0, y0, x1, y1);

	float distPtToLine =(abs((y1-y0)*circX - (x1-x0)*circY + x1*y0 - y1*x0)
						/sqrt(pow(y1-y0,2) + pow(x1-x0,2)));

	//printf("dist: %f\n", distPtToLine);

	// If dist == 0 then 2 else if dist == radius then 1 
	// else if dist > 0 and dist < inner-> radius == 2
	if(distPtToLine > inner->radius)
	{
		return 0;
	}
	else if(distPtToLine == inner->radius)
	{
		return 1;
	}
	else if(distPtToLine == 0)
	{
		return 2;
	}
	// Adds too many intersections if point is in corner
	/*else if(distPtToLine > 0 && distPtToLine < inner->radius)
	{
		return 2;
	} */
	// Should never return a negative distance but does return if 0 < dist < radius
	printf("Fail Safe\n");
	return 0;
}

/*
 * Checks if the center of a circle is in the polygon
 */
bool inOrOut(Circle* inner, Point *arrPoint, int numVertices)
{
	float circX = inner->center.x;
	float circY = inner->center.y;
	float minX = INFINITY;
	float minY = INFINITY;
	float maxX = -INFINITY;
	float maxY = -INFINITY;

	int i;
	for(i = 0; i < numVertices; i++)
	{
		if(arrPoint[i].x < minX)
		{
			minX = arrPoint[i].x;
		}
		if(arrPoint[i].y < minY)
		{
			minY = arrPoint[i].y;
		}
		if(arrPoint[i].x > maxX)
		{
			maxX = arrPoint[i].x;
		}
		if(arrPoint[i].y > maxY)
		{
			maxY = arrPoint[i].y;
		}
	}

	//printf("Circle: (%f, %f)\n", circX, circY);
	//printf("minX: %f minY: %f maxX: %f maxY: %f\n", minX, minY, maxX, maxY);
	if(circX-inner->radius < minX || circX+inner->radius > maxX || 
		circY-inner->radius < minY || circY+inner->radius > maxY)
	{
		return false;
	}
	return true;
}

/*
 * Methods that the move function calls
 */
static bool isCircleInsideCircle(Circle *outer, Circle *inner)
{
	float distance = sqrt(
		pow((outer->center.x - inner->center.x),2) +
		pow((outer->center.y - inner->center.y),2));
	return distance <= outer->radius - inner->radius;
}

static bool isCircleInPolygon(Polygon *area, Circle *inner)
{
	int i;
	Point arrPoint[10];
	for(i = 0; i < area->numVertices; i++)
    {
    	arrPoint[i] = area->vertices[i];
    	//printf("Point (%f, %f)\n", arrPoint[i].x, arrPoint[i].y);
    }
    //printf("%f\n", arrPoint[0].x);

    int j;
    int intersectPts = 0;
    for(j = 0; j < area->numVertices; j++)
    {
    	if(j == area->numVertices-1)
    	{
    		intersectPts += intersection(inner, arrPoint[area->numVertices-1], arrPoint[0]);
    	}
    	else
    	{
    		intersectPts += intersection(inner, arrPoint[j], arrPoint[j+1]);
    	}
    }

    printf("Intersection Points: %d\n", intersectPts);
    if(intersectPts == 2)
    {
    	// Guaranteed to be inside and outside of the shape
    	return false;
    }
    else
    {
    	// For intersect 0 and 1 check if point lies in a plane
    	return inOrOut(inner, arrPoint, area->numVertices);
    }
    printf("Fail Safe IP\n");
	return true;
}

static bool isPolygonInCircle(Circle *outer, Polygon *inner)
{
	int i;
	for(i = 0; i < inner->numVertices; i++)
	{
		printf("newX: %f newY: %f\n", inner->vertices[i].x, inner->vertices[i].y);
		float distance = sqrt(
			pow((outer->center.x - inner->vertices[i].x),2) +
			pow((outer->center.y - inner->vertices[i].y),2));
		//printf("radius: %f\n", outer->radius);
		//printf("dist: %f\n", distance);
		if(distance > outer->radius)
		{
			return false;
		}
	}
	return true;
}

/*
 * Move SHAPE to POINT. 
 *
 * Return TRUE if SHAPE remains entirely within the space defined by the 
 * ARENA parameter of setup(), FALSE otherwise.
 */
bool move(Shape *shape, Point *point) {
    if(a->type == CIRCLE && shape->type == CIRCLE)
    {
    	Circle * inner = (Circle *) shape;
    	inner->center.x = point->x;
    	inner->center.y = point->y;
    	return isCircleInsideCircle((Circle *) a, inner);
    }
    else if(a->type == POLYGON && shape->type == CIRCLE)
    {
    	Circle *circ = (Circle *) shape;
    	circ->center.x = point->x;
    	circ->center.y = point->y;
    	return isCircleInPolygon((Polygon *)a, circ);
    }
    else if(a->type == CIRCLE && shape->type == POLYGON)
    {
    	Polygon *poly = (Polygon *) shape;
    	int i;
    	float arrX[10];
    	float arrY[10];
    	for(i = 0; i < poly->numVertices; i++)
    	{
    		arrX[i] = poly->vertices[i].x;
    		arrY[i] = poly->vertices[i].y;
    	}
    	int j;
    	for(j = 0; j < poly->numVertices; j++)
    	{
    		poly->vertices[j].x = arrX[j] + point->x;
    		poly->vertices[j].y = arrY[j] + point->y;
    	}
    	//printf("oldX: %f oldY: %f\n", poly->vertices[i].x, poly->vertices[i].y);
    	return isPolygonInCircle((Circle *) a, poly);
    }
    return true;
}