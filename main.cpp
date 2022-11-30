#include <png_reader/png_reader.h>

int main(int argc, char** argv){
    PNGReader reader;
    reader.readPNG(argv[1]);
    // print data
    std::cout << reader;

    return 0;
}
