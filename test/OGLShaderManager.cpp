#include "OGLShaderManager.h"
#include "utils.h"
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
    // testing code
    // core::uint program = GetShaderProgram("simple_ogl");
}


OGLShaderManager::~OGLShaderManager()
{
    for (auto cIter = shaderCache_.cbegin(); cIter != shaderCache_.cend(); ++cIter)
    {
        glDeleteProgram(cIter->second);
    }
}

GLuint OGLShaderManager::CompileProgram(std::string const& programName)
{
    std::string vertexShaderName = programName + ".vsh";
    std::string fragmentShaderName = programName + ".psh";
    
    // Need to wrap the shader program here to be exception-safe
    std::vector<GLchar> sourceCode;
    
    core::load_file_contents(vertexShaderName, sourceCode);
    GLuint vertexShader = CompileShader(sourceCode, GL_VERTEX_SHADER);
    
    /// This part is not exception safe:
    /// if the VS compilation succeeded
    /// and PS compilation fails then VS object will leak
    /// fix this by wrapping the shaders into a class
    core::load_file_contents(fragmentShaderName, sourceCode);
    GLuint fragmentShader = CompileShader(sourceCode, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    glLinkProgram(program);
    
    GLint result = GL_TRUE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    
	if(result == GL_FALSE)
    {
		GLint length = 0;
        std::vector<char> log;
        
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		
        log.resize(length);
		
        glGetProgramInfoLog(program, length, &result, &log[0]);
        
        glDeleteProgram(program);
        
        throw std::runtime_error(std::string(log.begin(), log.end()));
	}
    
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