#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <span>
#include <deque>
#include <functional>

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>

#include <fmt/core.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

using str = const std::string&;

struct VulkanImage {
	VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkImage image;
	VkImageView imageView;
};

struct AllocatedImage : VulkanImage {
	VkImage image;
	VmaAllocation allocation;
	VkExtent3D imageExtent;
};

struct AllocatedBuffer {
	VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationInfo info;
};

#define VK_RETURN(x)                                                                                                             \
	{                                                                                                                            \
		VkResult err = x;                                                                                                        \
		if (err != VK_SUCCESS)                                                                                                   \
			return err;                                                                                                          \
	}

#define ETA_CHECK(x)                                                                                                             \
	do {                                                                                                                         \
		VkResult err = x;                                                                                                        \
		if (err) {                                                                                                               \
			fmt::print("Detected Vulkan Error: {}", string_VkResult(err));                                                       \
			exit(err);                                                                                                           \
		}                                                                                                                        \
	} while (0)
