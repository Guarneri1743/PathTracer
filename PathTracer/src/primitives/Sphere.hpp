#ifndef _SPHERE_
#define _SPHERE_
#include <PathTracer.hpp>

namespace Guarneri
{
	struct Sphere : public Primitive
	{
	public:
		Guarneri::Vector3 center;
		float radius;
		std::shared_ptr<Material> material;

	public:
		Sphere();
		Sphere(std::shared_ptr<Material> mat);
		Sphere(const Guarneri::Vector3& _center, const float& _radius);
		Sphere(const Sphere& other);
		std::shared_ptr<Material> get_material();
		bool intersect(const Ray& ray, Vector3& hit_pos, float& t, Vector3& normal);
		std::string str() const;
	};

	Sphere::Sphere(std::shared_ptr<Material> mat)
	{
		this->material = mat;
		center = Guarneri::Vector3();
		radius = 0;
	}

	std::shared_ptr<Material> Sphere::get_material()
	{
		return this->material;
	}

	Sphere::Sphere()
	{
		center = Guarneri::Vector3();
		radius = 0;
	}

	Sphere::Sphere(const Guarneri::Vector3& _center, const float& _radius)
	{
		this->center = _center;
		this->radius = _radius;
	}

	Sphere::Sphere(const Sphere& other)
	{
		this->center = other.center;
		this->radius = other.radius;
	}

	bool Sphere::intersect(const Ray& ray, Vector3& hit_pos, float& t, Vector3& normal)
	{
		auto op = center - ray.origin;
		float eps = 1e-4;
		float b = Vector3::dot(op, ray.direction);
		float det = b * b - Vector3::dot(op, op) + radius * radius;
		if (det < 0.0f)
		{
			t = 0.0f;
			hit_pos = ray.origin + ray.direction * t;
			normal = (hit_pos - center).normalized();
			return false;
		}
		det = std::sqrt(det);
		if (b - det > eps)
		{
			t = b - det;
			hit_pos = ray.origin + ray.direction * t;
			normal = (hit_pos - center).normalized();
			return true;
		}
		if (b + det > eps)
		{
			t = b + det;
			hit_pos = ray.origin + ray.direction * t;
			normal = (hit_pos - center).normalized();
			return true;
		}
		t = 0.0f;
		hit_pos = ray.origin + ray.direction * t;
		normal = (hit_pos - center).normalized();
		return false;
	}

	std::string Sphere::str() const
	{
		std::stringstream ss;
		ss << "Sphere: [origin: " << this->center << ", radius: " << std::setprecision(FLOAT_LOG_PRECISION) << this->radius << "]";
		return ss.str();
	}

	static std::ostream& operator << (std::ostream& stream, const Sphere& Sphere)
	{
		stream << Sphere.str();
		return stream;
	}

	static std::stringstream& operator << (std::stringstream& stream, const Sphere& Sphere)
	{
		stream << Sphere.str();
		return stream;
	}
}
#endif