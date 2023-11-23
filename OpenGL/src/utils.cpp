#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS

unsigned int load_BMP(const char* path)
{
    /**
    * Loads a BMP image file and generates an OpenGL texture.
    *
    * @param path Path to the BMP file to be loaded.
    * @return Returns 0 on successful texture generation, -1 on failure.
    */

    // BMP header
    unsigned char header[54];

    // Position of data in the file
    unsigned int dataPos;

    // Width and height of the image
    unsigned int width, height;
    unsigned int imageSize;

    // Data container
    unsigned char* data;

    FILE* file;
    errno_t err = fopen_s(&file, path, "rb");
    if (err != 0 || file == nullptr) {
        std::cout << "File cannot be opened.\n";
        return -1;
    }

    // Read the header
    if (fread(header, 1, 54, file) != 54)
    {
        printf("Wrong file format\n");
        return -1;
    }

    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Wrong file format\n");
        return -1;
    }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if (imageSize == 0)
        imageSize = width * height * 3;

    if (dataPos == 0)
        dataPos = 54;

    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return 0;
}