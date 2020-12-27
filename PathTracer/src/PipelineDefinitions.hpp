#ifndef _PIPELINE_DEF_
#define _PIPELINE_DEF_

namespace Guarneri{
	#define LEFT_HANDED

	enum class ColorSpace
	{
		Gamma,
		Linear
	};

	enum class PBRWorkFlow
	{
		Metallic,
		Specular
	};

	enum class Projection {
		PERSPECTIVE,
		ORTHO
	};

	enum class WrapMode {
		REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class Filtering {
		POINT,
		BILINEAR,
		MAX,
		MIN
	};

	enum class TextureFormat {
		INVALID,
		rgb,
		rgba,
		rg,
		r32
	};

	enum class MaterialType
	{
		SCATTER,
		REFLECTION,
		REFRACTION
	};

#ifdef OPAQUE
#undef OPAQUE
#endif
#ifdef TRANSPARENT
#undef TRANSPARENT
#endif
}

#endif