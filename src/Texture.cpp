#include "Texture.hpp"

#define TEXTURE(n) GL_TEXTURE_##n

namespace zge
{

    Texture::Texture()
    {
        glGenTextures(1, &texture_id);
    }

    Texture::Texture(const std::string t_path)
    {
        doLoad(t_path);
    }

    void Texture::doLoad(const std::string t_path) 
    {
        Image t_image(t_path);

        // Create one OpenGL texture
        glGenTextures(1, &texture_id);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_image.width, t_image.height, 0, GL_BGR, GL_UNSIGNED_BYTE, t_image.data);

        // OpenGL has now copied the data. Free our own version

        // Poor filtering, or ...
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // ... nice trilinear filtering ...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        // ... which requires mipmaps. Generate them automatically.
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::doUse()
    {
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture_id);

        position = 0;
    }

    void Texture::doUse(int t_position)
    {
        glActiveTexture(GL_TEXTURE0 + t_position);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        position = t_position;
    }

    int Texture::texture_unit = 0;

    void Image::doLoad(const std::string i_path)
    {
        std::clog << "Reading image: " << i_path << '\n';

        // Data read from the header of the BMP file
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;

        // Open the file
        FILE * file = fopen(i_path.c_str(), "rb");
        if (!file) 
        {
            throw std::runtime_error(std::string("Image could not be opened: ") + i_path);
        }

        // Read the header, i.e. the 54 first bytes

        // If less than 54 bytes are read, problem
        if (fread(header, 1, 54, file) != 54) {
            fclose(file);
            throw std::runtime_error("Not a correct BMP file");
        }
        // A BMP files always begins with "BM"
        if (header[0] != 'B' || header[1] != 'M') {
            fclose(file);
            throw std::runtime_error("Not a correct BMP file");
        }
        // Make sure this is a 24bpp file
        if (*(int*)&(header[0x1E]) != 0) {
            fclose(file);
            throw std::runtime_error("Not a correct BMP file");
        }
        if (*(int*)&(header[0x1C]) != 24) {
            fclose(file);
            throw std::runtime_error("Not a correct BMP file");
        }

        // Read the information about the image
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);

        // Some BMP files are misformatted, guess missing information
        if (imageSize == 0) {
            // 3 : one byte for each Red, Green and Blue component
            imageSize = width*height * 3;
        }

        if (dataPos == 0) {
            dataPos = 54; // The BMP header is done that way
        }

        // Create a buffer
        data = new unsigned char[imageSize];

        // Read the actual data from the file into the buffer
        fread(data, 1, imageSize, file);

        // Everything is in memory now, the file can be closed.
        fclose(file);
    }

    Cubemap::Cubemap(const std::vector<std::string>& t_faces_paths)
    {
       glGenTextures(1, &texture_id);
       glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

       for (unsigned int i = 0; i < t_faces_paths.size(); i++) 
       {
            Image c_image(t_faces_paths[i]);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
                    GL_RGB, c_image.width, c_image.height, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, c_image.data);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
       }

    }

    void Cubemap::doUse()
    {
       glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id); 
    }

};
