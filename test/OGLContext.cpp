#include "OGLContext.h"
#include "Model.h"
#include "CompiledModel.h"
#include <GLUT/glut.h>

OGLContext::OGLContext()
{
    
}

OGLContext::~OGLContext()
{
    
}

void OGLContext::Init()
{
    GLint width = (GLint)glutGet(GLUT_WINDOW_WIDTH);
    GLint height = (GLint)glutGet(GLUT_WINDOW_HEIGHT);

    SetViewport(core::ui_rect(0, 0, static_cast<core::uint>(width), static_cast<core::uint>(height)));
}

void OGLContext::ResizeBuffer(core::ui_size const& size)
{
    
}

void OGLContext::SetViewport(core::ui_rect const& vp)
{
    glViewport(static_cast<GLint>(vp.x), static_cast<GLint>(vp.y), static_cast<GLsizei>(vp.w), static_cast<GLsizei>(vp.h));
}

void OGLContext::SetWorldMatrix(core::matrix4x4 const& worldMatrix)
{
    
}

void OGLContext::SetViewMatrix(core::matrix4x4 const& viewMatrix)
{
    
}

void OGLContext::SetProjectionMatrix(core::matrix4x4 const& projMatrix)
{
    
}

void OGLContext::DrawModel( /*CompiledModel const& model*/ )
{
    
}

void OGLContext::Clear(core::color_rgba const& color)
{
    glClearColor(color.x(), color.y(), color.z(), color.w());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLContext::Present()
{
    glutSwapBuffers();
}

IResourceManager& OGLContext::GetResourceManager()
{
    return *this;
}

std::unique_ptr<CompiledModel> OGLContext::CompileModel(Model const& model)
{
    GLuint vertexBufferId, indexBufferId;
    
    glGenBuffers(1, &vertexBufferId);
    glGenBuffers(1, &indexBufferId);
    
    // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    
    // fill data
    glBufferData(GL_ARRAY_BUFFER, model.GetVertexCount() * model.GetVertexSizeInBytes(), model.GetVertexArrayPointer(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.GetIndexCount() * sizeof(unsigned short), model.GetIndexArrayPointer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return std::unique_ptr<CompiledModel>(
        new CompiledModel(vertexBufferId, indexBufferId, std::bind(&OGLContext::OnReleaseModel, this, std::placeholders::_1))
                                        );
}

void OGLContext::OnReleaseModel( CompiledModel const& model )
{
    GLuint vertexBufferId = model.GetVertexBufferID();
    GLuint indexBufferId  = model.GetIndexBufferID();
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteBuffers(1, &indexBufferId);
}
