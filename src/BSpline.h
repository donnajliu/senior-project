#ifndef _H__BSPLINE_H
#define _H__BSPLINE_H

#include "Curve.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class BSpline : public Curve
{
public:
	BSpline();
	virtual ~BSpline();

	virtual void _on_way_point_added();

	Vector interpolate(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3);

    glm::vec3 deBoor(float t, int degree, std::vector<glm::vec3> points, std::vector<int> knots);
};

#endif
