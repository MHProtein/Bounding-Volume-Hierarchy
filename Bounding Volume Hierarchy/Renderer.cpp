#include "Renderer.h"

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>
#include <valarray>
#include <vector>
#include <format>
#include <glm/vec3.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include "Ray.h"
#include "Scene.h"
#include "Utilities.h"

void Renderer::Render(Scene* scene, uint32_t spp, const glm::vec3& eye_pos)
{
	std::vector<glm::vec3> frame_buffer(scene->WIDTH * scene->HEIGHT); //storage color of pixels

	float scale = std::tan(Utilities::DegreeToRadians(scene->FOV * 0.5f));
	float aspect_ratio = scene->WIDTH / scene->HEIGHT;

	uint32_t thread_num = 20;
	uint32_t thread_height = scene->HEIGHT / thread_num;
	std::vector<std::thread> threads(thread_num);
	std::cout << "Sample Per Pixel : " << spp << std::endl;

	std::mutex mtx;
	float process = 0.0f;
	float height_inv = 1.0f / scene->HEIGHT;

	auto CastRay = [&](int thread_index)
		{
			int height = thread_height * (thread_index + 1);
			for (uint32_t j = height - thread_height; j < height; ++j)
			{
				for(uint32_t i = 0; i < scene->WIDTH; ++i)
				{
					float x = (2.0f * (i + 0.5f) / static_cast<float>(scene->WIDTH) - 1.0f) * aspect_ratio * scale;
					float y = (1.0f - 2.0f * (j + 0.5f) / static_cast<float>(scene->HEIGHT)) * scale;

					glm::vec3 direction = glm::normalize(glm::vec3(-x, y, 1.0f));
					for (uint32_t k = 0; k < spp; ++k)
					{
						frame_buffer[j * scene->WIDTH + i] += scene->PathTracing(Ray(eye_pos, direction), 0) / spp;
						//std::cout << frame_buffer[k] << std::endl;
					}
				}
				mtx.lock();
				process = process + height_inv;
				Utilities::UpdateProgress(process);
				mtx.unlock();
			}
		};

	for (uint32_t i = 0; i < thread_num; ++i)
		threads[i] = std::thread(CastRay, i);

	for (uint32_t i = 0; i < thread_num; ++i)
		threads[i].join();

	Utilities::UpdateProgress(1.f);
	std::cout << std::endl;
	unsigned char color[3];
	std::fstream file("output.ppm", std::fstream::binary | std::fstream::out);
	std::string description = std::format("P6\n{} {}\n255\n", scene->WIDTH, scene->HEIGHT);
	file.write(description.c_str(), description.size());
	description.resize(3);
	for (auto i = 0; i < scene->HEIGHT * scene->WIDTH; ++i) {
		
		color[0] = static_cast<unsigned char>(255 * std::pow(Utilities::clamp(0.0f, 1.0f, frame_buffer[i].x), 0.6f));
		color[1] = static_cast<unsigned char>(255 * std::pow(Utilities::clamp(0.0f, 1.0f, frame_buffer[i].y), 0.6f));
		color[2] = static_cast<unsigned char>(255 * std::pow(Utilities::clamp(0.0f, 1.0f, frame_buffer[i].z), 0.6f));

		file.write(reinterpret_cast<const char*>(color), 3 * sizeof(unsigned char));
	}
}

