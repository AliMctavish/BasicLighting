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
	CUBEID++;
}

void Cube::Draw()
{
	m_Shader.setVec3("light.ambiant", material.Ambiant);
	m_Shader.setVec3("light.diffuse", material.Diffuse);
	m_Shader.setVec3("light.specular", material.Specular);
	m_Shader.setFloat("material.Shininess", material.shininess);
	m_Shader.SetMat4("model", m_Model);
	m_Shader.setInt("texture0", texture.m_TextureIndex);
	texture.Bind();
	UseColor("objectColor");
	BaseObject::Draw();
	texture.UnBind();
}

void Cube::SetTextureData(std::string fileName)
{
	m_TexutrePath = fileName;
	texture.SetTexture(fileName.c_str(),1);
}



void Cube::Update()
{
	BaseObject::Update();
	Rotate(rotateX);
	RotateOnY(rotateY);
	BaseObject::Resize();
}

void Cube::Rotate(float angle)
{
	m_Model = glm::rotate(m_Model, rotateX, glm::vec3(1, 0, 0));
}

void Cube::RotateOnY(float angle)
{
	m_Model = glm::rotate(m_Model, rotateY, glm::vec3(0, 1, 0));
}

Cube::~Cube()
{

}
