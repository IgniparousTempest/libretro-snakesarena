#include <iostream>
#include "image.hpp"

enum BMPFormat {
    BITMAPV5HEADER = 124
};

struct BITMAPFILEHEADER {
    char bfType[2];
    char bfSize[4];
    char bfReserved1[2];
    char bfReserved2[2];
    char bfOffBits[4];
};

struct BITMAPINFOHEADER {
    char biSize[4];
    char biWidth[4];
    char biHeight[4];
    char biPlanes[2];
    char biBitCount[2];
    char biCompression[4];
    char biSizeImage[4];
    char biXPelsPerMeter[4];
    char biYPelsPerMeter[4];
    char biClrUsed[4];
    char biClrImportant[4];
};

struct BMP_FILE {
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
};

Texture *Image::LoadBMP(const std::string &file_path) {
    std::ifstream input_file(file_path, std::ios::binary);

    if (input_file.fail())
        throw std::runtime_error("File not found: " + file_path);

    // Bitmap File Header
    BITMAPFILEHEADER bitmap_file_header{};
    input_file.read((char*)&bitmap_file_header, sizeof(bitmap_file_header));

    // Bitmap Info Header
    BITMAPINFOHEADER bitmap_info_header{};
    input_file.read((char*)&bitmap_info_header, sizeof(bitmap_info_header));

    if (Int(bitmap_info_header.biSize) != BITMAPV5HEADER)
        throw std::runtime_error("Only NT V5 bitmaps are supported.");

    // Read data
    input_file.seekg(Int(bitmap_file_header.bfOffBits), std::ifstream::beg);
    int data_size = Int(bitmap_info_header.biSizeImage);
    int channels = 4;
    int width = Int(bitmap_info_header.biWidth);
    int height = Int(bitmap_info_header.biHeight);
    char *data = new char[data_size];
    input_file.read(data, data_size);
    auto *image_data = new uint32_t[data_size / channels];
    int i;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            i = x + (height - 1 - y) * width;
            image_data[x + y * width] = Int(&data[i * channels]);
        }
    }
    delete[] data;

    // Assemble BMP File
    BMP_FILE bmp_file{};
    bmp_file.file_header = bitmap_file_header;
    bmp_file.info_header = bitmap_info_header;
//    ExplainBMP(bmp_file);

    return new Texture(image_data, Int(bmp_file.info_header.biWidth), Int(bmp_file.info_header.biHeight));
}

void Image::ExplainBMP(BMP_FILE image) {
    BITMAPFILEHEADER *file = &image.file_header;
    BITMAPINFOHEADER *info = &image.info_header;
    std::cout << "bfType:          " << std::string(file->bfType, 2) << "; must always be set to 'BM' to declare that this is a .bmp-file." << std::endl;
    std::cout << "bfSize:          " << Int(file->bfSize) << " bytes; specifies the size of the file in bytes." << std::endl;
    std::cout << "bfReserved1:     " << Short(file->bfReserved1) << "; Reserved, actual value depends on the application that creates the image." << std::endl;
    std::cout << "bfReserved2:     " << Short(file->bfReserved2) << "; Reserved, actual value depends on the application that creates the image." << std::endl;
    std::cout << "bfOffBits:       " << Int(file->bfOffBits) << "; The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found." << std::endl;
    std::cout << "biSize:          " << Int(info->biSize) << "; the size of this header (determines which format is used)." << std::endl;
    std::cout << "biWidth:         " << Int(info->biWidth) << " px; " << std::endl;
    std::cout << "biHeight:        " << Int(info->biHeight) << " px; " << std::endl;
    std::cout << "biPlanes:        " << Short(info->biPlanes) << "; " << std::endl;
    std::cout << "biBitCount:      " << Short(info->biBitCount) << "; " << std::endl;
    std::cout << "biCompression:   " << Int(info->biCompression) << "; " << std::endl;
    std::cout << "biSizeImage:     " << Int(info->biSizeImage) << "; " << std::endl;
    std::cout << "biXPelsPerMeter: " << Int(info->biXPelsPerMeter) << "; " << std::endl;
    std::cout << "biYPelsPerMeter: " << Int(info->biYPelsPerMeter) << "; " << std::endl;
    std::cout << "biClrUsed:       " << Int(info->biClrUsed) << "; " << std::endl;
    std::cout << "biClrImportant:  " << Int(info->biClrImportant) << "; " << std::endl;
}

uint16_t Image::Short(char *data) {
    return ((uint8_t)data[1] << 8) | (uint8_t)data[0];
}

uint32_t Image::Int(char *data) {
    return ((uint8_t)data[3] << 24) | ((uint8_t)data[2] << 16) | ((uint8_t)data[1] << 8) | (uint8_t)data[0];
}

uint32_t * Image::ParseASCII(std::basic_istream<char, std::char_traits<char>> &file, int width, int height, int max_value) {
    auto image = new uint32_t[width * height];
    std::string line;
    unsigned int pixel, r, g, b;
    unsigned int line_i = 0;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        iss >> pixel;
        switch (line_i % 3) {
            case 0:
                r = pixel;
                break;
            case 1:
                g = pixel;
                break;
            case 2:
                b = pixel;

                image[line_i / 3] = b | g << 8 | r << 16 | 255 << 24;
                break;
        }
        line_i++;
    }
    return image;
}

uint32_t *
Image::ParseBinary(std::basic_istream<char, std::char_traits<char>> &file, int width, int height, int max_value,
                   int channels) {
    assert(channels == 3 || channels == 4);
    auto image = new uint32_t[width * height];
    unsigned int r, g, b, a = 255;
    int data_size = width * height * channels;
    auto *data = new unsigned char[data_size];
    file.read((char*)data, data_size);
    for (int i = 0; i < data_size; i += channels) {
        r = data[i];
        g = data[i + 1];
        b = data[i + 2];
        if (channels == 4)
            a = data[i + 3];
        image[i / channels] = b | g << 8 | r << 16 | a << 24;
    }
    delete[] data;

    return image;
}

uint32_t *
Image::ParsePPM(std::basic_istream<char, std::char_traits<char>> &file, const std::string &magic_number, int *width,
                int *height) {
    int max_value;

    std::string line;
    unsigned int line_i = 1;
    while (std::getline(file, line))
    {
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        switch (line_i++) {
            // width and height
            case 1:
                iss >> *width >> *height;
                break;
                // Maximum value
            case 2:
                iss >> max_value;

                // Data
                if (magic_number == "P3")
                    return ParseASCII(file, *width, *height, max_value);
                return ParseBinary(file, *width, *height, max_value);
        }
    }

    throw std::runtime_error("Did not find image data in PPM file.");
}

uint32_t *Image::ParsePAM(std::basic_istream<char, std::char_traits<char>> &file, int *width, int *height) {
    int max_value, depth;
    std::string label, tuple_type;

    std::string line;
    while (std::getline(file, line))
    {
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> label;
        if (label == "WIDTH")
            iss >> *width;
        else if (label == "HEIGHT")
            iss >> *height;
        else if (label == "DEPTH")
            iss >> depth;
        else if (label == "MAXVAL")
            iss >> max_value;
        else if (label == "TUPLTYPE")
            iss >> tuple_type;
        else if (label == "ENDHDR")
            return ParseBinary(file, *width, *height, max_value, depth);
    }

    throw std::runtime_error("Did not find image data in PAM file.");
}

Texture *Image::LoadPNM(const std::string &file_path) {
    try {
        std::ifstream infile(file_path);
        std::string magic_number;
        int width, height;

        if (infile.fail())
            throw std::runtime_error("File not found: " + file_path);

        std::string line;
        if (std::getline(infile, line))
        {
            // File format, aka magic number
            if (line == "P3" || line == "P6")
                return new Texture(ParsePPM(infile, line, &width, &height), width, height);
            if (line == "P7")
                return new Texture(ParsePAM(infile, &width, &height), width, height);
            else
                throw std::runtime_error("Unknown magic number: " + line);
        }
        throw std::runtime_error("Unknown Error");
    }
    catch( const std::exception & ex ) {
        std::cerr << ex.what() << std::endl;
        throw ex;
    }
}
