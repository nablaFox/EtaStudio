#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <span>
#include <deque>
#include <functional>
#include <fstream>

#include <VkBootstrap.h>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vk_mem_alloc.h>

#include <fmt/core.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

using str = const std::string&;

using Index = uint32_t;

struct Vertex {
	glm::vec3 position;
	float uv_x;
	glm::vec3 normal;
	float uv_y;
	glm::vec4 color;
};

struct VulkanImage {
	VkImageLayout currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	VkImage image;
	VkImageView imageView;
};

struct AllocatedImage : VulkanImage {
	VmaAllocation allocation;
	VkExtent3D imageExtent;
	VkFormat imageFormat;
};

struct AllocatedBuffer {
	VkBuffer buffer;
	VmaAllocation allocation;
	VmaAllocationInfo info = {
		.offset = 0,
		.size = 0,
	};
};

struct GPUMeshData {
	AllocatedBuffer indexBuffer;
	AllocatedBuffer vertexBuffer;
	VkDeviceAddress vertexBufferAddress;
	size_t indexCount;
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

#define ETA_CHECK_MSG(x, msg)                                                                                                    \
	do {                                                                                                                         \
		VkResult err = x;                                                                                                        \
		if (err) {                                                                                                               \
			fmt::print("Detected Error: {} - {}", string_VkResult(err), msg);                                                    \
			exit(err);                                                                                                           \
		}                                                                                                                        \
	} while (0)
