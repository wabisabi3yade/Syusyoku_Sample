#pragma once

namespace HashiTaku
{
	/// @brief アニメーション間の補間種類
	enum AnimInterpolateKind
	{
		CrossFade,	// クロスフェード
		Inertialization	// 慣性補間
	};

	/// @brief キー間の補間の種類
	enum KeyInterpolateKind
	{
		Lerp,	// 線形
		Bezier,	// ベジェ
		Catmul	// Catmul
	};
}