#include "../include/eta_app.hpp"
#include "violin.hpp"
#include "geometry.hpp"
#include "pendulum.hpp"

using namespace eta;

class DemoApplication : public EtaApp {
	void setup() override {
		registerAsset<PendulumMesh>("PendulumMesh");
		registerAsset<PendulumScene>("PendulumScene");
		registerSystem<PendulumSystem>();

		registerAsset<PyramidMesh>("PyramidMesh");
		registerAsset<CubeMesh>("CubeMesh");
		registerAsset<GeometryScene>("GeometryScene");

		registerAsset<ViolinModel>("ViolinModel");
		registerAsset<ViolinScene>("ViolinScene");

		registerSystem<RotationSystem>();
		registerSystem<MovementSystem>();

		switchScene("GeometryScene");
	};
};

int main(int argc, char* argv[]) {
	DemoApplication app;

	app.init();

	app.run();

	app.destroy();

	return 0;
}
