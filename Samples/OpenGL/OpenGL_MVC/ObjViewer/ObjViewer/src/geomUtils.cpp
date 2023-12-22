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

#include <cmath>
#include <iostream>

double computeTetrahedronVolume(const float v1[3], const float v2[3], const float v3[3]);
double computeTriangleArea(const float v1[3], const float v2[3], const float v3[3]);



///////////////////////////////////////////////////////////////////////////////
// compute the volume of an arbitrary 3D mesh using shoelace formula
// Mesh volume = SUM(Volume(tetrahedron))
// Volume(tetrahedron) = 1/6 * Volume(parallelCylinder)
//                                |x1 y1 z1|
//                     = 1/6 * det|x2 y2 z2|
//                                |x3 y3 z3|
// where v1=(x1,y1,z1), v2=(x2,y2,z2), v3=(x3,y3,z3)
double computeVolume(const float* vertices, const unsigned int* indices, int indexCount)
{
    double sum = 0;
    for(int i = 0; i < indexCount; i += 3)
    {
        const float* v1 = &vertices[indices[i]*3];
        const float* v2 = &vertices[indices[i+1]*3];
        const float* v3 = &vertices[indices[i+2]*3];
        sum += computeTetrahedronVolume(v1, v2, v3);
        //std::cout << i << ": " << computeTetrahedronVolume(v1, v2, v3) << std::endl;
    }
    return sum;
}

///////////////////////////////////////////////////////////////////////////////
// compute the volume of a tetrahedron constructed by 3 vertices of a triangle
// and the origin where v1=(x1,y1,z1), v2=(x2,y2,z2), v3=(x3,y3,z3)
//
// Volume(tetra) = 1/6 * Volume(parallelCylinder)
//                          |x1 y1 z1|
//               = 1/6 * det|x2 y2 z2|
//                          |x3 y3 z3|
//               = 1/6 * (x1(y2z3-y3z2) - x2(y1z3-y3z1) + x3(y1z2-y2z1))
//               = 1/6 * (x1y2z3 - x1y3z2 - x2y1z3 + x2y3z1 + x3y1z2 - x3y2z1)
///////////////////////////////////////////////////////////////////////////////
double computeTetrahedronVolume(const float v1[3], const float v2[3], const float v3[3])
{
    double det1 = v2[1]*v3[2] - v3[1]*v2[2];                 // determinant X1
    double det2 = v1[1]*v3[2] - v3[1]*v1[2];                 // determinant X2
    double det3 = v1[1]*v2[2] - v2[1]*v1[2];                 // determinant X3
    return 1.0 / 6 * (v1[0]*det1 - v2[0] * det2 + v3[0] * det3);
}



///////////////////////////////////////////////////////////////////////////////
// compute surface area of an arbitrary mesh
// Surface Area = SUM(Area(triangle))
// Area(triangle) = 1/2 * Area(parallelogram)
//                = 1/2 * Mag(v12 x v13)
// where v12 = v2 - v1, v13 = v3 -v1
double computeArea(const float* vertices, const unsigned int* indices, int indexCount)
{
    double sum = 0;
    for(int i = 0; i < indexCount; i += 3)
    {
        const float* v1 = &vertices[indices[i]*3];
        const float* v2 = &vertices[indices[i+1]*3];
        const float* v3 = &vertices[indices[i+2]*3];
        sum += computeTriangleArea(v1, v2, v3);
        //std::cout << i << ": " << computeTriangleArea(v1, v2, v3) << std::endl;
    }
    return sum;
}

double computeTriangleArea(const float v1[3], const float v2[3], const float v3[3])
{
    float v12[3];           // v2 - v1
    v12[0] = v2[0] - v1[0];
    v12[1] = v2[1] - v1[1];
    v12[2] = v2[2] - v1[2];
    float v13[3];           // v3 - v1
    v13[0] = v3[0] - v1[0];
    v13[1] = v3[1] - v1[1];
    v13[2] = v3[2] - v1[2];

    // v12 * v13 (cross product)
    double cross[3];
    cross[0] = v12[1]*v13[2] - v13[1]*v12[2];
    cross[1] = v12[2]*v13[0] - v13[2]*v12[0];
    cross[2] = v12[0]*v13[1] - v13[0]*v12[1];

    return 1.0 / 2 * sqrt(cross[0]*cross[0] + cross[1]*cross[1] + cross[2]*cross[2]);
}


#endif

