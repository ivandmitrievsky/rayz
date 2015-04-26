//
//  obj.cpp
//  rayz
//
//  Created by Иван Дмитриевский on 25/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#include "obj.h"
#include "tinyobjloader/tiny_obj_loader.h"

obj::obj(const char *filename) : _box(vec3{}, vec3{}) {

  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;

  auto err = tinyobj::LoadObj(shapes, materials, filename);

  if (!err.empty()) {
    printf("%s\n", err.c_str());
  }

  vec3 min, max;

  for (size_t i = 0; i < shapes.size(); ++i) {
    for (size_t j = 0; j < shapes[i].mesh.indices.size() / 3; ++j) {
      std::array<vec3, 3> triplet;
      for (int k = 0; k < 3; ++k) {
        size_t idx = shapes[i].mesh.indices[3 * j + k];

        triplet[k] = { shapes[i].mesh.positions[3 * idx + 0],
                       shapes[i].mesh.positions[3 * idx + 1],
                       shapes[i].mesh.positions[3 * idx + 2] };
      }
      _triangles.emplace_back(triplet);

      for (int k = 0; k < 3; ++k) {
        min = glm::min(min, triplet[k]);
        max = glm::max(triplet[k], max);
      };
    }
  }

  _box = box(min, max);
}

geometry::ray_path obj::intersect(ray ray) const {
  ray_path list;

  if (_box.intersect(ray).empty()) {
    return list;
  }

  for (const auto &tri : _triangles) {
    for (const auto &point : tri.intersect(ray)) {
      list.push_back(point);
    }
  }

  list.sort();

  return list;
}

num_t obj::get_color(vec3 point) const { return 0; }

vec3 obj::get_normal(vec3 point) const { return glm::normalize(point); }