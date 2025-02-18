#pragma once
#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <glm/vec3.hpp>


class Intersection;
class BVHTree;
class Ray;
class Light;
class Object;

class Scene
{
public:
	uint32_t WIDTH;
	uint32_t HEIGHT;
	float FOV;
	float russian_roulette;
	BVHTree* bvh;
	std::vector<Object*> objects;

public:
	Scene();
	~Scene();
	void BuildBVH(uint32_t max_prims_in_node);
	Scene(uint32_t width, uint32_t height, float fov, float russian_roulette);
	void AddObject(Object* object);
	std::tuple<Intersection, float> SampleLight();
	Intersection Intersect(const Ray& ray);
	glm::vec3 PathTracing(const Ray& ray, const float depth);

};

#endif
