#pragma once


#include "BufferLayout.h"


struct DataPtr
{
    DataPtr();
    DataPtr(void* ptr, unsigned count, unsigned itemSize);
    
    template<class T>
    DataPtr(const std::vector<T>& items):
        DataPtr((void*)&items[0], items.size(), sizeof(T))
    {}
    template<class T>
    DataPtr(const std::initializer_list<T>& items):
        DataPtr((void*)&items[0], items.size(), sizeof(T))
    {}

    void* Ptr;
    unsigned Count;
    unsigned ItemSize;
};

struct Buffer
{
    virtual ~Buffer();

    Buffer& Data(const DataPtr& val) { data = val; return *this; }
    Buffer& Layout(const BufferLayout& val) { layout = val; return *this; }
    Buffer& DrawType(const unsigned& val) { drawType = val; return *this; }
    Buffer& Type(const unsigned& val) { type = val; return *this; }
    Buffer& Mode(const unsigned& val) { mode = val; return *this; }
    
    
    DataPtr data = {};
    BufferLayout layout = {};
    unsigned drawType = GL_TRIANGLES;
    unsigned type = GL_ARRAY_BUFFER;
    unsigned mode = GL_STATIC_DRAW;

    virtual unsigned Create();
};