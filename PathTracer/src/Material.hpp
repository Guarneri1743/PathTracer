#ifndef _MATERIAL_
#define _MATERIAL_
#include <PathTracer.hpp>

namespace Guarneri
{
	class Material : public Object
	{
	public:
		std::unordered_map<property_name, float> name2float;
		std::unordered_map<property_name, Vector4> name2float4;
		std::unordered_map<property_name, int> name2int;
		std::unordered_map<property_name, std::shared_ptr<Texture>> name2tex;
		MaterialType material_type;

	public:
		Material();
		~Material();

		static std::unique_ptr<Material> create();
		static std::unique_ptr<Material> create(const Material& other);
		void set_int(const property_name& name, const int& val);
		void set_float4(const property_name& name, const Vector4& val);
		void set_float(const property_name& name, const float& val);
		void set_texture(const property_name& name, std::shared_ptr<Texture> tex);
		int get_int(const property_name& name) const;
		Vector4 get_float4(const property_name& name) const;
		float get_float(const property_name& name) const;
		std::shared_ptr<Texture> get_texture(const property_name& name) const;
		std::string str() const;
	};


	Material::Material()
	{
	}

	Material::~Material()
	{}

	std::unique_ptr<Material> Material::create()
	{
		return std::make_unique<Material>();
	}

	std::unique_ptr<Material> Material::create(const Material& other)
	{
		return std::make_unique<Material>(other);
	}

	void Material::set_int(const property_name& name, const int& val)
	{
		name2int[name] = val;
	}

	void Material::set_float4(const property_name& name, const Vector4& val)
	{
		name2float4[name] = val;
	}

	void Material::set_float(const property_name& name, const float& val)
	{
		name2float[name] = val;
	}

	void Material::set_texture(const property_name& name, std::shared_ptr<Texture> tex)
	{
		if (tex == nullptr)
		{
			return;
		}
		name2tex[name] = tex;
	}

	int Material::get_int(const property_name& name) const
	{
		if (name2int.count(name) > 0)
		{
			return name2int.at(name);
		}
		return 0;
	}

	Vector4 Material::get_float4(const property_name& name) const
	{
		if (name2float4.count(name) > 0)
		{
			return name2float4.at(name);
		}
		return 0;
	}

	float Material::get_float(const property_name& name) const
	{
		if (name2float.count(name) > 0)
		{
			return name2float.at(name);
		}
		return 0;
	}

	std::shared_ptr<Texture> Material::get_texture(const property_name& name) const
	{
		if (name2tex.count(name) > 0)
		{
			return name2tex.at(name);
		}
		return nullptr;
	}

	std::string Material::str() const
	{
		std::stringstream ss;
		ss << "Material[" << this->id << "]";
		return ss.str();
	}
}
#endif