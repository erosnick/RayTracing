#include "Scene.h"
#include "Sphere.h"

Scene::Scene()
{
	spheres.emplace_back(std::make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, 0.0f), 0.5f, 0));
	spheres.emplace_back(std::make_shared<Sphere>(glm::vec3(0.25f, 0.0f, 0.0f), 0.25f, 1));
	spheres.emplace_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, 0.0f), 100.0f, 2));

	materials.push_back(Material{ {1.0f, 0.0f, 0.0f}, 0.25f, 0.0f });
	materials.push_back(Material{ {0.0f, 0.0f, 1.0f}, 0.5f, 0.0f });
	materials.push_back(Material{ {0.5f, 1.0f, 0.5f}, 0.5f, 0.0f });
}

Scene::~Scene()
{
}