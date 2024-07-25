#include "pch.h"
#include "NodeAnim.h"

NodeAnim::NodeAnim()
{
}

NodeAnim::~NodeAnim()
{
}

DirectX::SimpleMath::Vector3 NodeAnim::GetScaling(float _playTime) const
{
    return DirectX::SimpleMath::Vector3();
}

DirectX::SimpleMath::Quaternion NodeAnim::GetQuaternion(float _playTime) const
{
    return DirectX::SimpleMath::Quaternion();
}

DirectX::SimpleMath::Vector3 NodeAnim::GetPosition(float _playTime) const
{
    return DirectX::SimpleMath::Vector3();
}

void NodeAnim::SetBoneIdx(u_int _boneIdx)
{
    boneIdx = _boneIdx;
}

u_int NodeAnim::GetBodeIdx() const
{
    return boneIdx;
}

u_int NodeAnim::FindPreviousScaleKey(float _playingTime)
{
    return u_int();
}

u_int NodeAnim::FindPreviousRotKey(float _playingTime)
{
    return u_int();
}

u_int NodeAnim::FindPreviousPosKey(float _playingTime)
{
    return u_int();
}
