/* layermesh/include/mesh.hpp
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

#ifndef __LAYERMESH_MESH_HPP__
#define __LAYERMESH_MESH_HPP__

#include <gvec.hpp>
#include <string>
#include <array>
#include <vector>

namespace layermesh {

  // It is assumed that each inner vector has 3 entries. The unsigneds are
  // indices into the gvec_list, which should be in order such that:
  // normal = (v1 - v0) ^ (v2 - v0)
  // (where the normal points from the solid phase to the void.)
  // This is the convention which is used in the STL format.
  typedef std::array<unsigned,3> facet_triple;
  typedef std::vector<facet_triple> facet_triples;

  class Facet {
    private:
      gvec_list vertices;
    public:
      gvec_list get_vertices();
      gvec get_normal();
  };

  typedef std::shared_ptr<std::vector<Facet> > memsafe_facet_list;

  class Mesh {
    private:
      memsafe_gvec_list points;
      facet_triples facets;
      void save_ascii_stl(std::string filename);
      void save_binary_stl(std::string filename);
    public:
      Mesh(memsafe_gvec_list points, facet_triples facets) :
        points(points), facets(facets) {};
      ~Mesh() {};
      void save_stl(std::string filename, bool binary = true);
  };

}

#endif
