#include "IRenderable.h"

#include <assert.h>
#include <iostream>

#include "OpenGL/ErrorHandle.h"


IRenderable::IRenderable(const Buffer &vbo):
    _handler(0),
    _modelMatrix(glm::mat4(1.f))
{
    Setup(vbo);
}

IRenderable::~IRenderable()
{
    Release();
    GLCall(glDeleteVertexArrays(1, &_handler))
}

void IRenderable::Render()
{
    if ( !_bVisible || !_vbo.data.Ptr )
        return;
    
    Bind();
    GLCall(glDrawArrays(_vbo.drawType, 0, _vbo.data.Count))
}

bool IRenderable::Setup(const Buffer& vbo)
{
    if (!vbo.data.Ptr)
        return false;
    
    _vbo = vbo;
    
    GLCall(glGenVertexArrays(1, &_handler))
    GLCall(glBindVertexArray(_handler))

    unsigned vboId = _vbo.Create();
    GLCall(glBindVertexArray(0))
    
    if (vboId == 0)
        return false;
    
    GLCall(glDeleteBuffers(1, &vboId))
    return true;
}

void IRenderable::SetShader(Shader* shader)
{
    if (_shaderPtr)
        _shaderPtr->DetachFrom(this);
    
    _shaderPtr = shader;
    
    shader->AttachTo(this);
}

void IRenderable::Bind()
{
    GLCall(glBindVertexArray(_handler))
}

void IRenderable::Release()
{
    GLCall(glBindVertexArray(0))
}
