#include <png_reader/png_reader.h>

int main(int argc, char** argv){
    PNGReader reader;
    reader.readPNG(argv[1]);
    // print data
    std::vector<unsigned char> data = reader.getRawData();
    for (int i = 0; i < data.size(); i++)
    {
        std::cout << (int)data[i] << " ";
    }

    return 0;
}
