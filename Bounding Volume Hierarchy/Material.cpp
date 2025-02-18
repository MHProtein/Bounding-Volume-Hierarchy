#include "Material.h"

#include <stdexcept>
#include <valarray>
#include <glm/gtx/type_trait.hpp>

#include "Utilities.h"

Material::Material()
	:type(MaterialType::DIFFUSE), emission(glm::vec3()), color(glm::vec3())
{

}

Material::Material(MaterialType type, const glm::vec3& emission, const glm::vec3& color)
	: type(type), emission(emission), color(color)
{

}


float Material::PDF(const glm::vec3& incident, const glm::vec3& reflected, const glm::vec3& normal) const
{
	switch (type) {
	case MaterialType::DIFFUSE:
		{
		if (glm::dot(reflected, normal) > 0)
			return 0.5f / PI; // 1 / 2 * pi   Area of a hemisphere with radius equals to 1 is 2 * pi
		else
			return 0.0f;
			break;
		}
	default: 
		throw std::runtime_error("Non-existing Material Type in BRDF");
	}
}

glm::vec3 Material::BRDF(const glm::vec3& incident, const glm::vec3& reflected, const glm::vec3& normal) const
{
	switch (type)
	{
		case MaterialType::DIFFUSE:
		{
			if (glm::dot(reflected, normal) > 0.0f)
				return color / PI;
			else
				return glm::vec3(0.0f);
			break;
		}
		default:
			throw std::runtime_error("Non-existing Material Type in Material::BRDF");
			break;
	}
}

glm::vec3 Material::ToWorldCoords(const glm::vec3& vector, const glm::vec3& normal) const
{
	glm::vec3 A, B;
	if(std::abs(normal.x) > std::abs(normal.y))
	{
		float inv_length = 1.0f / (normal.x * normal.x + normal.z + normal.z);
		A = glm::vec3(normal.z * inv_length, 0.0f, -normal.x * inv_length); // [x , y, z] * [z, 0, -x] =  0 => orthonormal
	}
	else
	{
		float inv_length = 1.0f / (normal.y * normal.y + normal.z * normal.z);
		A = glm::vec3(0.0f, normal.z * inv_length, -normal.y * inv_length);  //likewise
	}
	B = glm::cross(A, normal);

	return vector.x * A + vector.y * B + vector.z * normal;
}

glm::vec3 Material::Sample(const glm::vec3& normal) const
{
	switch (type) {
	case MaterialType::DIFFUSE:
		{
			//uniform sample on a hemisphere to calculate a randomly bouncing direction
			//using spherical coordinates
			float z = Utilities::GetRandomFloat();
			float radius = std::sqrt(1.0f - z * z);
			float phi = 2 * PI * Utilities::GetRandomFloat();
			glm::vec3 bounced_ray = glm::vec3(radius * std::cos(phi), radius * std::sin(phi), z); //spherical coords
			return ToWorldCoords(bounced_ray, normal);
		break;
		}
	default: 
		throw std::runtime_error("Non-existing Material Type in Material::Sample");
		break;
	}
}

bool Material::HasEmission() const
{
	
	if (static_cast<float>(glm::length(GetEmission())) > EPSILON)
		return true;
	return false;
}

glm::vec3 Material::GetEmission() const
{
	return emission;
}

