#pragma once
#ifndef BVH_H
#define BVH_H
#include "Utilities.h"
#include "BoundingBox.h"

class Ray;
class Intersection;

class Object;

struct BVHNode
{
	BoundingBox bounding_box;
	std::vector<Object*> objects;
	BVHNode* left;
	BVHNode* right;
	float area;

	Axis split_axis;
	uint32_t primitives_count = 0;

	BVHNode();
	~BVHNode();
};

class BVHTree
{
public:
	BVHTree(std::vector<Object*>& objects, uint32_t max_prims_in_node);
	~BVHTree();
	Intersection Intersect(const Ray& ray) const;
	std::tuple<Intersection, float> Sample() const;
	void DebugPrint() const;

private:
	void Clear();
	void RecursiveClear(BVHNode* node);
	void DebugPrint(BVHNode* t, std::string prefix, bool IsLeft) const;
	BVHNode* RecuriveBuild(std::vector<Object*>& objects);
	Intersection Intersect(const Ray& ray, BVHNode* node, const std::array<uint32_t, 3>& dir_is_neg) const;
	std::tuple<Intersection, float> Sample(BVHNode* node, float p) const;

private:
	BVHNode* root;
	const uint32_t max_prims_in_node;
	std::vector<Object*> primitives;
};

#endif
