#pragma once

namespace HashiTaku
{
	/// @brief アニメーションのオブザーバークラス
	class IAnimationObserver
	{

	public:
		IAnimationObserver() {}
		~IAnimationObserver() = default;

		/// @brief アニメーション終了時の更新
		/// @param _animState 終了したアニメーションState
		virtual void OnFinishAnimation(u_int _animState) = 0;
	};

	class AnimationSubject
	{
		std::list<IAnimationObserver*> pObservers;

	public:
		AnimationSubject() {}
		~AnimationSubject() = default;

		/// @brief オブザーバーを追加
		/// @param _addObserver 追加するオブザーバー
		void AddObserver(IAnimationObserver& _addObserver);

		/// @brief オブザーバーを削除
		/// @param _addObserver 
		void RemoveObserver(IAnimationObserver& _addObserver);

		/// @brief アニメーション終了したと通知する
		/// @param _animState 終了したアニメーションステート
		void NotifyFinish(u_int _animState);
	};
}