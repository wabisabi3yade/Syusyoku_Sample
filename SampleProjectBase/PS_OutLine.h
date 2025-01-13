#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
    class PS_OutLine : public PixelShader
    {

    public:
        PS_OutLine();
        ~PS_OutLine() {}

        /// @brief  必要なアセットを用意
        void InitAssets() override;
        
        void OnceUpdateBuffer() override;
    };
}


