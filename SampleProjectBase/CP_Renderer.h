#pragma once
#include "Component.h"

namespace HashiTaku
{
	// メッシュ
	class SingleMesh;

	class VertexShader;
	class PixelShader;

	// 描画コンポーネントのベース
	class CP_Renderer : public Component
	{
	protected:

		/// @brief メッシュを描画
		/// @param _mesh 描画するメッシュ
		void DrawMesh(const SingleMesh& _mesh);
	public:
		CP_Renderer() {}
		~CP_Renderer() {}

		/// @brief 各レンダラーの描画
		virtual void Draw() {};
	};
}