#pragma once
// �Z�[�u�E���[�h
#include "ISaveLoad.h"

class GameObject;

// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
class Transform : public ISaveLoad
{
	/// @brief �g�����X�t�H�[�������I�u�W�F�N�g
	GameObject* pGameObject;

	/// @brief �e�g�����X�t�H�[��
	Transform* pParent;

	/// @brief �q�g�����X�t�H�[��
	std::list<Transform*> pChilds;

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

public:
	Transform(GameObject* _pGameObject, bool _isInit = true);
	Transform(const Transform& _other);
	virtual ~Transform();

	Transform& operator=(const Transform& _other);

	// �����x�N�g�����X�V����
	void UpdateVector();

	// ���̍��W�ɃI�u�W�F�N�g��������
	void LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector = DirectX::SimpleMath::Vector3::Up);

	/// @brief �e�q�֌W����������
	void RemoveParentChild();

	/// @brief �q�g�����X�t�H�[������������
	/// @param _removeTransform ��������g�����X�t�H�[��
	void RemoveChild(Transform& _removeTransform);

	/// @brief �e�g�����X�t�H�[����ݒ肷��
	/// @param _parent �e�g�����X�t�H�[���̎Q��
	void SetParent(Transform& _parent);

	/// @brief �q�g�����X�t�H�[����ݒ肷��
	/// @param _parent �q�g�����X�t�H�[���̎Q��
	virtual void SetChild(Transform& _child);

	// ���[���h
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& _pos);
	virtual void SetScale(const DirectX::SimpleMath::Vector3& _scale);
	virtual void SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
	virtual void SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

	// ���[�J��
	virtual void SetLocalPos(const DirectX::SimpleMath::Vector3& _position);
	virtual void SetLocalScale(const DirectX::SimpleMath::Vector3& _scale);
	virtual void SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
	virtual void SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

	virtual void SetGameObject(GameObject& _go);

	// ���[���h
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetScale() const;
	DirectX::SimpleMath::Vector3 GetEularAngles() const;
	DirectX::SimpleMath::Quaternion GetRotation() const;

	// ���[�J��
	DirectX::SimpleMath::Vector3 GetLocalPosition() const;
	DirectX::SimpleMath::Vector3 GetLocalScale() const;
	DirectX::SimpleMath::Vector3 GetLocalEularAngles() const;
	DirectX::SimpleMath::Quaternion GetLocalRotation() const;

	DirectX::SimpleMath::Vector3 Right()const { return right; }	// �E�x�N�g��
	DirectX::SimpleMath::Vector3 Up()const { return up; }	// ��x�N�g��
	DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// ���ʃx�N�g��

	GameObject& GetGameObject();

	// �e�g�����X�t�H�[�����擾
	Transform* GetParent();

	//�@�q�g�����X�t�H�[���̐����擾
	u_int GetChilidCnt() const;

	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����f�[�^ 
	void Load(const nlohmann::json& _transformData) override;

private:
	void Copy(const Transform& _other);

	//  �q�g�����X�t�H�[���p�����[�^���X�V(�ċA�֐�)
	// ���W
	void UpdateHierarchyPositions();
	// �X�P�[��
	void UpdateHierarchyScales();
	// ��]
	void UpdateHierarchyRotations();
};