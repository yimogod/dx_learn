#pragma once
#include <vector>
#include <graphics/dx11/DX11InputLayout.h>

namespace plu{
	class DX11InputLayoutManager{
	public:
		~DX11InputLayoutManager();
		DX11InputLayoutManager();

		// Management functions.  The Unbind(vbuffer) removes all pairs that
		// involve vbuffer.  The Unbind(vshader) removes all pairs that involve
		// vshader.
		DX11InputLayout* Bind(ID3D11Device* device, VertexBuffer const* vbuffer,
			Shader const* vshader);
		bool Unbind(VertexBuffer const* vbuffer, Shader const* vshader);
		bool Unbind(VertexBuffer const* vbuffer);
		bool Unbind(Shader const* vshader);
		void UnbindAll();
		DX11InputLayout* Get(VertexBuffer const* vbuffer, Shader const* vshader)
			const;
		bool HasElements() const;

	private:
		typedef std::pair<VertexBuffer const*, Shader const*> VBSPair;

		class LayoutMap : public ThreadSafeMap<VBSPair, DX11InputLayout*>{
		public:
			virtual ~LayoutMap();
			LayoutMap();

			void GatherMatch(VertexBuffer const* vbuffer,
				std::vector<VBSPair>& matches);

			void GatherMatch(Shader const* vshader,
				std::vector<VBSPair>& matches);
		};

		LayoutMap mMap;
	};

}
