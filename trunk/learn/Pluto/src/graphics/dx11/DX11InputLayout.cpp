#include <vector>
#include <graphics/dx11/DX11InputLayout.h>
using namespace plu;

DX11InputLayout::~DX11InputLayout(){
	if(mLayout){
		mLayout->Release();
		mLayout = nullptr;
	}
}

DX11InputLayout::DX11InputLayout(ID3D11Device* device, VertexBuffer* vbuffer, Shader* vshader){
	mLayout = nullptr;
	mNumElements = 0;
    memset(&mElements[0], 0, VA_MAX_ATTRIBUTES*sizeof(mElements[0]));

    if (vbuffer && vshader){
        VertexFormat& format = vbuffer->GetFormat();
        mNumElements = format.getAttributesNum();
        for (int i = 0; i < mNumElements; ++i){
            VASemantic semantic;
            DFType type;
            int unit, offset;
            format.setAttribute(i, semantic, type, unit, offset);

            D3D11_INPUT_ELEMENT_DESC& element = mElements[i];
            element.SemanticName = msSemantic[semantic];
            element.SemanticIndex = unit;
            element.Format = static_cast<DXGI_FORMAT>(type);
            element.InputSlot = 0;  // TODO: Streams not yet supported.
            element.AlignedByteOffset = offset;
            element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            element.InstanceDataStepRate = 0;
        }

        std::vector<char>& compiledCode =
            vshader->GetCompiledCode();
        HRESULT hr = device->CreateInputLayout(mElements, (UINT)mNumElements,
            &compiledCode[0], compiledCode.size(), &mLayout);
    }
}

void DX11InputLayout::Enable(ID3D11DeviceContext* context){
    if (mLayout){
        context->IASetInputLayout(mLayout);
    }
}

void DX11InputLayout::Disable(ID3D11DeviceContext* context){
    if (mLayout){
        // TODO: Verify that mLayout is the active input layout.
        context->IASetInputLayout(nullptr);
    }
}

HRESULT DX11InputLayout::SetName(std::string& name){
    mName = name;
    //return SetPrivateName(mLayout, mName);
	return 0;
}

std::string& DX11InputLayout::GetName(){
    return mName;
}



char const* DX11InputLayout::msSemantic[VA_NUM_SEMANTICS] = {
    "",
    "POSITION",
    "BLENDWEIGHT",
    "BLENDINDICES",
    "NORMAL",
    "PSIZE",
    "TEXCOORD",
    "TANGENT",
    "BINORMAL",
    "TESSFACTOR",
    "POSITIONT",
    "COLOR",
    "FOG",
    "DEPTH",
    "SAMPLE"
};
