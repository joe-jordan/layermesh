/* layermesh/test/stl_helper.hpp
 * Copyright Joe Jordan <joe@joe-jordan.co.uk> 2017.
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
#include <fstream>


inline void EXPECT_VALID_STL(const char* filename, bool binary) {

  std::ifstream infile(filename);
  EXPECT_TRUE(infile.good()) << "didn't create a file";

  // use `admesh` to verify the STL file is valid and complete:
  char* admesh_command = NULL;
  asprintf(&admesh_command, "admesh -fundev %s > log.txt", filename);
  int admesh_ret = system(admesh_command);
  ASSERT_EQ(admesh_ret, 0) << "admesh couldn't verify the generated STL.";

  free(admesh_command);

  int grep_ret;

  if (binary) {
    grep_ret = system(
        "grep -E \"File type +?: Binary STL file\" log.txt > /dev/null");
    EXPECT_EQ(grep_ret, 0) << "not recognised as an Binary STL file.";
  } else {
    grep_ret = system(
        "grep -E \"File type +?: ASCII STL file\" log.txt > /dev/null");
    EXPECT_EQ(grep_ret, 0) << "not recognised as an ASCII STL file.";
  }

  grep_ret = system(
      "grep -E \"Total disconnected facets +?: +?0 +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained disconnected facets";

  grep_ret = system("grep -E \"Number of parts +?: +?1\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained more than one component";

  grep_ret = system("grep -E \"Degenerate facets +?: +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained degenerate facets";

  grep_ret = system("grep -E \"Edges fixed +?: +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained invalid edges";

  grep_ret = system("grep -E \"Facets removed +?: +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained unrepairable edges";

  grep_ret = system("grep -E \"Facets added +?: +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained missing facets";

  grep_ret = system("grep -E \"Facets reversed +?: +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained inside out facets";

  grep_ret = system("grep -E \"Backwards edges +?: +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained invalid vertex ordering";

  grep_ret = system("grep -E \"Normals fixed +?: +?0\" log.txt > /dev/null");
  EXPECT_EQ(grep_ret, 0) << "file contained invalid normals";

  system("rm log.txt");

}

