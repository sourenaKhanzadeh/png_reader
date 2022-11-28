#include <png_reader/png_reader.h>

int main(){
    PNGReader reader;
    reader.readPNG("pngs/test.png");
    // print data
    std::vector<unsigned char> data = reader.getRawData();
    for (int i = 0; i < data.size(); i++)
    {
        std::cout << (int)data[i] << " ";
    }

    return 0;
}
