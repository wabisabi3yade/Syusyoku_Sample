#include "pch.h"
#include "TreeNode.h"

#include "Bone.h"

void TreeNode::SetParent(TreeNode& _parentNode)
{
    pParentNode = &_parentNode;
}

void TreeNode::AddChild(std::unique_ptr<TreeNode> _chiledNode)
{
    _chiledNode->SetParent(*this);

    pChildNodes.push_back(std::move(_chiledNode));
}

void TreeNode::RemoveChiled(TreeNode& _chiledNode)
{
    for (auto& n : pChildNodes)
    {
        if (&_chiledNode == n.get())
        {
            pChildNodes.remove(n);
            return;
        }
    }

    HASHI_DEBUG_LOG(_chiledNode.GetName() + "は子ノードにありませんでした");
}

bool TreeNode::HasBone() const
{
    return pLinkBone != nullptr;
}

void TreeNode::SetNodeName(const std::string& _nodeName)
{
    nodeName = _nodeName;
}

void TreeNode::SetTransformMtx(const DirectX::SimpleMath::Matrix& _transformMtx)
{
    transformMtx = _transformMtx;
}

void TreeNode::SetBone(Bone& _bone)
{
    pLinkBone = &_bone;
}

TreeNode* TreeNode::GetChild(u_int _arrayIdx)
{
    if (_arrayIdx >= GetChildNum())
    {
        assert(!"要素が大きすぎます");
        return nullptr;
    }

    auto itr = pChildNodes.begin();

    for (u_int a_i = 0; a_i < _arrayIdx; a_i++)
        itr++;

    return (*itr).get();
}

u_int TreeNode::GetChildNum() const
{
    u_int u = static_cast<u_int>(pChildNodes.size());

    return static_cast<u_int>(pChildNodes.size());
}

std::string TreeNode::GetName() const
{
    return nodeName;
}

Bone& TreeNode::GetBone()
{
    return *pLinkBone;
}

DirectX::SimpleMath::Matrix TreeNode::GetTransformMtx() const
{
    return transformMtx;
}
