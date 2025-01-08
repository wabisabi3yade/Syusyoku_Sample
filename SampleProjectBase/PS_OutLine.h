#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
    class PS_OutLine : public PixelShader
    {

    public:
        PS_OutLine();
        ~PS_OutLine() {}
        
        void OnceUpdateBuffer() override;
    };
}


