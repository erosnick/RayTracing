#include "Renderer.h"
#include "Walnut/Random.h"

void Renderer::Render()
{
	for (uint32_t i = 0; i < finalImage->GetHeight(); i++)
	{
		for (uint32_t j = 0; j < finalImage->GetWidth(); j++)
		{
			imageData[i * finalImage->GetWidth() + j] = Random::UInt();
			imageData[i * finalImage->GetWidth() + j] |= 0xff000000;
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
