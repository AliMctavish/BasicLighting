#include "Cube.h"

Cube::Cube()
{
	Init();
}

void Cube::Init()
{
	cubeId = CUBEID;
	angle = 0;
	rotateX = 0;
	rotateY = 0;
	Size = glm::vec3(2, 2, 2);
	m_Model = glm::mat4(1.0f);
	Position = glm::vec3(50, 0, 20);
	m_Program = 0;
	CUBEID++;
}

void Cube::Draw()
{
	BaseObject::Draw();
}

void Cube::SetShaderData()
{
}


void Cube::Update()
{
	BaseObject::Update();
	Rotate(rotateX);
	RotateOnY(rotateY);
	Resize(Size);
}

void Cube::Rotate(float angle)
{
	m_Model = glm::rotate(m_Model,rotateX,glm::vec3(1,0,0));
}

void Cube::RotateOnY(float angle)
{
	m_Model = glm::rotate(m_Model, rotateY, glm::vec3(0, 1, 0));
}

void Cube::Resize(glm::vec3& size)
{
	m_Model = glm::scale(m_Model, size);
}




Cube::~Cube()
{

}


