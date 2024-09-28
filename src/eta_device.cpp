#include "../include/eta_descriptor.hpp"
#include "../include/eta_device.hpp"
#include "../include/eta_window.hpp"
#include "../include/eta_vulkan_init.hpp"
#include "../include/eta_vulkan_images.hpp"

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

using namespace eta;

VkResult EtaDevice::init(EtaWindow& window) {
	vkb::InstanceBuilder builder;

	auto instRet = builder.set_app_name("Vulkan Engine")
					   .request_validation_layers(ENABLE_VALIDATION_LAYERS)
					   .use_default_debug_messenger()
					   .require_api_version(1, 3, 0)
					   .build();

	vkb::Instance vkbInst = instRet.value();

	m_vkInstance = vkbInst.instance;
	m_debugMessenger = vkbInst.debug_messenger;

	// physical and logical devices
	VK_RETURN(glfwCreateWindowSurface(m_vkInstance, window.getWindow(), nullptr, &m_surface));

	VkPhysicalDeviceVulkan13Features features13{};
	features13.dynamicRendering = true;
	features13.synchronization2 = true;

	VkPhysicalDeviceVulkan12Features features12{};
	features12.bufferDeviceAddress = true;
	features12.descriptorIndexing = true;

	VkPhysicalDeviceFeatures features{};
	features.shaderStorageImageMultisample = true;

	vkb::PhysicalDeviceSelector selector{vkbInst};
	vkb::PhysicalDevice phyisicalDevice = selector.set_minimum_version(1, 3)
											  .set_required_features_13(features13)
											  .set_required_features_12(features12)
											  .set_required_features(features)
											  .set_surface(m_surface)
											  .select()
											  .value();

	vkb::DeviceBuilder deviceBuilder{phyisicalDevice};
	vkb::Device vkbDevice = deviceBuilder.build().value();

	m_chosenGPU = vkbDevice.physical_device;
	m_device = vkbDevice.device;

	m_graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
	m_graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

	VmaAllocatorCreateInfo allocatorInfo = {
		.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
		.physicalDevice = m_chosenGPU,
		.device = m_device,
		.instance = m_vkInstance,
	};

	VK_RETURN(vmaCreateAllocator(&allocatorInfo, &m_allocator));

	VK_RETURN(createCommandPool(&m_immData._commandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT));
	VK_RETURN(allocateCommandBuffer(&m_immData._commandBuffer, m_immData._commandPool));
	VK_RETURN(createFence(&m_immData._fence, VK_FENCE_CREATE_SIGNALED_BIT));

	std::vector<EtaDescriptorAllocator::PoolSizeRatio> frameSizes = {
		{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 3},
		{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3},
		{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 3},
		{VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4},
	};

	for (uint32_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		VK_RETURN(createCommandPool(&m_frames[i]._commandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT));
		VK_RETURN(allocateCommandBuffer(&m_frames[i]._commandBuffer, m_frames[i]._commandPool));

		VK_RETURN(createSemaphore(&m_frames[i]._swapchainSemaphore));
		VK_RETURN(createSemaphore(&m_frames[i]._renderSemaphore));
		VK_RETURN(createFence(&m_frames[i]._renderFence, VK_FENCE_CREATE_SIGNALED_BIT));
	}

	m_globalDescriptorAllocator = std::make_shared<EtaDescriptorAllocator>();
	VK_RETURN(m_globalDescriptorAllocator->initPool(m_device, 100, frameSizes));

	m_graphicsPushConstantRange = {
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.offset = 0,
		.size = sizeof(DrawPushConstants),
	};

	m_swapchain.init(window.getExtent(), VK_FORMAT_B8G8R8A8_UNORM);

	// get max number of samples
	VkPhysicalDeviceProperties properties;
	vkGetPhysicalDeviceProperties(m_chosenGPU, &properties);

	VkSampleCountFlags counts =
		properties.limits.framebufferColorSampleCounts & properties.limits.framebufferDepthSampleCounts;

	if (counts & VK_SAMPLE_COUNT_64_BIT)
		m_msaaSamples = VK_SAMPLE_COUNT_64_BIT;
	else if (counts & VK_SAMPLE_COUNT_32_BIT)
		m_msaaSamples = VK_SAMPLE_COUNT_32_BIT;
	else if (counts & VK_SAMPLE_COUNT_16_BIT)
		m_msaaSamples = VK_SAMPLE_COUNT_16_BIT;
	else if (counts & VK_SAMPLE_COUNT_8_BIT)
		m_msaaSamples = VK_SAMPLE_COUNT_8_BIT;
	else if (counts & VK_SAMPLE_COUNT_4_BIT)
		m_msaaSamples = VK_SAMPLE_COUNT_4_BIT;
	else if (counts & VK_SAMPLE_COUNT_2_BIT)
		m_msaaSamples = VK_SAMPLE_COUNT_2_BIT;
	else
		m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	VK_RETURN(createDrawImage(window.getExtent(), m_drawImage, m_msaaSamples));

	VK_RETURN(createDrawImage(window.getExtent(), m_resolveImage, VK_SAMPLE_COUNT_1_BIT));

	VK_RETURN(createDepthImage(window.getExtent(), m_depthImage));

	m_deletionQueue.push_function([this] {
		m_globalDescriptorAllocator->resetDescriptorPool(*this);
		m_globalDescriptorAllocator->destroyPoll(*this);
		destroyImage(m_drawImage);
		destroyImage(m_resolveImage);
		destroyImage(m_depthImage);
		vmaDestroyAllocator(m_allocator);
	});

	return VK_SUCCESS;
}

void EtaDevice::destroy() {
	for (auto& [name, pipeline] : m_pipelines) {
		pipeline->destroy(m_device);
	}

	m_deletionQueue.flush();
	m_swapchain.destroy();
	vkDestroySurfaceKHR(m_vkInstance, m_surface, nullptr);
	vkDestroyDevice(m_device, nullptr);
	vkb::destroy_debug_utils_messenger(m_vkInstance, m_debugMessenger);
	vkDestroyInstance(m_vkInstance, nullptr);
}

VkResult EtaDevice::uploadMesh(std::span<Vertex> vertices, std::span<Index> indices, GPUMeshData& meshData) {
	const size_t vertexBufferSize = vertices.size() * sizeof(Vertex);
	const size_t indexBufferSize = indices.size() * sizeof(uint32_t);

	VK_RETURN(createBuffer(indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
						   VMA_MEMORY_USAGE_GPU_ONLY, meshData.indexBuffer));

	VK_RETURN(createBuffer(vertexBufferSize,
						   VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT |
							   VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
						   VMA_MEMORY_USAGE_GPU_ONLY, meshData.vertexBuffer));

	VkBufferDeviceAddressInfo deviceAdressInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO,
		.buffer = meshData.vertexBuffer.buffer,
	};

	meshData.indexCount = indices.size();
	meshData.vertexBufferAddress = vkGetBufferDeviceAddress(m_device, &deviceAdressInfo);

	return updateMesh(vertices, indices, meshData);
}

VkResult EtaDevice::updateMesh(std::span<Vertex> vertices, std::span<Index> indices, GPUMeshData& meshData) {
	const size_t vertexBufferSize = vertices.size() * sizeof(Vertex);
	const size_t indexBufferSize = indices.size() * sizeof(uint32_t);
	meshData.indexCount = indices.size();

	AllocatedBuffer staging;
	void* data;

	VK_RETURN(createStagingBuffer(vertexBufferSize + indexBufferSize, staging, data));

	memcpy(data, vertices.data(), vertexBufferSize);
	memcpy((char*)data + vertexBufferSize, indices.data(), indexBufferSize);

	immediateSubmit([&](VkCommandBuffer cmd) {
		VkBufferCopy vertexCopy{
			.srcOffset = 0,
			.dstOffset = 0,
			.size = vertexBufferSize,
		};

		VkBufferCopy indexCopy{
			.srcOffset = vertexBufferSize,
			.dstOffset = 0,
			.size = indexBufferSize,
		};

		vkCmdCopyBuffer(cmd, staging.buffer, meshData.vertexBuffer.buffer, 1, &vertexCopy);
		vkCmdCopyBuffer(cmd, staging.buffer, meshData.indexBuffer.buffer, 1, &indexCopy);
	});

	return destroyBuffer(staging);
}

VkResult EtaDevice::createUBO(size_t allocSize, AllocatedBuffer& uboBuffer) {
	return createBuffer(allocSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, uboBuffer);
}

VkResult EtaDevice::createBuffer(size_t allocSize,
								 VkBufferUsageFlags usage,
								 VmaMemoryUsage memoryUsage,
								 AllocatedBuffer& buffer) {
	VkBufferCreateInfo bufferInfo = {
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.pNext = nullptr,
		.size = allocSize,
		.usage = usage,
	};

	VmaAllocationCreateInfo vmaallocInfo = {
		.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT,
		.usage = memoryUsage,
	};

	VK_RETURN(vmaCreateBuffer(m_allocator, &bufferInfo, &vmaallocInfo, &buffer.buffer, &buffer.allocation, nullptr));

	buffer.info.size = allocSize;
	buffer.info.offset = 0;

	return VK_SUCCESS;
}

VkResult EtaDevice::fillBuffer(AllocatedBuffer& buffer, void* data, size_t size, size_t offset) {
	void* mappedData;
	VK_RETURN(vmaMapMemory(m_allocator, buffer.allocation, &mappedData));
	memcpy((char*)mappedData + offset, data, size);
	vmaUnmapMemory(m_allocator, buffer.allocation);
	return VK_SUCCESS;
}

VkResult EtaDevice::createStagingBuffer(size_t allocSize, AllocatedBuffer& stagingBuffer, void*& data) {
	VK_RETURN(createBuffer(allocSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer));
	data = stagingBuffer.allocation->GetMappedData();
	return VK_SUCCESS;
}

VkResult EtaDevice::destroyBuffer(AllocatedBuffer& buffer) {
	vmaDestroyBuffer(m_allocator, buffer.buffer, buffer.allocation);
	return VK_SUCCESS;
}

VkResult EtaDevice::destroyMesh(GPUMeshData& meshData) {
	destroyBuffer(meshData.vertexBuffer);
	destroyBuffer(meshData.indexBuffer);
	return VK_SUCCESS;
}

VkResult EtaDevice::createCommandPool(VkCommandPool* pool, VkCommandPoolCreateFlags flags) {
	auto commandPoolCreateInfo = etainit::commandPoolCreateInfo(m_graphicsQueueFamily, flags);
	m_deletionQueue.push_function([this, pool] { vkDestroyCommandPool(m_device, *pool, nullptr); });
	return vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, pool);
}

VkResult EtaDevice::allocateCommandBuffer(VkCommandBuffer* buffer, VkCommandPool pool) {
	auto cmdAllocInfo = etainit::commandBufferAllocateInfo(pool);
	return vkAllocateCommandBuffers(m_device, &cmdAllocInfo, buffer);
}

VkResult EtaDevice::createSemaphore(VkSemaphore* semaphore, VkSemaphoreCreateFlags flags) {
	auto semaphoreCreateInfo = etainit::semaphoreCreateInfo(flags);
	m_deletionQueue.push_function([this, semaphore] { vkDestroySemaphore(m_device, *semaphore, nullptr); });
	return vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, semaphore);
}

VkResult EtaDevice::createFence(VkFence* fence, VkFenceCreateFlags flags) {
	auto fenceCreateInfo = etainit::fenceCreateInfo(flags);
	m_deletionQueue.push_function([this, fence] { vkDestroyFence(m_device, *fence, nullptr); });
	return vkCreateFence(m_device, &fenceCreateInfo, nullptr, fence);
}

VkResult EtaDevice::createShaderModule(std::vector<uint32_t>& code, VkShaderModule& shaderModule) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.codeSize = code.size() * sizeof(uint32_t);
	createInfo.pCode = code.data();

	return vkCreateShaderModule(m_device, &createInfo, nullptr, &shaderModule);
}

VkResult EtaDevice::destroyShaderModule(VkShaderModule& shaderModule) {
	vkDestroyShaderModule(m_device, shaderModule, nullptr);
	return VK_SUCCESS;
}

VkResult EtaDevice::submitCommand(int submitCount, VkSubmitInfo2* submitInfo, VkFence fence) {
	VK_RETURN(vkQueueSubmit2(m_graphicsQueue, submitCount, submitInfo, fence));
	return VK_SUCCESS;
}

VkResult EtaDevice::immediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function) {
	VkCommandBuffer cmd = m_immData._commandBuffer;

	ETA_CHECK(vkResetFences(m_device, 1, &m_immData._fence));
	ETA_CHECK(vkResetCommandBuffer(cmd, 0));

	VkCommandBufferBeginInfo cmdBeginInfo =
		etainit::commandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	ETA_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

	function(cmd);

	ETA_CHECK(vkEndCommandBuffer(cmd));

	VkCommandBufferSubmitInfo cmdinfo = etainit::commandBufferSubmitInfo(cmd);
	VkSubmitInfo2 submit = etainit::submitInfo(&cmdinfo, nullptr, nullptr);

	ETA_CHECK(submitCommand(1, &submit, m_immData._fence));
	ETA_CHECK(vkWaitForFences(m_device, 1, &m_immData._fence, true, 9999999999));

	return VK_SUCCESS;
}

VkResult EtaDevice::createImage(VkExtent3D size,
								VkFormat format,
								VkImageUsageFlags usage,
								AllocatedImage& handle,
								VkSampleCountFlagBits numSamples,
								bool mipmapped) {
	handle.imageFormat = format;
	handle.imageExtent = size;

	VkImageCreateInfo imgInfo = etainit::imageCreateInfo(format, usage, size, numSamples);

	if (mipmapped)
		imgInfo.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(size.width, size.height)))) + 1;

	VmaAllocationCreateInfo allocInfo = {
		.usage = VMA_MEMORY_USAGE_GPU_ONLY,
		.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
	};

	VK_RETURN(vmaCreateImage(m_allocator, &imgInfo, &allocInfo, &handle.image, &handle.allocation, nullptr));

	VkImageAspectFlags aspectFlag =
		format == VK_FORMAT_D32_SFLOAT ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
	VkImageViewCreateInfo viewInfo = etainit::imageViewCreateInfo(format, handle.image, aspectFlag);
	viewInfo.subresourceRange.levelCount = imgInfo.mipLevels;

	return vkCreateImageView(m_device, &viewInfo, nullptr, &handle.imageView);
}

VkResult EtaDevice::fillImage(AllocatedImage& image, void* data) {
	size_t imageSize = image.imageExtent.width * image.imageExtent.height * image.imageExtent.depth * 4;

	void* mappedData;
	AllocatedBuffer stagingBuffer;
	VK_RETURN(createStagingBuffer(imageSize, stagingBuffer, mappedData));

	memcpy(mappedData, data, imageSize);

	immediateSubmit([&](VkCommandBuffer cmd) {
		etautil::makeCopyable(cmd, image);

		VkBufferImageCopy copyRegion = {
			.bufferOffset = 0,
			.bufferRowLength = 0,
			.bufferImageHeight = 0,
		};

		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.mipLevel = 0;
		copyRegion.imageSubresource.baseArrayLayer = 0;
		copyRegion.imageSubresource.layerCount = 1;
		copyRegion.imageExtent = image.imageExtent;

		vkCmdCopyBufferToImage(cmd, stagingBuffer.buffer, image.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
							   &copyRegion);

		etautil::makeReadable(cmd, image);
	});

	return destroyBuffer(stagingBuffer);
}

VkResult EtaDevice::createFilledImage(AllocatedImage& image,
									  void* data,
									  VkExtent3D size,
									  VkFormat format,
									  VkImageUsageFlags usage) {
	VK_RETURN(
		createImage(size, format, usage | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, image));
	return fillImage(image, data);
}

VkResult EtaDevice::createDrawImage(VkExtent2D extent, AllocatedImage& image, VkSampleCountFlagBits numSamples) {
	VkExtent3D drawImageExtent = {
		extent.width,
		extent.height,
		1,
	};

	image.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
	image.imageExtent = drawImageExtent;

	VkImageUsageFlags drawImageUsages{};
	drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
	drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	return createImage(drawImageExtent, image.imageFormat, drawImageUsages, image, numSamples);
}

VkResult EtaDevice::createDepthImage(VkExtent2D extent, AllocatedImage& image) {
	return createImage({extent.width, extent.height, 1}, VK_FORMAT_D32_SFLOAT,
					   VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, image, m_msaaSamples);
}

VkResult EtaDevice::createSampler(VkSampler* sampler, VkSamplerCreateInfo* info) {
	return vkCreateSampler(m_device, info, nullptr, sampler);
}

VkResult EtaDevice::destroyImage(AllocatedImage& handle) {
	vkDestroyImageView(m_device, handle.imageView, nullptr);
	vmaDestroyImage(m_allocator, handle.image, handle.allocation);
	return VK_SUCCESS;
}

VkResult EtaDevice::destroySampler(VkSampler& sampler) {
	vkDestroySampler(m_device, sampler, nullptr);
	return VK_SUCCESS;
}

VkResult EtaDevice::startFrame(glm::vec4 viewport, glm::vec4 clearColor) {
	VK_RETURN(vkWaitForFences(m_device, 1, &getCurrentFrame()._renderFence, true, 1000000000));
	VK_RETURN(vkResetFences(m_device, 1, &getCurrentFrame()._renderFence));

	getCurrentFrame()._deletionQueue.flush();

	m_swapchain.acquireImage(getCurrentFrame()._swapchainSemaphore);

	VK_RETURN(vkResetCommandBuffer(currentCmd(), 0));

	m_drawExtent.width = m_drawImage.imageExtent.width;
	m_drawExtent.height = m_drawImage.imageExtent.height;

	VkCommandBufferBeginInfo cmdBeginInfo =
		etainit::commandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	VK_RETURN(vkBeginCommandBuffer(currentCmd(), &cmdBeginInfo));

	etautil::setViewport(currentCmd(), viewport);
	etautil::setScissor(currentCmd(), m_drawExtent);

	etautil::makeColorWriteable(currentCmd(), m_drawImage);
	VkRenderingAttachmentInfo colorAttachment =
		etainit::attachmentInfo(m_drawImage.imageView, nullptr, m_drawImage.currentLayout);

	etautil::makeDepthWriteable(currentCmd(), m_depthImage);
	VkRenderingAttachmentInfo depthAttachment =
		etainit::depthAttachmentInfo(m_depthImage.imageView, m_depthImage.currentLayout);

	VkRenderingInfo renderInfo = etainit::renderingInfo(m_drawExtent, &colorAttachment, &depthAttachment);

	vkCmdBeginRendering(currentCmd(), &renderInfo);

	return VK_SUCCESS;
}

VkResult EtaDevice::endFrame() {
	vkCmdEndRendering(currentCmd());

	etautil::resolveMultisampledImage(currentCmd(), m_drawImage, m_resolveImage, m_drawExtent);

	etautil::copyImageToImage(currentCmd(), m_resolveImage, m_swapchain.getCurrentImage());

	etautil::makePresentable(currentCmd(), m_swapchain.getCurrentImage());

	VK_RETURN(vkEndCommandBuffer(currentCmd()));

	VkSemaphoreSubmitInfo waitInfo = etainit::semaphoreSubmitInfo(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR,
																  getCurrentFrame()._swapchainSemaphore);
	VkSemaphoreSubmitInfo signalInfo =
		etainit::semaphoreSubmitInfo(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, getCurrentFrame()._renderSemaphore);

	VkCommandBufferSubmitInfo cmdinfo = etainit::commandBufferSubmitInfo(currentCmd());
	VkSubmitInfo2 submitInfo = etainit::submitInfo(&cmdinfo, &signalInfo, &waitInfo);

	submitCommand(1, &submitInfo, getCurrentFrame()._renderFence);
	m_swapchain.presentOnScreen(getCurrentFrame()._renderSemaphore);

	m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

	return VK_SUCCESS;
}
