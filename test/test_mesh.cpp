/* layermesh/test/test_mesh.cpp
 * Copyright Joe Jordan <joe@joe-jordan.co.uk> 2016-17.
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

#include <stdlib.h>
#include <gtest/gtest.h>
#include <mesh.hpp>
#include "stl_helper.hpp"


using namespace std;
using namespace layermesh;

// inherit from abstract base class Mesh with simple tetrahedron implementation:

namespace notlayermesh {

  gvec_list generate_corner_points() {
    gvec_list points;

    points.push_back(gvec(0.0, 0.0, 0.0));
    points.push_back(gvec(1.0, 0.0, 0.0));
    points.push_back(gvec(0.0, 1.0, 0.0));
    points.push_back(gvec(0.0, 0.0, 1.0));

    return points;
  }

  facet_triples generate_corner_facets() {
    facet_triples facets(4);

    facets[0] = {{0, 1, 3}};
    facets[1] = {{0, 3, 2}};
    facets[2] = {{0, 2, 1}};
    facets[3] = {{1, 2, 3}};

    return facets;
  }

  class Tetrahedron : layermesh::Mesh {
    public:
      virtual ~Tetrahedron() {};
      virtual void save_stl(std::string filename, bool binary) {
        auto points = generate_corner_points();
        auto facets = generate_corner_facets();
        save_stl_inner(filename, binary, points, facets);
      }
  };
}

TEST(Mesh, test_can_instantiate_mesh) {
  notlayermesh::Tetrahedron t;
}

#define FILENAME "foo.stl"

TEST(Mesh, test_can_generate_valid_ascii_stl_file) {
  notlayermesh::Tetrahedron t;

  t.save_stl(FILENAME, false);

  EXPECT_VALID_STL(FILENAME, false);

  system("rm " FILENAME);
}

TEST(Mesh, test_can_generate_valid_binary_stl_file) {
  notlayermesh::Tetrahedron t;

  t.save_stl(FILENAME, true);

  EXPECT_VALID_STL(FILENAME, true);

  system("rm " FILENAME);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

