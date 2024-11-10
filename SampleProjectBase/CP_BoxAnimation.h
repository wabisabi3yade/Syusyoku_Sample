#pragma once
#include "CloneComponent.h"
class CP_BoxAnimation :
    public Component, public CloneComponent<CP_BoxAnimation> 
{
    /// @brief �J�ڏ��
    struct TransitionBase
    {
        float x0;   // ��������
        float v0;   // �������x
        float t1;   // �J�ڏI���̎���
        float a0;   // ���������x
        float A;
        float B;
        float C;
        DirectX::SimpleMath::Vector3 axis;  // ���x�N�g��
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

