#pragma once
#include "Component.h"
#include "CP_PostEffect.h"

namespace HashiTaku
{
    class CP_MeshRenderer;

    /// @brief モーションブラーをかけるコンポーネント
    class CP_MotionBlur : public CP_PostEffect
    {
       /// @brief 前フレームのメッシュのスクリーン座標行列
       RenderParam::WVP prevDrawMeshWVP;

       /// @brief メッシュ描画クラス
       CP_MeshRenderer* pMeshRenderer;

       /// @brief ブラーを描画する
       bool isDrawingBlur;
    public:
        CP_MotionBlur();
        ~CP_MotionBlur() {}

        /// @brief 開始
        void Start() override;

        /// @brief 描画
        void Draw() override;

        /// @brief ブラーを描画
        /// @param _drawBlur ブラー描画
        void SetDrawBlur(bool _drawBlur);

    private:
        /// @brief レンダーターゲットにブラーのベクトルを書く
        /// @param _curDrawMeshWVP 現在のメッシュのWVP
        void DrawBlur(const RenderParam::WVP& _curDrawMeshWVP);
    };
}


