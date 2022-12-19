#pragma once

#include "Walnut/Image.h"
#include <memory>

using namespace Walnut;

class Renderer
{
public:

	Renderer() = default;

	void Render();

	void OnResize(uint32_t width, uint32_t height);
	auto GetFinadlImage() const { return finalImage; }
private:
	void ResizeImageData(uint32_t width, uint32_t height);
private:
	std::shared_ptr<Image> finalImage = nullptr;
	uint32_t* imageData = nullptr;
};