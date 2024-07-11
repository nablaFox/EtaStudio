#include "../include/eta_mesh.hpp"
#include "../include/eta_device.hpp"

using namespace eta;

void EtaMeshAsset::load() {
	if (m_meshFileData.filePath.empty()) {
		ETA_CHECK_MSG(m_device.uploadMesh(m_vertices, m_indices, m_gpuMeshData), "Failed to upload mesh" + getName());
		m_gpuMeshData.indexCount = m_indices.size();
		fmt::print("Mesh {} uploaded\n", getName());
		return;
	}

	// load mesh data from file
}

void EtaMeshAsset::update() {
	if (!m_dirty)
		return;

	// update GPUMeshData
}
