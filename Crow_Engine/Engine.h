#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <string>
#include <vector>
#include  <vulkan/vulkan_core.h>
#include <optional>

struct QueueFamilyIndices 
{
	std::optional<uint32_t> graphicsFamily;
};

class Engine
{
public:
	Engine();
	~Engine();
	void Init();
	void Run();
	void Update();
	void Shutdown();
private:
	GLFWwindow* window;
	VkInstance instance;
	std::string appName = "No Name";
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	void CreateLogicalDevice();
	bool CheckValidationLayerSupport();
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	VkQueue graphicsQueue;

	const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
};

