//
//  triangle.cpp
//  rayz
//
//  Created by Иван Дмитриевский on 13/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#include "triangle.h"

triangle::triangle(std::array<dvec3, 3> vertices) : _vertices(vertices){};

bool triangle::intersect(ray ray, dvec3 *intersectionPoint) const {
  return false;
}

double triangle::get_color(dvec3 intersectionPoint) const { return 0; }

dvec3 triangle::get_normal(dvec3 point) const { return { 0, 0, 0 }; }
