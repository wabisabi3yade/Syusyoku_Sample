#include "pch.h"
#include "TreeNode.h"

void TreeNode::SetParent(TreeNode& _parentNode)
{
    pParentNode = &_parentNode;
}

void TreeNode::AddChiled(TreeNode& _chiledNode)
{
    for (auto& n : pChildNodes)
    {
        if (n == &_chiledNode)
        {
            HASHI_DEBUG_LOG(_chiledNode.GetName() + "���Ƀm�[�h�͓����Ă��܂�");
            return;
        }
    }

    pChildNodes.push_back(&_chiledNode);
}

void TreeNode::RemoveChiled(TreeNode& _chiledNode)
{
    for (auto& n : pChildNodes)
    {
        if (n == &_chiledNode)
        {
            pChildNodes.remove(&_chiledNode);
            return;
        }
    }

    HASHI_DEBUG_LOG(_chiledNode.GetName() + "�͎q�m�[�h�ɂ���܂���ł���");
}

u_int TreeNode::GetChildNum() const
{
    return static_cast<u_int>(pChildNodes.size());
}

std::string TreeNode::GetName() const
{
    return nodeName;
}
