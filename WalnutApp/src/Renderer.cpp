#include "Renderer.h"
#include "Walnut/Random.h"
#include "Utils.h"

Renderer::Renderer()
{
	spheres.emplace_back(std::make_shared<Sphere>(glm::vec3(-0.5f, 0.0f, 0.0f), 0.5f, Material{ {1.0f, 0.0f, 0.0f} }));
	spheres.emplace_back(std::make_shared<Sphere>(glm::vec3(0.25f, 0.0f, 0.0f), 0.25f, Material{ {0.0f, 0.0f, 1.0f} }));
	spheres.emplace_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, 0.0f), 100.0f, Material{ {0.5f, 1.0f, 0.5f} }));
}

void Renderer::Render(const Camera& camera)
{
	Ray ray;
	ray.origin = camera.GetPosition();

	for (uint32_t y = 0; y < finalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < finalImage->GetWidth(); x++)
		{
			auto rayDirection = camera.GetRayDirections()[y * finalImage->GetWidth() + x];
			ray.direction = rayDirection;

			auto color = TraceRay(ray);

		    imageData[y * finalImage->GetWidth() + x] = Utils::convertToRGBA(color);
		}
	}

	finalImage->SetData(imageData);
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (finalImage)
	{
		// No resize necessary
		if (finalImage->GetWidth() == width && finalImage->GetHeight() == height)
		{
			return;
		}
		finalImage->Resize(width, height);
		aspect = static_cast<float>(width) / height;
	}
	else
	{
		finalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	ResizeImageData(width, height);
}

void Renderer::ResizeImageData(uint32_t width, uint32_t height)
{
	delete[] imageData;
	imageData = new uint32_t[width * height];
}

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
	Intersection intersection;
	bool hit = false;
	float closestSoFar = std::numeric_limits<float>::max();

	for (const auto& sphere : spheres)
	{
		if (sphere->intersect(ray, 0.001f, closestSoFar, intersection))
		{
			hit = true;
			closestSoFar = intersection.t;
		}
	}
	
	if (!hit)
	{
		auto alpha = 0.5f * (ray.direction.y + 1.0f);
		auto color = (1.0f - alpha) * glm::vec3(1.0f, 1.0f, 1.0f) + alpha * glm::vec3(0.5f, 0.7f, 1.0f);

		return glm::vec4(color, 1.0f);
	}

	auto color = intersection.normal * 0.5f + 0.5f;

	color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));

	lightDirection = glm::normalize(lightDirection);

	auto diffuse = glm::max(0.0f, glm::dot(intersection.normal, -lightDirection));

	//return glm::vec4(color, 1.0f);
	return glm::vec4(intersection.material.albedo * diffuse, 1.0f);
}
