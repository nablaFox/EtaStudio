#include "../include/eta_system.hpp"
#include "../include/eta_engine.hpp"

using namespace eta;

EtaSceneAsset& EtaSystem::currentScene() { return m_engine.currentScene(); }
