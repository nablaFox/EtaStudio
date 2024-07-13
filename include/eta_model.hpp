#pragma once

#include "eta_asset.hpp"
#include "eta_device.hpp"

#include <fastgltf/types.hpp>

namespace eta {

class EtaMeshAsset;
class EtaMaterial;
class EtaTextureAsset;

class EtaModelAsset : public EtaAsset {

public:
	using EtaAsset::EtaAsset;

	void load() override;

	void destroy() override {
		m_device.destroyBuffer(m_metallicMaterialBuffer);
		m_descriptorAllocator.destroyPoll(m_device);
	}

	void setModelPath(str path) { m_modelPath = path; }

	struct Node {
		std::weak_ptr<Node> parent;
		std::vector<std::shared_ptr<Node>> children;
		std::weak_ptr<EtaMeshAsset> mesh;
		std::weak_ptr<EtaMaterial> material;
		glm::mat4 localTransform;
	};

private:
	std::vector<std::string> m_meshes;
	std::vector<std::string> m_materials;
	std::vector<std::string> m_textures;

	std::shared_ptr<Node> m_rootNode;

	EtaDescriptorAllocator m_descriptorAllocator;
	AllocatedBuffer m_metallicMaterialBuffer;
	// AllocatedBuffer m_specularGlossinessMaterialBuffer;

	std::filesystem::path m_modelPath;
	std::string m_modelName;

	// TODO: move this to a utility class
	std::string getTextureName(size_t index);
	std::string getMaterialName(fastgltf::Material& material);
	std::string getMeshName(fastgltf::Mesh& mesh);

	static void loadImageDataFromURI(fastgltf::sources::URI& filePath, VkExtent3D* extent, void*& data);
	static void loadImageDataFromVector(fastgltf::sources::Vector& vector, VkExtent3D* extent, void*& data);
	static void loadImageDataFromBuffer(fastgltf::sources::BufferView& bufferView, fastgltf::Asset& asset, VkExtent3D* extent,
										void*& data);
};

}; // namespace eta
