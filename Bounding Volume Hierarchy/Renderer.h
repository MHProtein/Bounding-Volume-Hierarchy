#pragma once
#ifndef RENDERER_H
#define RENDERER_H
#include <cstdint>
#include <glm/vec3.hpp>

class Scene;

class Renderer
{
public:
	void Render(Scene* scene, uint32_t spp, const glm::vec3& eye_position); //Sample Per Pixel
};

#endif

