#include "BSpline.h"
#include <iostream>

BSpline::BSpline()
: Curve()
{}

BSpline::~BSpline()
{}

void BSpline::_on_way_point_added()
{
	if(_way_points.size() < 4)
	{
		return;
	}

	int new_control_point_index=static_cast<int>(_way_points.size()) - 1;

	int pt=new_control_point_index - 3;

	for(int i=0; i<=_steps; i++)
	{
		double u=(double)i / (double)_steps;

		add_node(interpolate(u, _way_points[pt], _way_points[pt+1], _way_points[pt+2], _way_points[pt+3]));
	}
}

Vector BSpline::interpolate(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3)
{
	Vector point;

    point = u*u*u*((-1) * P0 + 3 * P1 - 3 * P2 + P3) / 6;
    point += u*u*(3*P0 - 6 * P1+ 3 * P2) / 6;
    point += u*((-3) * P0 + 3 * P2) / 6;
    point += (P0+4*P1+P2) / 6;

	return point;
}

glm::vec3 BSpline::deBoor(float t, int degree, std::vector<glm::vec3> points, std::vector<int> knots) {

    int i,j,s,l;
    int n = points.size();    // points count
    int d = 3; // point dimensionality

    if (points.size() < 3) {
        return glm::vec3(-1, -1, -1);
    }

    if(degree < 1) {
        throw std::invalid_argument("Degree must be at least 1 (linear)");
    }
    if(degree > (n-1)) {
        throw std::invalid_argument("degree must be less than or equal to point count - 1");
    }

    std::vector<float> weights;
    for(i = 0; i < n; i++) {
        weights.push_back(1.f);
    }

    if(knots.size() == 0) {
      // build knot vector of length [n + degree + 1]
      for(i=0; i < n+degree+1; i++) {
        knots.push_back(i);
      }
    } else {
      if((int) knots.size() != n+degree+1) {
          throw std::invalid_argument("bad knot vector length");
      }
    }

    int domain [2] = {
      degree,
      (int) knots.size() - 1 - degree
    };

    // remap t to the domain where the spline is defined
    int low  = knots[domain[0]];
    int high = knots[domain[1]];
    t = t * (high - low) + low;

    if(t < low || t > high) {
        throw std::invalid_argument("t out of bounds");
    }


    // find s (the spline segment) for the [t] value provided
    for(s = domain[0]; s < domain[1]; s++) {
      if(t >= knots[s] && t <= knots[s+1]) {
        break;
      }
    }

    // convert points to homogeneous coordinates
    float v[n][d];
    for(i=0; i < n; i++) {
      for(j=0; j < d; j++) {
        v[i][j] = points[i][j] * weights[i];
      }
      v[i][d] = weights[i];
    }

    // l (level) goes from 1 to the curve degree + 1
    float alpha;
    for(l = 1; l <= degree+1; l++) {
      // build level l of the pyramid
      for(i = s; i > (s-degree - 1 + l); i--) {
        alpha = (t - knots[i]) / (knots[i+degree+1-l] - knots[i]);

        // interpolate each component
        for(j = 0; j < d+1; j++) {
          v[i][j] = (1 - alpha) * v[i-1][j] + alpha * v[i][j];
        }
      }
    }

    // convert back to cartesian and return
    glm::vec3 result;
    for(i=0; i < d; i++) {
      result[i] = v[s][i];
    }

    return result;
}

