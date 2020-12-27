#ifndef _PRIMITIVE_FACTORY_
#define _PRIMITIVE_FACTORY_
#include <PathTracer.hpp>

namespace Guarneri
{
	class PrimitiveFactory
	{
	public:
		static std::unique_ptr<Model> cube(std::unique_ptr<Material>& material);
	};

	Vertex cube_vertices[36] = {
		Vertex(Vector4(-1.0f, -1.0f, -1.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f), Vector2(0.0f,  0.0f)),
		Vertex(Vector4(-1.0f,-1.0f, 1.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f), Vector2(1.0f,  1.0f)),
		Vertex(Vector4(1.0f, 1.0f,-1.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f), Vector2(1.0f,  1.0f)),
		Vertex(Vector4(-1.0f,-1.0f,-1.0f, 1.0f), Vector3(0.0f,  0.0f, -1.0f), Vector2(0.0f,  1.0f)),
		Vertex(Vector4(-1.0f, 1.0f,-1.0f,  1.0f), Vector3(0.0f,  0.0f, -1.0f), Vector2(0.0f,  0.0f)),

		Vertex(Vector4(1.0f,-1.0f, 1.0f, 1.0f), Vector3(0.0f,  0.0f, 1.0f), Vector2(0.0f,  0.0f)),
		Vertex(Vector4(-1.0f,-1.0f,-1.0f, 1.0f), Vector3(0.0f,  0.0f, 1.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(1.0f,-1.0f,-1.0f, 1.0f), Vector3(0.0f,  0.0f, 1.0f), Vector2(1.0f,  1.0f)),
		Vertex(Vector4(1.0f, 1.0f,-1.0f, 1.0f), Vector3(0.0f,  0.0f, 1.0f), Vector2(1.0f, 1.0f)),
		Vertex(Vector4(1.0f,-1.0f,-1.0f, 1.0f), Vector3(0.0f,  0.0f, 1.0f), Vector2(0.0f, 1.0f)),
		Vertex(Vector4(-1.0f,-1.0f,-1.0f, 1.0f), Vector3(0.0f,  0.0f, 1.0f), Vector2(0.0f,  0.0f)),

		Vertex(Vector4(-1.0f,-1.0f,-1.0f, 1.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector2(1.0f,  1.0f)),
		Vertex(Vector4(-1.0f, 1.0f,-1.0f, 1.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector2(0.0f,  1.0f)),
		Vertex(Vector4(1.0f,-1.0f, 1.0f, 1.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector2(0.0f,  1.0f)),
		Vertex(Vector4(-1.0f,-1.0f, 1.0f, 1.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector2(0.0f,  0.0f)),
		Vertex(Vector4(-1.0f,-1.0f,-1.0f, 1.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector2(1.0f,  0.0f)),

		Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(-1.0f,-1.0f, 1.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f), Vector2(1.0f,  1.0f)),
		Vertex(Vector4(1.0f,-1.0f, 1.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f), Vector2(0.0f,  1.0f)),
		Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f), Vector2(0.0f,  1.0f)),
		Vertex(Vector4(1.0f,-1.0f,-1.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f), Vector2(0.0f,  0.0f)),
		Vertex(Vector4(1.0f, 1.0f,-1.0f, 1.0f), Vector3(1.0f,  0.0f,  0.0f), Vector2(1.0f,  0.0f)),

		Vertex(Vector4(1.0f,-1.0f,-1.0f, 1.0f), Vector3(0.0f, -1.0f,  0.0f), Vector2(0.0f,  1.0f)),
		Vertex(Vector4(1.0f, 1.0f, 1.0f,1.0f), Vector3(0.0f, -1.0f,  0.0f), Vector2(1.0f,  1.0f)),
		Vertex(Vector4(1.0f,-1.0f, 1.0f, 1.0f), Vector3(0.0f, -1.0f,  0.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, -1.0f,  0.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(1.0f, 1.0f,-1.0f,1.0f), Vector3(0.0f, -1.0f,  0.0f), Vector2(0.0f,  0.0f)),
		Vertex(Vector4(-1.0f, 1.0f,-1.0f, 1.0f), Vector3(0.0f, -1.0f,  0.0f), Vector2(0.0f,  1.0f)),

		Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f,  0.0f), Vector2(0.0f,  1.0f)),
		Vertex(Vector4(-1.0f, 1.0f,-1.0f, 1.0f), Vector3(0.0f, 1.0f,  0.0f), Vector2(1.0f,  1.0f)),
		Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f,  0.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f,  0.0f), Vector2(1.0f,  0.0f)),
		Vertex(Vector4(-1.0f, 1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f,  0.0f), Vector2(0.0f,  0.0f)),
		Vertex(Vector4(1.0f,-1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f,  0.0f), Vector2(0.0f,  1.0f)),
	};

	uint32_t cube_indices[36] = {
		0, 1, 2, 3, 4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21, 22, 23,
		24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35,
	};

	Vector3 sphere_parametric_equation(const float& radius, const float& alpha, const float& phi)
	{
		Vector3 p;
		p.x = radius * std::sin(alpha * PI / 180.0f) * std::cos(phi * PI / 180.0f);
		p.y = radius * std::sin(alpha * PI / 180.0f) * std::sin(phi * PI / 180.0f);
		p.z = radius * std::cos(alpha * PI / 180.0f);
		return p;
	}

	std::unique_ptr<Model> PrimitiveFactory::cube(std::unique_ptr<Material>& material)
	{
		const std::vector<Vertex> vert(cube_vertices, cube_vertices + 36);
		const std::vector<uint32_t> ind(cube_indices, cube_indices + 36);
		return Model::create(vert, ind, material);
	}
}
#endif