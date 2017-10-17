/* layermesh/test/header_only/test_hull.hpp
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

#include <stdexcept>
#include <gtest/gtest.h>
#include <atom.hpp>

using namespace std;
using namespace layermesh;

class Tetrahedron : Atom {
  private:
    gvec_list points;
  public:
    // for the small number of points usually needed to initialise a hull,
    // a copy constructor for gvec_list would probably do.
    Tetrahedron(gvec_list points) : points(points) {
      if (points.size() != 4) {
        throw invalid_argument("A tetrahedron has four points.");
      }
    };
    virtual ~Tetrahedron() {};
    virtual memsafe_gvec_list point_cloud();
    virtual unsigned internal_points_start_index() const;
    virtual gsphere get_boundary();
};

memsafe_gvec_list Tetrahedron::point_cloud() {
  unsigned i;
  memsafe_gvec_list point_cloud = make_shared<gvec_list>();
  gvec centroid;

  for (i = 0; i < 4; ++i) {
    point_cloud->push_back(points[i]);
    centroid = centroid + points[i];
  }

  // (note, the tetrahedron does *not* need an internal point to tesselate
  // correctly, but we provide one anyway for testing purposes.)
  point_cloud->push_back(centroid / 4);

  return point_cloud;
}

unsigned Tetrahedron::internal_points_start_index() const {
  return 4;
}

gsphere Tetrahedron::get_boundary() {
  gsphere ret;

  ret.centre = (*point_cloud())[internal_points_start_index()];

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

TEST(Atom, can_derive_class) {
  vector<gvec> points;
  points.push_back(gvec(1.2, 3.4, 5.6));
  points.push_back(gvec(-0.2, 13.4, -6.5));
  points.push_back(gvec(7.7, 4.4, 9.2));
  points.push_back(gvec(-3.4, -5.6, -1.2));

  Tetrahedron t(points);
}

TEST(Atom, point_retrieval_doesnt_leak_memory) {
  vector<gvec> points;
  points.push_back(gvec(1.2, 3.4, 5.6));
  points.push_back(gvec(-0.2, 13.4, -6.5));
  points.push_back(gvec(7.7, 4.4, 9.2));
  points.push_back(gvec(-3.4, -5.6, -1.2));

  Tetrahedron t(points);

  memsafe_gvec_list point_cloud = t.point_cloud();

  EXPECT_EQ(point_cloud->size(), 5) << "unexpected point_cloud length.";

}

TEST(Atom, boundary_retrieval_doesnt_leak_memory) {
  vector<gvec> points;
  points.push_back(gvec(0.0, 0.0, 0.0));
  points.push_back(gvec(1.0, 0.0, 0.0));
  points.push_back(gvec(0.0, 1.0, 0.0));
  points.push_back(gvec(0.0, 0.0, 1.0));

  Tetrahedron t(points);

  gsphere boundary = t.get_boundary();

  EXPECT_EQ(boundary.centre[0], 0.25) << "incorrect centroid X";
  EXPECT_EQ(boundary.centre[1], 0.25) << "incorrect centroid Y";
  EXPECT_EQ(boundary.centre[2], 0.25) << "incorrect centroid Z";

}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

