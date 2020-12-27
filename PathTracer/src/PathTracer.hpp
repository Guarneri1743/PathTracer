#ifndef _GUARNERI_
#define _GUARNERI_

#include <cstdlib>
#include <cmath>
#include <limits.h>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <filesystem>
#include <thread>
#include <mutex>
#include <atomic>

#define NOMINMAX
#include <windows.h>
#include <tchar.h>
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif
#if WIN32
#define WIN32_LEAN_AND_MEAN
#endif

template <class T>
inline void unused(T const&)
{}
#define REF(obj) unused(obj);

// PathTracer Core
#include <PipelineDefinitions.hpp>
#include <ThreadPool.hpp>
#include <SafeQueue.hpp>
#include <GDIWindow.hpp>
#include <Singleton.hpp>
#include <IdAllocator.hpp>
#include <ResourceManager.hpp>
#include <CommonDefinitions.hpp>
#include <Object.hpp>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <Color.hpp>
#include <Matrix4x4.hpp>
#include <Matrix3x3.hpp>
#include <Light.hpp>
#include <RawBuffer.hpp>
#include <Texture.hpp>
#include <Plane.hpp>
#include <Ray.hpp>
#include <Ray2D.hpp>
#include <Line.hpp>
#include <Segment.hpp>
#include <Sphere.hpp>
#include <Cylinder.hpp>
#include <Capsule.hpp>
#include <BoundingBox.hpp>
#include <BoundingBox2D.hpp>
#include <Vertex.hpp>
#include <Triangle.hpp>
#include <InputManager.hpp>
#include <Transform.hpp>
#include <SegmentDrawer.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Model.hpp>
#include <Camera.hpp>
#include <PrimitiveFactory.hpp>
#include <Misc.hpp>
#include <FrameTile.hpp>
#include <Scene.hpp>

namespace Guarneri
{
	class PathTracer
	{
	public:
		const static uint32_t TILE_SIZE;

		const static uint32_t TILE_TASK_SIZE;

		const static uint32_t MAX_DEPTH;

		static uint32_t row_tile_count;

		static uint32_t col_tile_count;

		static uint32_t tile_length;

		static uint32_t sample_size;

		static uint32_t shaded_tile_length;

		static uint32_t dispatched_tile_length;

		static bool repaint;

		static FrameTile* tiles;

		static ThreadPool* thread_pool;

		static std::mutex task_mutex;

		static int stay_frame_count;

		static std::unique_ptr<RawBuffer<color_bgra>> framebuffer;

		static void prepare(const uint32_t w, const uint32_t h, LPCSTR title);

		static bool ray_triangle(const Ray& ray, const float& distance, const Matrix4x4& model, const Vertex& a, const Vertex& b, const Vertex& c, Vector4& uvw);

		static Color ray_scene(const uint32_t& row, const uint32_t& col, const Ray& ray, Scene& scene, const int& depth);

		static void path_trace_tile(FrameTile& tile, Scene& scene);

		static void path_trace_tiles(const int& start, const int& end, Scene& scene);

		static void path_trace(Scene& scene);

		static void kick_off(Scene& scene);
	};

	const uint32_t PathTracer::MAX_DEPTH = 16;

	const uint32_t PathTracer::TILE_SIZE = 4;

	const uint32_t PathTracer::TILE_TASK_SIZE = 1;

	uint32_t PathTracer::sample_size = 1;

	std::unique_ptr<RawBuffer<color_bgra>> PathTracer::framebuffer;

	uint32_t PathTracer::row_tile_count;

	uint32_t PathTracer::col_tile_count;

	uint32_t PathTracer::tile_length;

	uint32_t PathTracer::shaded_tile_length;

	uint32_t PathTracer::dispatched_tile_length;

	bool PathTracer::repaint;

	FrameTile* PathTracer::tiles;

	ThreadPool* PathTracer::thread_pool;

	std::mutex PathTracer::task_mutex;

	int PathTracer::stay_frame_count;

	void PathTracer::prepare(const uint32_t w, const uint32_t h, LPCSTR title)
	{
		auto thread_size = (size_t)std::thread::hardware_concurrency();
		thread_pool = new ThreadPool(thread_size);

		Window().initialize(w, h, title, InputMgr().event_callback);
		InputMgr().add_on_key_down_evt([](KeyCode code, void* data)
		{
			REF(data);
			if (code == KeyCode::ESC) Window().dispose();
		}, nullptr);
		framebuffer = std::make_unique<RawBuffer<color_bgra>>(Window().framebuffer, Window().width, Window().height, [](color_bgra* ptr)
		{
			REF(ptr);
		});

		repaint = true;

		// prepare tiles
		int row_rest = h % TILE_SIZE;
		int col_rest = w % TILE_SIZE;
		row_tile_count = h / TILE_SIZE + (row_rest > 0 ? 1 : 0);
		col_tile_count = w / TILE_SIZE + (col_rest > 0 ? 1 : 0);
		tile_length = static_cast<int>(static_cast<long>(row_tile_count) * static_cast<long>(col_tile_count));
		tiles = new FrameTile[tile_length];
		FrameTile::build_tiles(tiles, TILE_SIZE, row_tile_count, col_tile_count, row_rest, col_rest);

		InputMgr().add_on_mouse_move_evt([](Vector2 prev, Vector2 pos, void* data)
		{
			if (InputMgr().is_mouse_down(MouseButton::RIGHT) || InputMgr().is_mouse_down(MouseButton::MIDDLE))
			{
				repaint = true;
			}
		}, nullptr);

		InputMgr().add_on_mouse_wheel_rolling_evt([](MouseWheel rolling, void* data)
		{
			repaint = true;
		}, nullptr);
	}

	bool PathTracer::ray_triangle(const Ray& ray, const float& distance, const Matrix4x4& model, const Vertex& v0, const Vertex& v1, const Vertex& v2, Vector4& uvw)
	{
		Vector3 a = (model * v0.position).xyz();
		Vector3 b = (model * v1.position).xyz();
		Vector3 c = (model * v2.position).xyz();

		Vector3 ab = b - a;
		Vector3 ac = c - a;
		Vector3 p = ray.origin;
		Vector3 q = ray.origin + ray.direction * distance;
		Vector3 qp = p - q;

		Vector3 n = Vector3::cross(ab, ac);

		float d = Vector3::dot(qp, n);
		if (d <= 0.0f) return false;

		Vector3 ap = p - a;
		float t = Vector3::dot(ap, n);

		if (t < 0.0f) return false;
		if (t > d) return false;

		Vector3 e = Vector3::cross(qp, ap);
		float v = Vector3::dot(ac, e);
		if (v < 0.0f || v > d) return false;
		float w = -Vector3::dot(ab, e);
		if (w < 0.0f || v + w > d) return false;

		float ood = 1.0f / d;

		t *= ood;
		v *= ood;
		w *= ood;

		float u = 1.0f - v - w;

		uvw.x = u;
		uvw.y = v;
		uvw.z = w;
		uvw.w = t * distance;

		return true;
	}

	Color PathTracer::ray_scene(const uint32_t& row, const uint32_t& col, const Ray& ray, Scene& scene, const int& depth)
	{
		if (depth > MAX_DEPTH)
		{
			return Color::BLACK;
		}
		int vertex_count = 0;
		Vertex verts[3];
		float near = FLT_MAX;
		Vertex near_verts[3];
		Color ret = Color::BLACK;
		for (auto& obj : scene.objects)
		{
			auto mat = obj->material;
			auto tint = mat->get_float4(tint_color_prop);
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
						if (ray_triangle(ray, scene.ray_max_distance, model, verts[0], verts[1], verts[2], uvwt))
						{
							if (uvwt.w < near)
							{
								near = uvwt.w;
								near_verts[0] = verts[0];
								near_verts[1] = verts[1];
								near_verts[2] = verts[2];
								auto n1 = verts[0].normal;
								auto n2 = verts[1].normal;
								auto n3 = verts[2].normal;
								auto n = (n1 + n2 + n3) / 3.0f;
								for (auto& pl : scene.point_lights)
								{

								}
								float linear_z = uvwt.w / scene.ray_max_distance;
								Color c = Color(linear_z * tint);
								//c = Color(Vector3::abs(n));
								c.a = 1.0f;
								Vector3 hit = ray.origin + ray.direction * uvwt.w;
								float rx = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
								float ry = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
								float rz = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
								auto dir = Vector3(rx, ry, rz);
								dir = dir * 2.0f - 1.0f;
								Ray reflect_ray(hit, dir.normalized());
								ret = c + ray_scene(row, col, reflect_ray, scene, depth + 1);
							}
						}
						vertex_count = 0;
					}
				}
			}
		}

		return ret;
	}

	void PathTracer::path_trace_tile(FrameTile& tile, Scene& scene)
	{
		for (uint32_t row = tile.row_start; row < tile.row_end; row++)
		{
			for (uint32_t col = tile.col_start; col < tile.col_end; col++)
			{
				float fov = 0.5f / std::tanf(scene.main_cam->fov * PI * 0.5f / 180.f);
				Color color = Color::BLACK;
				for (uint32_t s = 0; s < sample_size; s++)
				{
					float ru = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					float rv = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

					ru = ru * 2.0f - 1.0f;

					rv = rv * 2.0f - 1.0f;

					ru *= 0.0f;
					
					rv *= 0.0f;

					float u = ((float)col + 0.5f + ru) / (float)(Window().width - 1) - 0.5f;

					float v = ((float)row + 0.5f + rv) / (float)(Window().height - 1) - 0.5f;

					Ray ray(scene.main_cam->position, Vector3::normalize(v * scene.main_cam->up + u * scene.main_cam->right + fov * scene.main_cam->forward));

					color += ray_scene(row, col, ray, scene, 0);
				}
				color /= (float)sample_size;
				framebuffer->write(row, col, Color::encode_bgra(color));
			}
		}
	}

	void PathTracer::path_trace_tiles(const int& start, const int& end, Scene& scene)
	{
		{
			std::unique_lock<std::mutex> lock(task_mutex);
			for (int tidx = start; tidx < end; tidx++)
			{
				auto tile = tiles[tidx];
				path_trace_tile(tile, scene);
				shaded_tile_length++;
			}
		}
	}

	void PathTracer::path_trace(Scene& scene)
	{
		thread_pool->cancel_tasks();
		int task_size = TILE_TASK_SIZE;
		int task_rest = tile_length % task_size;
		int task_count = tile_length / task_size;
		for (auto tid = 0; tid < task_count; tid++)
		{
			int start = tid * task_size;
			int end = (static_cast<long>(tid) + 1) * task_size;
			thread_pool->enqueue([&](const int& from, const int& to)
			{
				path_trace_tiles(from, to, scene);
			}, start, end);
		}
		int last_start = task_count * task_size;
		int last_end = last_start + task_rest;
		if (last_end > last_start)
		{
			thread_pool->enqueue([&](const int& from, const int& to)
			{
				path_trace_tiles(from, to, scene);
			}, last_start, last_end);
		}
	}

	void PathTracer::kick_off(Scene& scene)
	{
		while (Window().is_valid())
		{
			InputMgr().update();

			if (repaint)
			{
				shaded_tile_length = 0;
				framebuffer->clear(color_bgra());
				path_trace(scene);
				repaint = false;
			}

			const int w = 800;
			const int h = 20;
			{
				std::stringstream ss;
				ss << "CameraPosition: " << scene.main_cam->position;
				Window().draw_text(w, h, ss.str().c_str());
			}
			{
				std::stringstream ss;
				ss << "CameraDirection: " << scene.main_cam->forward;
				Window().draw_text(w, h, ss.str().c_str());
			}
			{
				std::stringstream ss;
				ss << "Shaded Tiles: " << shaded_tile_length << "/" << tile_length;
				Window().draw_text(w, h, ss.str().c_str());
			}

			Window().flush();
			Sleep(1);
		}
	}
}
#endif