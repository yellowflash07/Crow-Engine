#include "Engine.h"

Engine::Engine()
{

}

Engine::~Engine()
{
    //delete window;
}

void Engine::Init()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(800, 600, "Crow Engine", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    if (enableValidationLayers && !CheckValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    std::cout << extensionCount << " extensions supported\n";

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Crow Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    if (enableValidationLayers) 
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }

    std::cout << "Vulkan instance created\n";

    PickPhysicalDevice();


}

void Engine::Run()
{
    Init();
    Update();
    Shutdown();
}

void Engine::Update()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void Engine::Shutdown()
{
    vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

    glfwTerminate();
}

void Engine::CreateLogicalDevice()
{

}

bool Engine::CheckValidationLayerSupport()
{
    uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers)
    {
		bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
				layerFound = true;
				break;
			}
		}

        if (!layerFound)
        {
			return false;
		}
	}

    return true;

}


void Engine::PickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::cout << deviceCount << " devices found\n";


    for (const auto& device : devices)
    {
        if (IsDeviceSuitable(device))
        {
			physicalDevice = device;
			break;
		}
	}

    if (physicalDevice == VK_NULL_HANDLE)
    {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool Engine::IsDeviceSuitable(VkPhysicalDevice device)
{
    	//return true;
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
    
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	bool isSuitable =  deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;

    if (isSuitable)
    {
		std::cout << "Device picked: " << deviceProperties.deviceName << std::endl;
	}
    QueueFamilyIndices indices = FindQueueFamilies(device);

    return indices.graphicsFamily.has_value();
  //  return isSuitable;
}

QueueFamilyIndices Engine::FindQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    // Logic to find queue family indices to populate struct with
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        i++;
    }
    

    return indices;
}
