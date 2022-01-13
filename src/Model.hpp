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

static std::vector<unsigned int> VEC_UINT_DEFAULT_VALUE{}; // Empty vectors as default values.
static std::vector<Vector3>      VEC_VEC3_DEFAULT_VALUE{};
static std::vector<Vector2>      VEC_VEC2_DEFAULT_VALUE{};

struct Model : public Asset
{
       Model();
       Model(std::string m_path);
       Model(const std::vector<Vector3> &vertices, 
             const std::vector<Vector2> &uvs = VEC_VEC2_DEFAULT_VALUE, 
             const std::vector<Vector3> &normals = VEC_VEC3_DEFAULT_VALUE);
       ~Model();

       void doLoad(std::string m_path); // Change the internal model later.
       void setVertices(std::vector<Vector3> verts); // Simply set the vertices (for simple meshes)
       void doUse();
       virtual void doRender(Engine& eng);
    
       std::vector<Vector3> vertices, normals;
       std::vector<Vector2> uvs;

       std::shared_ptr<Texture> texture = nullptr;


    public:

       std::string debug_name; // TODO change this

       void createContext();

       GLuint model_vao, 
              model_vertices_vbo,  
              model_uvs_vbo, 
              model_normals_vbo, 
              model_elements_vbo;
};

struct TransformingModel
{
   Model model_first, model_second;

   TransformingModel(std::string m_f_path, std::string m_o_path);

   void doRender(Engine& eng);
   void createContext();

    GLuint model_vao;

    float morph_factor = 0.0f;
};

} // namespace zge

#endif /* MODEL_HPP */

