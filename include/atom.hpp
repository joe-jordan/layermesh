/* layermesh/include/hull.hpp
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

#ifndef __LAYERMESH_HULL_HPP__
#define __LAYERMESH_HULL_HPP__

#include <gvec.hpp>
#include <mesh.hpp>

namespace layermesh {

  // Abstract interface that you must implement for all atoms (convex objects)
  // you wish to use in layered meshes. Note that you do not need to compute
  // the convex hull; this type contains a routine that will compute the facets
  // of the convex hull for you.
  class Atom : public Mesh {
    public:
      virtual ~Atom() {};
      // It is assumed that point_cloud() returns all boundary points
      // continguously at the start of the vector. If additional internal
      // points are specified (sometimes necessary to avoid triangulation
      // symmetry errors) these should be at the end of the vector.
      // Return is a shared_ptr: if there is any computation to be done to
      // obtain the vector, it should be done once and the result cached on
      // the instance for future accesses by the same method.
      virtual memsafe_gvec_list point_cloud() = 0;
      // The starting index of the optional internal points. If no internal
      // points were required, should return == point_cloud().size().
      virtual unsigned internal_points_start_index() const = 0;
      // In order to efficiently compare points within hulls, we must know the
      // approximate position and size of the hull. get_boundary() should
      // return a gsphere which completely contains the hull, (ideally as small
      // as possible, but an approximation is fine.)
      virtual gsphere get_boundary() = 0;
      // For rendering TIFFs, if the derived class has a simple geometrical
      // test for whether it contains a particular point, it is best to
      // implement it here. Otherwise, the default implementation will be
      // included (which is slow, because it uses the convex hull calculation.)
      virtual bool contains(gvec point);
      // This method also has a default implementation which uses the convex
      // hull calculation. Again, if you can provide the facets for your Atom
      // in format required by layermesh::Mesh, then override this method.
      virtual void save_stl(std::string filename, bool binary);
  };

}

#endif
