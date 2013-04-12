#ifndef OGLSHADERMANAGER_H
#define OGLSHADERMANAGER_H

#include <GLUT/GLUT.h>
#include <string>
#include <map>

class OGLShaderManager
{
public:
    OGLShaderManager();
    ~OGLShaderManager();
    
    GLuint GetShaderProgram(std::string const& programName);
    
private:
    GLuint CompileProgram(std::string const& programName);
    
    OGLShaderManager(OGLShaderManager const&);
    OGLShaderManager& operator = (OGLShaderManager const&);
    
    std::map<std::string, GLuint> shaderCache_;
};


#endif