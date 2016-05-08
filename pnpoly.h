#pragma once

#include <vector>
#include <memory>
#include "point4d.h"

int pnpoly(std::vector<std::shared_ptr<VART::Point4D>> points, float testx, float testy)
{
	unsigned int i, j, c = 0;
	for (i = 0, j = points.size()-1; i < points.size(); j = i++) {
	float vertx_i = (float)points[i]->GetX();
	float verty_i = (float)points[i]->GetY();
	float vertx_j = (float)points[j]->GetX();
	float verty_j = (float)points[j]->GetY();


		if ( ((verty_i>testy) != (verty_j>testy)) &&
			(testx < (vertx_j-vertx_i) * (testy-verty_i) / (verty_j-verty_i) + vertx_i)) {
			c = !c;
		}
	}
  return c;
}
