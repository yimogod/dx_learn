#pragma once
#include <graphics/DrawingState.h>

namespace plu{

class RasterizerState : public DrawingState{
public:
    enum FillMode{
        FILL_SOLID,
        FILL_WIREFRAME
    };

    enum CullMode{
        CULL_NONE,
        CULL_FRONT,
        CULL_BACK
    };

    // Construction.
    RasterizerState();

    // Member access.  The members are intended to be write-once before
    // you create an associated graphics state.
    FillMode fillMode;              // default: FILL_SOLID
    CullMode cullMode;              // default: CULL_BACK
    bool frontCCW;                  // default: true
    int depthBias;                  // default: 0
    float depthBiasClamp;           // default: 0
    float slopeScaledDepthBias;     // default: 0
    bool enableDepthClip;           // default: true
    bool enableScissor;             // default: false
    bool enableMultisample;         // default: false
    bool enableAntialiasedLine;     // default: false
};

}
