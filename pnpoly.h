#pragma once

#include <vector>
#include <memory>
#include "point4d.h"

int pnpoly(std::vector<std::shared_ptr<VART::Point4D>> points, VART::Transform &transform, float testx, float testy)
{
	unsigned int i, j, c = 0;
	for (i = 0, j = points.size()-1; i < points.size(); j = i++) {
	
		VART::Point4D tpoint_i(points[i].get());
		VART::Point4D tpoint_j(points[j].get());

		transform.ApplyTo(&tpoint_i);
		transform.ApplyTo(&tpoint_j);

		float vertx_i = (float)tpoint_i.GetX();
		float verty_i = (float)tpoint_i.GetY();
		float vertx_j = (float)tpoint_j.GetX();
		float verty_j = (float)tpoint_j.GetY();


		if ( ((verty_i>testy) != (verty_j>testy)) &&
			(testx < (vertx_j-vertx_i) * (testy-verty_i) / (verty_j-verty_i) + vertx_i)) {
			c = !c;
		}
	}
  return c;
}
