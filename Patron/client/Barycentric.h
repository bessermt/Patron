/*
 * Barycentric.h
 *
 *  Created on: May 6, 2014
 *      Author: bessermt
 */

#ifndef BARYCENTRIC_H_
#define BARYCENTRIC_H_

typedef int Coord_t; // if Coord_t is not an integral type (ex. double), rounding in function barycentricToCartesian() should not round.

typedef struct
{
	Coord_t x, y;
} Point2D;

typedef struct
{
	Point2D A, B, C;
} Triangle2D;

typedef struct
{
	double a, b, c;
} BarycentricCoord;

// Given a 2D Triangle, convert a 2 dimensional point to Barycentric coordinates.
int cartesianToBarycentric(BarycentricCoord* const barycentricCoord, const Triangle2D* const triangle, const Point2D* const point);

extern const int CARTESIAN_TO_BARYCENTRIC_INVALID_INPUT;
extern const int CARTESIAN_TO_BARYCENTRIC_COLLINEAR_TRIANGLE;
extern const int CARTESIAN_TO_BARYCENTRIC_INSIDE;
extern const int CARTESIAN_TO_BARYCENTRIC_EDGE;
extern const int CARTESIAN_TO_BARYCENTRIC_OUTSIDE;

// Given a 2D Triangle, convert a Barycentric coordinate to a cartesianCoordinate.
int barycentricToCartesian(Point2D* const cartesianCoord, const Triangle2D* const triangle, BarycentricCoord* const barycentricCoord);

extern const int BARYCENTRIC_TO_CARTESIAN_INVALID_INPUT;
extern const int BARYCENTRIC_TO_CARTESIAN_SUCCESS;


#endif /* BARYCENTRIC_H_ */
