#include "Buffer.h"

#include "OpenGL/ErrorHandle.h"


DataPtr::DataPtr():
    Ptr(nullptr),
    Count(0),
    ItemSize(0)
{
    
}

DataPtr::DataPtr(void* ptr, unsigned count, unsigned itemSize):
    Ptr(ptr),
    Count(count),
    ItemSize(itemSize)
{

}

Buffer::~Buffer()
{
}

unsigned Buffer::Create()
{
    unsigned handler = 0;
    if (!data.Ptr)
        return handler;
    
    GLCall(glGenBuffers(1, &handler))
    GLCall(glBindBuffer(type, handler))
    GLCall(glBufferData(type, data.Count * data.ItemSize, data.Ptr, mode))

    unsigned offset = 0;
    for (int i = 0; i < layout.Variables.size(); ++i)
    {
        GLCall(glEnableVertexAttribArray(i))
        GLCall(glVertexAttribPointer(i,
                              layout.Variables[i].Count,
                              layout.Variables[i].Type,
                              layout.Variables[i].Normalized ? GL_FALSE : GL_TRUE,
                              layout.Size,
                              (void*)offset))
        offset += layout.Variables[i].Size * layout.Variables[i].Count;
    }
    
    return handler;
}
