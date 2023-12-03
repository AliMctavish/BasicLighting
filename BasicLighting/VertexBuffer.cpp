#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(VertexType type)
{
	m_Type = type;
	m_VertexBuffer = 0;
}

VertexBuffer::~VertexBuffer()
{
	UnBind();
}

void VertexBuffer::Bind()
{
	if (m_Type == VertexType::Cube_Type) {
		InitializeVertexBuffer(m_Type);
	}
	else if (m_Type == VertexType::Pyramid_Type) {
		InitializeVertexBuffer(m_Type);
	}
	else if (m_Type == VertexType::Surface_Type) {
		InitializeVertexBuffer(m_Type);
	}
}

void VertexBuffer::InitializeVertexBuffer(const VertexType& type)
{
	float* vertices = nullptr;
	RawVertexData rawData;
	int size;

	switch (m_Type)
	{
	case VertexType::Cube_Type:
		vertices = rawData.CubeWithNormalsData();
		size = Cube_With_Normals_And_Texture_Data_Buffer;
		break;
	case VertexType::Surface_Type:
		vertices = rawData.FlatSurface();
		size = Flat_Surface_Data_Buffer_With_Texture;
		break;
	case VertexType::Pyramid_Type:
		vertices = rawData.PyramidData();
		size = Pyramid_Data_Buffer;
		break;
	default:
		break;
	}
	float* vert = new float[size];

	for (int i = 0; i < size; i++)
		vert[i] = vertices[i];

	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), &vert, GL_STATIC_DRAW);

	delete[] vertices;
	delete[] vert;
}

void VertexBuffer::SetCubeAttributes()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void VertexBuffer::SetCubeWithNormalsAttributes()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}	

void VertexBuffer::SetCubeWithNormalsAndTexturesAttributes()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void VertexBuffer::UnBind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
