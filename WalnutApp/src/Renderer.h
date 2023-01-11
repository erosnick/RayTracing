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
	struct Settings
	{
		bool accumulate = true;
	};
public:

	Renderer();

	void Render(const Scene& scene, const Camera& camera);

	void OnResize(uint32_t width, uint32_t height);
	auto GetFinadlImage() const { return finalImage; }
	void resetFrameIndex() { frameIndex = 1; }
	uint32_t getFrameIndex() const { return frameIndex; }
	Settings& getSettings() { return settings; }
public:
	glm::vec3 lightDirection{ -1.0f };
	const Scene* activeScene = nullptr;
	const Camera* activeCamera = nullptr;
private:
	void ResizeImageData(uint32_t width, uint32_t height);

	glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen shader
	
	Intersection TraceRay(const Ray& ray);

	Intersection ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex);

	Intersection Miss(const Ray& ray); // Miss shader

	inline glm::vec4 TraceRay(const Scene& scene, const Ray& ray);
private:
	std::shared_ptr<Image> finalImage = nullptr;
	uint32_t* imageData = nullptr;
	glm::vec4* accumulationData = nullptr;
	uint32_t frameIndex = 1;
	float aspect = 1.0f;
	Settings settings;
};