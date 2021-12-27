#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <memory>
#include <vector>

#include "Common.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"

namespace zge
{
    class Engine;

    class Object
    {

    public:
        
        Object();

        virtual void doUpdate(Engine& eng) {}
        virtual void doRender(Engine& eng) {}

        std::shared_ptr<Model> model           = nullptr;
        std::shared_ptr<RigidBody> rigid_body  = nullptr;
        std::shared_ptr<CubeCollider> collider = nullptr;
        
        void setModelMatrix(Matrix4x4 mat) {model_matrix = mat;}
        Matrix4x4& getModelMatrix() {return model_matrix;}

        std::string name;
        
        static std::vector<std::shared_ptr<Object>> objects;
        static void removeNullObjects();
        
    protected: 
        Matrix4x4 model_matrix;
        bool exists;
    };


};

#endif /* OBJECT_HPP */
