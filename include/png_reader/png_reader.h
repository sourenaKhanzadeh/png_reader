#ifndef PNG_READER_H
#define PNG_READER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>



class PNGReader
{
public:
    PNGReader();
    ~PNGReader();

    bool readPNG(const std::string& filename);
    bool writePNG(const std::string& filename);

    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void getPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b);

    int getWidth() const;
    int getHeight() const;

    std::vector<unsigned char> getRawData() const;

    friend std::ostream& operator<<(std::ostream& os, const PNGReader& reader);

private:
    bool readHeader(std::ifstream& file);
    bool readData(std::ifstream& file);
    bool writeHeader(std::ofstream& file);
    bool writeData(std::ofstream& file);
    bool png_sig_cmp(const unsigned char* sig, size_t start, size_t num_to_check);

    int width;
    int height;
    std::vector<unsigned char> data;
};

// implementation

PNGReader::PNGReader()
    : width(0)
    , height(0)
{
}

PNGReader::~PNGReader()
{
}

bool PNGReader::readPNG(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Could not open file " << filename << std::endl;
        return false;
    }

    if (!readHeader(file))
    {
        std::cout << "Could not read header" << std::endl;
        return false;
    }

    if (!readData(file))
    {
        std::cout << "Could not read data" << std::endl;
        return false;
    }

    return true;
}

bool PNGReader::writePNG(const std::string& filename)
{
    std::ofstream file(filename.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Could not open file " << filename << std::endl;
        return false;
    }

    if (!writeHeader(file))
    {
        std::cout << "Could not write header" << std::endl;
        return false;
    }

    if (!writeData(file))
    {
        std::cout << "Could not write data" << std::endl;
        return false;
    }

    return true;
}

void PNGReader::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    int index = (y * width + x) * 3;
    data[index] = r;
    data[index + 1] = g;
    data[index + 2] = b;
}

void PNGReader::getPixel(int x, int y, unsigned char& r, unsigned char& g, unsigned char& b)
{
    int index = (y * width + x) * 3;
    r = data[index];
    g = data[index + 1];
    b = data[index + 2];
}

int PNGReader::getWidth() const
{
    return width;
}

int PNGReader::getHeight() const
{
    return height;
}

bool PNGReader::readHeader(std::ifstream &file)
{
    // read header
    unsigned char header[8];
    file.read((char*)header, 8);

    // check if it is a png file
    if (png_sig_cmp(header, 0, 8))
    {
        std::cout << "File is not a png file" << std::endl;
        return false;
    }

    // read IHDR chunk
    unsigned int length;
    file.read((char*)&length, 4);
    length = __builtin_bswap32(length);

    unsigned char type[4];
    file.read((char*)type, 4);

    if (std::string((char*)type, 4) != "IHDR")
    {
        std::cout << "File does not contain IHDR chunk" << std::endl;
        return false;
    }

    file.read((char*)&width, 4);
    width = __builtin_bswap32(width);

    file.read((char*)&height, 4);
    height = __builtin_bswap32(height);

    unsigned char bit_depth;
    file.read((char*)&bit_depth, 1);

    unsigned char color_type;
    file.read((char*)&color_type, 1);

    unsigned char compression_method;
    file.read((char*)&compression_method, 1);

    unsigned char filter_method;
    file.read((char*)&filter_method, 1);

    unsigned char interlace_method;
    file.read((char*)&interlace_method, 1);

    unsigned int crc;
    file.read((char*)&crc, 4);
    crc = __builtin_bswap32(crc);

    return true;

}


bool PNGReader::readData(std::ifstream &file)
{
    // read data
    file.seekg(0, std::ios::end);
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    data.resize(size);
    file.read((char*)&data[0], size);

    // check if it is a png file
    if (data[0] != 137 || data[1] != 80 || data[2] != 78 || data[3] != 71 || data[4] != 13 || data[5] != 10 || data[6] != 26 || data[7] != 10)
    {
        std::cout << "Not a PNG file" << std::endl;
        return false;
    }

    // check if it is a 24 bit file
    if (data[25] != 8)
    {
        std::cout << "Not a 24 bit file" << std::endl;
        return false;
    }

    // check if it is a RGB file
    if (data[24] != 2)
    {
        std::cout << "Not a RGB file" << std::endl;
        return false;
    }

    // get width and height
    width = data[16] * 256 * 256 * 256 + data[17] * 256 * 256 + data[18] * 256 + data[19];

    height = data[20] * 256 * 256 * 256 + data[21] * 256 * 256 + data[22] * 256 + data[23];

    return true;
}

bool PNGReader::writeHeader(std::ofstream &file)
{
    // write header
    unsigned char header[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    file.write((char*)header, 8);

    // write IHDR chunk
    unsigned int length = __builtin_bswap32(13);
    file.write((char*)&length, 4);

    unsigned char type[4] = {'I', 'H', 'D', 'R'};
    file.write((char*)type, 4);

    unsigned int width = __builtin_bswap32(this->width);
    file.write((char*)&width, 4);

    unsigned int height = __builtin_bswap32(this->height);
    file.write((char*)&height, 4);

    unsigned char bit_depth = 8;
    file.write((char*)&bit_depth, 1);

    unsigned char color_type = 2;
    file.write((char*)&color_type, 1);

    unsigned char compression_method = 0;
    file.write((char*)&compression_method, 1);

    unsigned char filter_method = 0;
    file.write((char*)&filter_method, 1);

    unsigned char interlace_method = 0;
    file.write((char*)&interlace_method, 1);

    unsigned int crc = __builtin_bswap32(0x0d0a1a0a);
    file.write((char*)&crc, 4);

    return true;
}

bool PNGReader::writeData(std::ofstream &file)
{
    // write data
    file.write((char*)&data[0], data.size());

    return true;
}


std::vector<unsigned char> PNGReader::getRawData() const
{
    return data;
}

bool PNGReader::png_sig_cmp(const unsigned char* sig, size_t start, size_t num_to_check)
{
    if (sig[start] != 137 || sig[start + 1] != 80 || sig[start + 2] != 78 || sig[start + 3] != 71 || sig[start + 4] != 13 || sig[start + 5] != 10 || sig[start + 6] != 26 || sig[start + 7] != 10)
    {
        return true;
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, const PNGReader& png)
{
    os << "Width: " << png.width << std::endl;
    os << "Height: " << png.height << std::endl;
    os << "Data size: " << png.data.size() << std::endl;
    // print all channels in a 3d grid
    for (int y = 0; y < png.height; y++)
    {
        for (int x = 0; x < png.width; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                os << (int)png.data[16 + y * png.width * 3 + x * 3 + c] << " ";
            }
            os << " | ";
        }
        os << std::endl;
    }

    return os;
}



#endif