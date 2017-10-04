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
#include <stdint.h>
#include <assert.h>

using namespace std;

namespace layermesh {

  void Mesh::save_stl_inner(std::string filename,
                            bool binary,
                            const memsafe_gvec_list& points,
                            const facet_triples& facets) {
    if (binary)
      save_binary_stl(filename, points, facets);
    else
      save_ascii_stl(filename, points, facets);
  }

  string gvec_ascii(gvec v) {
    ostringstream s;
    s << static_cast<float>(v[0]) << " ";
    s << static_cast<float>(v[1]) << " ";
    s << static_cast<float>(v[2]);
    return s.str();
  }

  void Mesh::save_ascii_stl(string filename,
                            const memsafe_gvec_list& points,
                            const facet_triples& facets) {
    ofstream f(filename.c_str());
    f << "solid layermesh" << endl;
    facet_triples::const_iterator fit = facets.begin();
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

  void gvec_binary(char* buffer, gvec v) {
    // writes 12 bytes (4 * float32) into the buffer.
    unsigned i;
    float r;
    uint32_t* rb = &reinterpret_cast<uint32_t&>(r);
    uint32_t* buffer32 = reinterpret_cast<uint32_t*>(buffer);
    for (i = 0; i < 3; ++i) {
      r = static_cast<float>(v[i]);
      buffer32[i] = *rb;
    }
  }

  void Mesh::save_binary_stl(string filename,
                             const memsafe_gvec_list& points,
                             const facet_triples& facets) {
    ofstream f(filename.c_str(), ios::out | ios::binary);
    char a[81] = {'l', 'a', 'y', 'e', 'r', 'm', 'e', 's', 'h'};
    f.write(a, 80);

    uint32_t num_facets = facets.size();
    f.write(reinterpret_cast<char*>(&num_facets), 4);

    unsigned facets_written = 0;
    uint16_t attributes = 0;

    facet_triples::const_iterator fit = facets.begin();
    char b[13];
    for (; fit != facets.end(); ++fit) {
      gvec o = (*points)[(*fit)[0]];
      gvec i = (*points)[(*fit)[1]];
      gvec j = (*points)[(*fit)[2]];

      gvec n = (i - o) ^ (j - o);
      n = n / layermesh::modulus(n);

      gvec_binary(b, n);
      f.write(b, 12);

      gvec_binary(b, o);
      f.write(b, 12);

      gvec_binary(b, i);
      f.write(b, 12);

      gvec_binary(b, j);
      f.write(b, 12);

      f.write(reinterpret_cast<char*>(&attributes), 2);
      ++facets_written;
    }
    assert(facets_written == num_facets);
    f.close();
  }

}

