#pragma once

/// @brief  慣性補間クラス
class InertialInterpolation
{
public:
    /// @brief 遷移情報
    struct TranslateBase
    {
        float x0;   // 初期差分
        float v0;   // 初期速度
        float t1;   // 遷移終了の時間
        float a0;   // 初期加速度
        float A;
        float B;
        float C;
    };

    
  /*  static TranslateBase InitTransition(const DirectX::SimpleMath::Vector3& _curValue, const DirectX::SimpleMath::Vector3& _nextValue, float _blendTime, float _deltaTime);*/
};

