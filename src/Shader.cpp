#include "Shader.hpp"
#include "Common.hpp"

#include <fstream>
#include <vector>

static void compileShader(GLuint& shaderID, std::string file) {
    // read shader code from the file
    std::string shaderCode;
    std::ifstream shaderStream(file, std::ios::in);

    if (shaderStream.is_open()) 
    {
        std::string line = "";
        
        while (getline(shaderStream, line)) 
        {
            shaderCode += "\n" + line;
        }

        shaderStream.close();
    } 
    else 
    {
        throw std::runtime_error(std::string("Can't open shader file: ") + file);
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // compile Vertex Shader
    std::clog << "Compiling shader: " << file << '\n';
    char const* sourcePointer = shaderCode.c_str();
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    // check Vertex Shader
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    
    if (infoLogLength > 0) 
    {
        std::vector<char> shader_error_message(infoLogLength + 1);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shader_error_message[0]);
        //throw runtime_error(string(&shaderErrorMessage[0]));
        std::clog << &shader_error_message[0] << '\n';
    }
}

namespace zge 
{
    Shader::Shader(std::string v_s_path, std::string f_s_path)
    {
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        compileShader(vertexShaderID, v_s_path);

        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        compileShader(fragmentShaderID, f_s_path);

        // Link the program
        //cout << "Linking shaders... " << endl;
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);

        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        // Check the program
        GLint result = GL_FALSE;
        int infoLogLength;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        if (infoLogLength > 0) 
        {
            std::vector<char> programErrorMessage(infoLogLength + 1);
            glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
            //throw runtime_error(string(&programErrorMessage[0]));
            std::clog << &programErrorMessage[0] << '\n';
        }

        glDetachShader(programID, vertexShaderID);
        glDeleteShader(vertexShaderID);

        glDetachShader(programID, fragmentShaderID);
        glDeleteShader(fragmentShaderID);

        program_id = programID;
    }

    // void Shader::createUniform(std::string u_n)
    // {
    //     uniform_locations[u_n] = glGetUniformLocation(program_id, u_n.c_str());
    // }

    void Shader::sendUniform(std::string u_n, const Matrix4x4 &matrix)
    {
       glUniformMatrix4fv(glGetUniformLocation(program_id, u_n.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }

    void Shader::sendUniform(std::string u_n, int n)
    {
        glUniform1i(glGetUniformLocation(program_id, u_n.c_str()), n);
    }

    void Shader::sendUniform(std::string u_n, const Vector3& vec)
    {
        glUniform3f(glGetUniformLocation(program_id, u_n.c_str()), vec.x, vec.y, vec.z);
    }

}
