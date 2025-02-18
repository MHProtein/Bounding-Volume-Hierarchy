#include "BVH.h"

#include <array>
#include <iomanip>
#include <iostream>
#include <tuple>

#include "BoundingBox.h"
#include "Intersection.h"
#include "Object.h"
#include "Ray.h"
#include "chrono"

BVHNode::BVHNode()
	: left(nullptr), right(nullptr)
{
	
}

BVHNode::~BVHNode()
{
	if (objects.empty())
		return;
	for (auto & object : objects)
	{
		delete object;
		object = nullptr;
	}
}

BVHTree::BVHTree(std::vector<Object*>& objects, uint32_t max_prims_in_node)
	: primitives(objects), max_prims_in_node(max_prims_in_node)
{
	auto begin = std::chrono::high_resolution_clock::now();
	root = RecuriveBuild(primitives);
	auto end = std::chrono::high_resolution_clock::now();

	auto time_interval = std::chrono::duration<float>(end - begin);

	std::cout << "BVH Generation Complete, Duration : " << std::fixed << std::setprecision(5) << time_interval << " Seconds." << std::endl;
	std::cout << std::endl;

}

BVHTree::~BVHTree()
{
	Clear();
}

BVHNode* BVHTree::RecuriveBuild(std::vector<Object*>& objects)
{
	BVHNode* node = new BVHNode;

	if(objects.size() <= max_prims_in_node)
	{
		BoundingBox box;
		for (auto object : objects)
		{
			node->objects.push_back(object);
			box = Union(box, object->GetBoundingBox());
			node->area += object->GetArea();
			node->primitives_count++;
		}
		node->bounding_box = box;
		node->left = nullptr;
		node->right = nullptr;
		return node;
	}
	else
	{
		BoundingBox centroid_box;
		for (const auto & object : objects)
		{
			centroid_box = Union(centroid_box, object->GetBoundingBox().Centroid());
		}

		Axis axis = centroid_box.MaxExtent(); //the axis that objects stretch the longest

		switch (axis)
		{
			case Axis::X :
				{
					std::sort(objects.begin(), objects.end(), [](Object* obj1, Object* obj2)
						{
							return obj1->GetBoundingBox().Centroid().x < obj2->GetBoundingBox().Centroid().x;
						});
					break;
				}
			case Axis::Y :
				{
					std::sort(objects.begin(), objects.end(), [](Object* obj1, Object* obj2)
						{
							return obj1->GetBoundingBox().Centroid().y < obj2->GetBoundingBox().Centroid().y;
						});
					break;
				}
			case Axis::Z :
				{
					std::sort(objects.begin(), objects.end(), [](Object* obj1, Object* obj2)
						{
							return obj1->GetBoundingBox().Centroid().z < obj2->GetBoundingBox().Centroid().z;
						});
					break;
				}
		}

		auto beg = objects.begin();
		auto end = objects.end();
		auto middle = beg + (objects.size() / 2);

		std::vector<Object*> left_objs{ beg, middle };
		std::vector<Object*> right_objs{ middle, end };

		node->left = RecuriveBuild(left_objs);
		node->right = RecuriveBuild(right_objs);
		node->objects = {};
		node->area = node->left->area + node->right->area;
		BoundingBox box = Union(node->left->bounding_box, node->right->bounding_box);
		node->bounding_box = box;
		node->split_axis = axis;
		node->primitives_count = node->left->primitives_count + node->right->primitives_count;
		return node;
	}
	return node;
}

Intersection BVHTree::Intersect(const Ray& ray) const
{
	if (root == nullptr)
		return {};

	std::array<uint32_t, 3> dir_is_neg;
	dir_is_neg[0] = ray.direction.x > 0.0f ? 0 : 1;
	dir_is_neg[1] = ray.direction.y > 0.0f ? 0 : 1;
	dir_is_neg[2] = ray.direction.z > 0.0f ? 0 : 1;

	Intersection inter;
	inter = Intersect(ray, root, dir_is_neg);
	/*if(inter.coords != glm::vec3())
		std::cout << inter.coords << std::endl;*/
	return inter;
}

Intersection BVHTree::Intersect(const Ray& ray, BVHNode* node, const std::array<uint32_t, 3>& dir_is_neg) const
{
	if (node == nullptr)
		return {};

	Object* intersected_obj;

	if (!node->bounding_box.Intersect(ray, dir_is_neg))
	{
		return {};
	}

	if (node->left == nullptr && node->right == nullptr)
	{
		Intersection inter;
		for (const auto& object : node->objects)
		{
			Intersection temp = object->GetIntersection(ray);
			if (temp.happened == true)
			{
				if (temp.distance < inter.distance)
					std::swap(temp, inter);
			}
		}
		return inter;
	}
	else
	{
		auto i1 = Intersect(ray, node->left, dir_is_neg);
		auto i2 = Intersect(ray, node->right, dir_is_neg);
		return i1.distance < i2.distance ? i1 : i2;
	}
}

std::tuple<Intersection, float> BVHTree::Sample() const
{
	if (root == nullptr)
		return {};
	float p = Utilities::GetRandomFloat() * root->area;
	auto[pos, pdf] = Sample(root, p);

	return { pos, pdf };
}

void BVHTree::DebugPrint() const
{
	if (root == nullptr)
		return;
	DebugPrint(root, " ", false);
}

void BVHTree::Clear()
{
	if (root == nullptr)
		return;
	RecursiveClear(root);
}

void BVHTree::RecursiveClear(BVHNode* node)
{
	if (node != nullptr)
	{
		RecursiveClear(node->left);
		RecursiveClear(node->right);

		delete node;
		node = nullptr;
	}
}

void BVHTree::DebugPrint(BVHNode* t, std::string prefix, bool IsLeft) const
{
	if (t == nullptr)
		return;

	std::cout << prefix;
	std::cout << (IsLeft ? "├──" : "└──");
	if(!t->objects.empty())
	{
		for (const auto& object : t->objects)
		{
			std::cout << object->GetName() << " ";
		}
		std::cout << std::endl;
	}
	else
		std::cout << "internal node" << std::endl;
	DebugPrint(t->left, prefix + (IsLeft ? "│  " : "   "), true);
	DebugPrint(t->right, prefix + (IsLeft ? "│  " : "   "), false);
}

std::tuple<Intersection, float> BVHTree::Sample(BVHNode* node, float p) const
{
	if(node->left == nullptr && node->right == nullptr)
	{
		auto[pos, pdf] = node->objects[Utilities::GetRandomIntBetween(0, max_prims_in_node - 1)]->Sample();
		
		return { pos, pdf };
	}
	if (p < node->left->area)
		return Sample(node->left, p);
	else
		return Sample(node->right, p);
}