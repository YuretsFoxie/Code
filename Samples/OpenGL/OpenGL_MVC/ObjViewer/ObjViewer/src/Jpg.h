///////////////////////////////////////////////////////////////////////////////
// Jpg.h
// =====
// JPG image loader/writer class using libjpeg-turbo (or libjpeg) lib.
// http://www.libjpeg-turbo.org
//
// Dependency: This class requires libjpeg-turbo (or libjpeg)
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2013-06-11
// UPDATED: 2013-06-13
///////////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_JPG_H
#define IMAGE_JPG_H

#include <string>
#include <vector>

namespace Image
{
    class Jpg
    {
    public:
        // ctor/dtor
        Jpg();
        Jpg(const Jpg& rhs);
        ~Jpg();

        Jpg& operator=(const Jpg &rhs);             // assignment operator

        // load image header and data from a jpeg file
        bool read(const char* fileName);

        // save an image as JPG format
        // quality is the percentage value between 0~100
        bool save(const char* fileName, int width, int height, int channelCount,
                  const unsigned char* data, int quality=90);

        // getters
        int getWidth() const;                       // return width of image in pixel
        int getHeight() const;                      // return height of image in pixel
        int getBitCount() const;                    // return the number of bits per pixel (8, 24, or 32)
        std::size_t getDataSize() const;            // return data size in bytes
        const unsigned char* getData() const;       // return the pointer to image data

        void printSelf() const;                     // print itself for debug purpose
        const char* getError() const;               // return last error message

    protected:


    private:
        // member functions
        void init();                                // clear the existing values

        // member variables
        int width;
        int height;
        int bitCount;
        std::size_t dataSize;
        unsigned char* data;                        // data with default BGR order
        std::string errorMessage;
    };


    ///////////////////////////////////////////////////////////////////////////
    // inline functions
    ///////////////////////////////////////////////////////////////////////////
    inline int Jpg::getWidth() const { return width; }
    inline int Jpg::getHeight() const { return height; }

    // return bits per pixel, 8 means grayscale, 24 means RGB color, 32 means RGBA
    inline int Jpg::getBitCount() const { return bitCount; }

    inline std::size_t Jpg::getDataSize() const { return dataSize; }
    inline const unsigned char* Jpg::getData() const { return &data[0]; }

    inline const char* Jpg::getError() const { return errorMessage.c_str(); }
}

#endif // IMAGE_JPG_H
