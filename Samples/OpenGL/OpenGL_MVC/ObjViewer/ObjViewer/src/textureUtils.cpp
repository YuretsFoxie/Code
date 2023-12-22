//
// textureUtils.cpp
// ================
// OpenGL texture helper functions
// Dependency: Tga.h/cpp, Bmp.h/cpp, Png.h/cpp (lodepng.h/cpp)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2011-09-06
// UPDATED: 2013-06-13
//
// Copyright 2009 Song Ho Ahn. All rights reserved.
///////////////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include <string>
#include <algorithm>
#include "Tga.h"
#include "Bmp.h"
#include "Png.h"
#include "Jpg.h"
#include "textureUtils.h"
#include "glext.h"          // GL_CLAMP_TO_EDGE, GL_GENERATE_MIPMAP



///////////////////////////////////////////////////////////////////////////////
// load a texture into a OpenGL texture object
///////////////////////////////////////////////////////////////////////////////
GLuint loadTexture(const std::string& fileName, bool repeat)
{
    Image::Bmp bmp;
    Image::Tga tga;
    Image::Png png;
    Image::Jpg jpg;
    int width = 0;
    int height = 0;
    int bitCount = 0;
    const unsigned char* data = 0;
    GLuint texId = 0;
    GLenum format = 0;
    bool result = false;

    // get file extension
    std::string extension = fileName.substr(fileName.find_last_of(".") + 1);
    // convert yo lower case
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    // read the image
    if(extension == "bmp")
    {
        result = bmp.read(fileName.c_str());
        width = bmp.getWidth();
        height = bmp.getHeight();
        bitCount = bmp.getBitCount();
        data = bmp.getDataRGB();
    }
    else if(extension == "tga")
    {
        result = tga.read(fileName.c_str());
        width = tga.getWidth();
        height = tga.getHeight();
        bitCount = tga.getBitCount();
        data = tga.getDataRGB();
    }
    else if(extension == "png")
    {
        result = png.read(fileName.c_str());
        width = png.getWidth();
        height = png.getHeight();
        bitCount = png.getBitCount();
        data = png.getData();
    }
    else if(extension == "jpg" || extension == "jpeg")
    {
        result = jpg.read(fileName.c_str());
        width = jpg.getWidth();
        height = jpg.getHeight();
        bitCount = jpg.getBitCount();
        data = jpg.getData();
    }

    if(!result)
    {
        std::cout << "[ERROR] Failed to open a texture, \"" << fileName << "\"." << std::endl;
        return 0;
    }

    // determine the format
    if(bitCount == 8)
        format = GL_ALPHA;
    else if(bitCount == 24)
        format = GL_RGB;
    else if(bitCount == 32)
        format = GL_RGBA;

    // OpenGL texture object
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    if(repeat)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);    // unbind

    std::cout << "Loaded a texture, \"" << fileName << "\"." << std::endl;
    return texId;
}

