#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <inttypes.h>
#include <stdint.h>

#include <fmt/core.h>

#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>
#include <fastgltf/tools.hpp>

#include "../include/eta_asset_manager.hpp"
#include "../include/eta_model.hpp"
#include "../include/eta_default_materials.hpp"

using namespace eta;

using byte = unsigned char*;

void EtaModelAsset::loadImageDataFromURI(fastgltf::sources::URI& filePath, VkExtent3D* extent, void*& data) {
	assert(filePath.fileByteOffset == 0);
	assert(filePath.uri.isLocalPath());

	int width, height, nrChannels;

	const std::string path(filePath.uri.path().begin(), filePath.uri.path().end());
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 4);

	if (data) {
		fmt::print("Failed to load texture from path: {}\n", path);
		return;
	}

	extent->width = width;
	extent->height = height;
	extent->depth = 1;
}

void EtaModelAsset::loadImageDataFromVector(fastgltf::sources::Vector& vector, VkExtent3D* extent, void*& data) {
	int width, height, nrChannels;

	data = stbi_load_from_memory(reinterpret_cast<byte>(vector.bytes.data()), static_cast<int>(vector.bytes.size()), &width,
								 &height, &nrChannels, 4);

	if (!data) {
		fmt::print("Failed to load texture from vector\n");
		return;
	}

	extent->width = width;
	extent->height = height;
	extent->depth = 1;
}

void EtaModelAsset::loadImageDataFromBuffer(fastgltf::sources::BufferView& view, fastgltf::Asset& asset, VkExtent3D* extent,
											void*& data) {
	auto& bufferView = asset.bufferViews[view.bufferViewIndex];
	auto& buffer = asset.buffers[bufferView.bufferIndex];

	int width, height, nrChannels;

	std::visit(fastgltf::visitor{
				   [](auto& arg) {},
				   [&](fastgltf::sources::Array& array) {
					   data = stbi_load_from_memory(reinterpret_cast<byte>(array.bytes.data() + bufferView.byteOffset),
													static_cast<int>(bufferView.byteLength), &width, &height, &nrChannels, 4);
				   },
				   [&](fastgltf::sources::Vector& vector) {
					   data = stbi_load_from_memory(reinterpret_cast<byte>(vector.bytes.data() + bufferView.byteOffset),
													static_cast<int>(bufferView.byteLength), &width, &height, &nrChannels, 4);
				   }},
			   buffer.data);

	if (data == nullptr) {
		fmt::print("Failed to load texture from buffer\n");
		return;
	}

	extent->width = width;
	extent->height = height;
	extent->depth = 1;
}

std::string EtaModelAsset::getTextureName(size_t index) { return m_modelName + "_texture_" + std::to_string(index); }

std::string EtaModelAsset::getMaterialName(fastgltf::Material& material) {
	return m_modelName + "_material_" + material.name.data();
}

std::string EtaModelAsset::getMeshName(fastgltf::Mesh& mesh) { return m_modelName + "_mesh_" + mesh.name.data(); }

void EtaModelAsset::load() {
	fastgltf::Parser parser;

	auto data = fastgltf::GltfDataBuffer::FromPath(m_modelPath);
	m_modelName = m_modelPath.stem().string();

	if (data.error() != fastgltf::Error::None) {
		fmt::print("Failed to load model from path: {}\n", m_modelPath.c_str());
		return;
	}

	auto model = parser.loadGltfBinary(data.get(), m_modelPath, fastgltf::Options::None);

	if (model.error() != fastgltf::Error::None) {
		fmt::print("Failed to parse model from path: {}\n", m_modelPath.c_str());
		return;
	}

	size_t textureIndex = 0;
	for (auto& texture : model->images) {
		const auto& textureName = getTextureName(textureIndex++);
		void* textureData = nullptr;
		VkExtent3D textureExtent;

		std::visit(fastgltf::visitor{
					   [](auto& arg) {},
					   [&](fastgltf::sources::URI& filePath) { loadImageDataFromURI(filePath, &textureExtent, textureData); },
					   [&](fastgltf::sources::Vector& vector) { loadImageDataFromVector(vector, &textureExtent, textureData); },
					   [&](fastgltf::sources::BufferView& view) {
						   loadImageDataFromBuffer(view, model.get(), &textureExtent, textureData);
					   },
				   },
				   texture.data);

		auto textureAsset = m_manager.addAsset<EtaTextureAsset>(textureName);
		textureAsset->textureData = textureData;
		textureAsset->setFormat(VK_FORMAT_R8G8B8A8_UNORM);
		textureAsset->setMagFilter(VK_FILTER_NEAREST);
		textureAsset->setMinFilter(VK_FILTER_NEAREST);
		textureAsset->setExtent(textureExtent.width, textureExtent.height, textureExtent.depth);
		textureAsset->load();
		stbi_image_free(textureData);
	}

	m_device.createUBO(EtaMetallicRoughnessMaterial::size * model->materials.size(), m_metallicMaterialBuffer);

	int dataIndex = 0;

	// for now load only pbr metallic roughness materials; i.e. treat all materials as pbr metallic roughness
	for (auto& material : model->materials) {
		auto materialAsset = m_manager.addAsset<EtaMetallicRoughnessMaterial>(getMaterialName(material));
		materialAsset->setSharedBuffer(m_metallicMaterialBuffer, dataIndex * EtaMetallicRoughnessMaterial::size);
		materialAsset->setup();

		auto colorFactor = material.pbrData.baseColorFactor;
		materialAsset->setBaseColorFactor(colorFactor[0], colorFactor[1], colorFactor[2], colorFactor[3]);

		materialAsset->setRoughnessFactor(material.pbrData.roughnessFactor);

		materialAsset->setMetallicFactor(material.pbrData.metallicFactor);

		if (material.pbrData.metallicRoughnessTexture.has_value()) {
			auto texture = m_manager.getAsset<EtaTextureAsset>(
				getTextureName(material.pbrData.metallicRoughnessTexture.value().textureIndex));
			if (texture)
				materialAsset->setMetallicRoughnessTexture(texture);
		}

		if (material.pbrData.baseColorTexture.has_value()) {
			auto texture =
				m_manager.getAsset<EtaTextureAsset>(getTextureName(material.pbrData.baseColorTexture.value().textureIndex));
			if (texture)
				materialAsset->setBaseColorTexture(texture);
		}

		materialAsset->load();

		++dataIndex;
	}

	// load meshes

	std::vector<Vertex> vertices;
	std::vector<Index> indices;

	for (auto& mesh : model->meshes) {
		auto meshAsset = m_manager.addAsset<EtaMeshAsset>(getMeshName(mesh));

		vertices.clear();
		indices.clear();

		for (auto&& primitive : mesh.primitives) {
			EtaMeshAsset::MeshSurface surface;

			surface.firstIndex = static_cast<uint32_t>(indices.size());
			surface.indexCount = (uint32_t)model->accessors[primitive.indicesAccessor.value()].count;

			size_t firstVertex = vertices.size();

			{
				fastgltf::Accessor& accessor = model->accessors[primitive.indicesAccessor.value()];
				indices.reserve(indices.size() + accessor.count);

				fastgltf::iterateAccessor<std::uint32_t>(model.get(), accessor,
														 [&](std::uint32_t idx) { indices.push_back(idx + firstVertex); });
			}

			{
				fastgltf::Accessor& accessor = model->accessors[primitive.findAttribute("POSITION")->accessorIndex];
				vertices.resize(vertices.size() + accessor.count);

				fastgltf::iterateAccessorWithIndex<fastgltf::math::nvec3>(
					model.get(), accessor, [&](fastgltf::math::nvec3 position, std::size_t idx) {
						Vertex vertex;
						vertex.position = {position.x(), position.y(), position.z()};
						vertex.normal = {1.0f, 0.0f, 0.0f};
						vertex.color = glm::vec4(1.0f);
						vertex.uv_x = 0.0f;
						vertex.uv_y = 0.0f;
						vertices[firstVertex + idx] = vertex;
					});
			}

			auto normals = primitive.findAttribute("NORMAL");
			if (normals != primitive.attributes.end()) {
				fastgltf::Accessor& accessor = model->accessors[normals->accessorIndex];

				fastgltf::iterateAccessorWithIndex<fastgltf::math::nvec3>(
					model.get(), accessor, [&](fastgltf::math::nvec3 normal, std::size_t idx) {
						vertices[firstVertex + idx].normal = {normal.x(), normal.y(), normal.z()};
					});
			}

			auto uv = primitive.findAttribute("TEXCOORD_0");
			if (uv != primitive.attributes.end()) {
				fastgltf::Accessor& accessor = model->accessors[uv->accessorIndex];

				fastgltf::iterateAccessorWithIndex<fastgltf::math::nvec2>(model.get(), accessor,
																		  [&](fastgltf::math::nvec2 uv, std::size_t idx) {
																			  vertices[firstVertex + idx].uv_x = uv.x();
																			  vertices[firstVertex + idx].uv_y = uv.y();
																		  });
			}

			auto colors = primitive.findAttribute("COLOR_0");
			if (colors != primitive.attributes.end()) {
				fastgltf::Accessor& accessor = model->accessors[colors->accessorIndex];

				fastgltf::iterateAccessorWithIndex<fastgltf::math::nvec4>(
					model.get(), accessor, [&](fastgltf::math::nvec4 color, std::size_t idx) {
						vertices[firstVertex + idx].color = {color.x(), color.y(), color.z(), color.w()};
					});
			}

			meshAsset->addSurface(surface);
		}

		meshAsset->setVertices(vertices);
		meshAsset->setIndices(indices);

		meshAsset->load();
	}

	// When doing the setup of the material set the descriptor allocator to that of this
	// class
	// add necessary textures bindings and one buffer binding at index 0 passing the local material buffer

	// load all meshes and register them as assets, then also save them in m_meshes

	// create model graph

	fmt::print("Info: Loaded model {} \n", m_modelName);
}
