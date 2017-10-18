/* layermesh/test/test_gvec.cpp
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

#include <gtest/gtest.h>
#include <gvec.hpp>

TEST(gvec, test_component_access) {
  layermesh::gvec a(0.0, 1.0, 2.0);

  EXPECT_EQ(a[0], 0.0) << "cannot read via operator[].";
  EXPECT_EQ(a[1], 1.0) << "cannot read via operator[].";
  EXPECT_EQ(a[2], 2.0) << "cannot read via operator[].";

  layermesh::gvec b;
  b[0] = 2.0;
  b[1] = 1.0;
  b[2] = 0.0;

  EXPECT_EQ(b[0], a[2]) << "cannot set via operator[].";
  EXPECT_EQ(b[1], a[1]) << "cannot set via operator[].";
  EXPECT_EQ(b[2], a[0]) << "cannot set via operator[].";
}

TEST(gvec, test_plus_minus) {
  layermesh::gvec a(0.0, 1.0, 2.0);
  layermesh::gvec b(2.0, 1.0, 0.0);

  layermesh::gvec c = a + b;

  EXPECT_EQ(c[0], 2.0) << "cannot add vectors.";
  EXPECT_EQ(c[1], 2.0) << "cannot add vectors.";
  EXPECT_EQ(c[2], 2.0) << "cannot add vectors.";

  c = a - b;

  EXPECT_EQ(c[0], -2.0) << "cannot subtract vectors.";
  EXPECT_EQ(c[1], 0.0) << "cannot subtract vectors.";
  EXPECT_EQ(c[2], 2.0) << "cannot subtract vectors.";

  c = -a;

  EXPECT_EQ(a[0], -c[0]) << "cannot negate vectors.";
  EXPECT_EQ(a[1], -c[1]) << "cannot negate vectors.";
  EXPECT_EQ(a[2], -c[2]) << "cannot negate vectors.";


  EXPECT_EQ(a[0], 0.0) << "addition or subtraction mutates instances.";
  EXPECT_EQ(a[1], 1.0) << "addition or subtraction mutates instances.";
  EXPECT_EQ(a[2], 2.0) << "addition or subtraction mutates instances.";
  EXPECT_EQ(b[0], 2.0) << "addition or subtraction mutates instances.";
  EXPECT_EQ(b[1], 1.0) << "addition or subtraction mutates instances.";
  EXPECT_EQ(b[2], 0.0) << "addition or subtraction mutates instances.";

}

TEST(gvec, test_scaling) {
  layermesh::gvec a(0.0, 1.0, 2.0);
  layermesh::gvec c = a * 5.0;

  EXPECT_EQ(c[0], 0.0) << "cannot scale vectors up.";
  EXPECT_EQ(c[1], 5.0) << "cannot scale vectors up.";
  EXPECT_EQ(c[2], 10.0) << "cannot scale vectors up.";

  c = a * 5;

  EXPECT_EQ(c[0], 0.0) << "cannot scale vectors up by an integer.";
  EXPECT_EQ(c[1], 5.0) << "cannot scale vectors up by an integer.";
  EXPECT_EQ(c[2], 10.0) << "cannot scale vectors up by an integer.";

  c = c / 5.0;

  EXPECT_EQ(c[0], 0.0) << "cannot scale vectors down.";
  EXPECT_EQ(c[1], 1.0) << "cannot scale vectors down.";
  EXPECT_EQ(c[2], 2.0) << "cannot scale vectors down.";
}

TEST(gvec, test_dot) {
  layermesh::gvec a(0.0, 1.0, 2.0);

  double b = a * a;
  EXPECT_EQ(b, 5.0) << "dot product doesn't work.";
  EXPECT_EQ(a[0], 0.0) << "dot product with self mutates instance.";
  EXPECT_EQ(a[1], 1.0) << "dot product with self mutates instance.";
  EXPECT_EQ(a[2], 2.0) << "dot product with self mutates instance.";
}

TEST(gvec, test_cross) {
  layermesh::gvec x(1.0, 0.0, 0.0);
  layermesh::gvec y(0.0, 1.0, 0.0);

  layermesh::gvec z = x ^ y;

  EXPECT_EQ(z[0], 0.0) << "cannot compute cross product.";
  EXPECT_EQ(z[1], 0.0) << "cannot compute cross product.";
  EXPECT_EQ(z[2], 1.0) << "cannot compute cross product.";
}

TEST(gvec, test_modulus) {
  layermesh::gvec pythagorean(3.0, 4.0, 0.0);

  double hypotenuse = modulus(pythagorean);
  EXPECT_EQ(hypotenuse, 5.0) << "cannot compute modulus.";
}

#define epsilon 0.00000000000001

TEST(gvec, test_algebraic_expressions) {
  // backwards subtraction:
  layermesh::gvec x(1., 0., 0.);
  layermesh::gvec y(0., 1., 0.);

  auto f = -y + x;

  EXPECT_EQ(f[0], 1.) << "cannot reverse subtraction order.";
  EXPECT_EQ(f[1], -1.) << "cannot reverse subtraction order.";
  EXPECT_EQ(f[2], 0.) << "cannot reverse subtraction order.";

  // projection of a vector onto a normal:
  layermesh::gvec normal(1.0, 0.0, 0.0);
  layermesh::gvec to_project(13.745, 101.3, -17.2);

  //                                 scale     dot
  layermesh::gvec projection = normal * (normal * to_project);

  EXPECT_EQ(projection[0], 13.745) << "cannot scale by dot result.";
  EXPECT_EQ(projection[1], 0.0) << "cannot scale by dot result.";
  EXPECT_EQ(projection[2], 0.0) << "cannot scale by dot result.";

  // find unit normal perpendicular to both of two vectors.
  layermesh::gvec a(10.3, -17.2, 3.14);
  layermesh::gvec b(2.4587, -13.5, -7.9);

  normal = a ^ b;
  normal = normal / modulus(normal);

  EXPECT_LT((modulus(normal) - 1.0), epsilon) << "couldn't compute the normal.";
  EXPECT_LT((normal * a), epsilon) << "couldn't compute the normal.";
  EXPECT_LT((normal * b), epsilon) << "couldn't compute the normal.";
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
