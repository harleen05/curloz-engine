#include "texture/imageloader.h"
#include <iostream>

GLuint Texture::loadImageFromFile(std::string imagePath, bool flip_image)
{
        int imageWidth = 0, imageHeight = 0, nrChannels = 0; 

        GLuint texture;
        stbi_set_flip_vertically_on_load(flip_image);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *textureData = stbi_load(imagePath.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
        if(!textureData)
        {
                std::cerr << "Could not load texture: " << imagePath << std::endl;
                return -1;
        }
        switch(nrChannels)
        {
                case(1):
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imageWidth, imageHeight, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
                        glGenerateMipmap(GL_TEXTURE_2D);
                        break;
                case(3):
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
                        glGenerateMipmap(GL_TEXTURE_2D);
                        break;
                case(4):
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA ,imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
                        glGenerateMipmap(GL_TEXTURE_2D);
                        break;

                default:
                        std::cerr << "Invalid texture" <<std::endl;
                        return -1;
                        break;
        }


        stbi_image_free(textureData);
        return texture;
}
