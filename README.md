Please check out [OpenSCAD](https://github.com/openscad/openscad), which does very well what LayerMesh wanted to do.

# Layermesh
*A library for manipulating 3D meshes as set operations on convex objects* [![Build Status](https://travis-ci.org/joe-jordan/layermesh.svg?branch=master)](https://travis-ci.org/joe-jordan/layermesh)


Layermesh allows you build complicated non-convex 3D meshes by combining convex
objects, and export the resultant mesh as an STL or TIFF file for 3D printing.

Layermesh is currently in development.
It is written in C++, and built on CGAL, the
[Computational Geometry Algorithms Library,](http://www.cgal.org/)
specifically on its
[Triangulation
3D](http://doc.cgal.org/latest/Manual/packages.html#PkgTriangulation3Summary) module, which is licensed under GPLv3 (and therefore so is this project.)

