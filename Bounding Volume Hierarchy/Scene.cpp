#include "Scene.h"

#include <iostream>
#include <tuple>
#include "Intersection.h"
#include "Object.h"
#include "BVH.h"
#include "Material.h"
#include "Mesh.h"
#include "Ray.h"
#include "Triangle.h"

Scene::Scene()
	: WIDTH(1920), HEIGHT(1080), FOV(45.0f), russian_roulette(0.8f)
{
}

void Scene::BuildBVH(uint32_t max_prims_in_node)
{
	std::cout << "Generating BVH Tree ... " << std::endl;
	std::cout << std::endl;
	bvh = new BVHTree(objects, max_prims_in_node);
	
}

Scene::Scene(uint32_t width, uint32_t height, float fov, float russian_roulette = 0.8f)
	: WIDTH(width), HEIGHT(height), FOV(fov), russian_roulette(russian_roulette)
{
}

Scene::~Scene()
{
}

void Scene::AddObject(Object* object)
{
	objects.push_back(object);
}

std::tuple<Intersection, float> Scene::SampleLight()
{
	float emit_area = 0.0f;
	for (const auto& object : objects)
	{
		if (object->HasEmission())
			emit_area += object->GetArea();
	}

	float p = Utilities::GetRandomFloat() * emit_area;
	emit_area = 0.0f;
	for (const auto& object : objects)
	{
		if (object->HasEmission())
		{
			emit_area += object->GetArea();
			if (p <= emit_area)
				return object->Sample();
		}
	}
}

Intersection Scene::Intersect(const Ray& ray)
{
	//return bvh->Intersect(ray);
	Intersection inter;
	for (const auto & object : objects)
	{
		for (const auto & triangle : reinterpret_cast<Mesh*>(object)->triangles)
		{
			Intersection temp = triangle->GetIntersection(ray);
			if(temp.happened)
			{
				if (temp.distance < inter.distance)
					inter = temp;
			}
		}
	}
	return inter;
}

glm::vec3 Scene::PathTracing(const Ray& ray, const float depth)
{
	glm::vec3 radiance_dir = glm::vec3();
	glm::vec3 radiance_indir = glm::vec3();
	Intersection point = Intersect(ray);

	if (!point.happened)
	{
		return radiance_dir;
	}

	if (point.material->HasEmission())
	{
		if (depth == 0)
			return point.material->GetEmission();
		else
			return { 0.0f, 0.0f, 0.0f };
	}

	auto [light_sample, pdf] = SampleLight();
	Ray point_to_light(point.coords, glm::normalize(light_sample.coords - point.coords));

	Intersection inter = Intersect(point_to_light);

	float distance = glm::length(light_sample.coords - point.coords);

	if (inter.happened && inter.distance - distance > -EPSILON)
	{
		radiance_dir = light_sample.material->GetEmission() * 
			point.material->BRDF(ray.direction, point_to_light.direction, normalize(point.normal)) *
			glm::dot(point_to_light.direction, normalize(point.normal)) * 
			glm::dot(-point_to_light.direction, normalize(light_sample.normal)) / 
			(distance * distance) / pdf;
	}

	if (Utilities::GetRandomFloat() > russian_roulette)
		return radiance_dir;

	glm::vec3 incident = normalize(point.material->Sample(normalize(point.normal)));
	Ray object_to_point(point.coords, incident);

	auto inter_obj_to_p = Intersect(object_to_point);

	if (inter_obj_to_p.happened && !inter_obj_to_p.material->HasEmission())
	{
		radiance_indir = PathTracing(object_to_point, depth + 1) * 
			point.material->BRDF(-ray.direction, incident, normalize(point.normal)) *
			glm::dot(incident, normalize(point.normal)) / 
			point.material->PDF(ray.direction, incident, normalize(point.normal)) / 
			russian_roulette;
	}

	return radiance_dir + radiance_indir;
}