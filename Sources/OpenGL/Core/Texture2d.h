#pragma once

#include <vector>
#include <GL/glew.h>
#include "glm/glm.hpp"


class Texture2d
{
public:
	Texture2d(const glm::uvec2& size, unsigned char* data = nullptr);
	~Texture2d();

	void Bind(unsigned slotId = 0) const;
	static void Release();

	bool UpdateTexture(const std::vector<unsigned char>& rawData) const;
	bool UpdateTexture(unsigned char* rawData, size_t count) const;

	uint8_t GetChannelCount() const { return _channelCount; }
	int GetPixelFormat() const { return _pixelFormat; }
	glm::uvec2 GetSize() const { return _size; }
	size_t GetBufferSize() const { return _totalSize; }

private:
	unsigned _handler;
	glm::uvec2 _size;
	int _pixelFormat;
	uint8_t _channelCount;
	
	size_t _totalSize;
};
