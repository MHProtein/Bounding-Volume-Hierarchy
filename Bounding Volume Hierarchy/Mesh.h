#pragma once
#ifndef MESH_H
#define MESH_H
#include <vector>

#include "Object.h"

class BVHTree;
class BoundingBox;
class Triangle;
class Material;

class Mesh : public Object
{
	friend class Scene;
public:
	Mesh(const std::string& name, const std::string& file_path, Material* material);
	~Mesh() override;
	void LoadObject(const std::string& FilePath);
	std::tuple<Intersection, float> Sample() const override;
	float GetArea() const override;
	BoundingBox GetBoundingBox() const override;
	Intersection GetIntersection(const Ray& ray) override;
	std::string GetName() const override;
	glm::vec3 GetEmission() const override;
	bool HasEmission() const override;
	
private:
	std::string name;
	BoundingBox* bounding_box;
	std::vector<Triangle*> triangles;
	Material* material;
	BVHTree* bvh;
	float area;
};

#endif

