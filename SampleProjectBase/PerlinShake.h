#pragma once
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	/// @brief パーリンノイズでシェイクするベクトルを計算するクラス
	class PerlinShake
	{
		/// @brief 現在揺らしているパラメータ
		PerlinShakeParameter curShakeParameter;

		/// @brief 乱数で決めたパーリンノイズのオフセット値(毎回変わる)
		DXSimp::Vector3 randPerlinOffset;

		/// @brief 計算して求めたシェイクのオフセット
		DXSimp::Vector3 shakeOffset;

		/// @brief 揺らしてからの経過時間
		float shakeElapsedTime;

		/// @brief 揺れているか？
		bool isShaking;
	public:
		PerlinShake();
		~PerlinShake() = default;

		void Update(float _deltaTime);

		/// @brief 揺れを開始する
		/// @param _vector 揺れの方向
		/// @param _power 揺れの力
		/// @param _time 揺れ時間(0以下は永続)
		/// @param _speed 揺らす速度
		/// @param _isFadeOut フェードアウトさせていくか？
		void BeginShake(const DXSimp::Vector3& _vector,
			float _power,
			float _time, 
			float _speed,
			bool _isFadeOut = true);

		/// @brief 揺れを開始する
		/// @param _shakeParam カメラ揺れパラメータ
		void BeginShake(const PerlinShakeParameter& _shakeParam);

		/// @brief シェイクを終わらせる
		void EndShake();

		/// @brief 揺れているか取得する
		/// @return 揺れているフラグ
		bool GetIsShaking() const;

		const DXSimp::Vector3& GetShakeOffset() const;
	private:
		/// @brief 揺れのオフセット値を取得する
		/// @param _outVector 求めたカメラの揺れベクトル
		void CalcShakeOffset(DXSimp::Vector3& _outVector);
	};
}


