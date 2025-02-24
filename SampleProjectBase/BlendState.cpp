#include "pch.h"
#include "BlendState.h"

namespace HashiTaku
{
    bool BlendState::Init(ID3D11Device& _pDevice, BlendStateType _blendType)
    {
        //// RenderTarget0へのAlphaブレンド描画設定
        //D3D11_BLEND_DESC BlendState;
        //ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
        //BlendState.AlphaToCoverageEnable = FALSE;
        //BlendState.IndependentBlendEnable = FALSE;
        //BlendState.RenderTarget[0].BlendEnable = TRUE;
        //BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;

        //// 背景色に掛け合わせる係数
        //BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        //BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        //BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        //BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        //BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        //BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        //auto hr = _pDevice.CreateBlendState(&BlendState, &pBlendState);
        //if (FAILED(hr)) // 失敗
        //{
        //    return false;
        //}

        //return true;

        // RenderTarget0へのAlphaブレンド描画設定
        D3D11_BLEND_DESC blendStateDesc;
        ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

        // 作成するブレンドステートの種類で設定を変更
        switch (_blendType)
        {
        case BlendStateType::None:  // なにもなしは作成しない
            return true;

        case BlendStateType::Alpha:
            CreateAlphaBlendDesc(blendStateDesc);
            break;
        case BlendStateType::Additive:
            CreateAdditiveBlendDesc(blendStateDesc);
            break;
        case BlendStateType::Multiply:
            CreateMultiplyBlendDesc(blendStateDesc);
            break;
        default:
            assert(!"ブレンドステート作成でエラー");
            break;
        }

        // ブレンドステートを作成
        auto hr = _pDevice.CreateBlendState(&blendStateDesc, pBlendState.GetAddressOf());
        if (FAILED(hr)) // 失敗
        {
            assert(!"ブレンドステート作成失敗");
            return false;
        }

        return true;
    }

    void BlendState::CreateAlphaBlendDesc(D3D11_BLEND_DESC& _desc)
    {
        _desc.RenderTarget[0].BlendEnable = TRUE;
        _desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        _desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        _desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        _desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    void BlendState::CreateAdditiveBlendDesc(D3D11_BLEND_DESC& _desc)
    {
        _desc.RenderTarget[0].BlendEnable = TRUE;
        _desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    void BlendState::CreateMultiplyBlendDesc(D3D11_BLEND_DESC& _desc)
    {
        _desc.RenderTarget[0].BlendEnable = TRUE;
        _desc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
        _desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        _desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        _desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        _desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        _desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
}