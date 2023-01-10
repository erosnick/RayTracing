#include "Renderer.h"
#include "Walnut/Random.h"
#include "Utils.h"

Renderer::Renderer()
{
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	activeScene = &scene;
	activeCamera = &camera;

	for (uint32_t y = 0; y < finalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < finalImage->GetWidth(); x++)
		{
			auto color = PerPixel(x, y);
			//auto color = TraceRay(scene, ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
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

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.origin = activeCamera->GetPosition();
	auto rayDirection = activeCamera->GetRayDirections()[y * finalImage->GetWidth() + x];
	ray.direction = rayDirection;

	int32_t bounces = 2;
	glm::vec3 finalColor = glm::vec3(0.0f);
	float multiplier = 1.0f;

	for (auto i = 0; i < bounces; i++)
	{
		auto intersection = TraceRay(ray);

		if (intersection.hitDistance < 0.0f)
		{
			auto alpha = 0.5f * (ray.direction.y + 1.0f);
			auto color = (1.0f - alpha) * glm::vec3(1.0f, 1.0f, 1.0f) + alpha * glm::vec3(0.5f, 0.7f, 1.0f);
			
			finalColor += color * multiplier;
			break;
		}

		auto color = intersection.normal * 0.5f + 0.5f;

		color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));

		lightDirection = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));

		auto diffuse = glm::max(glm::dot(intersection.normal, -lightDirection), 0.0f);

		const auto& sphere = activeScene->getSpheres()[intersection.objectIndex];

		auto sphereColor = sphere->material.albedo * diffuse;

		finalColor += sphereColor * multiplier;
		multiplier *= 0.5f;

		ray.origin = intersection.position + intersection.normal * 0.0001f;
		ray.direction = glm::reflect(ray.direction, intersection.normal);
	}

	return glm::vec4(finalColor, 1.0f);
}

glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
{
	Intersection intersection;
	bool hit = false;
	float closestSoFar = std::numeric_limits<float>::max();

	for (const auto& sphere : scene.getSpheres())
	{
		if (sphere->intersect(ray, 0.001f, closestSoFar, intersection))
		{
			hit = true;
			closestSoFar = intersection.hitDistance;
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

Intersection Renderer::TraceRay(const Ray& ray)
{
	Intersection intersection;

	bool hit = false;
	float closestSoFar = std::numeric_limits<float>::max();
	int closestSphere = -1;

	for (auto i = 0; i < activeScene->getSpheres().size(); i++)
	{
		const auto& sphere = activeScene->getSpheres()[i];

		if (sphere->intersect(ray, 0.001f, closestSoFar, intersection))
		{
			hit = true;
			closestSoFar = intersection.hitDistance;
			closestSphere = i;
		}
	}

	if (!hit)
	{
		return Miss(ray);
	}

	return ClosestHit(ray, closestSoFar, closestSphere);
}

Intersection Renderer::ClosestHit(const Ray& ray, float hitDistance, uint32_t objectIndex)
{
	auto sphere = activeScene->getSpheres()[objectIndex];

	Intersection intersection;

	intersection.hitDistance = hitDistance;
	intersection.objectIndex = objectIndex;

	intersection.position = ray.origin + ray.direction * hitDistance;
	intersection.normal = glm::normalize(intersection.position - sphere->center);
	intersection.material = sphere->material;

	return intersection;
}

Intersection Renderer::Miss(const Ray& ray)
{
	Intersection intersection;

	intersection.hitDistance = -1.0f;

	return intersection;
}
