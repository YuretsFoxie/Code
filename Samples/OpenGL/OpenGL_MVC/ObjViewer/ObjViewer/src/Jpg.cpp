///////////////////////////////////////////////////////////////////////////////
// Jpg.cpp
// =======
// JPG image loader/writer class using libjpeg-turbo (or libjpeg) lib.
// http://www.libjpeg-turbo.org
//
// Dependency: This class requires libjpeg-turbo (or libjpeg)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-06-11
// UPDATED: 2013-06-13
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <cstring>                      // for memcpy()
#include <cstdio>
#include "jpeglib.h"
#include "Jpg.h"

using namespace Image;



///////////////////////////////////////////////////////////////////////////////
// default constructor
///////////////////////////////////////////////////////////////////////////////
Jpg::Jpg() : width(0), height(0), bitCount(0), dataSize(0), data(0), errorMessage("No error.")
{
}



///////////////////////////////////////////////////////////////////////////////
// copy constructor
// We need DEEP COPY for dynamic memory variables because the compiler inserts
// default copy constructor automatically for you, BUT it is only SHALLOW COPY
///////////////////////////////////////////////////////////////////////////////
Jpg::Jpg(const Jpg& rhs)
{
    // copy member variables from right-hand-side object
    width = rhs.getWidth();
    height = rhs.getHeight();
    bitCount = rhs.getBitCount();
    dataSize = rhs.getDataSize();
    errorMessage = rhs.getError();

    if(rhs.getData())       // allocate memory only if the pointer is not NULL
    {
        data = new unsigned char[dataSize];
        memcpy(data, rhs.getData(), dataSize); // deep copy
    }
    else
        data = 0;           // array is not allocated yet, set to 0
}



///////////////////////////////////////////////////////////////////////////////
// default destructor
///////////////////////////////////////////////////////////////////////////////
Jpg::~Jpg()
{
    // deallocate data array
    delete [] data;
    data = 0;
}



///////////////////////////////////////////////////////////////////////////////
// clear out the exsiting values
///////////////////////////////////////////////////////////////////////////////
void Jpg::init()
{
    width = height = bitCount = 0;
    dataSize = 0;
    errorMessage = "No error.";

    delete [] data;
    data = 0;
}



///////////////////////////////////////////////////////////////////////////////
// override assignment operator
///////////////////////////////////////////////////////////////////////////////
Jpg& Jpg::operator=(const Jpg &rhs)
{
    if(this == &rhs)        // avoid self-assignment (A = A)
        return *this;

    // copy member variables
    width = rhs.getWidth();
    height = rhs.getHeight();
    bitCount = rhs.getBitCount();
    dataSize = rhs.getDataSize();
    errorMessage = rhs.getError();

    if(rhs.getData())       // allocate memory only if the pointer is not NULL
    {
        data = new unsigned char[dataSize];
        memcpy(data, rhs.getData(), dataSize);
    }
    else
        data = 0;

    return *this;
}



///////////////////////////////////////////////////////////////////////////////
// print itself for debug
///////////////////////////////////////////////////////////////////////////////
void Jpg::printSelf() const
{
    std::cout << "===== Jpg =====\n"
              << "Width: " << width << " pixels\n"
              << "Height: " << height << " pixels\n"
              << "Bit Count: " << bitCount << " bits\n"
              << "Data Size: " << dataSize  << " bytes\n"
              << std::endl;
}



///////////////////////////////////////////////////////////////////////////////
// read a JPG image header infos and datafile and load
// It uses libjpeg-turbo lib.
///////////////////////////////////////////////////////////////////////////////
bool Jpg::read(const char* fileName)
{
    this->init();   // clear out all values

    // check NULL pointer
    if(!fileName)
    {
        errorMessage = "File name is not defined (NULL pointer).";
        return false;
    }

    // open a JPG file as binary mode
    FILE* inFile = fopen(fileName, "rb"); // binary mode
    if(!inFile)
    {
        errorMessage = "Failed to open a JPG file to read.";
        return false;            // exit if failed
    }

    // init error handler
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    // init decompressor
    jpeg_create_decompress(&cinfo);

    // specify data source
    jpeg_stdio_src(&cinfo, inFile);

    // read image parameters
    jpeg_read_header(&cinfo, true);

    // start decompressor
    // after this call, the final output image parameters are available
    jpeg_start_decompress(&cinfo);

    // set image parameters
    width = cinfo.output_width;
    height = cinfo.output_height;
    bitCount = cinfo.output_components * 8;
    dataSize = width * height * cinfo.output_components;
    data = new unsigned char[dataSize];

    // allocate tmp buffer
    int bytesPerRow = cinfo.output_width * cinfo.output_components;
    JSAMPLE* row = new JSAMPLE[bytesPerRow * sizeof(JSAMPLE)]; // JSAMPLE = unsigned char

    // read line by line
    int i = 0;
    while(cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, &row, 1);
        memcpy(&data[i*bytesPerRow], row, bytesPerRow);
        ++i;
    }

    // finish decompressor
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    delete [] row;

    fclose(inFile);
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// save an image as a JPG format
///////////////////////////////////////////////////////////////////////////////
bool Jpg::save(const char* fileName, int w, int h, int channelCount,
               const unsigned char* data, int quality)
{
    // reset error message
    errorMessage = "No error.";

    if(!fileName || !data)
    {
        errorMessage = "File name is not specified (NULL pointer).";
        return false;
    }

    if(w == 0 || h == 0)
    {
        errorMessage = "Zero width or height.";
        return false;
    }

    FILE* outFile = fopen(fileName, "wb");
    if(!outFile)
    {
        errorMessage = "Failed to open the output file.";
        return false;
    }

    // init error handler
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outFile);

    // set image attributes
    cinfo.image_width = w;
    cinfo.image_height = h;
    cinfo.input_components = channelCount;
    if(channelCount == 1)
    {
        cinfo.in_color_space = JCS_GRAYSCALE;
    }
    else if(channelCount == 3)
    {
        cinfo.in_color_space = JCS_RGB;
    }
    else
    {
        errorMessage = "The channelCount must be 1, 3 or 4.";
        fclose(outFile);
        return false;
    }

    // use default encode method
    jpeg_set_defaults(&cinfo);

    // set image quality 0 ~ 100
    jpeg_set_quality(&cinfo, quality, true);

    // start compressor
    jpeg_start_compress(&cinfo, true);

    // copy a single scanline at a time
    int bytesPerRow = w * channelCount;
    JSAMPROW row;
    while(cinfo.next_scanline < cinfo.image_height)
    {
        row = (JSAMPROW)(data + (cinfo.next_scanline * bytesPerRow));
        jpeg_write_scanlines(&cinfo, &row, 1);
    }

    // finish compressor
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(outFile);
    return true;
}
