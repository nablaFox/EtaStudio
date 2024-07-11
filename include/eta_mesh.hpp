#pragma once

#include "eta_asset.hpp"

namespace eta {
class EtaMeshAsset : public EtaAsset {
	friend class EtaRenderingSystem;

public:
	using EtaAsset::EtaAsset;

	struct MeshFileData {
		std::string meshName;
		std::string filePath;
	};

	void setVertices(std::vector<Vertex> vertices) {
		m_vertices = vertices;
		m_dirty = true;
	}

	void setIndices(std::vector<uint32_t> indices) {
		m_indices = indices;
		m_dirty = true;
	}

public:
	void load() override;
	void update();

private:
	MeshFileData m_meshFileData;
	GPUMeshData m_gpuMeshData;
	std::span<Vertex> m_vertices;
	std::span<Index> m_indices;

	bool m_dirty = false;
};

}; // namespace eta
