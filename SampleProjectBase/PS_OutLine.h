#pragma once
#include "PixelShader.h"

namespace HashiTaku
{
    class PS_OutLine : public PixelShader
    {

    public:
        PS_OutLine();
        ~PS_OutLine() {}

        /// @brief  �K�v�ȃA�Z�b�g��p��
        void InitAssets() override;
        
        void OnceUpdateBuffer() override;
    };
}


