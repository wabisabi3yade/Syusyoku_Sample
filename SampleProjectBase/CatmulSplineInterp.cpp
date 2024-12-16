#include "pch.h"
#include "CatmulSplineInterp.h"


namespace HashiTaku
{
    using namespace DXSimp;

    Vector3 CatmulSplineInterp::CalcVector3(const Vector3& _from, const Vector3& _to, const Vector3& _prev, const Vector3& _next, float _ratio)
    {
        // fromとtoまでの間の制御点2点
        Vector3 a = 2.0f * _from;
        Vector3 b = _to - _prev;
        Vector3 c = 2.0f * _prev - 5.0f * _from + 4.0f * _to - _next;
        Vector3 d = -_prev + 3.0f * _from - 3.0f * _to + _next;

        return 0.5f * (a + (b * _ratio) + (c * _ratio * _ratio) + (d * _ratio * _ratio * _ratio));
    }

    DirectX::SimpleMath::Quaternion CatmulSplineInterp::CalcQuaternion(const DirectX::SimpleMath::Quaternion& _from, const DirectX::SimpleMath::Quaternion& _to, const DirectX::SimpleMath::Quaternion& _prev, const DirectX::SimpleMath::Quaternion& _next, float _ratio)
    {
        // 接線ベクトルの計算
        Quaternion T1 = Quat::Multiply(Quaternion::Slerp(_prev, _to, 0.5f), XMQuaternionConjugate(_from));
        Quaternion T2 = Quat::Multiply(Quaternion::Slerp(_from, _next, 0.5f), XMQuaternionConjugate(_to));

        // クォータニオンの補間
        Quaternion _from__to = Quaternion::Slerp(_from, _to, _ratio);
        Quaternion T1_T2 = Quaternion::Slerp(Quat::Multiply(_from, T1), Quat::Multiply(_to, T2), _ratio);

        // 最終的なCatmull-Rom補間
        return Quaternion::Slerp(_from__to, T1_T2, _ratio);
    }
}