#ifndef SHADER_H__
#define SHADER_H__

#include <string>
#include "define.h"
#include "vector3.h"

class Shader
{
    public:    
        bool Load(const std::string& vertFile, const std::string& fragFile, bool verbose = false);
        void Use() const;

        GLuint GetProgramID() const;
        GLint GetUniformLocation(const std::string& name) const;
        GLint GetAttribLocation(const std::string& name) const;

        void UpdateIntUniform(GLint name, GLint value);
        void UpdateFloatUniform(GLint name, GLfloat value);
        void UpdateVec2Uniform(GLint name, GLfloat x, GLfloat y);
		void UpdateVec3Uniform(GLint name, GLfloat x, GLfloat y, GLfloat z);
        void UpdateVec3Uniform(GLint name, const Vector3f& vector);
        void UpdateVec4Uniform(GLint name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

        static void Disable();

    private:
        GLenum m_program;
        GLenum m_vertexShader;
        GLenum m_fragmentShader;

    private:
        bool CheckShaderError(GLenum shader, bool verbose);
        bool CheckProgramError(GLenum program, bool showWarning, bool verbose);
};


#endif // SHADER_H__
