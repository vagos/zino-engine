#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <memory>
#include <vector>

#include "Common.hpp"
#include "Model.hpp"
#include "Shader.hpp"

namespace zge
{
    class Engine;

    class Object
    {
    public:
        
        Object();

        static std::vector<std::shared_ptr<Object>> objects;

        virtual void doUpdate(Engine& eng) {}
        virtual void doRender(Engine& eng) {}
        
        Vector3 position;
    
    private:

        bool exists;
    };


    class ModeledObject : public Object 
    {
        public:
            ModeledObject(std::shared_ptr<Model>& model);
            ModeledObject();

            std::shared_ptr<Model> model_file = nullptr;
            std::shared_ptr<Shader> shader_file = nullptr;

            void setModelMatrix(Matrix4x4 mat) {model_matrix = mat;}
            Matrix4x4& getModelMatrix() {return model_matrix;}

            void setModelFile(std::shared_ptr<Model>& model) {model_file = model;}
            void setShaderFile(const std::shared_ptr<Shader>& shader) {shader_file = shader;}

            void doRender(Engine &eng) override;

            Matrix4x4 model_matrix;
    };

    class Plane : public ModeledObject 
    {
        public:

            Plane(float w, float h);
    };
};

#endif /* OBJECT_HPP */
