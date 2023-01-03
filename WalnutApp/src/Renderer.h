#pragma once

#include "Ray.h"
#include "Sphere.h"
#include "Camera.h"
#include "Walnut/Image.h"

#include <memory>

#include <glm/glm.hpp>

#include "Scene.h"

#include "Intersection.h"

using namespace Walnut;

class Renderer
{
public:

	Renderer();

	void Render(const Scene& scene, const Camera& camera);

	void OnResize(uint32_t width, uint32_t height);
	auto GetFinadlImage() const { return finalImage; }

public:
	glm::vec3 lightDirection{ -1.0f };
	const Scene* activeScene = nullptr;
	const Camera* activeCamera = nullptr;
private:
	void ResizeImageData(uint32_t width, uint32_t height);

	glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen
	
	Intersection TraceRay(const Ray& ray);

	Intersection ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex);

	Intersection Miss(const Ray& ray);

	inline glm::vec4 TraceRay(const Scene& scene, const Ray& ray);
private:
	std::shared_ptr<Image> finalImage = nullptr;
	uint32_t* imageData = nullptr;
	float aspect = 1.0f;
};