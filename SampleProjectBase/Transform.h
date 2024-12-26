#pragma once
// �Z�[�u�E���[�h
#include "ISaveLoad.h"

namespace HashiTaku
{
	class GameObject;

	// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
	class Transform : public ISaveLoad
	{
		/// @brief ���[���h��ԃg�����X�t�H�[���s��
		DXSimp::Matrix worldMatrix;

		/// @brief ���[�J����ԃg�����X�t�H�[���s��
		DXSimp::Matrix localMatrix;

		/// @brief �g�����X�t�H�[�������I�u�W�F�N�g
		GameObject* pGameObject;

		/// @brief �e�g�����X�t�H�[��
		Transform* pParent;

		/// @brief �q�g�����X�t�H�[��
		std::list<Transform*> childTransforms;

		// �e�����̒P�ʃx�N�g��
		DXSimp::Vector3 up;	// ��
		DXSimp::Vector3 right;	// �E
		DXSimp::Vector3 forward;	// �O

		// ���[���h
		DXSimp::Vector3 position;	// ���W
		DXSimp::Vector3 scale;	// �X�P�[��
		DXSimp::Vector3 eularAngles;	// ��]�p�x
		DXSimp::Quaternion rotation;	// �N�H�[�^�j�I��


		// ���[�J��
		DXSimp::Vector3 localPosition;	// ���W
		DXSimp::Vector3 localScale;	// �X�P�[��
		DXSimp::Vector3 localEularAngles;	// ��]�p�x
		DXSimp::Quaternion localRotation;	// �N�H�[�^�j�I��

	protected:
		/// @brief �e�g�����X�t�H�[�������邩�H
		bool isHaveParent;

		bool isNull;

	public:
		Transform(GameObject* _pGameObject, bool _isInit = true);
		virtual ~Transform();

		// �����x�N�g�����X�V����
		void UpdateVector();

		// ���̍��W�ɃI�u�W�F�N�g��������
		void LookAt(const DXSimp::Vector3& _worldPos, const DXSimp::Vector3& _upVector = DXSimp::Vector3::Up);

		/// @brief �e�g�����X�t�H�[������������
		virtual void RemoveParent();

		/// @brief �q�g�����X�t�H�[������������
		/// @param _removeTransform ��������g�����X�t�H�[��
		virtual void RemoveChild(Transform& _removeTransform);

		/// @brief �e�g�����X�t�H�[����ݒ肷��
		/// @param _parent �e�g�����X�t�H�[���̎Q��
		virtual void SetParent(Transform& _parent);

		/// @brief �q�g�����X�t�H�[����ݒ肷��
		/// @param _parent �q�g�����X�t�H�[���̎Q��
		virtual void SetChild(Transform& _child);

		// ���[���h
		virtual void SetPosition(const DXSimp::Vector3& _pos);
		virtual void SetScale(const DXSimp::Vector3& _scale);
		virtual void SetEularAngles(const DXSimp::Vector3& _eularAngles);
		virtual void SetRotation(const DXSimp::Quaternion& _quaternion);

		// ���[�J��
		virtual void SetLocalPosition(const DXSimp::Vector3& _position);
		virtual void SetLocalScale(const DXSimp::Vector3& _scale);
		virtual void SetLocalEularAngles(const DXSimp::Vector3& _eularAngles);
		virtual void SetLocalRotation(const DXSimp::Quaternion& _quaternion);

		// ���[���h
		const DXSimp::Vector3& GetPosition() const;
		const DXSimp::Vector3& GetScale() const;
		const DXSimp::Vector3& GetEularAngles() const;
		const DXSimp::Quaternion& GetRotation() const;
		const DXSimp::Matrix& GetWorldMatrix() const;

		// ���[�J��
		const DXSimp::Vector3& GetLocalPosition() const;
		const DXSimp::Vector3& GetLocalScale() const;
		const DXSimp::Vector3& GetLocalEularAngles() const;
		const DXSimp::Quaternion& GetLocalRotation() const;
		const DXSimp::Matrix& GetLocalMatrix() const;

		const DXSimp::Vector3& Right()const { return right; }	// �E�x�N�g��
		const DXSimp::Vector3& Up()const { return up; }	// ��x�N�g��
		const DXSimp::Vector3& Forward()const { return forward; }	// ���ʃx�N�g��

		/// @brief �Q�[���I�u�W�F�N�g���擾����
		/// @return �Q�[���I�u�W�F�N�g
		GameObject& GetGameObject();

		/// @brief �Q�[���I�u�W�F�N�g�����擾����
		/// @return �I�u�W�F�N�g��
		const std::string& GetObjectName() const;

		// �e�g�����X�t�H�[�����擾
		Transform* GetParent();

		/// @brief �q�g�����X�t�H�[�����擾
		/// @param _childIdx �q��ID
		/// @return �q�g�����X�t�H�[��
		Transform* GetChild(u_int _childIdx);

		//�@�q�g�����X�t�H�[���̐����擾
		u_int GetChildCnt() const;

		/// @brief �Z�[�u����
		/// @param _sceneData �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param  _transformData ���[�h����f�[�^ 
		void Load(const json& _transformData) override;

		/// @brief �e�q�֌W�����[�h����
		/// @param  _transformData ���[�h����f�[�^ 
		void LoadChildTransform(const json& _transformData);

	private:
		/// @brief ���g�̃��[���h�s����X�V����
		/// @param _parentWorldMtx �e�̃��[���h�s��
		void UpdateWorldMatrix(const DXSimp::Matrix& _parentWorldMtx);

		/// @brief ���݂̃��[�J���p�����[�^���烍�[�J���s����X�V
		void UpdateLocalMatrix();

		void UpdateLocalMatrixFromWorld();

		/// @brief ���W�ύX���I�u�W�F�N�g�ɓ`����
		void ChangePosNotify();

		/// @brief �X�P�[���ύX���I�u�W�F�N�g�ɓ`����
		void ChangeScaleNotify();

		/// @brief ��]�ύX���I�u�W�F�N�g�ɓ`����
		void ChangeRotNotify();
	};
}