#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <unordered_map>

#include "Common.hpp"
#include "Asset.hpp"

namespace zge
{

class Shader : public Asset
{
    public:
        Shader(std::string v_s_path, std::string f_s_path);

        void doUse() { glUseProgram(program_id); }

        void sendUniform(const std::string u_n, const Matrix4x4& matrix);
        void sendUniform(const std::string u_n, const Vector3& vec);
        void sendUniform(const std::string u_n, float f);
        void sendUniform(const std::string u_n, int n);

    private:
        std::unordered_map<std::string, GLuint> uniform_locations;
        GLuint program_id;

};

}

#endif /* SHADER_HPP */
