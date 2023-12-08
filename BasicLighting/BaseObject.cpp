#include "BaseObject.h"

void BaseObject::Update()
{
	m_Model = glm::mat4(1.0f);
	m_Model = glm::translate(m_Model, Position);
}

void BaseObject::Draw()
{
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void BaseObject::SinMove()
{
	m_Model = glm::mat4(1.0f);
	m_Model = glm::translate(m_Model, Position + glm::vec3(0, glm::sin(sineValue) / 2 + 1,0));
	sineValue += 0.01;
}

void BaseObject::Resize()
{
	m_Model = glm::translate(m_Model, glm::vec3(Size.x * 0.5f, Size.y * 0.5f, Size.z * 0.5f));
	m_Model = glm::scale(m_Model, glm::vec3(Size));
}

void BaseObject::SetShader(Shader &shader)
{
	m_Shader = shader;
}

void BaseObject::UseColor(const char* name)
{
	int colorLoc = glGetUniformLocation(m_Shader.shader_program, name);
	glUniform3f(colorLoc, Color[0], Color[1], Color[2]);
}

void BaseObject::SetObjectColor(float x, float y, float z)
{
	Color[0] = x;
	Color[1] = y;
	Color[2] = z;
}

void BaseObject::SetName(std::string name)
{
	_name = name;
}
