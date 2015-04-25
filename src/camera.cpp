//
//  camera.cpp
//  rayz
//
//  Created by Иван Дмитриевский on 14/04/15.
//  Copyright (c) 2015 Ivan Dmitrievsky. All rights reserved.
//

#include "camera.h"
#include "glm/gtx/rotate_vector.hpp"

camera::camera(dvec3 pos, double fovx, double fovy, dvec3 lookat)
    : _eye(pos),
      _lookat(lookat),
      _up({ 0, 0, 1 }),
      _w(glm::normalize(_eye - _lookat)),
      _u(glm::normalize(glm::cross(_up, _w))),
      _v(glm::cross(_w, _u)),
      _dist(1),
      _fovx(fovx),
      _fovy(fovy) {}

camera::camera(dvec3 pos, double fovx, double fovy, double heading,
               double pitch, double roll)
    : camera(pos, fovx, fovy, pos + dvec3(0, 1, 0)) {

  double dist = glm::distance(_lookat, _eye);

  _w = glm::rotate(_w, -glm::radians(heading), _v);
  _u = glm::rotate(_u, -glm::radians(heading), _v);

  _w = glm::rotate(_w, glm::radians(pitch), _u);
  _v = glm::rotate(_v, glm::radians(pitch), _u);

  _u = glm::rotate(_u, -glm::radians(roll), _w); //
  _v = glm::rotate(_v, -glm::radians(roll), _w); //

  _lookat = _eye - _w * dist;
}

png::image<png::rgba_pixel_16> camera::render(const scene &scene, long resX,
                                              long resY) {
  png::image<png::rgba_pixel_16> img(resX, resY);
  std::vector<std::vector<double> > closest_points(
      resY, std::vector<double>(resX, -1));

  double pxw = 2 * _dist * glm::tan(glm::radians(_fovx / 2)) / resX;
  double pxh = 2 * _dist * glm::tan(glm::radians(_fovy / 2)) / resY;

  double max_intensity = 0;

  double x = -(resX * pxw / 2);
  double y = -(resY * pxh / 2);

  for (long i = resY - 1; i >= 0; --i) {
    for (long j = 0; j < resX; ++j) {
      for (auto &node : scene.nodes()) {
        ray world_ray;
        world_ray.origin = _eye;
        world_ray.dir = glm::normalize(x * _u + y * _v - _dist * _w);

        if (i == resX / 2 && j == resY / 2) {
          printf("%s", "");
        }

        glm::dvec3 closest_point;
        auto intersected = node->intersect(world_ray);
        if (!intersected.empty()) {
          closest_point =
              world_ray.origin + world_ray.dir * intersected.front().first;
          if (closest_points[i][j] < 0) {
            closest_points[i][j] = glm::distance(_eye, closest_point);
          }
        }
      }
      max_intensity = glm::max(max_intensity, closest_points[i][j]);
      x += pxw;
    }
    x = -(resX * pxw / 2);
    y += pxh;
  }

  for (long i = 0; i < resY; ++i) {
    for (long j = 0; j < resX; ++j) {
      if (closest_points[i][j] >= 0) {
        double intensity = closest_points[i][j] / (2 * max_intensity);
        uint16_t val = 0xFFFF - intensity * 0xFFFF;
        img.set_pixel(j, i, { val, val, val, 0xFFFF });
      } else {
        img.set_pixel(j, i, { 0, 0, 0, 0 });
      }
    }
  }

  return img;
}
