#pragma once
#include "CloneComponent.h"
class CP_BoxAnimation :
    public Component, public CloneComponent<CP_BoxAnimation> 
{
    /// @brief 遷移情報
    struct TransitionBase
    {
        float x0;   // 初期差分
        float v0;   // 初期速度
        float t1;   // 遷移終了の時間
        float a0;   // 初期加速度
        float A;
        float B;
        float C;
        DirectX::SimpleMath::Vector3 axis;  // 軸ベクトル
    };

    TransitionBase transition;

    DirectX::SimpleMath::Vector3 requestPose;

    DirectX::SimpleMath::Quaternion lastQuat;
    float lastTime;
    DirectX::SimpleMath::Quaternion secondQuat;

    float secondTime;

    bool isStop = false;

    DirectX::SimpleMath::Quaternion transQuat;

    DirectX::SimpleMath::Vector3 vec;

    float dis = 3.0f;

    bool isTransition = false;

    float blendTime = 1.0f;
    float culElapsedBlend = 0.0f;
public:
    void ImGuiDebug() override;

    void Update() override;

    void Draw() override;

    nlohmann::json Save() override;
    void Load(const nlohmann::json& _data) override;

private:
    void Transition();

    void TransitionStart();

    float CalcBlend(TransitionBase trans, float blendTime);
};

