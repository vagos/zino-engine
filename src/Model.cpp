#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Model.hpp"

#include <stdexcept>

namespace zge
{

static std::vector<unsigned int> VEC_UINT_DEFAUTL_VALUE{}; // What the honk is this???

Model::Model(std::string m_path)
{
    doLoad(m_path);
}

Model::Model()
{
}

Model::~Model()
{
    std::clog << "Got destroyed!\n";
    glDeleteBuffers(1, &model_vertices_vbo);
    glDeleteBuffers(1, &model_uvs_vbo);
    glDeleteVertexArrays(1, &model_vao);
}

void Model::doLoad(std::string m_path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;

    std::vector<unsigned int>& indices = VEC_UINT_DEFAUTL_VALUE; // TODO what is this???

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, nullptr, &error, m_path.c_str()))
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
    
    createContext();
    
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

    if (uvs.size())
    {
        glGenBuffers(1, &model_uvs_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, model_uvs_vbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2),
            &uvs[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
    }

    std::clog << vertices.size() << ' ' << uvs.size() << '\n';
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
}

}

