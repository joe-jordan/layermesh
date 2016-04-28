/* layermesh/src/mesh.cpp
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

#include <mesh.hpp>
#include <sstream>
#include <fstream>

using namespace std;

namespace layermesh {

  gvec_list Facet::get_vertices() {
    // return the vector<gvec> by value. 3 struct copies is cheap.
    return vertices;
  }
  gvec Facet::get_normal() {
    return (vertices[1] - vertices[0]) ^ (vertices[2] - vertices[0]);
  }

  void Mesh::save_stl(string filename, bool binary) {
    if (binary)
      save_binary_stl(filename);
    else
      save_ascii_stl(filename);
  }

  string gvec_ascii(gvec v) {
    ostringstream s;
    s << static_cast<float>(v[0]) << " ";
    s << static_cast<float>(v[1]) << " ";
    s << static_cast<float>(v[2]);
    return s.str();
  }

  void Mesh::save_ascii_stl(string filename) {
    ofstream f(filename.c_str());
    f << "solid layermesh" << endl;
    facet_triples::iterator fit = facets.begin();
    for (; fit != facets.end(); ++fit) {
      gvec o = (*points)[(*fit)[0]];
      gvec i = (*points)[(*fit)[1]];
      gvec j = (*points)[(*fit)[2]];

      gvec n = (i - o) ^ (j - o);
      n = n / layermesh::modulus(n);

      f << "  facet normal " << gvec_ascii(n) << endl;
      f << "    outer loop" << endl;
      f << "      vertex " << gvec_ascii(o) << endl;
      f << "      vertex " << gvec_ascii(i) << endl;
      f << "      vertex " << gvec_ascii(j) << endl;
      f << "    endloop" << endl;
      f << "  endfacet" << endl;
    }
    f << "endsolid layermesh" << endl;
    f.close();
  }
  void Mesh::save_binary_stl(string filename) {

  }

}

