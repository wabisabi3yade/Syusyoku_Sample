#pragma once

/// @brief  ������ԃN���X
class InertialInterpolation
{
public:
    /// @brief �J�ڏ��
    struct TranslateBase
    {
        float x0;   // ��������
        float v0;   // �������x
        float t1;   // �J�ڏI���̎���
        float a0;   // ���������x
        float A;
        float B;
        float C;
    };

    
  /*  static TranslateBase InitTransition(const DirectX::SimpleMath::Vector3& _curValue, const DirectX::SimpleMath::Vector3& _nextValue, float _blendTime, float _deltaTime);*/
};

