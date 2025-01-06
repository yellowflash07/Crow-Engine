#pragma once


#include <string>
#include <vulkan/vulkan_core.h>
#include <stdexcept>

#include "Common.h"


struct Texture
{
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager();
	VkCommandPool commandPool;
	VkQueue graphicsQueue;
	VkDevice device;
	VkPhysicalDevice physicalDevice;
	VkSampler textureSampler;
	VkImageView textureImageView;
	bool LoadTexture(std::string filePath);
	void CreateTextureImageView();
	void CreateTextureSampler();
private:
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldImageLayout, VkImageLayout newImageLayout);
	Texture* texture;
	//std::map<std::string, Texture*> mapTexToName
};

