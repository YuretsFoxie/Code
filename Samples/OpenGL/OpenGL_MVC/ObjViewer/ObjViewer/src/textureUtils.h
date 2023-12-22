//
// textureUtils.h
// ==============
// OpenGL texture helper functions
// Dependency: Tga.h/cpp, Bmp.h/cpp, Png.h/cpp (lodepng.h/cpp)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2011-09-06
// UPDATED: 2013-01-23
//
// Copyright 2011 Song Ho Ahn. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#ifndef TEXTURE_UTILS_H
#define TEXTURE_UTILS_H

#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <windows.h>
#include <GL/gl.h>
#endif

#include <string>

GLuint loadTexture(const std::string& name, bool repeat=false);

#endif
