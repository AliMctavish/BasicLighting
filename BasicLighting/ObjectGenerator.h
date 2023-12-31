#pragma once
#include <vector>
#include "Enemy.h"

class ObjectGenerator {
private :
	float enemySpawnTime = 10;
	float lightSpawnTime = 10;
	ModelLoader m_ModelLoader;
public : 
	void inline SetModelLoader(ModelLoader& modelLoader) { m_ModelLoader = modelLoader; }
	void GenerateEnemy(std::vector<Enemy>& enemies , Shader &shader)
	{
		enemySpawnTime -= 0.01;
		if (enemySpawnTime < 0)
		{
			Enemy enemy;
			enemy.SetShader(shader);
			//enemy.SetModel(m_ModelLoader);
			enemy.SetRandomLocation();
			enemy.SetName("enemy" + std::to_string(enemies.size()));
			enemies.push_back(enemy);
			enemySpawnTime = 10;
		}
	}
	void GenerateLight(std::vector<Light>& lights , Shader &shader)
	{
		lightSpawnTime -= 0.01;
		if (lightSpawnTime < 0)
		{
			Light light;
			light.SetShader(shader);
			//light.SetModel(m_ModelLoader);
			light.SetRandomLocation();
			light.SetRandomColor();
			light.SetName("light" + std::to_string(lights.size()));
			lights.push_back(light);
			lightSpawnTime = 10;
		}
	}
};
