#pragma once

#include <iostream>
#include <glm/glm.hpp>

namespace Utils
{
	inline uint32_t convertToRGBA(const glm::vec4& color)
	{
		uint8_t red = static_cast<uint8_t>(color.r * 255.999f);
		uint8_t green = static_cast<uint8_t>(color.g * 255.999f);
		uint8_t blue = static_cast<uint8_t>(color.b * 255.999f);
		uint8_t alpha = static_cast<uint8_t>(color.a * 255.999f);

		return (alpha << 24) + (blue << 16) + (green << 8) + red;
	}
}