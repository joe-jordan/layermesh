/* layermesh/include/tetrahedron.hpp
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
#include <atom.hpp>

namespace layermesh {

  class Tetrahedron : public Atom {
    private:
      gvec_list points;
      gvec centroid;
      void compute_centroid();
      gvec_list facet_normals;
      void compute_normals_and_triples();
      facet_triples _facet_triples;
    public:
      // for the small number of points usually needed to initialise an atom,
      // a copy constructor for gvec_list would probably do.
      Tetrahedron(gvec_list points) : points(points) {
        if (points.size() != 4) {
          throw std::invalid_argument("A tetrahedron has four points.");
        }
        compute_centroid();
      };
      virtual ~Tetrahedron() {};
      virtual memsafe_gvec_list point_cloud();
      virtual unsigned internal_points_start_index() const;
      virtual gsphere get_boundary();
      virtual bool contains(gvec point);
      virtual void save_stl(std::string filename, bool binary);
  };
}


