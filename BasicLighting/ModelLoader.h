#pragma once
#include "Model.h"
class ModelLoader {
private:
	Model Sphere;
	Model Enemy;
	Model Key;
	Model Machine;
public:
	enum Type {
		Type_Sphere = 0,
		Type_Enemy = 1,
		Type_Key = 2,
		Type_Machine = 3,
	};
	void InitializeModels()
	{
		Sphere = Model("Models/Test/dot.obj");
		Enemy = Model("Models/Enemy/Enemy.obj");
		Key = Model("Models/Objects/Key/key.obj");
		Machine = Model("Models/Objects/Machine/Machine.obj");
	}

	Model GetModel(Type type)
	{
		Model selectedModel;
		switch (type)
		{
		case ModelLoader::Type_Sphere:
			selectedModel = Sphere;
			break;
		case ModelLoader::Type_Enemy:
			selectedModel = Enemy;
			break;
		case ModelLoader::Type_Key:
			selectedModel = Key;
			break;
		case ModelLoader::Type_Machine:
			selectedModel = Machine;
		default:
			break;
		}

		return selectedModel;
	}
};
