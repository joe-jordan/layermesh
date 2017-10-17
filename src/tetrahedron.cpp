/* layermesh/src/tetrahedron.cpp
 * Copyright Joe Jordan <joe@joe-jordan.co.uk> 2016.
 *
 * This file is part of Layermesh.
 *
 * Layermesh is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Layermesh is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Layermesh.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <array>
#include <tetrahedron.hpp>

using namespace std;
using namespace layermesh;

void Tetrahedron::compute_centroid() {
  unsigned i;
  for (i = 0; i < 4; ++i) {
    centroid = centroid + points[i];
  }

  centroid = centroid / 4.0;
}

void Tetrahedron::compute_normals_and_triples() {
  // Notice that index 0 of each inner array == outer index. This is relied
  // upon by the loop below to reduce indirection, and in contains() to find a
  // point on each facet to use as the origin.
  array<array<unsigned, 3>, 4> facet_indices = {{
    {{0, 1, 2}},
    {{1, 0, 3}},
    {{2, 0, 3}},
    {{3, 2, 1}}
  }};

  unsigned i;

  for (i = 0; i < 4; ++i) {
    array<unsigned, 3> js(facet_indices[i]);
    gvec normal = (points[js[1]] - points[i]) ^
                  (points[js[2]] - points[i]);
    double proj = normal * (centroid - points[i]);
    if (proj > 0.0) {
      normal = normal * -1.0;
      // We also swap the indices, to make sure they satisfy:
      // normal = (v1 - v0) ^ (v2 - v0)
      // (where the normal points from the solid phase to the void.)
      unsigned tmp = js[2];
      js[2] = js[1];
      js[1] = tmp;
    }

    normal = normal / layermesh::modulus(normal);

    facet_normals.push_back(normal);
    _facet_triples.push_back(js);
  }
}

memsafe_gvec_list Tetrahedron::point_cloud() {
  return make_shared<gvec_list>(points);
}

unsigned Tetrahedron::internal_points_start_index() const {
  return 4;
}

gsphere Tetrahedron::get_boundary() {
  gsphere ret;
  ret.centre = centroid;

  ret.radius = 0.0;
  unsigned i;
  double d;

  for (i = 0; i < 4; ++i) {
    d = layermesh::modulus(ret.centre - points[i]);
    if (d > ret.radius) {
      ret.radius = d;
    }
  }

  return ret;
}

bool Tetrahedron::contains(gvec point) {
  if (facet_normals.size() == 0) {
    compute_normals_and_triples();
  }

  bool contained = true;
  unsigned i;

  for (i = 0; i < 4; ++i) {
    double proj = facet_normals[i] * (point - points[i]);

    if (proj > 0.0) {
      contained = false;
      break;
    }
  }

  return contained;
}

void Tetrahedron::save_stl(std::string filename, bool binary) {
  if (_facet_triples.size() == 0) {
    compute_normals_and_triples();
  }

  save_stl_inner(filename, binary, points, _facet_triples);
}


