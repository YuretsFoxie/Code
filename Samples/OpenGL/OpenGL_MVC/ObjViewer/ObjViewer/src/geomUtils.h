///////////////////////////////////////////////////////////////////////////////
// geomUtils.h
// ===========
// Utility functions for geometry
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2020-10-01
// UPDATED: 2020-10-04
///////////////////////////////////////////////////////////////////////////////

#ifndef GEOM_UTILS_DEF
#define GEOM_UTILS_DEF

///////////////////////////////////////////////////////////////////////////////
// compute the volume of an arbitrary 3D mesh using shoelace formula
// Mesh volume = SUM(Volume(tetrahedron))
// Volume(tetrahedron) = 1/6 * Volume(parallelCylinder)
//                                |x1 y1 z1|
//                     = 1/6 * det|x2 y2 z2|
//                                |x3 y3 z3|
// where v1=(x1,y1,z1), v2=(x2,y2,z2), v3=(x3,y3,z3)
double computeVolume(const float* vertices, const unsigned int* indices, int indexCount);



///////////////////////////////////////////////////////////////////////////////
// compute surface area of an arbitrary mesh
// Surface Area = SUM(Area(triangle))
// Area(triangle) = 1/2 * Area(parallelogram)
//                = 1/2 * Mag(v12 x v13)
// where v12 = v2 - v1, v13 = v3 -v1
double computeArea(const float* vertices, const unsigned int* indices, int indexCount);

#endif

