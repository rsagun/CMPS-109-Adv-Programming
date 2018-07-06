/*
 * Copyright (C) 2018 David C. Harrison. All right reserved.
 *
 * You may not use, distribute, publish, or modify this code without 
 * the express written permission of the copyright holder.
 */

/* Base code and format from: 
 * https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=2be08498-8350-4514-b8fe-06e265820e39
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <string>

#include "circle.h"
#include "polygon.h"
#include "reuleauxtriangle.h"
#include "sphere.h"
#include "cube.h"
#include "reuleauxtetrahedron.h"

#define USAGE "USAGE %s test-file\n"

static Point2D get2DPoint(std::string &token)
{
	double x,y;
	std::stringstream vstream(token);
	std::string value;
	if(std::getline(vstream, value, ','))
	{
		// if not double throw exception
		std::stringstream(value) >> x;
		if(std::getline(vstream, value, ','))
		{
			std::stringstream(value) >> y;
			return Point2D(x,y);
		}
	}
	throw "invalid point " + token;
}

static Point3D get3DPoint(std::string &token)
{
	double x,y,z;
	std::stringstream vstream(token);
	std::string value;
	if(std::getline(vstream, value, ','))
	{
		std::stringstream(value) >> x;
		if(std::getline(vstream, value, ','))
		{
			std::stringstream(value) >> y;
			if(std::getline(vstream, value, ','))
			{
				std::stringstream(value) >> z;
				return Point3D(x,y,z);
			}
		}
	}
	throw "invalid point " + token;
}

static double getRadius(std::string &token)
{
	return std::stod(token);
}

static void parse(const std::string line)
{
	std::stringstream stream(line);
	std::string token;
	std::string note;

	// Add More Variables later for each shape
	//int i = 0; // Use to check if the inner has been found already

	Circle *cInner = NULL;
	Circle *cOuter = NULL;

	RegularConvexPolygon *pInner = NULL;
	RegularConvexPolygon *pOuter = NULL;

	ReuleauxTriangle *rtInner = NULL;
	ReuleauxTriangle *rtOuter = NULL;

	Sphere *sInner = NULL;
	Sphere *sOuter = NULL;

	Cube *cubeInner = NULL;
	Cube *cubeOuter = NULL;

	ReuleauxTetrahedron *rtetInner = NULL;
	ReuleauxTetrahedron *rtetOuter = NULL;

	std::string first = "";
	std::string second = "";

	bool expected = false;

	while(stream >> token)
	{
		/* 
		 *  Geometry building for 2D shapes
		 */
		//std::cout << token << std::endl;
		if(token == "Circle")
		{
			stream >> token;	//gets next token from parsed line passed in
			Point2D center = get2DPoint(token);
			stream >> token;
			double radius = getRadius(token);
			if(first == "")	// If first not created then create it
			{
				cInner = new Circle(center, radius);
				first = "Circle";
			}
			else // If first created then create the second
			{
				cOuter = new Circle(center, radius);
				second = "Circle";
			}
		}
		else if(token == "RegularConvexPolygon")
		{
			std::vector<Point2D> vertices; // create vector to hold points for constructor
			char * buffer = new char[10];
			while(stream >> token)	// Extracts consecutive tokens in order
			{
				Point2D vertex = get2DPoint(token);
				vertices.push_back(vertex);
				if(stream.peek() == 32)
				{
					stream.read(buffer,1);
					if(stream.peek() == 67 || stream.peek() == 80 || stream.peek() == 82 || stream.peek() == 102 || stream.peek() == 116)
					{
						break;
					}
				}
			}

			if(first == "")
			{
				pInner = new RegularConvexPolygon(vertices);
				first = "Polygon";
			}
			else
			{
				pOuter = new RegularConvexPolygon(vertices);
				second = "Polygon";
			}
		}
		else if(token == "ReuleauxTriangle")
		{
			Point2D vector[3];
			int i;
			for(i = 0; i < 3; i++)
			{
				stream >> token;
				//std::cout << "token " << token << std::endl;
				Point2D vertex = get2DPoint(token);
				vector[i] = vertex;
			}
			if(first == "")
			{
				//std::cout << vector[0].x << " " << vector[0].y << std::endl;
				//std::cout << vector[1].x << " " << vector[1].y << std::endl;
				//std::cout << vector[2].x << " " << vector[2].y << std::endl;
				//std::cout << "First RT" << std::endl;
				rtInner = new ReuleauxTriangle(vector);
				first = "ReuleauxTriangle";
			}
			else
			{
				//std::cout << vector[0].x << " " << vector[0].y << std::endl;
				//std::cout << vector[1].x << " " << vector[1].y << std::endl;
				//std::cout << vector[2].x << " " << vector[2].y << std::endl;
				//std::cout << "Second RT" << std::endl;
				rtOuter = new ReuleauxTriangle(vector);
				second = "ReuleauxTriangle";
			}
		}
		/* 
		 *	Geometry building for 3D shapes
		 */
		else if(token == "Sphere")
		{
			stream >> token;	//gets next token from parsed line passed in
			Point3D center = get3DPoint(token);
			stream >> token;
			double radius = getRadius(token);
			if(first == "")
			{
				//std::cout << "First Sphere" << std::endl;
				sInner = new Sphere(center, radius);
				first = "Sphere";
			}
			else
			{
				//std::cout << "Second Sphere" << std::endl;
				sOuter = new Sphere(center, radius);
				second = "Sphere";
			}
		}
		else if(token == "Cube")
		{
			Point3D upperFace[4];
			Point3D lowerFace[4];
			int i,j;
			for(i = 0; i < 4; i++)
			{
				stream >> token;
				//std::cout << "UPPER " << token << std::endl;
				Point3D vertex = get3DPoint(token);
				upperFace[i] = vertex;
			}
			for(j = 0; j < 4; j++)
			{
				stream >> token;
				//std::cout << "LOWER " << token << std::endl;
				Point3D vertex = get3DPoint(token);
				lowerFace[j] = vertex;
			}
			if(first == "")
			{
				//std::cout << "First Cube" << std::endl;
				cubeInner = new Cube(upperFace, lowerFace);
				first = "Cube";
			}
			else
			{
				//std::cout << "First Cube" << std::endl;
				cubeOuter = new Cube(upperFace, lowerFace);
				second = "Cube";
			}
		}
		else if(token == "ReuleauxTetrahedron")
		{
			Point3D vertices[4];
			int i;
			for(i = 0; i < 4; i++)
			{
				stream >> token;
				//std::cout << "RTT " << token << std::endl;
				Point3D vertex = get3DPoint(token);
				vertices[i] = vertex;
			}
			if(first == "")
			{
				//std::cout << "First Rtetra" << std::endl;
				rtetInner = new ReuleauxTetrahedron(vertices);
				first = "Rtet";
			}
			else
			{
				//std::cout << "Second Rtetra" << std::endl;
				rtetOuter = new ReuleauxTetrahedron(vertices);
				second = "Rtet";
			}
		}
		/*
		 *	Parsing tokens that are booleans or not valid
		 */
		else if(token == "true")
		{
			expected = true;
		}
		else
		{
			note.append(token);
			note.append(" ");
			//std::cout << "Nothing Happened" << std::endl;
		}
	}

	/*
	 *	Checking for invalid input
	 */
	if(first == "Circle" || first == "Polygon" || first == "ReuleauxTriangle")
	{
		if(second == "Sphere" || second == "Cube" || second == "ReuleauxTetrahedron")
		{
			std::cout << "Invalid Shape Configuration" << std::endl;
		}
	}
	else 
	{
		if(second == "Circle" || second == "Polygon" || second == "ReuleauxTriangle")
		{
			std::cout << "Invalid Shape Configuration" << std::endl;
		}
	}

	/*
	 *	Choosing which test to implement and return
	 */
	if(first == "Circle")
	{
		if(second == "Circle")
		{
			//std::cout << "Circle Circle" << std::endl;
			bool got = cInner->containedWithin(*cOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Polygon")
		{
			//std::cout << "Circle Poly" << std::endl;
			bool got = cInner->containedWithin(*pOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "ReuleauxTriangle")
		{
			//std::cout << "Circle RT" << std::endl;
			bool got = cInner->containedWithin(*rtOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
	}
	else if(first == "Polygon")
	{
		if(second == "Circle")
		{
			//std::cout << "Poly Circle" << std::endl;
			bool got = pInner->containedWithin(*cOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;		
		}
		else if(second == "Polygon")
		{
			//std::cout << "Poly Poly" << std::endl;
			bool got = pInner->containedWithin(*pOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "ReuleauxTriangle")
		{
			//std::cout << "Poly RT" << std::endl;
			bool got = pInner->containedWithin(*rtOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
	}
	else if(first == "ReuleauxTriangle")
	{
		if(second == "Circle")
		{
			//std::cout << "RT Circle" << std::endl;
			bool got = rtInner->containedWithin(*cOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Polygon")
		{
			//std::cout << "RT Polygon" << std::endl;
			bool got = rtInner->containedWithin(*pOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "ReuleauxTriangle")
		{
			//std::cout << "RT RT" << std::endl;
			bool got = rtInner->containedWithin(*rtOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
	}
	else if(first == "Sphere")
	{
		if(second == "Sphere")
		{
			//std::cout << "Sphere Sphere" << std::endl;
			bool got = sInner->containedWithin(*sOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Cube")
		{
			//std::cout << "Sphere Cube" << std::endl;
			bool got = sInner->containedWithin(*cubeOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Rtet")
		{
			//std::cout << "Sphere Rtet" << std::endl;
			bool got = sInner->containedWithin(*rtetOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
	}
	else if(first == "Cube")
	{
		if(second == "Sphere")
		{
			//std::cout << "Cube Sphere" << std::endl;
			bool got = cubeInner->containedWithin(*sOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Cube")
		{
			//std::cout << "Cube Cube" << std::endl;
			bool got = cubeInner->containedWithin(*cubeOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Rtet")
		{
			//std::cout << "Cube Rtet" << std::endl;
			bool got = cubeInner->containedWithin(*rtetOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
	}
	else if(first == "Rtet")
	{
		if(second == "Sphere")
		{
			//std::cout << "Rtet Sphere" << std::endl;
			bool got = rtetInner->containedWithin(*sOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Cube")
		{
			//std::cout << "Rtet Cube" << std::endl;
			bool got = rtetInner->containedWithin(*cubeOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
		else if(second == "Rtet")
		{
			//std::cout << "Rtet Rtet" << std::endl;
			bool got = rtetInner->containedWithin(*rtetOuter);
			std::cout << (got == expected ? "PASS" : "FAIL") << " " << note << std::endl;
		}
	}
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) {
        printf(USAGE, argv[0]);
        return -1;
    }
    
    //std::cout << "Test file: " << argv[1] << std::endl;
    // Open File and if it doesnt exist it stdlib sets it as a null pointer
    std::ifstream ifs(argv[1]);

    // If File stream is successfully created
    if(ifs)
    {
    	std::string line;
    	while(std::getline(ifs, line))
    	{
    		//std::cout << std::endl;
    		//std::cout << "New Test" << std::endl;
    		if (line.length() == 0 || line[0] == '#')
    		{

    		}
    		else
    		{
    			parse(line);
    		}
    	}
    }
    return 0;
}
