#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const void* data, unsigned int size);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

	inline unsigned int GetCount() const { return m_Count; }
};