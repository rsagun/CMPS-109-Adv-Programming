/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=956436f9-b748-4ab7-b7b7-2e27605f1667
 * - Circle Testing Live Demo
 */

#include <iostream>

#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"

static void test(std::string msg, bool expected, bool returned)
{
	std::cout << msg;
	if(expected == returned)
	{
		std::cout << "PASS\n";
	}
	else
	{
		std::cout << "FAIL\n";
	}
}


/*
 * Testing circles in different shapes
 */
static void testCirclesInCircles()
{
	// Test for circle contained in circle
	Circle inner = Circle(Point2D(0.0,0.0), 4.0);
    Circle outer = Circle(Point2D(0.0,0.0), 8.0); 
    test("Circle-in-Circle ", true, inner.containedWithin(outer));

    //Test for circle intersect circle
    inner = Circle(Point2D(5.0,5.0), 4.0);
    test("Circle-intersect-Circle ", false, inner.containedWithin(outer));

    inner = Circle(Point2D(-5.0,5.0), 4.0);
    test("Circle-intersect-Circle ", false, inner.containedWithin(outer));

    inner = Circle(Point2D(5.0,-5.0), 4.0);
    test("Circle-intersect-Circle ", false, inner.containedWithin(outer));

    inner = Circle(Point2D(-5.0,-5.0), 4.0);
    test("Circle-intersect-Circle ", false, inner.containedWithin(outer));

    // Test for circle outside of circle
    inner = Circle(Point2D(12.0,12.0), 4.0);
    test("Circle-outside-Circle ", false, inner.containedWithin(outer));

    inner = Circle(Point2D(-12.0,12.0), 4.0);
    test("Circle-outside-Circle ", false, inner.containedWithin(outer));

    inner = Circle(Point2D(12.0,-12.0), 4.0);
    test("Circle-outside-Circle ", false, inner.containedWithin(outer));

    inner = Circle(Point2D(-12.0,-12.0), 4.0);
    test("Circle-outside-Circle ", false, inner.containedWithin(outer));
}

static void testCirclesInTriangles()
{
	RegularConvexPolygon outer = RegularConvexPolygon({ Point2D(0, -7), Point2D(7, 7), Point2D(-7 ,7) });

	// Test for circle in triangle
	Circle inner = Circle(Point2D(0.0,0.0), 1.0);
	test("Circle-in-Triangle ", true, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,0.0), 2.0);
	test("Circle-in-Triangle ", true, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,0.0), 3.0);
	test("Circle-in-Triangle ", true, inner.containedWithin(outer));

	// Test for circle intersecting triangle
	inner = Circle(Point2D(-7.0,0.0), 1.0);
	test("Circle-in-Triangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(7.0,0.0), 1.0);
	test("Circle-in-Triangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,-7.0), 1.0);
	test("Circle-in-Triangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,7.0), 1.0);
	test("Circle-in-Triangle ", false, inner.containedWithin(outer));
}

static void testCirclesInRectangles()
{
	RegularConvexPolygon outer = RegularConvexPolygon({ Point2D(-5,5), Point2D(5, 5), Point2D(5 ,-5), Point2D(-5,-5) });

	// Test for circle in rectangle
	Circle inner = Circle(Point2D(0.0,0.0), 2.0);
	test("Circle-in-Rectangle ", true, inner.containedWithin(outer));

	inner = Circle(Point2D(-2.0,0.0), 2.0);
	test("Circle-in-Rectangle ", true, inner.containedWithin(outer));

	inner = Circle(Point2D(2.0,0.0), 2.0);
	test("Circle-in-Rectangle ", true, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,-2.0), 2.0);
	test("Circle-in-Rectangle ", true, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,2.0), 2.0);
	test("Circle-in-Rectangle ", true, inner.containedWithin(outer));

	// Test for circle intersecting triangle
	inner = Circle(Point2D(-4.0,0.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(4.0,0.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,-4.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,4.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));

	// Test for circle outside rectangle
	inner = Circle(Point2D(-7.0,0.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(7.0,0.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,-7.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(0.0,7.0), 2.0);
	test("Circle-in-Rectangle ", false, inner.containedWithin(outer));
}

static void testCirclesInReuleauxTriangle()
{
	Point2D arr[3] = { Point2D(0, 0), Point2D(3, 5), Point2D(6, 0) };
	ReuleauxTriangle outer = ReuleauxTriangle(arr);

	//Test for Circle contained in RT
	Circle inner = Circle(Point2D(3, 1.5), 1);
	test("Circle-in-RT ", true, inner.containedWithin(outer));

	inner = Circle(Point2D(3, 1.5), 2);
	test("Circle-in-RT ", true, inner.containedWithin(outer));

	// Test for Circle intersect RT
	inner = Circle(Point2D(3, 1.5), 3);
	test("Circle-in-RT ", false, inner.containedWithin(outer));

	inner = Circle(Point2D(3, 1.5), 4);
	test("Circle-in-RT ", false, inner.containedWithin(outer));

	// Test for Circle outside RT
	inner = Circle(Point2D(-5, -5), 2);
	test("Circle-in-RT ", false, inner.containedWithin(outer));
}


/*
 * Testing polygons in different shapes
 */
static void testTrianglesInCircles()
{
	Circle outer = Circle(Point2D(0.0,0.0), 8.0);

	// Test for polygon in circle
	RegularConvexPolygon inner = RegularConvexPolygon({ Point2D(-1, -1), Point2D(0, 2), Point2D(1 ,-1) });
	test("Triangle-In-Circle: ", true, inner.containedWithin(outer));

	// Test for polygon intersect circle
	inner = RegularConvexPolygon({ Point2D(-8, 0), Point2D(-8, 5), Point2D(-10, 5) });
	test("Triangle-In-Circle: ", false, inner.containedWithin(outer));

	// Test for polygon outside circle
	inner = RegularConvexPolygon({ Point2D(-9, 0), Point2D(-9, 5), Point2D(-12, 5) });
	test("Triangle-In-Circle: ", false, inner.containedWithin(outer));
}

static void testTrianglesInRectangles()
{
	RegularConvexPolygon outer = RegularConvexPolygon({ Point2D(-6,6), Point2D(6, 6), Point2D(6 ,-6), Point2D(-6,-6) });
	// Test for polygon in polygon
	RegularConvexPolygon inner = RegularConvexPolygon({ Point2D(-4, -4), Point2D(-4, 4), Point2D(4, 4) });
	test("Triangle-in-Polygon: ", true, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(-4, -4), Point2D(-4, 4), Point2D(4, -4) });
	test("Triangle-in-Polygon: ", true, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(-4, -4), Point2D(4, 4), Point2D(4, -4) });
	test("Triangle-in-Polygon: ", true, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(-4, 4), Point2D(4, 4), Point2D(4, -4) });
	test("Triangle-in-Polygon: ", true, inner.containedWithin(outer));

	// Test for polygon intersect polygon
	inner = RegularConvexPolygon({ Point2D(-6, -6), Point2D(-6, 6), Point2D(6, 6) });
	test("Triangle-in-Polygon: ", false, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(-6, -6), Point2D(-6, 6), Point2D(6, -6) });
	test("Triangle-in-Polygon: ", false, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(-6, -6), Point2D(6, 6), Point2D(6, -6) });
	test("Triangle-in-Polygon: ", false, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(-6, 6), Point2D(6, 6), Point2D(6, -6) });
	test("Triangle-in-Polygon: ", false, inner.containedWithin(outer));
	
	// Test for polygon outside polygon
	inner = RegularConvexPolygon({ Point2D(-7, 0), Point2D(-7, 5), Point2D(-10, 5) });
	test("Triangle-in-Polygon: ", false, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(7, 0), Point2D(7, 5), Point2D(10, 5) });
	test("Triangle-in-Polygon: ", false, inner.containedWithin(outer));
}

static void testTrianglesInReuleauxTriangles()
{
	Point2D arr[3] = { Point2D(0, 0), Point2D(3, 5), Point2D(6, 0) };
	ReuleauxTriangle outer = ReuleauxTriangle(arr);

	// Test for polygon in RLT
	RegularConvexPolygon inner = RegularConvexPolygon({ Point2D(1, 1), Point2D(2, 2), Point2D(4, 1) });
	test("Polygon-in-RT: ", true, inner.containedWithin(outer));

	inner = RegularConvexPolygon({ Point2D(1, 1), Point2D(2, 2), Point2D(4, 1), Point2D(4, 2) });
	test("Polygon-in-RT: ", true, inner.containedWithin(outer));

	// Test for polygon intersect RLT
	inner = RegularConvexPolygon({ Point2D(-1, 1), Point2D(2, 2), Point2D(4, 1) });
	test("Polygon-in-RT: ", false, inner.containedWithin(outer));

	// Test for polygon outside RLT
	inner = RegularConvexPolygon({ Point2D(-5, 5), Point2D(-5, 1), Point2D(-3, 1) });
	test("Polygon-in-RT: ", false, inner.containedWithin(outer));
}

static void testReuleauxTriangleInCircle()
{
	Circle outer = Circle(Point2D(0.0,0.0), 10.0);

	// Test for RT in Circle
	Point2D arr[3] = { Point2D(-4, 0), Point2D(0, 4), Point2D(4, 0) };
	ReuleauxTriangle inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Circle ", true, inner.containedWithin(outer));


	arr[0] = Point2D(-2, 0);
	arr[1] = Point2D(0, 2);
	arr[2] = Point2D(2, 0);
	inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Circle ", true, inner.containedWithin(outer));

	// Test for RT intersect Circle
	arr[0] = Point2D(-6, 0);
	arr[1] = Point2D(0, 6);
	arr[2] = Point2D(6, 0);
	inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Circle ", false, inner.containedWithin(outer));

	// Test for RT outside Circle
	outer = Circle(Point2D(10,0.0), 2.0);
	arr[0] = Point2D(-2, 0);
	arr[1] = Point2D(0, 2);
	arr[2] = Point2D(2, 0);
	inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Circle ", false, inner.containedWithin(outer));

}

static void testReuleauxTriangleInPolygon()
{
	RegularConvexPolygon outer = RegularConvexPolygon({ Point2D(-15, -15), Point2D(-15, 15), Point2D(15, 15), Point2D(15, -15) });

	// Test for RT in Polygon
	Point2D arr[3] = { Point2D(-10, 0), Point2D(0, 10), Point2D(10, 0) };
	ReuleauxTriangle inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Polygon ", true, inner.containedWithin(outer));

	arr[0] = Point2D(-5, 0);
	arr[1] = Point2D(0, 5);
	arr[2] = Point2D(5, 0);
	inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Polygon ", true, inner.containedWithin(outer));

	// Test RT intersect Polygon
	arr[0] = Point2D(16, 16);
	arr[1] = Point2D(-16, 16);
	arr[2] = Point2D(16, -16);
	inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Polygon ", false, inner.containedWithin(outer));

	// Test RT outside of Polygon
	arr[0] = Point2D(-16, 5);
	arr[1] = Point2D(-16, 1);
	arr[2] = Point2D(-18, 3);
	inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-Polygon ", false, inner.containedWithin(outer));
}

static void testReuleauxTriangleInRT()
{

	Point2D arr[3] = { Point2D(-10, 0), Point2D(0, 10), Point2D(10, 0) };
	ReuleauxTriangle outer = ReuleauxTriangle(arr);

	// Test RT in RT
	Point2D arrI[3] = { Point2D(-1, 0), Point2D(0, 1), Point2D(1, 0) };
	ReuleauxTriangle inner = ReuleauxTriangle(arrI);
	test("ReuleauxTriangle-in-RT ", true, inner.containedWithin(outer));

	// Test RT intersect RT
	arr[0] = Point2D(-11, 0);
	arr[1] = Point2D(0, 11);
	arr[2] = Point2D(11, 0);
	inner = ReuleauxTriangle(arr);
	test("ReuleauxTriangle-in-RT ", false, inner.containedWithin(outer));
}

int main(int argc, char *argv[]) 
{
	testCirclesInCircles();		// Circle in Circle
	testCirclesInTriangles();	// Circle in Polygon
	testCirclesInRectangles();	// Circle in Polygon
	testCirclesInReuleauxTriangle(); // Circle in RT

	testTrianglesInCircles();		// Polygon in Circle
	testTrianglesInRectangles();	// Polygon in Polygon
	testTrianglesInReuleauxTriangles(); // Polygon in RT

	testReuleauxTriangleInCircle(); // RT in Circle
	testReuleauxTriangleInPolygon(); // RT in Polygon
	testReuleauxTriangleInRT();		// RT in RT
}
