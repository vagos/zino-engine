#include "Common.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Model.hpp"
#include "Engine.hpp"

#include <stdexcept>


namespace zge
{

Model::Model(std::string m_path)
{
    doLoad(m_path);
    createContext();
    debug_name = m_path;
}

Model::Model()
{
}

Model::~Model()
{
    std::clog << "Got destroyed! " << "Name: " << debug_name << '\n';
    glDeleteBuffers(1, &model_vertices_vbo);
    glDeleteBuffers(1, &model_uvs_vbo);
    glDeleteVertexArrays(1, &model_vao);
}

void Model::doLoad(std::string m_path)
{
    std::clog << "Loading object: " << m_path;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;

    std::vector<unsigned int>& indices = VEC_UINT_DEFAULT_VALUE; // TODO what is this???

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, m_path.c_str()))
    {
        throw std::runtime_error(error);
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vector3 vertex( 
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            );

            if (attrib.texcoords.size() != 0) 
            {
                Vector2 uv
                (
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1 - attrib.texcoords[2 * index.texcoord_index + 1]
                );

                uvs.push_back(uv);
            }
            if (attrib.normals.size() != 0) 
            {
                Vector3 normal(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                    );

                normals.push_back(normal);
            }

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }
    
    
    // glGenBuffers(1, &elementVBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementVBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
    //              &indices[0], GL_STATIC_DRAW);
}

void Model::createContext()
{
    glGenVertexArrays(1, &model_vao);
    glBindVertexArray(model_vao);

    if (vertices.size())
    {
        glGenBuffers(1, &model_vertices_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, model_vertices_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3),
                     &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
    }

    if (normals.size())
    {
        glGenBuffers(1, &model_normals_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, model_normals_vbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3),
                     &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
    }

    if (uvs.size())
    {
        glGenBuffers(1, &model_uvs_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, model_uvs_vbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2),
            &uvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);
    }

    std::clog << vertices.size() << ' ' << uvs.size() << ' ' << normals.size() << '\n';
}


void Model::setVertices(std::vector<Vector3> verts)
{
    if (vertices.size()) throw std::runtime_error("Don't set vertices on already loaded objects!");

    vertices = verts;
    createContext();
}

void Model::doUse()
{
    glBindVertexArray(model_vao);

    if (texture) texture->doUse();
}

void Model::doRender(Engine &eng)
{
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}


Model::Model(const std::vector<Vector3> &vertices, const std::vector<Vector2> &uvs, const std::vector<Vector3> &normals)
    :vertices(vertices), uvs(uvs), normals(normals)
{
   createContext(); 
}

TransformingModel::TransformingModel(std::string m_f_path, std::string m_s_path)
{
    model_first.doLoad(m_f_path);    
    model_second.doLoad(m_s_path);

    createContext();
}

void TransformingModel::createContext()
{
    glGenVertexArrays(1, &model_vao);
    glBindVertexArray(model_vao);

    glGenBuffers(1, &model_first.model_vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model_first.model_vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, model_first.vertices.size() * sizeof(Vector3),
            &model_first.vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &model_second.model_vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model_second.model_vertices_vbo);
    glBufferData(GL_ARRAY_BUFFER, model_second.vertices.size() * sizeof(Vector3),
            &model_second.vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
}

void TransformingModel::doRender(Engine &eng)
{
    glBindVertexArray(model_vao);

    auto texture_shader = eng_getAssetTyped("Transform Shader", Shader);
    texture_shader->doUse();

    auto mvp = eng.camera.getProjection() * eng.camera.getView()
        * glm::translate(Matrix4x4(1), Vector3(10.0f, 3.0f, 10.0f));

    texture_shader->sendUniform("mvp", mvp);
    texture_shader->sendUniform("morph_factor", morph_factor);

    if (eng.isKeyHeld(Key(X)))
    {
        morph_factor += eng.getElapsedTime();
    }

    if (eng.isKeyHeld(Key(Z)))
    {
        morph_factor -= eng.getElapsedTime();
    }

    morph_factor = glm::clamp(morph_factor, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 
            morph_factor < 0.5f ? 
            model_first.vertices.size() : model_second.vertices.size());

}

}
