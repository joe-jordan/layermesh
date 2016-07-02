/* layermesh/test/test_mesh.cpp
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

#include <fstream>
#include <check.h>
#include <mesh.hpp>
#include <stdlib.h>


using namespace std;
using namespace layermesh;

memsafe_gvec_list generate_corner_points() {
  memsafe_gvec_list points = make_shared<gvec_list>();

  points->push_back(gvec(0.0, 0.0, 0.0));
  points->push_back(gvec(1.0, 0.0, 0.0));
  points->push_back(gvec(0.0, 1.0, 0.0));
  points->push_back(gvec(0.0, 0.0, 1.0));

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

START_TEST(test_can_instantiate_mesh) {
  memsafe_gvec_list points = generate_corner_points();

  facet_triples facets = generate_corner_facets();

  Mesh m(points, facets);
} END_TEST

void ck_assert_admesh_output() {
  int grep_ret = system(
      "grep -E \"Total disconnected facets +?: +?0 +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained disconnected facets");

  grep_ret = system("grep -E \"Number of parts +?: +?1\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained more than one component");

  grep_ret = system("grep -E \"Degenerate facets +?: +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained degenerate facets");

  grep_ret = system("grep -E \"Edges fixed +?: +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained invalid edges");

  grep_ret = system("grep -E \"Facets removed +?: +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained unrepairable edges");

  grep_ret = system("grep -E \"Facets added +?: +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained missing facets");

  grep_ret = system("grep -E \"Facets reversed +?: +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained inside out facets");

  grep_ret = system("grep -E \"Backwards edges +?: +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained invalid vertex ordering");

  grep_ret = system("grep -E \"Normals fixed +?: +?0\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "file contained invalid normals");

}

START_TEST(test_can_generate_valid_ascii_stl_file) {
  memsafe_gvec_list points = generate_corner_points();

  facet_triples facets = generate_corner_facets();

  Mesh m(points, facets);

  m.save_stl("foo.stl", false);

  std::ifstream infile("foo.stl");
  ck_assert_msg(infile.good(), "didn't create a file");

  // use `admesh` to verify the STL file is valid and complete:
  int admesh_ret = system("admesh -fundev foo.stl > log.txt");
  ck_assert_msg(admesh_ret == 0, "admesh couldn't verify the generated STL.");

  int grep_ret = system(
      "grep -E \"File type +?: ASCII STL file\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "not recognised as an ASCII STL file.");

  ck_assert_admesh_output();

  system("rm foo.stl log.txt");
} END_TEST

START_TEST(test_can_generate_valid_binary_stl_file) {
  memsafe_gvec_list points = generate_corner_points();

  facet_triples facets = generate_corner_facets();

  Mesh m(points, facets);

  m.save_stl("foo.stl", true);

  std::ifstream infile("foo.stl");
  ck_assert_msg(infile.good(), "didn't create a file");

  // use `admesh` to verify the STL file is valid and complete:
  int admesh_ret = system("admesh -fundev foo.stl > log.txt");
  ck_assert_msg(admesh_ret == 0, "admesh couldn't verify the generated STL.");

  int grep_ret = system(
      "grep -E \"File type +?: Binary STL file\" log.txt > /dev/null");
  ck_assert_msg(grep_ret == 0, "not recognised as an ASCII STL file.");

  ck_assert_admesh_output();

  system("rm foo.stl log.txt");
} END_TEST

int main(void)
{
  Suite *s1 = suite_create("savable mesh object");
  TCase *tc1_1 = tcase_create("all");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_can_instantiate_mesh);
  tcase_add_test(tc1_1, test_can_generate_valid_ascii_stl_file);
  tcase_add_test(tc1_1, test_can_generate_valid_binary_stl_file);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
