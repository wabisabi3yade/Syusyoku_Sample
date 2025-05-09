#pragma once

namespace HashiTaku
{
	class CP_Animation;
	class CP_Character;

	/// @brief アクションコントローラーのインターフェース
	class IActionController
	{

	public:
		/// @brief 指定したベクトルの力を入れる
		/// @param _power 力
		virtual void AddForce(const DXSimp::Vector3& _power) = 0;

		/// @brief 指定したベクトルの力を衝撃として入れる
		/// @param _power 力
		virtual void AddImpulse(const DXSimp::Vector3& _power) = 0;

		/// @brief 速度をセット
		/// @param _velocity 速度
		virtual void SetVelocity(const DXSimp::Vector3& _velocity) = 0;

		/// @brief 指定したbool変数に値をセット
		/// @param _paramName パラメーター名
		/// @param _isBool セットする値
		virtual void SetAnimationBool(const std::string& _paramName, bool _isBool) = 0;

		/// @brief 指定したint変数に値をセット
		/// @param _paramName パラメーター名
		/// @param _intVall セットする値
		virtual void SetAnimationInt(const std::string& _paramName, int _intVal) = 0;

		/// @brief 指定したfloat変数に値をセット
		/// @param _paramName パラメーター名
		/// @param _floatVal セットする値
		virtual void SetAnimationFloat(const std::string& _paramName, float _floatVal) = 0;

		/// @brief 指定したトリガー変数をtrueにする
		/// @param _paramName パラメーター名
		virtual void SetAnimationTrigger(const std::string& _paramName) = 0;

		/// @brief キャラクターを取得する
		/// @return キャラクターコンポーネント
		virtual CP_Character& GetCharacter() = 0;

		/// @brief 自身のオブジェクトのトランスフォーム
		/// @return オブジェクトのトランスフォーム
		virtual Transform& GetMyTransform() = 0;

		/// @brief アニメーションコンポーネント取得
		/// @return アニメーションコンポーネント
		virtual CP_Animation* GetAnimation() = 0;

		/// @brief 速度を取得
		/// @param _outVelocity 取得用変数
		virtual void GetVelocity(DXSimp::Vector3& _outVelocity) = 0;

		/// @brief 速度を取得
		/// @return 速度
		virtual DXSimp::Vector3 GetVelocity() const = 0;

		/// @brief 指定したbool変数に値を取得
		/// @param _paramName パラメーター名
		/// @param _isBool 取得する値
		virtual bool GetAnimationBool(const std::string& _paramName) = 0;

		/// @brief 指定したint変数に値を取得
		/// @param _paramName パラメーター名
		/// @param _intVall 取得する値
		virtual int GetAnimationInt(const std::string& _paramName) = 0;

		/// @brief 指定したfloat変数に値を取得
		/// @param _paramName パラメーター名
		/// @param _floatVal 取得する値
		virtual float GetAnimationFloat(const std::string& _paramName) = 0;

		/// @brief Δtを取得する
		/// @return Δt
		virtual float DeltaTime() const = 0;
	};
}


