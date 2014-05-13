/*
 ============================================================================
 Name        : Barycentric.c
 Author      : bessermt
 Version     :
 Created on  : 2014-05-06
 Copyright   : (c) 2014 Mitch T. Besser. All rights reserved.
 Description : Barycentric coordinate conversion functions.
 ============================================================================
 */

#include <stddef.h>
#include "Barycentric.h"

const int CARTESIAN_TO_BARYCENTRIC_INVALID_INPUT = -2;
const int CARTESIAN_TO_BARYCENTRIC_COLLINEAR_TRIANGLE = -1;
const int CARTESIAN_TO_BARYCENTRIC_INSIDE = 0;
const int CARTESIAN_TO_BARYCENTRIC_EDGE = 1;
const int CARTESIAN_TO_BARYCENTRIC_OUTSIDE = 2;

const int BARYCENTRIC_TO_CARTESIAN_INVALID_INPUT = -2;
const int BARYCENTRIC_TO_CARTESIAN_SUCCESS = 0;

// For rounding, Sum_t should be double if Coord_t is an integral type.
// if_<is_integral<Coord_t>::value, double, int>::type Sum_t;

typedef double Sum_t;
static const Sum_t SUM = 1;

int cartesianToBarycentric(BarycentricCoord* const barycentricCoord, const Triangle2D* const triangle, const Point2D* const point)
{
	int result = CARTESIAN_TO_BARYCENTRIC_INVALID_INPUT;

	if (barycentricCoord != NULL && triangle != NULL && point != NULL)
	{
		result = CARTESIAN_TO_BARYCENTRIC_COLLINEAR_TRIANGLE;

		const Coord_t ax = triangle->A.x;
		const Coord_t ay = triangle->A.y;
		const Coord_t bx = triangle->B.x;
		const Coord_t by = triangle->B.y;
		const Coord_t cx = triangle->C.x;
		const Coord_t cy = triangle->C.y;

		const Coord_t dpxcx = point->x - cx;
		const Coord_t dpycy = point->y - cy;
		const Coord_t dcxbx = cx - bx;
		const Coord_t dbycy = by - cy;
		const Coord_t daxcx = ax - cx;
		const Coord_t dcyay = cy - ay;

		const Coord_t denom = dbycy*daxcx - dcxbx*dcyay;

		if (denom != 0)
		{
			result = CARTESIAN_TO_BARYCENTRIC_INSIDE;

			const double a = (double)(dbycy*dpxcx + dcxbx*dpycy) / denom;
			const double b = (double)(dcyay*dpxcx + daxcx*dpycy) / denom;
			const double c = SUM - a - b;

			if (a<0.0 || b<0.0 || c<0.0)
			{
				result = CARTESIAN_TO_BARYCENTRIC_OUTSIDE;
			}
			else if (a==0.0 || b==0.0 || c==0.0)
			{
				result = CARTESIAN_TO_BARYCENTRIC_EDGE;
			}

			barycentricCoord->a = a;
			barycentricCoord->b = b;
			barycentricCoord->c = c;
		}
	}

	return result;
}

int barycentricToCartesian(Point2D* const cartesianCoord, const Triangle2D* const triangle, BarycentricCoord* const barycentricCoord)
{
	int result = BARYCENTRIC_TO_CARTESIAN_INVALID_INPUT;

	if (cartesianCoord != NULL && triangle != NULL && barycentricCoord != NULL)
	{
		result = BARYCENTRIC_TO_CARTESIAN_SUCCESS;

		const double round = SUM/2;
		cartesianCoord->x = barycentricCoord->a*triangle->A.x + barycentricCoord->b*triangle->B.x + barycentricCoord->c*triangle->C.x + round;
		cartesianCoord->y = barycentricCoord->a*triangle->A.y + barycentricCoord->b*triangle->B.y + barycentricCoord->c*triangle->C.y + round;
	}

	return result;
}
