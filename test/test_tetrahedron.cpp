/* layermesh/test/test_tetrahedron.hpp
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

#include <stdlib.h>
#include <gtest/gtest.h>
#include <tetrahedron.hpp>
#include "stl_helper.hpp"

using namespace std;
using namespace layermesh;

TEST(Tetrahedron, test_can_instantiate_class) {
  vector<gvec> points;
  points.push_back(gvec(1.2, 3.4, 5.6));
  points.push_back(gvec(-0.2, 13.4, -6.5));
  points.push_back(gvec(7.7, 4.4, 9.2));
  points.push_back(gvec(-3.4, -5.6, -1.2));

  Tetrahedron t(points);
}

TEST(Tetrahedron, test_point_retrieval_doesnt_leak_memory) {
  vector<gvec> points;
  points.push_back(gvec(1.2, 3.4, 5.6));
  points.push_back(gvec(-0.2, 13.4, -6.5));
  points.push_back(gvec(7.7, 4.4, 9.2));
  points.push_back(gvec(-3.4, -5.6, -1.2));

  Tetrahedron t(points);

  memsafe_gvec_list point_cloud = t.point_cloud();

  EXPECT_EQ(point_cloud->size(), 4) << "unexpected point_cloud length.";

}

TEST(Tetrahedron, test_boundary_retrieval_doesnt_leak_memory) {
  vector<gvec> points;
  points.push_back(gvec(0.0, 0.0, 0.0));
  points.push_back(gvec(1.0, 0.0, 0.0));
  points.push_back(gvec(0.0, 1.0, 0.0));
  points.push_back(gvec(0.0, 0.0, 1.0));

  Tetrahedron t(points);

  gsphere boundary = t.get_boundary();

  EXPECT_EQ(boundary.centre[0], 0.25) << "incorrect centroid";
  EXPECT_EQ(boundary.centre[1], 0.25) << "incorrect centroid";
  EXPECT_EQ(boundary.centre[2], 0.25) << "incorrect centroid";

}

TEST(Tetrahedron, test_contains) {
  vector<gvec> points;
  points.push_back(gvec(0.0, 0.0, 0.0));
  points.push_back(gvec(1.0, 0.0, 0.0));
  points.push_back(gvec(0.0, 1.0, 0.0));
  points.push_back(gvec(0.0, 0.0, 1.0));

  Tetrahedron t(points);

  EXPECT_TRUE(t.contains(gvec(0.1, 0.1, 0.1))) << "can't detect point contained";

  EXPECT_FALSE(t.contains(gvec(-0.1, 0.1, 0.1))) << "doesn't reject point not contained";

}

TEST(Tetrahedron, test_can_generate_valid_stl) {
  vector<gvec> points;
  points.push_back(gvec(0.0, 0.0, 0.0));
  points.push_back(gvec(1.0, 0.0, 0.0));
  points.push_back(gvec(0.0, 1.0, 0.0));
  points.push_back(gvec(0.0, 0.0, 1.0));

  Tetrahedron t(points);

  // check we can use the atom directly to generate an STL:
#define FILENAME "foo.stl"
  t.save_stl(FILENAME, true);

  EXPECT_VALID_STL(FILENAME, true);

  system("rm " FILENAME);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


