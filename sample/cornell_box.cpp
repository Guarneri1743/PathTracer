#include <PathTracer.hpp>

using namespace Guarneri;
using namespace std;

bool auto_rotation = false;

int main()
{
	PathTracer::prepare(800, 600, "PathTracer");

	// setup main Light
	Scene demo_scene;

	PointLight pl1, pl2, pl3, pl4;
	pl1.position = Vector3(343.0f, 548.8f, 227.0f);
	pl1.ambient = Color(0.05f, 0.05f, 0.05f, 1.0f);
	pl1.diffuse = Color(1.0f, 0.0f, 0.0f, 1.0f);
	pl1.specular = Color(1.0f, 0.0f, 0.0f, 1.0f);
	pl1.intensity = 3.0f;

	pl2.position = Vector3(343.0f, 548.8f, 332.0f);
	pl2.ambient = Color(0.05f, 0.05f, 0.05f, 1.0f);
	pl2.diffuse = Color(0.0f, 1.0f, 0.0f, 1.0f);
	pl2.specular = Color(0.0f, 1.0f, 0.0f, 1.0f);
	pl2.intensity = 3.0f;

	pl3.position = Vector3(213.0f, 548.8f, 332.0f);
	pl3.ambient = Color(0.05f, 0.05f, 0.05f, 1.0f);
	pl3.diffuse = Color(0.0f, 0.0f, 1.0f, 1.0f);
	pl3.specular = Color(0.0f, 0.0f, 1.0f, 1.0f);
	pl3.intensity = 3.0f;

	pl4.position = Vector3(213.0f, 548.8f, 227.0f);
	pl4.ambient = Color(0.05f, 0.05f, 0.05f, 1.0f);
	pl4.diffuse = Color(0.0f, 0.0f, 1.0f, 1.0f);
	pl4.specular = Color(0.0f, 0.0f, 1.0f, 1.0f);
	pl4.intensity = 3.0f;

	demo_scene.add_point_light(pl1);
	demo_scene.add_point_light(pl2);
	demo_scene.add_point_light(pl3);

	// setup camera
	demo_scene.main_cam->position = Vector3(278.0f, 273.0f, -450.0f);
	demo_scene.main_cam->fov = 60.0f;
	demo_scene.main_cam->set_near(0.5f);
	demo_scene.main_cam->lookat(demo_scene.main_cam->position + Vector3(0.0f, 0.0f, 1.0f));


	uint32_t plane_indices[6] = {
		0, 1, 2, 2, 3, 0
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
		Vertex(Vector4(343.0f, 548.8f, 227.0f, 1.0f), Vector3::DOWN, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(343.0f, 548.8f, 332.0f, 1.0f), Vector3::DOWN, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(213.0f, 548.8f, 332.0f, 1.0f), Vector3::DOWN, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(213.0f, 548.8f, 227.0f, 1.0f), Vector3::DOWN, Vector2(0.0f, 1.0f))
	};

	// back 
	Vertex back_wall[4] = {
		Vertex(Vector4(549.6f, 0.0f, 559.2f, 1.0f), Vector3::FORWARD, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(0.0f, 0.0f, 559.2f, 1.0f), Vector3::FORWARD, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 548.8f, 559.2f, 1.0f), Vector3::FORWARD, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(556.0f, 548.8f, 559.2f, 1.0f), Vector3::FORWARD, Vector2(0.0f, 1.0f))
	};

	// right
	Vertex right_wall[4] = {
		Vertex(Vector4(0.0f, 0.0f, 559.2f, 1.0f), Vector3::LEFT, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector3::LEFT, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(0.0f, 548.8f, 0.0f, 1.0f), Vector3::LEFT, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(0.0f, 548.8f, 559.2f, 1.0f), Vector3::LEFT, Vector2(0.0f, 1.0f))
	};

	// left
	Vertex left_wall[4] = {
		Vertex(Vector4(552.8f, 0.0f, 0.0f, 1.0f), Vector3::RIGHT, Vector2(0.0f, 0.0f)),
		Vertex(Vector4(549.6f, 0.0f, 559.2f, 1.0f), Vector3::RIGHT, Vector2(1.0f, 0.0f)),
		Vertex(Vector4(556.0f, 548.8f, 559.2f, 1.0f), Vector3::RIGHT, Vector2(1.0f, 1.0f)),
		Vertex(Vector4(556.0f, 548.8f, 0.0f, 1.0f), Vector3::RIGHT, Vector2(0.0f, 1.0f))
	};

	std::vector<Vertex> ceiling_verts(ceiling, ceiling + 4);
	std::vector<uint32_t> ceiling_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> floor_verts(floor, floor + 4);
	std::vector<uint32_t> floor_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> hole_verts(hole, hole + 4);
	std::vector<uint32_t> hole_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> back_wall_verts(back_wall, back_wall + 4);
	std::vector<uint32_t> back_wall_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> right_wall_verts(right_wall, right_wall + 4);
	std::vector<uint32_t> right_wall_indices(plane_indices, plane_indices + 6);

	std::vector<Vertex> left_wall_verts(left_wall, left_wall + 4);
	std::vector<uint32_t> left_wall_indices(plane_indices, plane_indices + 6);

	auto ceiling_mat = std::make_unique<Material>();
	ceiling_mat->set_float4(tint_color_prop, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	auto ceiling_model = Model::create(ceiling_verts, ceiling_indices, ceiling_mat);

	auto floor_mat = std::make_unique<Material>();
	floor_mat->set_float4(tint_color_prop, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	auto floor_model = Model::create(floor_verts, floor_indices, floor_mat);

	auto hole_mat = std::make_unique<Material>();
	hole_mat->set_float4(tint_color_prop, Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	auto hole_model = Model::create(hole_verts, hole_indices, hole_mat);

	auto back_wall_mat = std::make_unique<Material>();
	back_wall_mat->set_float4(tint_color_prop, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	auto back_wall_model = Model::create(back_wall_verts, back_wall_indices, back_wall_mat);

	auto right_wall_mat = std::make_unique<Material>();
	right_wall_mat->set_float4(tint_color_prop, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	auto right_wall_model = Model::create(right_wall_verts, right_wall_indices, right_wall_mat);

	auto left_wall_mat = std::make_unique<Material>();
	left_wall_mat->set_float4(tint_color_prop, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	auto left_wall_model = Model::create(left_wall_verts, left_wall_indices, left_wall_mat);

	auto box1_mat = std::make_unique<Material>();
	box1_mat->set_float4(tint_color_prop, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	auto box1 = PrimitiveFactory::cube(box1_mat);
	box1->transform.scale(Vector3(80.0f, 80.0f, 80.0f));
	box1->transform.rotate(Vector3(0.0f, 1.0f, 0.0f), 60.0f);
	box1->transform.translate(Vector3(126.0f, 80.0f, 128.5f));

	auto box2_mat = std::make_unique<Material>();
	box2_mat->set_float4(tint_color_prop, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	auto box2 = PrimitiveFactory::cube(box2_mat);
	box2->transform.scale(Vector3(60.0f, 150.0f, 60.0f));
	box2->transform.rotate(Vector3(0.0f, 1.0f, 0.0f), 45.0f);
	box2->transform.translate(Vector3(368.0f, 150.0f, 351.0f));

	demo_scene.add(std::move(ceiling_model));
	demo_scene.add(std::move(floor_model));
	demo_scene.add(std::move(hole_model));
	demo_scene.add(std::move(back_wall_model));
	demo_scene.add(std::move(right_wall_model));
	demo_scene.add(std::move(left_wall_model));
	demo_scene.add(std::move(box1));
	demo_scene.add(std::move(box2));

	demo_scene.ray_max_distance = 2000.0f;

	PathTracer::kick_off(demo_scene);
	return 0;
}