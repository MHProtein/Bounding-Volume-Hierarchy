#include <chrono>
#include <iostream>
#include <vector>

#include "BVH.h"
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Scene.h"
#include "Utilities.h"

int main()
{
    auto begin = std::chrono::high_resolution_clock::now();
    Renderer renderer;
    Scene scene(1920, 1080, 90, 0.8f);

    //Create Materials

    Material* red = new Material(MaterialType::DIFFUSE, glm::vec3(0.0f), glm::vec3(0.63f, 0.065f, 0.05f));
    Material* green = new Material(MaterialType::DIFFUSE, glm::vec3(0.0f), glm::vec3(0.14f, 0.45f, 0.091f));
    Material* white = new Material(MaterialType::DIFFUSE, glm::vec3(0.0f), glm::vec3(0.725f, 0.71f, 0.68f));
    Material* light = new Material(MaterialType::DIFFUSE, (8.0f * glm::vec3(0.747f + 0.058f, 0.747f + 0.258f, 0.747f) +
	                                   15.6f * glm::vec3(0.740f + 0.287f, 0.740f + 0.160f, 0.740f) + 18.4f * 
                                       glm::vec3(0.737f + 0.642f, 0.737f + 0.159f, 0.737f)), 
                                       glm::vec3(0.65f));
    
    //Create Models
    Mesh* floor = new Mesh("floor", "models/bunny/bunny.obj", white);
    Mesh* shortbox = new Mesh("shortbox", "models/cornellbox/shortbox.obj", white);
    Mesh* tallbox = new Mesh("tallbox", "models/cornellbox/tallbox.obj", white);
    Mesh* left = new Mesh("left", "models/cornellbox/left.obj", red);
    Mesh* right = new Mesh("right", "models/cornellbox/right.obj", green);
    Mesh* light_ = new Mesh("light", "models/cornellbox/light.obj", light);

    scene.AddObject(floor);
    scene.AddObject(shortbox);
    scene.AddObject(tallbox);
    scene.AddObject(left);
    scene.AddObject(right);
    scene.AddObject(light_);

    scene.BuildBVH(1);
    
    renderer.Render(&scene, 32, glm::vec3(278, 273, -800));

	auto end = std::chrono::high_resolution_clock::now();

    auto time_interval = std::chrono::duration<float>(end - begin);

    std::cout << "Render Completed, " << std::fixed << std::setprecision(5) << time_interval << " Seconds Taken." << std::endl;
    std::cout << std::endl;

    return 0;
}
