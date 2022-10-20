#include "Texture2d.h"
#include "OpenGL/ErrorHandle.h"

Texture2d::Texture2d(const glm::uvec2& size, unsigned char* data):
	_handler(0),
	_size(size),
	_pixelFormat(GL_RGBA8),
	_channelCount(4)
{
	_totalSize = _size.x*_size.y*_channelCount;
	
	GLCall(glGenTextures(1, &_handler))
	Bind();
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, _pixelFormat, _size.x, _size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data))
	Release();
}

Texture2d::~Texture2d()
{
	GLCall(glDeleteTextures(1, &_handler))
}

void Texture2d::Bind(unsigned slotId) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slotId))
	GLCall(glBindTexture(GL_TEXTURE_2D, _handler))
}

void Texture2d::Release()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0))
}

bool Texture2d::UpdateTexture(const std::vector<unsigned char>& rawData) const
{
	if (rawData.size() != _totalSize)
		return false;
	
	Bind();
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, _pixelFormat, _size.x, _size.y, 0, _pixelFormat, GL_UNSIGNED_BYTE, rawData.data()))
	return true;
}

bool Texture2d::UpdateTexture(unsigned char* rawData, size_t count) const
{
	if (count != _totalSize)
		return false;
	
	Bind();
	GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _size.x, _size.y, GL_RGBA, GL_UNSIGNED_BYTE, rawData))
	return true;
}
