#ifndef _SCENE_
#define _SCENE_
#include <PathTracer.hpp>

namespace Guarneri
{
	class Scene
	{
	public:
		DirectionalLight main_light;
		std::vector<PointLight> point_lights;
		std::vector<std::shared_ptr<Model>> objects;
		std::vector<Primitive*> prims;
		std::unique_ptr<Camera> main_cam;
		float ray_max_distance;

	public:
		Scene();
		~Scene();
		void initialize();
		void add(std::shared_ptr<Model> obj);
		void add(Primitive* obj);
		void update();
		void set_main_light(const DirectionalLight& light);
		void add_point_light(const PointLight& light);
		RaytraceResult intersect(const Ray& ray);
	};


	Scene::Scene()
	{
		initialize();
	}

	Scene::~Scene()
	{}

	// todo: serialzie & deserialize Scene data
	void Scene::initialize()
	{
		main_light.intensity = 1.0f;
		main_light.diffuse = Color(1.0f, 0.8f, 0.8f, 1.0f);
		main_light.ambient = Color(0.1f, 0.05f, 0.2f, 1.0f);
		main_light.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
		main_cam = std::move(Camera::create(Vector3(5.0f, 5.0f, 5.0f), Window().aspect, 45.0f, 0.5f, 100.0f));
		main_cam->lookat(Vector3::ZERO);
		ray_max_distance = 10000.0f;
		InputMgr().add_on_mouse_move_evt([](Vector2 prev, Vector2 pos, void* data)
		{
			if (InputMgr().is_mouse_down(MouseButton::RIGHT))
			{
				Vector2 offset = (pos - prev) * Vector2(Window().width, Window().height) * CAMERA_ROTATE_SPEED;
				Scene* s = reinterpret_cast<Scene*>(data);
				s->main_cam->rotate(offset.x, offset.y);
			}
			if (InputMgr().is_mouse_down(MouseButton::MIDDLE))
			{
				Vector2 offset = (pos - prev) * Vector2(Window().width, Window().height) * CAMERA_ROTATE_SPEED;
				Scene* s = reinterpret_cast<Scene*>(data);
				s->main_cam->move_left(offset.x);
				s->main_cam->move_ascend(offset.y);
			}
		}, this);

		InputMgr().add_on_mouse_wheel_rolling_evt([](MouseWheel rolling, void* data)
		{
			Scene* s = reinterpret_cast<Scene*>(data);
			if (rolling == MouseWheel::UP)
			{
				s->main_cam->move_forward(CAMERA_ZOOM_SPEED);
			}
			else
			{
				s->main_cam->move_backward(CAMERA_ZOOM_SPEED);
			}
		}, this);
	}

	void Scene::add(std::shared_ptr<Model> obj)
	{
		objects.emplace_back(obj);
	}

	void Scene::add(Primitive* obj)
	{
		prims.emplace_back(obj);
	}

	void Scene::update()
	{
		if (InputMgr().is_mouse_down(MouseButton::MIDDLE))
		{
			main_cam->move_ascend(CAMERA_MOVE_SPEED);
		}
		if (InputMgr().is_mouse_down(MouseButton::MIDDLE))
		{
			main_cam->move_descend(CAMERA_MOVE_SPEED);
		}
	}

	void Scene::set_main_light(const DirectionalLight& light)
	{
		main_light = light;
	}

	void Scene::add_point_light(const PointLight& light)
	{
		point_lights.emplace_back(light);
	}

	RaytraceResult Scene::intersect(const Ray& ray)
	{
		int vertex_count = 0;
		Vertex verts[3];
		RaytraceResult ret;
		ret.hit = false;
		ret.normal = Vector3::ZERO;
		ret.pos = Vector3::ZERO;
		ret.t = 0.0f;
		ret.material = nullptr;
		float near = FLT_MAX;

		for (auto& obj : objects)
		{
			auto mat = obj->material;
			auto model = obj->transform.local2world;
			for (auto& mesh : obj->meshes)
			{
				for (auto& index : mesh->indices)
				{
					verts[vertex_count] = mesh->vertices[index];
					vertex_count++;
					if (vertex_count == 3)
					{
						Vector4 uvwt;
						Triangle tri(verts[0], verts[1], verts[2]);
						if (tri.intersect(ray, ray_max_distance, model, uvwt))
						{
							if (uvwt.w < near)
							{
								ret.hit = true;
								auto a = verts[0].position.xyz();
								auto b = verts[1].position.xyz();
								auto c = verts[2].position.xyz();
								auto ab = b - a;
								auto ac = c - a;
								auto n = Vector3::cross(ab, ac).normalized();
								ret.t = uvwt.w;
								ret.normalized_t = ret.t / ray_max_distance;
								ret.pos = ray.origin + ray.direction * ret.t;
								ret.normal = n;
								ret.material = mat;
								near = uvwt.w;
							}
						}
						vertex_count = 0;
					}
				}
			}
		}

		for (auto& obj : prims)
		{
			auto mat = obj->get_material();
			float t;
			Vector3 pos;
			Vector3 normal;
			if (obj->intersect(ray, pos, t, normal))
			{
				if (t < near)
				{
					ret.hit = true;
					ret.t = t;
					ret.normalized_t = ret.t / ray_max_distance;
					ret.pos = pos;
					ret.normal = normal;
					ret.material = mat;
					near = t;
				}
			}
		}

		return ret;
	}
}
#endif