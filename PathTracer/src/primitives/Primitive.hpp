#ifndef _PRIMITIVE_
#define _PRIMITIVE_
#include <Vector3.hpp>
#include <Ray.hpp>

namespace Guarneri
{
	struct Primitive
	{
	public:
		virtual std::shared_ptr<Material> get_material() = 0;
		virtual bool intersect(const Ray& ray, Vector3& hit_pos, float& t, Vector3& normal) = 0;
	};
}

#endif