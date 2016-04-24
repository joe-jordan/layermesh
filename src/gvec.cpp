/* layermesh/src/gvec.cpp
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

#include <gvec.hpp>
#include <cmath>

namespace layermesh {

  // constructors:
  gvec::gvec() {
    r[0] = 0.0;
    r[1] = 0.0;
    r[2] = 0.0;
  }

  gvec::gvec(double x, double y, double z) {
    r[0] = x;
    r[1] = y;
    r[2] = z;
  }

  // compenent access:
  double gvec::operator[](int index) const {
    return r[index];
  }
  double& gvec::operator[](int index) {
    return r[index];
  }

  // addition and subtraction:
  gvec operator+(const gvec& l, const gvec& r) {
    gvec t;
    t[0] = l[0] + r[0];
    t[1] = l[1] + r[1];
    t[2] = l[2] + r[2];
    return t;
  }

  gvec operator-(const gvec& l, const gvec& r) {
    gvec t;
    t[0] = l[0] - r[0];
    t[1] = l[1] - r[1];
    t[2] = l[2] - r[2];
    return t;
  }

  // scaling:
  gvec operator*(double l, const gvec& r) {
    gvec t;
    t[0] = l * r[0];
    t[1] = l * r[1];
    t[2] = l * r[2];
    return t;
  }

  gvec operator*(const gvec& l, double r) {
    gvec t;
    t[0] = r * l[0];
    t[1] = r * l[1];
    t[2] = r * l[2];
    return t;
  }

  gvec operator/(const gvec& l, double r) {
    gvec t;
    t[0] = l[0] / r;
    t[1] = l[1] / r;
    t[2] = l[2] / r;
    return t;
  }

  // dot product:
  double operator*(const gvec& l, const gvec& r) {
    return l[0] * r[0] + l[1] * r[1] + l[2] * r[2];
  }

  // cross product:
  gvec operator^(const gvec& l, const gvec& r) {
    gvec t;
    t[0] = l[1] * r[2] - l[2] * r[1];
    t[1] = l[2] * r[0] - l[0] * r[2];
    t[2] = l[0] * r[1] - l[1] * r[0];
    return t;
  }

  double modulus(const gvec& v) {
    return sqrt(v * v);
  }
}

