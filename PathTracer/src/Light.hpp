#ifndef _LIGHT_
#define _LIGHT_
#include <PathTracer.hpp>

namespace Guarneri {
	struct Light {
		Color ambient;
		Color diffuse;
		Color specular;
		float intensity;
	};

	struct DirectionalLight : Light {
		DirectionalLight() {
			ambient = Color::BLACK;
			diffuse = Color::WHITE;
			specular = Color::WHITE;
			intensity = 1.0f;
			this->forward = Vector3(1.0f, 1.0f, 1.0f);
		}

		Vector3 forward;
	};

	struct PointLight : Light {
		PointLight() {
			constant = 0.5f;
			linear = 0.15f;
			quadratic = 0.00003f;
			position = Vector3();
		}

		float constant;
		float linear;
		float quadratic;
		Vector3 position;
	};

	struct SpotLight : Light {
		Vector3 position;
		Vector3 spot_direction;
		float phi;
		float theta;
	};
}
#endif