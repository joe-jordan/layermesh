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
#include <check.h>
#include <hull.hpp>

class Tetrahedron : layermesh::IConvexHull {
  private:
    std::vector<layermesh::gvec> points;
  public:
    Tetrahedron(std::vector<layermesh::gvec> points) : points(points) {
      if (points.size() != 4) {
        throw std::invalid_argument("A tetrahedron has four points.");
      }
    };
    virtual ~Tetrahedron() {};
    virtual std::shared_ptr<std::vector<layermesh::gvec> > point_cloud();
    virtual unsigned internal_points_start_index();
    virtual layermesh::gsphere get_boundary();
};

std::shared_ptr<std::vector<layermesh::gvec> > Tetrahedron::point_cloud() {
  unsigned i;
  std::shared_ptr<std::vector<layermesh::gvec> > ptr =
    std::make_shared<std::vector<layermesh::gvec> >();
  std::vector<layermesh::gvec>* point_cloud = ptr.get();
  layermesh::gvec centroid;

  for (i = 0; i < 4; ++i) {
    point_cloud->push_back(points[i]);
    centroid = centroid + points[i];
  }

  // (note, the tetrahedron does *not* need an internal point to tesselate
  // correctly, but we provide one anyway for testing purposes.)
  point_cloud->push_back(centroid / 4);

  return ptr;
}

unsigned Tetrahedron::internal_points_start_index() {
  return 4;
}

layermesh::gsphere Tetrahedron::get_boundary() {
  layermesh::gsphere ret;

  ret.centre = (*(point_cloud().get()))[internal_points_start_index()];

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

START_TEST(test_can_derive_class) {
  std::vector<layermesh::gvec> points;
  points.push_back(layermesh::gvec(1.2, 3.4, 5.6));
  points.push_back(layermesh::gvec(-0.2, 13.4, -6.5));
  points.push_back(layermesh::gvec(7.7, 4.4, 9.2));
  points.push_back(layermesh::gvec(-3.4, -5.6, -1.2));

  Tetrahedron t(points);
}END_TEST

int main(void)
{
  Suite *s1 = suite_create("abstract class IConvexHull test");
  TCase *tc1_1 = tcase_create("all");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_can_derive_class);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}

