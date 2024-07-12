#include "../include/eta_app.hpp"
#include "cube.hpp"
#include "pendulum.hpp"

using namespace eta;

class DemoApplication : public EtaApp {
	void setup() override {
		registerAsset<PendulumMesh>("PendulumMesh");
		registerSystem<PendulumSystem>();
		registerAsset<PendulumScene>("PendulumScene");

		registerAsset<CubeMesh>("CubeMesh");
		registerSystem<CubeSystem>();
		registerAsset<CubeScene>("CubeScene");

		switchScene("PendulumScene");
	};
};

int main(int argc, char* argv[]) {
	DemoApplication app;

	app.init();

	app.run();

	app.destroy();

	return 0;
}
