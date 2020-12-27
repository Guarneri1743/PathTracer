#ifndef _GRAPHICS_UTILITY_
#define _GRAPHICS_UTILITY_
#include <PathTracer.hpp>

namespace Guarneri {
	struct MiscParameter {
		MiscParameter() {
			cam_near = 0.1f;
			cam_far = 500.0f;
			camera_pos = Vector3::ZERO;
			view_matrix = Matrix4x4::IDENTITY;
			proj_matrix = Matrix4x4::IDENTITY;
			main_light = DirectionalLight();
			workflow = PBRWorkFlow::Metallic;
			shadow_bias = 0.02f;
			enable_shadow = true;
			pcf_on = true;
		}

		float cam_near;
		float cam_far;
		Vector3 camera_pos;
		Matrix4x4 view_matrix;
		Matrix4x4 proj_matrix;
		DirectionalLight main_light;
		std::vector<PointLight> point_lights;
		bool enable_shadow;
		bool pcf_on;
		float shadow_bias;
		PBRWorkFlow workflow;
		ColorSpace color_space;
	};

	static MiscParameter misc_param;

	InputManager& InputMgr() {
		return Singleton<InputManager>::get();
	}

	GDIWindow& Window() {
		return Singleton<GDIWindow>::get();
	}

	ResourceManager<Texture>& TextureMgr() {
		return Singleton<ResourceManager<Texture>>::get();
	}
}
#endif