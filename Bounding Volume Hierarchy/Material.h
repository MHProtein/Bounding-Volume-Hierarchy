#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/vec3.hpp>

enum class MaterialType { DIFFUSE }; //for extension purposes

class Material
{


public:
	Material();
	Material(MaterialType type, const glm::vec3& emission, const glm::vec3& color);
	//calculate probability density function of the material
	float PDF(const glm::vec3& incident, const glm::vec3& reflected, const glm::vec3& normal) const;
	bool HasEmission() const;
	glm::vec3 GetEmission() const;
	auto GetType() const { return type; }
	auto GetColor() const { return color; }
	auto GetKs() const { return ks; }
	glm::vec3 BRDF(const glm::vec3& incident, const glm::vec3& reflected, const glm::vec3& normal) const;
	glm::vec3 ToWorldCoords(const glm::vec3& vector, const glm::vec3& normal) const;
	glm::vec3 Sample(const glm::vec3& normal) const;

private:
	MaterialType type;
	glm::vec3 color;
	glm::vec3 ks;
	glm::vec3 emission;
};

#endif
