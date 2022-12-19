#pragma once

#include <iostream>
#include <glm/glm.hpp>

inline uint32_t convertColor(const glm::vec3& color)
{
	uint8_t red = static_cast<uint8_t>(color.x * 255.999f);
	uint8_t green = static_cast<uint8_t>(color.y * 255.999f);
	uint8_t blue = static_cast<uint8_t>(color.z * 255.999f);

	return 0xff000000 + (blue << 16) + (green << 8) + red;
}