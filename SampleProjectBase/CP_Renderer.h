#pragma once
#include "Component.h"

// メッシュ
class SingleMesh;

// 描画コンポーネントのベース
class CP_Renderer :
    public Component
{
protected:

    /// @brief メッシュを描画
    /// @param _mesh 描画するメッシュ
    void DrawMesh(const SingleMesh& _mesh);
public:
    using Component::Component;

    /// @brief 各レンダラーの描画
    virtual void Draw() = 0;
};

