#include "Renderer.h"
#include "Walnut/Random.h"
#include "Utils.h"

void Renderer::Render()
{
	for (uint32_t y = 0; y < finalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < finalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { x / static_cast<float>(finalImage->GetWidth()), y / static_cast<float>(finalImage->GetHeight()) };
			coord = coord * 2.0f - 1.0f;

			coord.x *= aspect;

			auto color = PerPixel(coord);

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

glm::vec4 Renderer::PerPixel(const glm::vec2& coord)
{
	glm::vec3 center{ 0.0f, 0.0f, -1.0f };
	
	glm::vec3 rayOrigin{ 0.0f, 0.0f, 0.0f };

	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	//rayDirection = glm::normalize(rayDirection);

	float radius = 0.5f;

	// (bx^2 + by^2)t^2 + 2(axbx + ayby)t + (ax^2 + ay^2 - r^2) = 0
	// a = ray origin
	// b = ray direction
	// r = radius
	// t = hit distance
	auto oc = rayOrigin - center;
	auto a = glm::dot(rayDirection, rayDirection);
	auto b = 2.0f * glm::dot(oc, rayDirection);
	auto c = glm::dot(oc, oc) - radius * radius;

	// Quadratic formula discriminant
	// b^2 - 4ac;
	float discriminant = b * b - 4.0f * a * c;
	
	if (discriminant < 0.0f)
	{
		auto alpha = 0.5f * (rayDirection.y + 1.0f);
		auto color = (1.0f - alpha) * glm::vec3(1.0f, 1.0f, 1.0f) + alpha * glm::vec3(0.5f, 0.7f, 1.0f);

		return glm::vec4(color, 1.0f);
	}

	float discriminantSqrt = std::sqrtf(discriminant);

	// (-b +- sqrt(discriminant)) / 2a
	float t0 = (-b - discriminantSqrt) / (2.0f * a);
	float t1 = (-b + discriminantSqrt) / (2.0f * a);

	auto hitPosition0 = rayOrigin + rayDirection * t0;
	auto hitPosition1 = rayOrigin + rayDirection * t1;

	auto normal = glm::normalize(hitPosition0 - center);

	auto color = normal * 0.5f + 0.5f;

	color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));

	auto lightDirection = glm::normalize(glm::vec3(-1.0f));

	auto diffuse = glm::max(0.0f, glm::dot(normal, -lightDirection));

	//return glm::vec4(color, 1.0f);
	return glm::vec4(glm::vec3(diffuse), 1.0f);
}
