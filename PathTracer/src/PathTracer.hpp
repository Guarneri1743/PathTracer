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
#include <Primitive.hpp>
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
#include <Misc.hpp>
#include <FrameTile.hpp>
#include <Scene.hpp>

namespace Guarneri
{
	static std::vector<int> prim_nums = {
		  3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
		  1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
		  17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
		  187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
		  1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369 };


	class PathTracer
	{
	public:
		const static uint32_t TILE_SIZE;

		const static uint32_t TILE_TASK_SIZE;

		const static uint32_t SAMPLE_SIZE;

		const static uint32_t MAX_DEPTH;

		static uint32_t row_tile_count;

		static uint32_t col_tile_count;

		static uint32_t tile_length;

		static uint32_t shaded_tile_length;

		static uint32_t dispatched_tile_length;

		static bool repaint;

		static FrameTile* tiles;

		static ThreadPool* thread_pool;

		static std::mutex task_mutex;

		static int stay_frame_count;

		static std::unique_ptr<RawBuffer<color_bgra>> framebuffer;

		static void prepare(const uint32_t w, const uint32_t h, LPCSTR title);

		static Color sample_material(Scene& scene, const std::shared_ptr<Material>& mat, const Ray& ray, const RaytraceResult& prev_ret);

		static Color path_trace(const Ray& ray, Scene& scene, int depth);

		static void path_trace_tile(FrameTile& tile, Scene& scene);

		static void path_trace_tiles(const int& start, const int& end, Scene& scene);

		static void path_trace(Scene& scene);

		static void kick_off(Scene& scene);
	};

	const uint32_t PathTracer::MAX_DEPTH = 5;

	const uint32_t PathTracer::TILE_SIZE = 4;

	const uint32_t PathTracer::TILE_TASK_SIZE = 1;

	const uint32_t PathTracer::SAMPLE_SIZE = 16;

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

	Color PathTracer::sample_material(Scene& scene, const std::shared_ptr<Material>& mat, const Ray& ray, const RaytraceResult& ret)
	{
		auto emission = mat->get_float4(emission_prop);
		return emission;
	}

	inline static float rand01()
	{
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	float radical_inverse(int base, int i)
	{
		float digit, radical, inverse;
		digit = radical = 1.0f / (float)base;
		inverse = 0.0;
		while (i)
		{
			inverse += digit * (float)(i % base);
			digit *= radical;
			i /= base;
		}
		return inverse;
	}

	inline static float halton(int dim, int index)
	{
		auto pi = dim % prim_nums.size();
		return radical_inverse(prim_nums[pi], index);
	}

	Color PathTracer::path_trace(const Ray& ray, Scene& scene, int depth)
	{
		auto ret = scene.intersect(ray);

		if (!ret.hit)
		{
			return Color::BLACK;
		}
		
		auto tint = Color(ret.material->get_float4(tint_color_prop));
		Color emission = sample_material(scene, ret.material, ray, ret);

		if (++depth > MAX_DEPTH)
		{
			float max_c = std::max(tint.r, std::max(tint.g, tint.b));
			if (rand01() < max_c)
			{
				tint /= max_c;
				return emission;
			}
			else
			{
				return emission;
			}
			return emission;
		}
		
		auto d = ray.direction.normalized();
		auto n = ret.normal.normalized();
		auto nl = Vector3::dot(n, d) < 0.0f ? n : -n;
		auto refl = (d - 2.0f * n * Vector3::dot(n, d)).normalized();
		auto reflect_ray = Ray(ret.pos, refl);

		if (ret.material->material_type == MaterialType::REFRACTION)
		{
			// refract
			bool into = Vector3::dot(n, nl) > 0.0f;
			float nc = 1.0f; 
			float nt = ret.material->get_float(refractiveness_prop);
			float nnt = into ? (nc / nt) : (nt / nc);
			float ddn = Vector3::dot(nl, d);
			float cos2t = 1.0f - nnt * nnt * (1.0f - ddn * ddn);

			if (cos2t < 0.0f)
			{
				return emission + tint * path_trace(reflect_ray, scene, depth);
			}
				
			auto tdir = (d * nnt - n * ((into ? 1.0f : -1.0f) * (ddn * nnt + std::sqrt(cos2t)))).normalized();
			float a = nt - nc;
			float b = nt + nc;
			float r0 = (a * a) / (b * b);
			float c = 1.0f - (into ? -ddn : Vector3::dot(tdir, n));
			float re = r0 + (1.0f - r0) * c * c * c * c * c;
			float tr = 1.0f - re;
			float p = 0.25f + 0.5f * re;
			float rp = re / p;
			float tp = tr / (1.0f - p);

			auto tray = Ray(ret.pos, tdir);
			if (depth <= 2)
			{
				return emission + tint * (path_trace(tray, scene, depth) * tr + path_trace(reflect_ray, scene, depth) * re);
			}
			else
			{
				if (rand01() < p)
				{
					return emission + tint * path_trace(reflect_ray, scene, depth) * rp;
				}
				else
				{
					return emission + tint * path_trace(tray, scene, depth) * tp;
				}
			}
		}
		else if (ret.material->material_type == MaterialType::REFLECTION)
		{
			float reflectiveness = ret.material->get_float(reflectiveness_prop);
			return emission * (1.0f - reflectiveness) + tint * reflectiveness * path_trace(reflect_ray, scene, depth);
		}
		else
		{
			float r1 = 2.0f * PI * rand01();
			float r2 = rand01();
			float r2s = std::sqrt(r2);
			auto w = nl;
			auto u = (Vector3::cross((std::abs(w.x) > 0.1f ? Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f)), w)).normalized();
			auto v = Vector3::cross(w, u);
			auto dir = (u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.0f - r2)).normalized();
			auto sr = Ray(ret.pos, dir);
			return emission + tint * path_trace(sr, scene, depth);
		}
	}

	void PathTracer::path_trace_tile(FrameTile& tile, Scene& scene)
	{
		for (uint32_t row = tile.row_start; row < tile.row_end; row++)
		{
			for (uint32_t col = tile.col_start; col < tile.col_end; col++)
			{
				float fov = 0.5f / std::tanf(scene.main_cam->fov * PI * 0.5f / 180.f);
				Color color = Color::BLACK;

				for (uint32_t sx = 0; sx < 2; sx++)
				{
					for (uint32_t sy = 0; sy < 2; sy++)
					{
						uint32_t sample_size = SAMPLE_SIZE / 4;
						float weight = 1.0f / sample_size;
						for (uint32_t s = 0; s < sample_size; s++)
						{
							float r1 = 2.0f * rand01() - 1.0f;

							float r2 = 2.0f * rand01() - 1.0f;

							r1 *= 0.25f;

							r2 *= 0.25f;

							float u = ((float)col + 0.5f + r1) / (float)(Window().width - 1) - 0.5f;
							float v = ((float)row + 0.5f + r2) / (float)(Window().height - 1) - 0.5f;

							Ray ray(scene.main_cam->position, Vector3::normalize(v * scene.main_cam->up + u * scene.main_cam->right + fov * scene.main_cam->forward));
							int depth = 0;
							color += path_trace(ray, scene, depth) * 0.25f * weight;
						}
					}
				}
				float exposure = scene.exposure;
				color = Color::WHITE - Color::exp(-color * exposure);
				//color = color / (color + Color::WHITE);
				color = Color::pow(color, 1.0f / 2.2f);
				color = Color::saturate(color);
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