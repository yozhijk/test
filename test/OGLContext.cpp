#include "OGLContext.h"
#include "Mesh.h"
#include "CompiledMesh.h"
#include "utils.h"
#include <GLUT/glut.h>

using namespace core;


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

    SetViewport(ui_rect(0, 0, static_cast<uint>(width), static_cast<uint>(height)));
}

void OGLContext::ResizeBuffer(ui_size const& size)
{
    
}

void OGLContext::SetViewport(ui_rect const& vp)
{
    glViewport(static_cast<GLint>(vp.x), static_cast<GLint>(vp.y), static_cast<GLsizei>(vp.w), static_cast<GLsizei>(vp.h));
}

void OGLContext::SetWorldMatrix(matrix4x4 const& worldMatrix)
{
    worldMatrix_ = worldMatrix;
}

void OGLContext::SetViewMatrix(matrix4x4 const& viewMatrix)
{
    viewMatrix_ = viewMatrix;
}

void OGLContext::SetFrustum(frustum const& frustum)
{
    projMatrix_ = perspective_proj_fovy_matrix_rh_gl(frustum.fovy, frustum.aspect, frustum.nr, frustum.fr);
}

void OGLContext::DrawMesh(CompiledMesh const& mesh)
{
    matrix4x4 worldMatrix = worldMatrix_;
    matrix4x4 worldViewProj =  worldMatrix_ * /*viewMatrix_*/ projMatrix_;
    
    GLuint program = shaderManager_.GetShaderProgram("simple_ogl");
    
    glDisable(GL_DEPTH_TEST);
    
    glUseProgram(program);
    
    glUniformMatrix4fv(glGetUniformLocation(program, "g_mWorld"), 1, false, &(worldMatrix(0,0)));
    glUniformMatrix4fv(glGetUniformLocation(program, "g_mWorldViewProj"), 1, false, &(worldViewProj(0,0)));
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVertexBufferID());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndexBufferID());
    
    GLuint positionAttribId = glGetAttribLocation(program, "inPosition");
    GLuint normalAttribId   = glGetAttribLocation(program, "inNormal");
    GLuint texcoordAttribId = glGetAttribLocation(program, "inTexcoord");
    
    glVertexAttribPointer(positionAttribId, 3, GL_FLOAT, GL_FALSE, mesh.GetVertexSizeInBytes(), OFFSETOF(Mesh::Vertex,position));
    glVertexAttribPointer(normalAttribId, 3, GL_FLOAT, GL_FALSE, mesh.GetVertexSizeInBytes(), OFFSETOF(Mesh::Vertex,normal));
    glVertexAttribPointer(texcoordAttribId, 2, GL_FLOAT, GL_FALSE, mesh.GetVertexSizeInBytes(), OFFSETOF(Mesh::Vertex,texcoord));
    
    /// Move away this state changes later
    glEnableVertexAttribArray(positionAttribId);
    glEnableVertexAttribArray(normalAttribId);
    glEnableVertexAttribArray(texcoordAttribId);
    
    glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_SHORT, nullptr);
}

void OGLContext::Clear(color_rgba const& color)
{
    glClearColor(color.x(), color.y(), color.z(), color.w());
    glClearDepth(1.0);
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

std::unique_ptr<CompiledMesh> OGLContext::CompileMesh(const Mesh &mesh)
{
    GLuint vertexBufferId, indexBufferId;
    
    glGenBuffers(1, &vertexBufferId);
    glGenBuffers(1, &indexBufferId);
    
    // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
    
    // fill data
    glBufferData(GL_ARRAY_BUFFER, mesh.GetVertexCount() * mesh.GetVertexSizeInBytes(), mesh.GetVertexArrayPointer(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndexCount() * sizeof(unsigned short), mesh.GetIndexArrayPointer(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return std::unique_ptr<CompiledMesh>(
        new CompiledMesh(vertexBufferId, indexBufferId,  mesh.GetIndexCount(), mesh.GetVertexSizeInBytes(), std::bind(&OGLContext::OnReleaseMesh, this, std::placeholders::_1))
                                        );
}

void OGLContext::OnReleaseMesh(const CompiledMesh &mesh)
{
    GLuint vertexBufferId = mesh.GetVertexBufferID();
    GLuint indexBufferId  = mesh.GetIndexBufferID();
    glDeleteBuffers(1, &vertexBufferId);
    glDeleteBuffers(1, &indexBufferId);
}
