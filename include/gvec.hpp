/* layermesh/include/gvec.hpp
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

#ifndef __LAYERMESH_GVEC_HPP__
#define __LAYERMESH_GVEC_HPP__

#include <vector>
#include <memory>

namespace layermesh {

  // gvec = Geometrical Vector (as opposed to std::vector).
  class gvec {
    private:
      double r[3];
    public:
      gvec();
      gvec(double x, double y, double z);

      // component access:
      double operator[](int index) const;
      double& operator[](int index);
  };

  // addition and subtraction:
  gvec operator+(const gvec& l, const gvec& r);
  gvec operator-(const gvec& l, const gvec& r);

  // negation:
  gvec operator-(const gvec&r);

  // scaling:
  gvec operator*(double l, const gvec& r);
  gvec operator*(const gvec& l, double r);
  gvec operator/(const gvec& l, double r);

  // dot product:
  double operator*(const gvec& l, const gvec& r);

  // cross product:
  gvec operator^(const gvec& l, const gvec& r);

  double modulus(const gvec& v);

  // aliases for collections of gvec's:
  typedef std::vector<gvec> gvec_list;
  typedef std::shared_ptr<std::vector<gvec> > memsafe_gvec_list;

  // gsphere
  typedef struct {
    gvec centre;
    double radius;
  } gsphere;
}

#endif

