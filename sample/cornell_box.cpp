#include <PathTracer.hpp>

using namespace Guarneri;
using namespace std;

bool auto_rotation = false;

int main()
{
	PathTracer::prepare(800, 800, "PathTracer");

	Scene demo_scene;

	// setup camera
	demo_scene.main_cam->position = Vector3(278.0f, 273.0f, -400.0f);
	demo_scene.main_cam->fov = 60.0f;
	demo_scene.main_cam->set_near(0.5f);
	demo_scene.main_cam->lookat(demo_scene.main_cam->position + Vector3(0.0f, 0.0f, 1.0f));


	uint32_t plane_indices[6] = {
		0, 1, 2, 0, 2, 3
	};

	// ceiling
	Vertex ceiling[4] = {
		Vertex(Vector4(556.0f, 548.8f, 0.0f, 1.0f), Vector3::DOWN, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(556.0f, 548.8f, 559.2f, 1.0f), Vector3::DOWN, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 548.8f, 559.2f, 1.0f), Vector3::DOWN, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(0.0f, 548.8f, 0.0f, 1.0f), Vector3::DOWN, Vector2(0.0f, 1.0f))
	};

	// floor
	Vertex floor[4] = {
		Vertex(Vector4(552.8f, 0.0f, 0.0f, 1.0f), Vector3::UP, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector3::UP, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 0.0f, 559.2f, 1.0f), Vector3::UP, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(549.6f, 0.0f, 559.2f, 1.0f), Vector3::UP, Vector2(0.0f, 1.0f))
	};

	// hole
	Vertex hole[4] = {
		Vertex(Vector4(556.0f, 548.8f, 0.0f, 1.0f), Vector3::DOWN, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(556.0f, 548.8f, 559.2f, 1.0f), Vector3::DOWN, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 548.8f, 559.2f, 1.0f), Vector3::DOWN, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(0.0f, 548.8f, 0.0f, 1.0f), Vector3::DOWN, Vector2(0.0f, 1.0f))
	};

	// back 
	Vertex back_wall[4] = {
		Vertex(Vector4(549.6f, 0.0f, 559.2f, 1.0f), Vector3::BACK, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(0.0f, 0.0f, 559.2f, 1.0f), Vector3::BACK, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 548.8f, 559.2f, 1.0f), Vector3::BACK, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(556.0f, 548.8f, 559.2f, 1.0f), Vector3::BACK, Vector2(0.0f, 1.0f))
	};

	// front 
	Vertex front_wall[4] = {
		Vertex(Vector4(549.6f, 0.0f, 0.0f, 1.0f), Vector3::FORWARD, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector3::FORWARD, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 548.8f, 0.0f, 1.0f), Vector3::FORWARD, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(556.0f, 548.8f, 0.0f, 1.0f), Vector3::FORWARD, Vector2(0.0f, 1.0f))
	};

	// right
	Vertex right_wall[4] = {
		Vertex(Vector4(0.0f, 0.0f, 559.2f, 1.0f), Vector3::RIGHT, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector3::RIGHT, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 548.8f, 0.0f, 1.0f), Vector3::RIGHT, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(0.0f, 548.8f, 559.2f, 1.0f), Vector3::RIGHT, Vector2(0.0f, 1.0f))
	};

	// left
	Vertex left_wall[4] = {
		Vertex(Vector4(552.8f, 0.0f, 0.0f, 1.0f), Vector3::LEFT, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(549.6f, 0.0f, 559.2f, 1.0f), Vector3::LEFT, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(556.0f, 548.8f, 559.2f, 1.0f), Vector3::LEFT, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(556.0f, 548.8f, 0.0f, 1.0f), Vector3::LEFT, Vector2(0.0f, 1.0f))
	};

	std::vector<Vertex> ceiling_verts(ceiling, ceiling + 4);
	std::vector<uint32_t> ceiling_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> floor_verts(floor, floor + 4);
	std::vector<uint32_t> floor_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> hole_verts(hole, hole + 4);
	std::vector<uint32_t> hole_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> back_wall_verts(back_wall, back_wall + 4);
	std::vector<uint32_t> back_wall_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> front_wall_verts(front_wall, front_wall + 4);
	std::vector<uint32_t> front_wall_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> right_wall_verts(right_wall, right_wall + 4);
	std::vector<uint32_t> right_wall_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> left_wall_verts(left_wall, left_wall + 4);
	std::vector<uint32_t> left_wall_indices(plane_indices, plane_indices + 6);

	auto ceiling_mat = std::make_unique<Material>();
	ceiling_mat->set_float(reflectiveness_prop, 0.00f);
	ceiling_mat->set_float(glossiness_prop, 0.00f);
	ceiling_mat->set_float4(emission_prop, Vector4(0.1f, 0.1f, 0.05f, 1.0f));
	ceiling_mat->set_float4(tint_color_prop, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	ceiling_mat->material_type = MaterialType::SCATTER;
	auto ceiling_model = Model::create(ceiling_verts, ceiling_indices, ceiling_mat);

	auto floor_mat = std::make_unique<Material>();
	floor_mat->set_float(reflectiveness_prop, 0.00f);
	floor_mat->set_float(glossiness_prop, 0.00f);
	floor_mat->set_float4(emission_prop, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	floor_mat->set_float4(tint_color_prop, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	floor_mat->material_type = MaterialType::SCATTER;
	auto floor_model = Model::create(floor_verts, floor_indices, floor_mat);

	//auto hole_mat = std::make_unique<Material>();
	//hole_mat->set_float(reflectiveness_prop, 0.01f);
	//hole_mat->set_float(glossiness_prop, 0.00f);
	//hole_mat->set_float4(tint_color_prop, Vector4(0.2f, 0.2f, 0.2f, 1.0f));
	//auto hole_model = Model::create(hole_verts, hole_indices, hole_mat);
	//hole_model->transform.translate(Vector3(556.0f / 8.0f, -1.0f, 559.2f / 8.0f));
	//hole_model->transform.scale(Vector3(0.25f, 1.0f, 0.25f));

	auto back_wall_mat = std::make_unique<Material>();
	back_wall_mat->set_float(reflectiveness_prop, 0.00f);
	back_wall_mat->set_float(glossiness_prop, 0.00f);
	back_wall_mat->set_float4(emission_prop, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	back_wall_mat->set_float4(tint_color_prop, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	back_wall_mat->material_type = MaterialType::SCATTER;
	auto back_wall_model = Model::create(back_wall_verts, back_wall_indices, back_wall_mat);

	auto front_wall_mat = std::make_unique<Material>();
	front_wall_mat->set_float(reflectiveness_prop, 0.00f);
	front_wall_mat->set_float(glossiness_prop, 0.00f);
	front_wall_mat->set_float4(emission_prop, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	front_wall_mat->set_float4(tint_color_prop, Vector4(0.8f, 0.8f, 0.8f, 1.0f));
	front_wall_mat->material_type = MaterialType::SCATTER;
	auto front_wall_model = Model::create(front_wall_verts, front_wall_indices, front_wall_mat);

	auto right_wall_mat = std::make_unique<Material>();
	right_wall_mat->set_float(reflectiveness_prop, 0.00f);
	right_wall_mat->set_float(glossiness_prop, 0.00f);
	right_wall_mat->set_float4(emission_prop, Vector4(0.2f, 0.05f, 0.05f, 1.0f));
	right_wall_mat->set_float4(tint_color_prop, Vector4(0.8f, 0.3f, 0.3f, 1.0f));
	right_wall_mat->material_type = MaterialType::SCATTER;
	auto right_wall_model = Model::create(right_wall_verts, right_wall_indices, right_wall_mat);

	auto left_wall_mat = std::make_unique<Material>();
	left_wall_mat->set_float(reflectiveness_prop, 0.00f);
	left_wall_mat->set_float(glossiness_prop, 0.00f);
	left_wall_mat->set_float4(emission_prop, Vector4(0.05f, 0.2f, 0.05f, 1.0f));
	left_wall_mat->set_float4(tint_color_prop, Vector4(0.3f, 0.8f, 0.3f, 1.0f));
	left_wall_mat->material_type = MaterialType::SCATTER;
	auto left_wall_model = Model::create(left_wall_verts, left_wall_indices, left_wall_mat);

	auto sphere1_mat = std::make_shared<Material>();
	sphere1_mat->set_float4(emission_prop, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	sphere1_mat->set_float4(tint_color_prop, Vector4(0.999f, 0.999f, 0.999f, 1.0f));
	sphere1_mat->set_float(reflectiveness_prop, 1.0f);
	sphere1_mat->set_float(glossiness_prop, 64.0f);
	sphere1_mat->material_type = MaterialType::REFLECTION;
	auto sphere1 = new Sphere(Vector3(126.0f, 80.0f, 328.5f), 80.0f);
	sphere1->material = sphere1_mat;

	auto sphere2_mat = std::make_shared<Material>();
	sphere2_mat->set_float4(emission_prop, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	sphere2_mat->set_float4(tint_color_prop, Vector4(0.999f, 0.999f, 0.999f, 1.0f));
	sphere2_mat->set_float(refractiveness_prop, 1.52f);
	sphere2_mat->material_type = MaterialType::REFRACTION;
	auto sphere2= new Sphere(Vector3(398.0f, 100.0f, 180.0f), 100.0f);
	sphere2->material = sphere2_mat;

	auto sphere3_mat = std::make_shared<Material>();
	sphere3_mat->set_float4(emission_prop, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	sphere3_mat->set_float4(tint_color_prop, Vector4(0.3f, 0.8f, 0.7f, 1.0f));
	sphere3_mat->material_type = MaterialType::SCATTER;
	auto sphere3 = new Sphere(Vector3(300.0f, 100.0f, 388.0f), 100.0f);
	sphere3->material = sphere3_mat;

	auto light_mat = std::make_shared<Material>();
	light_mat->set_float4(emission_prop, 12.0f * Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	light_mat->set_float4(tint_color_prop, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	light_mat->material_type = MaterialType::SCATTER;

	float radius = 600.0f;
	auto pl1 = new Sphere(Vector3(443.0f, 548.8f + radius - 10.0f, 127.0f), radius);
	pl1->material = light_mat;

	auto pl2 = new Sphere(Vector3(443.0f, 548.8f + radius - 10.0f, 432.0f), radius);
	pl2->material = light_mat;

	auto pl3 = new Sphere(Vector3(113.0f, 548.8f + radius - 10.0f, 432.0f), radius);
	pl3->material = light_mat;

	auto pl4 = new Sphere(Vector3(113.0f, 548.8f + radius - 10.0f, 127.0f), radius);
	pl4->material = light_mat;


	demo_scene.add(std::move(ceiling_model));
	demo_scene.add(std::move(floor_model));
	//demo_scene.add(std::move(hole_model));
	demo_scene.add(std::move(back_wall_model));
	//demo_scene.add(std::move(front_wall_model));
	demo_scene.add(std::move(right_wall_model));
	demo_scene.add(std::move(left_wall_model));
	demo_scene.add(dynamic_cast<Primitive*>(sphere1));
	demo_scene.add(dynamic_cast<Primitive*>(sphere2));
	demo_scene.add(dynamic_cast<Primitive*>(sphere3));
	demo_scene.add(dynamic_cast<Primitive*>(pl1));
	demo_scene.add(dynamic_cast<Primitive*>(pl2));
	demo_scene.add(dynamic_cast<Primitive*>(pl3));
	demo_scene.add(dynamic_cast<Primitive*>(pl4));

	demo_scene.ray_max_distance = 2000.0f;
	demo_scene.exposure = 0.4;

	PathTracer::kick_off(demo_scene);
	return 0;
}