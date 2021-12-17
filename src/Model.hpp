#ifndef MODEL_HPP
#define MODEL_HPP

#include <string>
#include <vector>
#include <memory>

#include "Texture.hpp"
#include "Common.hpp"

namespace zge 
{

class Engine;

struct Model : public Asset
{
       Model(std::string m_path);
       Model();
       ~Model();

       void doLoad(std::string m_path); // Change the internal model later.
       void setVertices(std::vector<Vector3> verts); // Simply set the vertices (for simple meshes)
       void doUse();
       virtual void doRender(Engine& eng);
    
       std::vector<Vector3> vertices, normals;
       std::vector<Vector2> uvs;

       std::shared_ptr<Texture> texture = nullptr;


    private:

       std::string debug_name; // TODO change this

       void createContext();

       GLuint model_vao, 
              model_vertices_vbo,  
              model_uvs_vbo, 
              model_normals_vbo, 
              model_elements_vbo;
};

}

#endif /* MODEL_HPP */

