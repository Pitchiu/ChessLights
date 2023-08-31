#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

// Element ma typ, liczbe i info czy znormalizowany
struct VertexBufferElement
{
	VertexBufferElement(unsigned int type, unsigned int count, bool normalized)
		: type(type), count(count), normalized(normalized) {};
	unsigned int type;
	unsigned int count;
	bool normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;

		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	// How many bytes
	unsigned int m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {};

	template<typename T>
	void Push(int count);

	template<>
	void Push<float>(int count)
	{
		m_Elements.push_back(VertexBufferElement(GL_FLOAT, count, false ));
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<char>(int count)
	{
		m_Elements.push_back(VertexBufferElement(GL_UNSIGNED_BYTE, count, true));
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline unsigned int GetStride() const { return m_Stride; }
	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
};