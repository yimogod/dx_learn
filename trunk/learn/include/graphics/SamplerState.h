#pragma once

#include <math/algebra/Vector4D.h>
#include <graphics/DrawingState.h>

namespace plu{
	class SamplerState : public DrawingState{
	public:
		// The encoding involves minification (MIN), magnification (MAG), and
		// mip-level filtering (MIP).  After each is P (POINT) or L (LINEAR).
		enum Filter{
			MIN_P_MAG_P_MIP_P,
			MIN_P_MAG_P_MIP_L,
			MIN_P_MAG_L_MIP_P,
			MIN_P_MAG_L_MIP_L,
			MIN_L_MAG_P_MIP_P,
			MIN_L_MAG_P_MIP_L,
			MIN_L_MAG_L_MIP_P,
			MIN_L_MAG_L_MIP_L,
			ANISOTROPIC,
			COMPARISON_MIN_P_MAG_P_MIP_P,
			COMPARISON_MIN_P_MAG_P_MIP_L,
			COMPARISON_MIN_P_MAG_L_MIP_P,
			COMPARISON_MIN_P_MAG_L_MIP_L,
			COMPARISON_MIN_L_MAG_P_MIP_P,
			COMPARISON_MIN_L_MAG_P_MIP_L,
			COMPARISON_MIN_L_MAG_L_MIP_P,
			COMPARISON_MIN_L_MAG_L_MIP_L,
			COMPARISON_ANISOTROPIC
		};

		// Modes for handling texture coordinates at texture-image boundaries.
		enum Mode{
			WRAP,
			MIRROR,
			CLAMP,
			BORDER,
			MIRROR_ONCE
		};

		enum Comparison{
			NEVER,
			LESS,
			EQUAL,
			LESS_EQUAL,
			GREATER,
			NOT_EQUAL,
			GREATER_EQUAL,
			ALWAYS
		};

		SamplerState();

		Filter filter;
		Mode mode[3];
		float mipLODBias;
		unsigned int maxAnisotropy;
		Comparison comparison;
		Vector4D borderColor;
		float minLOD;
		float maxLOD;

	public:
		// For use by the Shader class for storing reflection information.
		static int const shaderDataLookup = 6;
	};

}
