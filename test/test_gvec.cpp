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

#include <check.h>
#include <gvec.hpp>

START_TEST(test_component_access) {
  layermesh::gvec a(0.0, 1.0, 2.0);

  ck_assert_msg(a[0] == 0.0 &&
                a[1] == 1.0 &&
                a[2] == 2.0, "cannot read via operator[].");

  layermesh::gvec b;
  b[0] = 2.0;
  b[1] = 1.0;
  b[2] = 0.0;

  ck_assert_msg(b[0] == a[2] &&
                b[1] == a[1] &&
                b[2] == a[0], "cannot set via operator[].");
} END_TEST

START_TEST(test_plus_minus) {
  layermesh::gvec a(0.0, 1.0, 2.0);
  layermesh::gvec b(2.0, 1.0, 0.0);

  layermesh::gvec c = a + b;

  ck_assert_msg(c[0] == 2.0 &&
                c[1] == 2.0 &&
                c[2] == 2.0, "cannot add vectors.");

  c = a - b;

  ck_assert_msg(c[0] == -2.0 &&
                c[1] == 0.0 &&
                c[2] == 2.0, "cannot subtract vectors.");

  ck_assert_msg(a[0] == 0.0 &&
                a[1] == 1.0 &&
                a[2] == 2.0 &&
                b[0] == 2.0 &&
                b[1] == 1.0 &&
                b[2] == 0.0, "addition or subtraction mutates instances.");
} END_TEST

START_TEST(test_scaling) {
  layermesh::gvec a(0.0, 1.0, 2.0);
  layermesh::gvec c = a * 5.0;

  ck_assert_msg(c[0] == 0.0 &&
                c[1] == 5.0 &&
                c[2] == 10.0, "cannot scale vectors up.");

  c = a * 5;

  ck_assert_msg(c[0] == 0.0 &&
                c[1] == 5.0 &&
                c[2] == 10.0, "cannot scale vectors up by an integer.");

  c = c / 5.0;

  ck_assert_msg(c[0] == 0.0 &&
                c[1] == 1.0 &&
                c[2] == 2.0, "cannot scale vectors down.");
} END_TEST

START_TEST(test_dot) {
  layermesh::gvec a(0.0, 1.0, 2.0);

  double b = a * a;
  ck_assert_msg(b == 5.0, "dot product doesn't work.");
  ck_assert_msg(a[0] == 0.0 &&
                a[1] == 1.0 &&
                a[2] == 2.0, "dot product with self mutates instance.");
} END_TEST

START_TEST(test_cross) {
  layermesh::gvec x(1.0, 0.0, 0.0);
  layermesh::gvec y(0.0, 1.0, 0.0);

  layermesh::gvec z = x ^ y;

  ck_assert_msg(z[0] == 0.0 &&
                z[1] == 0.0 &&
                z[2] == 1.0, "cannot compute cross product.");
} END_TEST

START_TEST(test_modulus) {
  layermesh::gvec pythagorean(3.0, 4.0, 0.0);

  double hypotenuse = modulus(pythagorean);
  ck_assert_msg(hypotenuse == 5.0, "cannot compute modulus.");
} END_TEST

#define epsilon 0.00000000000001

START_TEST(test_algebraic_expressions) {
  // projection of a vector onto a normal:
  layermesh::gvec normal(1.0, 0.0, 0.0);
  layermesh::gvec to_project(13.745, 101.3, -17.2);

  //                                 scale     dot
  layermesh::gvec projection = normal * (normal * to_project);

  ck_assert_msg(projection[0] == 13.745 &&
                projection[1] == 0.0 &&
                projection[2] == 0.0, "cannot scale by dot result.");

  // find unit normal perpendicular to both of two vectors.
  layermesh::gvec a(10.3, -17.2, 3.14);
  layermesh::gvec b(2.4587, -13.5, -7.9);

  normal = a ^ b;
  normal = normal / modulus(normal);

  ck_assert_msg((modulus(normal) - 1.0) < epsilon &&
                (normal * a) < epsilon &&
                (normal * b) < epsilon, "couldn't compute the normal.");
} END_TEST

int main(void)
{
  Suite *s1 = suite_create("gvec geometrical vector type");
  TCase *tc1_1 = tcase_create("all");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_component_access);
  tcase_add_test(tc1_1, test_plus_minus);
  tcase_add_test(tc1_1, test_scaling);
  tcase_add_test(tc1_1, test_dot);
  tcase_add_test(tc1_1, test_cross);
  tcase_add_test(tc1_1, test_modulus);
  tcase_add_test(tc1_1, test_algebraic_expressions);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
