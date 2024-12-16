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
		DirectX::SimpleMath::Matrix worldMatrix;

		/// @brief ���[�J����ԃg�����X�t�H�[���s��
		DirectX::SimpleMath::Matrix localMatrix;

		/// @brief �g�����X�t�H�[�������I�u�W�F�N�g
		GameObject* pGameObject;

		/// @brief �e�g�����X�t�H�[��
		Transform* pParent;

		/// @brief �q�g�����X�t�H�[��
		std::list<Transform*> childTransforms;

		// �e�����̒P�ʃx�N�g��
		DirectX::SimpleMath::Vector3 up;	// ��
		DirectX::SimpleMath::Vector3 right;	// �E
		DirectX::SimpleMath::Vector3 forward;	// �O

		// ���[���h
		DirectX::SimpleMath::Vector3 position;	// ���W
		DirectX::SimpleMath::Vector3 scale;	// �X�P�[��
		DirectX::SimpleMath::Vector3 eularAngles;	// ��]�p�x
		DirectX::SimpleMath::Quaternion rotation;	// �N�H�[�^�j�I��


		// ���[�J��
		DirectX::SimpleMath::Vector3 localPosition;	// ���W
		DirectX::SimpleMath::Vector3 localScale;	// �X�P�[��
		DirectX::SimpleMath::Vector3 localEularAngles;	// ��]�p�x
		DirectX::SimpleMath::Quaternion localRotation;	// �N�H�[�^�j�I��

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
		void LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector = DirectX::SimpleMath::Vector3::Up);

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
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& _pos);
		virtual void SetScale(const DirectX::SimpleMath::Vector3& _scale);
		virtual void SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
		virtual void SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

		// ���[�J��
		virtual void SetLocalPosition(const DirectX::SimpleMath::Vector3& _position);
		virtual void SetLocalScale(const DirectX::SimpleMath::Vector3& _scale);
		virtual void SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
		virtual void SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

		// ���[���h
		const DirectX::SimpleMath::Vector3& GetPosition() const;
		const DirectX::SimpleMath::Vector3& GetScale() const;
		const DirectX::SimpleMath::Vector3& GetEularAngles() const;
		const DirectX::SimpleMath::Quaternion& GetRotation() const;
		const DirectX::SimpleMath::Matrix& GetWorldMatrix() const;

		// ���[�J��
		const DirectX::SimpleMath::Vector3& GetLocalPosition() const;
		const DirectX::SimpleMath::Vector3& GetLocalScale() const;
		const DirectX::SimpleMath::Vector3& GetLocalEularAngles() const;
		const DirectX::SimpleMath::Quaternion& GetLocalRotation() const;
		const DirectX::SimpleMath::Matrix& GetLocalMatrix() const;

		const DirectX::SimpleMath::Vector3& Right()const { return right; }	// �E�x�N�g��
		const DirectX::SimpleMath::Vector3& Up()const { return up; }	// ��x�N�g��
		const DirectX::SimpleMath::Vector3& Forward()const { return forward; }	// ���ʃx�N�g��

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
		nlohmann::json Save() override;

		/// @brief ���[�h����
		/// @param  _transformData ���[�h����f�[�^ 
		void Load(const nlohmann::json& _transformData) override;

		/// @brief �e�q�֌W�����[�h����
		/// @param  _transformData ���[�h����f�[�^ 
		void LoadChildTransform(const nlohmann::json& _transformData);

	private:
		/// @brief ���g�̃��[���h�s����X�V����
		/// @param _parentWorldMtx �e�̃��[���h�s��
		void UpdateWorldMatrix(const DirectX::SimpleMath::Matrix& _parentWorldMtx);

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