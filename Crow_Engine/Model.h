#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <array>
#include <vector>
#include <stdexcept>
#include "Common.h"

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
	glm::vec2 uv;

    static VkVertexInputBindingDescription GetBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		// Position attribute
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		// Color attribute
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, uv);

		return attributeDescriptions;
	}
};

struct Transform {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Model
{
public:
	Model(VkCommandPool commandPool, VkQueue graphicsQueue);
	~Model();
	Transform transform{};
	void Bind(VkCommandBuffer& commandBuffer);
	void Draw(VkCommandBuffer& commandBuffer);
	void CreateVertexBuffer(std::vector<Vertex> vertices, VkDevice device, VkPhysicalDevice physicalDevice);
	void CreateIndexBuffer(std::vector<uint32_t> indices, VkDevice device, VkPhysicalDevice physicalDevice);
private:

	std::vector<uint32_t> indices;
	//void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice device, VkPhysicalDevice physicalDevice);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
	VkCommandPool commandPool;
	VkQueue graphicsQueue;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	uint32_t vertexCount;
	uint32_t indexCount;

	VkDevice device;
	VkPhysicalDevice physicalDevice;


	//void UpdateUniformBuffers(double time);

	std::vector<Vertex> vertices;
	//uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice device);
};

