/*
 * NearestPoints.cpp
 */

#include <limits>
#include <thread>
#include <algorithm>
#include <cmath>
#include <math.h>

#include "NearestPoints.h"
#include "Point.h"

const double MAX_DOUBLE = std::numeric_limits<double>::max();

Result::Result(double dmin, Point p1, Point p2) {
	this->dmin = dmin;
	this->p1 = p1;
	this->p2 = p2;
}

Result::Result() {
	this->dmin = MAX_DOUBLE;
	this->p1 = Point(0,0);
	this->p2 = Point(0,0);
}

/**
 * Auxiliary functions to sort vector of points by X or Y axis.
 */
static void sortByX(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.x < q.x || (p.x == q.x && p.y < q.y); });
}

static void sortByY(vector<Point> &v, int left, int right)
{
	std::sort(v.begin( ) + left, v.begin() + right + 1,
		[](Point p, Point q){ return p.y < q.y || (p.y == q.y && p.x < q.x); });
}

/**
 * Brute force algorithm O(N^2).
 */
Result nearestPoints_BF(vector<Point> &vp) {
	Result res;
	for (int i = 0; i < vp.size(); i++) {
		Point p1 = vp.at(i);
		for (int j = i+1; j < vp.size(); j++) {
			Point p2 = vp.at(j);
			double dist = p1.distance(p2);
			if (dist < res.dmin) {
				res.dmin = dist;
				res.p1 = p1;
				res.p2 = p2;
			}
		}
	}
	return res;
}

/**
 * Improved brute force algorithm, that first sorts points by X axis.
 */
Result nearestPoints_BF_SortByX(vector<Point> &vp) {
	Result res;
	sortByX(vp, 0, vp.size()-1);


	return res;
}


/**
 * Auxiliary function to find nearest points in strip, as indicated
 * in the assignment, with points sorted by Y coordinate.
 * The strip is the part of vp between indices left and right (inclusive).
 * "res" contains initially the best solution found so far.
 */
static void npByY(vector<Point> &vp, int left, int right, Result &res)
{
	for (int i = left; i <= right; i++) {
		for (int j = i+1; j <= right; j++) {
			if (abs(vp.at(j).y - vp.at(i).y) >= res.dmin) break;
			else
				if (vp.at(i).distance(vp.at(j)) < res.dmin) {
					res.p1 = vp.at(i);
					res.p2 = vp.at(j);
					res.dmin = vp.at(i).distance(vp.at(j));
				}
		}
	}
}

/**
 * Recursive divide and conquer algorithm.
 * Finds the nearest points in "vp" between indices left and right (inclusive),
 * using at most numThreads.
 */
static Result np_DC(vector<Point> &vp, int left, int right, int numThreads) {
	// Base case of two points
	if (right - left == 1) {
		return Result(vp.at(left).distance(vp.at(right)), vp.at(left), vp.at(right));
	}

	// Base case of a single point: no solution, so distance is MAX_DOUBLE
	if (right <= left) return Result(MAX_DOUBLE, vp.at(left), vp.at(right));

	// Divide in halves (left and right) and solve them recursively,
	// possibly in parallel (in case numThreads > 1)
	int middle = (left+right)/2;
	Result left_res = np_DC(vp, left, middle, numThreads);
	Result right_res = np_DC(vp, middle+1, right, numThreads);

	// Select the best solution from left and right
	Result best = left_res.dmin > right_res.dmin ? right_res : left_res;

	// Determine the strip area around middle point
	Point middle_p = vp.at(middle);
	int l_strip = middle, r_strip = middle+1;
	for (; vp.at(l_strip).distance(middle_p) < best.dmin && l_strip >= left; l_strip--);
	for (; vp.at(r_strip).distance(middle_p) < best.dmin && r_strip <= right; r_strip++);

	// Order points in strip area by Y coordinate
	sortByY(vp, l_strip, r_strip);

	// Calculate nearest points in strip area (using npByY function)
	npByY(vp, l_strip, r_strip, best);

	// Reorder points in strip area back by X coordinate
	sortByX(vp, l_strip, r_strip);

	return best;
}


/**
 * Defines the number of threads to be used.
 */
static int numThreads = 1;
void setNumThreads(int num)
{
	numThreads = num;
}

/*
 * Divide and conquer approach, single-threaded version.
 */
Result nearestPoints_DC(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, 1);
}


/*
 * Multi-threaded version, using the number of threads specified
 * by setNumThreads().
 */
Result nearestPoints_DC_MT(vector<Point> &vp) {
	sortByX(vp, 0, vp.size() -1);
	return np_DC(vp, 0, vp.size() - 1, numThreads);
}
