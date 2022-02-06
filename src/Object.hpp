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
        enum class Type 
        {
            WORLD,
            BALL,
            CATCH_BALL,
            MONSTER
        };

    public:
        
        Object();

        virtual void doUpdate(Engine& eng) {}
        virtual void doRender(Engine& eng) {}

        std::shared_ptr<Model> model           = nullptr;
        std::shared_ptr<RigidBody> rigid_body  = nullptr;
        std::shared_ptr<CubeCollider> collider = nullptr;
        
        Matrix4x4 getModelMatrix();
        void setModelMatrix(Matrix4x4 m); 

        void resetModelMatrix() { model_matrix = Matrix4x4(1); }

        void applyTransofrmation(Matrix4x4&& m);

        std::string name;
        
        static std::vector<std::shared_ptr<Object>> objects;
        static void removeNullObjects();

        Type type = Type::WORLD;

        bool exists;
        
    private: 
        Matrix4x4 model_matrix;
        Matrix4x4 base_model_matrix;

    };


};

#endif /* OBJECT_HPP */
