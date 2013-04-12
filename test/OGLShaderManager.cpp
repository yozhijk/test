#include "OGLShaderManager.h"
#include <vector>
#include <string>
#include <stdexcept>

static GLuint CompileShader(std::vector<GLchar> const& shaderSource, GLenum type)
{
    GLuint shader = glCreateShader(type);
    
    GLint len                 = static_cast<GLint>(shaderSource.size());
    GLchar const* sourceArray = &shaderSource[0];
    
    glShaderSource(shader, 1, &sourceArray, &len);
    glCompileShader(shader);
    
    GLint result = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	
    if(result == GL_FALSE)
    {
        std::vector<char> log;
        
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		
        log.resize(len);
        
		glGetShaderInfoLog(shader, len, &result, &log[0]);
		
		glDeleteShader(shader);
        
        throw std::runtime_error(std::string(log.begin(), log.end()));
		
        return 0;
	}
    
    return shader;
}


OGLShaderManager::OGLShaderManager()
{
}


OGLShaderManager::~OGLShaderManager()
{
    
}

GLuint OGLShaderManager::CompileProgram(std::string const& programName)
{
    std::string vertexShaderName = programName + ".vsh";
    std::string pixelShaderName = programName + ".psh";
    
    // Load source
    
    // Compile shaders
    
    // Attach to program
    
    // Check integrity
    
    // Return 
    
    return 0;
}

GLuint OGLShaderManager::GetShaderProgram(std::string const& programName)
{
    auto iter = shaderCache_.find(programName);
    
    if (iter != shaderCache_.end())
    {
        return iter->second;
    }
    else
    {
        GLuint program = CompileProgram(programName);
        shaderCache_[programName] = program;
    }
    
    return 0;
}