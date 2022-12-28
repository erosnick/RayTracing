#pragma once

#include "Walnut/Image.h"
#include "Camera.h"
#include "Ray.h"

#include <memory>

#include <glm/glm.hpp>

using namespace Walnut;

class Renderer
{
public:

	Renderer() = default;

	void Render(const Camera& camera);

	void OnResize(uint32_t width, uint32_t height);
	auto GetFinadlImage() const { return finalImage; }

public:
	glm::vec3 objectColor{ 1.0f };
	glm::vec3 lightDirection{ -1.0f };
private:
	void ResizeImageData(uint32_t width, uint32_t height);
	inline glm::vec4 TraceRay(const Ray& ray);
private:
	std::shared_ptr<Image> finalImage = nullptr;
	uint32_t* imageData = nullptr;
	float aspect = 1.0f;
};