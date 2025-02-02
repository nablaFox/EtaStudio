#pragma once

#include "eta_pch.hpp"
#include "eta_vulkan_init.hpp"

namespace etautil {

inline void transitionImage(VkCommandBuffer cmd, VkImage image, VkImageLayout currentLayout, VkImageLayout newLayout) {
	VkImageMemoryBarrier2 imageBarrier{.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2};
	imageBarrier.pNext = nullptr;

	imageBarrier.srcStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.srcAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT;
	imageBarrier.dstStageMask = VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT;
	imageBarrier.dstAccessMask = VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT;

	imageBarrier.oldLayout = currentLayout;
	imageBarrier.newLayout = newLayout;

	VkImageAspectFlags aspectMask =
		(newLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
	imageBarrier.subresourceRange = etainit::imageSubResourceRange(aspectMask);
	imageBarrier.image = image;

	VkDependencyInfo depInfo{};
	depInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
	depInfo.pNext = nullptr;

	depInfo.imageMemoryBarrierCount = 1;
	depInfo.pImageMemoryBarriers = &imageBarrier;

	vkCmdPipelineBarrier2(cmd, &depInfo);
}

inline void copyImageToImage(VkCommandBuffer cmd,
							 VkImage source,
							 VkImage destination,
							 VkExtent3D srcSize,
							 VkExtent3D dstSize) {
	VkImageBlit2 blitRegion{.sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2, .pNext = nullptr};

	blitRegion.srcOffsets[1].x = srcSize.width;
	blitRegion.srcOffsets[1].y = srcSize.height;
	blitRegion.srcOffsets[1].z = 1;

	blitRegion.dstOffsets[1].x = dstSize.width;
	blitRegion.dstOffsets[1].y = dstSize.height;
	blitRegion.dstOffsets[1].z = 1;

	blitRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blitRegion.srcSubresource.baseArrayLayer = 0;
	blitRegion.srcSubresource.layerCount = 1;
	blitRegion.srcSubresource.mipLevel = 0;

	blitRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	blitRegion.dstSubresource.baseArrayLayer = 0;
	blitRegion.dstSubresource.layerCount = 1;
	blitRegion.dstSubresource.mipLevel = 0;

	VkBlitImageInfo2 blitInfo{.sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr};
	blitInfo.dstImage = destination;
	blitInfo.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	blitInfo.srcImage = source;
	blitInfo.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	blitInfo.filter = VK_FILTER_LINEAR;
	blitInfo.regionCount = 1;
	blitInfo.pRegions = &blitRegion;

	vkCmdBlitImage2(cmd, &blitInfo);
}

inline void makeWriteable(VkCommandBuffer cmd, VulkanImage& image) {
	transitionImage(cmd, image.image, image.currentLayout, VK_IMAGE_LAYOUT_GENERAL);
	image.currentLayout = VK_IMAGE_LAYOUT_GENERAL;
}

inline void makeColorWriteable(VkCommandBuffer buffer, VulkanImage& image) {
	transitionImage(buffer, image.image, image.currentLayout, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	image.currentLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
}

inline void makeDepthWriteable(VkCommandBuffer buffer, VulkanImage& image) {
	transitionImage(buffer, image.image, image.currentLayout, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
	image.currentLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
}

inline void makePresentable(VkCommandBuffer cmd, VulkanImage& image) {
	transitionImage(cmd, image.image, image.currentLayout, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	image.currentLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
}

inline void makeReadable(VkCommandBuffer cmd, VulkanImage& image) {
	transitionImage(cmd, image.image, image.currentLayout, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	image.currentLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

inline void makeTransferable(VkCommandBuffer cmd, VulkanImage& image) {
	transitionImage(cmd, image.image, image.currentLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	image.currentLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
}

inline void makeCopyable(VkCommandBuffer cmd, VulkanImage& image) {
	transitionImage(cmd, image.image, image.currentLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	image.currentLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
}

inline void copyImageToImage(VkCommandBuffer cmd, VulkanImage& src, VulkanImage& dst) {
	makeTransferable(cmd, src);
	makeCopyable(cmd, dst);

	copyImageToImage(cmd, src.image, dst.image, src.imageExtent, dst.imageExtent);
}

inline void resolveMultisampledImage(VkCommandBuffer cmd, VulkanImage& msaSrc, VulkanImage& resDst, VkExtent2D extent) {
	makeTransferable(cmd, msaSrc);

	makeCopyable(cmd, resDst);

	VkImageResolve2 resolveRegion{
		.sType = VK_STRUCTURE_TYPE_IMAGE_RESOLVE_2,
		.pNext = nullptr,
		.extent = {.width = extent.width, .height = extent.height, .depth = 1},
	};

	resolveRegion.srcSubresource = {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.mipLevel = 0,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};

	resolveRegion.dstSubresource = {
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.mipLevel = 0,
		.baseArrayLayer = 0,
		.layerCount = 1,
	};

	VkResolveImageInfo2 resolveInfo{
		.sType = VK_STRUCTURE_TYPE_RESOLVE_IMAGE_INFO_2,
		.pNext = nullptr,
		.srcImage = msaSrc.image,
		.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		.dstImage = resDst.image,
		.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.regionCount = 1,
		.pRegions = &resolveRegion,
	};

	vkCmdResolveImage2(cmd, &resolveInfo);
}

inline void setViewport(VkCommandBuffer cmd, glm::vec4 view, float minDepth = 0.0f, float maxDepth = 1.0f) {
	VkViewport viewport = {
		.x = view.x,
		.y = view.y,
		.width = view.z,
		.height = view.w,
		.minDepth = minDepth,
		.maxDepth = maxDepth,
	};

	vkCmdSetViewport(cmd, 0, 1, &viewport);
}

inline void setScissor(VkCommandBuffer cmd, VkExtent2D extent, int x = 0, int y = 0) {
	VkRect2D scissor = {};
	scissor.offset.x = x;
	scissor.offset.y = y;
	scissor.extent.width = extent.width;
	scissor.extent.height = extent.height;

	vkCmdSetScissor(cmd, 0, 1, &scissor);
}

inline void setClearColor(glm::vec4 color,
						  VkAttachmentLoadOp loadOp,
						  VkAttachmentStoreOp storeOp,
						  VkRenderingAttachmentInfo* attachment) {
	attachment->loadOp = loadOp;
	attachment->storeOp = storeOp;
	attachment->clearValue = {{color.r, color.g, color.b, color.a}};
}

inline void setClearDepth(VkClearDepthStencilValue clearValue,
						  VkAttachmentLoadOp loadOp,
						  VkAttachmentStoreOp storeOp,
						  VkRenderingAttachmentInfo* attachment) {
	attachment->loadOp = loadOp;
	attachment->storeOp = storeOp;
	attachment->clearValue = {1.0f, 0};
}

}  // namespace etautil
