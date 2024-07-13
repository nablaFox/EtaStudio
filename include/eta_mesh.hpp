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

	struct MeshSurface {
		uint32_t firstIndex;
		uint32_t indexCount;
	};

	void setVertices(std::vector<Vertex>& vertices) {
		m_vertices = vertices;
		m_dirty = true;
	}

	void setIndices(std::vector<uint32_t>& indices) {
		m_indices = indices;
		m_dirty = true;
	}

	void setSurfaces(std::vector<MeshSurface>& surfaces) { m_meshSurfaces = surfaces; }

	void addSurface(MeshSurface& surface) { m_meshSurfaces.push_back(surface); }

	void reserveVertices(size_t size) {
		m_vertices.assign(size, Vertex());
		m_dirty = true;
	}

	void reserveIndices(size_t size) {
		m_indices.assign(size, 0);
		m_dirty = true;
	}

public:
	void load() override;
	void destroy() override;
	void update();

private:
	MeshFileData m_meshFileData;
	GPUMeshData m_gpuMeshData;
	std::vector<Vertex> m_vertices;
	std::vector<Index> m_indices;

	std::vector<MeshSurface> m_meshSurfaces;

	bool m_dirty = false;
};

}; // namespace eta
