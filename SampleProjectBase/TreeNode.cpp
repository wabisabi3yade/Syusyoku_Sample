#include "pch.h"
#include "TreeNode.h"

#include "Bone.h"

namespace HashiTaku
{
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

    const TreeNode* TreeNode::FindTreeNode(int _boneIdx) const
    {
        // ボーンがあり　かつ　インデックスが一致しているなら
        if (pLinkBone != nullptr && pLinkBone->GetIndex())
        {
            return this;
        }

        // 子ノードから探す
        for (const auto& child : pChildNodes)
        {
            const TreeNode* pFindBone = child->FindTreeNode(_boneIdx);

            if (pFindBone == nullptr) continue;
            return pFindBone;
        }

        HASHI_DEBUG_LOG("ボーンIDに対応したノードが見つかりませんでした");
        return nullptr;
    }

    void TreeNode::SetNodeName(const std::string& _nodeName)
    {
        nodeName = _nodeName;
    }

    void TreeNode::SetTransformMtx(const DXSimp::Matrix& _transformMtx)
    {
        transformMtx = _transformMtx;
    }

    void TreeNode::SetBone(Bone& _bone)
    {
        pLinkBone = &_bone;
        pLinkBone->SetLocalNodeMtx(transformMtx);
    }

    const TreeNode* TreeNode::GetChild(u_int _arrayIdx) const
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

    const std::string& TreeNode::GetName() const
    {
        return nodeName;
    }

    const Bone* TreeNode::GetBone() const
    {
        return pLinkBone;
    }

    u_int TreeNode::GetBoneIdx() const
    {
        return pLinkBone->GetIndex();
    }

    const DXSimp::Matrix& TreeNode::GetTransformMtx() const
    {
        return transformMtx;
    }
}
