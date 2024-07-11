#include "../include/eta_mesh.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

void EtaMeshAsset::load() {
	if (m_meshFileData.filePath.empty()) {
		update();
		fmt::print("Mesh {} uploaded\n", getName());
		return;
	}

	// load mesh data from file
}

void EtaMeshAsset::destroy() { m_device.destroyMesh(m_gpuMeshData); }

void EtaMeshAsset::update() {
	if (!m_dirty)
		return;

	const size_t newVertexBufferSize = m_vertices.size() * sizeof(Vertex);
	const size_t newIndexBufferSize = m_indices.size() * sizeof(Index);

	bool needReallocation = false;

	if (newVertexBufferSize > m_gpuMeshData.vertexBuffer.info.size) {
		needReallocation = true;
	}

	if (newIndexBufferSize > m_gpuMeshData.indexBuffer.info.size) {
		needReallocation = true;
	}

	if (needReallocation) {
		ETA_CHECK_MSG(m_device.uploadMesh(m_vertices, m_indices, m_gpuMeshData), "Failed to upload mesh" + getName());
		return;
	}

	ETA_CHECK_MSG(m_device.updateMesh(m_vertices, m_indices, m_gpuMeshData), "Failed to update mesh" + getName());
	m_dirty = false;
}
