#pragma once

namespace plu{
	enum GraphicsObjectType{
		GT_GRAPHICS_OBJECT,  // abstract
			GT_RESOURCE,  // abstract
				GT_BUFFER,  // abstract
					GT_CONSTANT_BUFFER,
					GT_TEXTURE_BUFFER,
					GT_VERTEX_BUFFER,
					GT_INDEX_BUFFER,
					GT_STRUCTURED_BUFFER,
					GT_TYPED_BUFFER,
					GT_RAW_BUFFER,
					GT_INDIRECT_ARGUMENTS_BUFFER,
				GT_TEXTURE,  // abstract
					GT_TEXTURE_SINGLE,  // abstract
						GT_TEXTURE1,
						GT_TEXTURE2,
						GT_TEXTURE_RT,
						GT_TEXTURE_DS,
						GT_TEXTURE3,
					GT_TEXTURE_ARRAY,  // abstract
						GT_TEXTURE1_ARRAY,
						GT_TEXTURE2_ARRAY,
						GT_TEXTURE_CUBE,
						GT_TEXTURE_CUBE_ARRAY,
				GT_SHADER,  // abstract
					GT_VERTEX_SHADER,
					GT_GEOMETRY_SHADER,
					GT_PIXEL_SHADER,
					GT_COMPUTE_SHADER,
				GT_DRAWING_STATE,  // abstract
					GT_SAMPLER_STATE,
					GT_BLEND_STATE,
					GT_DEPTH_STENCIL_STATE,
					GT_RASTERIZER_STATE,
				GT_NUM_TYPES
	};

	class GraphicsObject{
	public:
		GraphicsObject();
		~GraphicsObject();

		bool isBuffer();
		bool isTexture();
		bool isTextureArray();
		bool isShader();
		bool isDrawingState();

		GraphicsObjectType GetType();

	protected:
		GraphicsObjectType _graphType;
	};


}