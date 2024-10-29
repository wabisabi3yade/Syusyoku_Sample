#pragma once

namespace HashiTaku
{
	/// @brief �A�j���[�V�����̃I�u�U�[�o�[�N���X
	class IAnimationObserver
	{

	public:
		IAnimationObserver() {}
		~IAnimationObserver() = default;

		/// @brief �A�j���[�V�����I�����̍X�V
		/// @param _animState �I�������A�j���[�V����State
		virtual void OnFinishAnimation(u_int _animState) = 0;
	};

	class AnimationSubject
	{
		std::list<IAnimationObserver*> pObservers;

	public:
		AnimationSubject() {}
		~AnimationSubject() = default;

		/// @brief �I�u�U�[�o�[��ǉ�
		/// @param _addObserver �ǉ�����I�u�U�[�o�[
		void AddObserver(IAnimationObserver& _addObserver);

		/// @brief �I�u�U�[�o�[���폜
		/// @param _addObserver 
		void RemoveObserver(IAnimationObserver& _addObserver);

		/// @brief �A�j���[�V�����I�������ƒʒm����
		/// @param _animState �I�������A�j���[�V�����X�e�[�g
		void NotifyFinish(u_int _animState);
	};
}