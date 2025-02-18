#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Mesh.h"

#include <iostream>
#include <stdexcept>
#include <glm/gtx/io.hpp>
#include "Triangle.h"
#include "BoundingBox.h"
#include "Material.h"
#include "Intersection.h"
#include "BVH.h"

Mesh::Mesh(const std::string& name, const std::string& file_path, Material* material)
	: name(name)
{
	this->material = material;
	LoadObject(file_path);
}

Mesh::~Mesh()
{
	for (auto& triangle : triangles)
	{
		delete triangle;
		triangle = nullptr;
	}
	if (material != nullptr)
	{
		delete material;
		material = nullptr;
	}
}

void Mesh::LoadObject(const std::string& FilePath)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, FilePath.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	glm::vec3 min_vertex = glm::vec3(std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	glm::vec3 max_vertex = glm::vec3(-std::numeric_limits<float>::infinity(),
		-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

	for (const auto& shape : shapes)
	{
		uint32_t tri = 0;
		for (uint32_t i = 0; i < shape.mesh.indices.size(); i += 3)
		{
			std::vector<glm::vec3> triangle_vertices(3);

			for (uint32_t j = 0; j < 3; ++j)
			{
				triangle_vertices[j] = glm::vec3(attrib.vertices[shape.mesh.indices[i + j].vertex_index * 3],
					attrib.vertices[shape.mesh.indices[i + j].vertex_index * 3 + 1],
					attrib.vertices[shape.mesh.indices[i + j].vertex_index * 3 + 2]);

				min_vertex = glm::vec3(std::min(min_vertex.x, triangle_vertices[j].x),
					std::min(min_vertex.y, triangle_vertices[j].y),
					std::min(min_vertex.z, triangle_vertices[j].z));

				max_vertex = glm::vec3(std::max(max_vertex.x, triangle_vertices[j].x),
					std::max(max_vertex.y, triangle_vertices[j].y),
					std::max(max_vertex.z, triangle_vertices[j].z));
			}

			triangles.push_back(new Triangle(triangle_vertices[0],
				triangle_vertices[1],
				triangle_vertices[2], material));
			area += triangles.back()->GetArea();
		}

		std::vector<Object*> primitives{ triangles.begin(), triangles.end() };
		if (primitives.size() < 10)
			bvh = new BVHTree(primitives, 2);
		else if (primitives.size() < 100)
			bvh = new BVHTree(primitives, 5);
		else if (primitives.size() < 1000)
			bvh = new BVHTree(primitives, 10);
		else
			bvh = new BVHTree(primitives, 20);

		bounding_box = new BoundingBox(min_vertex, max_vertex);
	}
}

std::tuple<Intersection, float> Mesh::Sample() const
{
	/*auto [pos, pdf] = bvh->Sample();
	return { pos, pdf };*/
	return triangles[Utilities::GetRandomIntBetween(0, triangles.size() - 1)]->Sample();
}

float Mesh::GetArea() const
{
	return area;
}

BoundingBox Mesh::GetBoundingBox() const
{
	return *bounding_box;
}

Intersection Mesh::GetIntersection(const Ray& ray)
{
	return bvh->Intersect(ray);
}

std::string Mesh::GetName() const
{
	return name;
}

glm::vec3 Mesh::GetEmission() const
{
	return material->GetEmission();
}

bool Mesh::HasEmission() const
{
	return material->HasEmission();
}