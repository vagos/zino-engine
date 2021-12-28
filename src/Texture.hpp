#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <vector>

#include "Common.hpp"
#include "Asset.hpp"

namespace zge 
{

struct Image 
{
    unsigned char* data = nullptr;
    unsigned int width, height;

    Image(const std::string i_path)
    {
        doLoad(i_path);
    }

    void doLoad(const std::string i_path);

    ~Image()
    {
        delete[] data;
    }
};

class Texture : public Asset
{
    public:
        Texture(const std::string t_path); 
        Texture();
        
        void doLoad(const std::string t_path); // TODO: only loads BMP files for now.
        virtual void doUse();
        virtual void doUse(int t_position);

        // void createContext();

        int getTextureUnit() {return texture_unit;}
        GLuint texture_id;
        int position = 0;

    protected:
        static int texture_unit;
    
};

class Cubemap : public Texture
{
    public:
        Cubemap(const std::vector<std::string>& t_faces_paths);

        void doUse() override;
};


};

#endif /* TEXTURE_HPP */
