#pragma once
#include "Component.h"
#include "IAnimationObserver.h"
#include "AnimControllPlayer.h"

// ���f��
class TreeNode;
class CP_MeshRenderer;

/// @brief �A�j���[�V�����R���|�[�l���g
class CP_Animation : public Component
{
	static constexpr u_int MAX_BONEMTX{ 100 };	// �V�F�[�_�[�̓n���{�[���s��̍ő吔

	// �V�F�[�_�[�ɓn���{�[���s��\����
	struct BoneCombMtricies
	{
		DirectX::SimpleMath::Matrix matrix[MAX_BONEMTX];
	};

	/// @brief �{�[���̃R���r�l�[�V�����s��̃o�b�t�@�[
	static BoneCombMtricies boneCombBuffer;

	/// @brief ���[�g�{�[���ɑ΂���I�t�Z�b�g�s��
	DirectX::SimpleMath::Matrix rootOffsetMtx;

	/// @brief �X�P���^�����b�V��
	SkeletalMesh* pSkeletalMesh;

	/// @brief ���ۂɓ������{�[��
	std::unique_ptr<BoneList> pMoveBoneList;

	/// @brief �A�j���[�V�����R���g���[���[
	AnimationController* pAnimController;

	/// @brief �A�j���[�V�����R���g���[���Đ��@�\
	std::unique_ptr<AnimControllPlayer> pAnimConPlayer;
public:
	CP_Animation();
	virtual ~CP_Animation() {}

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[��ǉ�
	/// @param _observer �I�u�U�[�o�[
	void AddChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[���폜
	/// @param _observer �I�u�U�[�o�[
	void RemoveChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	/// @brief �w�肵��bool�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _isBool �Z�b�g����l
	void SetBool(const std::string& _paramName, bool _isBool);

	/// @brief �w�肵��int�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _intVall �Z�b�g����l
	void SetInt(const std::string& _paramName, int _intVal);

	/// @brief �w�肵��float�ϐ��ɒl���Z�b�g
	/// @param _paramName �p�����[�^�[��
	/// @param _floatVal �Z�b�g����l
	void SetFloat(const std::string& _paramName, float _floatVal);

	/// @brief �w�肵���g���K�[�ϐ���true�ɂ���
	/// @param _paramName �p�����[�^�[��
	void SetTrigger(const std::string& _paramName);

	// �A�j���[�V�����R���g���[���[���Z�b�g
	void SetAnimationController(AnimationController& _controller);

	/// @brief �R���g���[���[�v���C���[�̍Đ����x���Z�b�g����
	/// @param _setSpeed �X�s�[�h���Z�b�g
	void SetControllerPlaySpeed(float _setSpeed);

	/// @brief ���݂̃m�[�h�Đ����x���Z�b�g����
	/// @param _setSpeed �X�s�[�h���Z�b�g
	void SetCurNodePlayerSpeed(float _setSpeed);

	/// @brief �w�肵��bool�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _isBool �擾����l
	bool GetBool(const std::string& _paramName);

	/// @brief �w�肵��int�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _intVall �擾����l
	int GetInt(const std::string& _paramName);

	/// @brief �w�肵��float�ϐ��ɒl���擾
	/// @param _paramName �p�����[�^�[��
	/// @param _floatVal �擾����l
	float GetFloat(const std::string& _paramName);

	/// @brief �A�j���[�V�����p�����[�^�̃|�C���^�[���擾����
	/// @tparam T �p�����[�^�̃t���O
	/// @param _paramName �p�����[�^��
	/// @return �A�j���[�V�����p�����[�^�̃|�C���^�[
	template<HashiTaku::AnimParam::AnimParamConcept T>
	const T* GetParameterPointer(const std::string& _paramName) const;

	/// @brief ���ݍĐ����Ă���A�j���[�V�����̃��[�g���[�V�����̍��W�ړ����x��n��
	/// @return ���W�ړ����x(s)
	const DirectX::SimpleMath::Vector3& GetMotionPosSpeedPerSec() const;

	/// @brief �v���C���[�Đ����x���擾����
	/// @return ���݂̍Đ����x
	float GetControllerPlaySpeed() const;

	/// @brief �v���C���[�Đ��������擾����
	/// @return ���݂̍Đ�����
	float GetCurrentPlayRatio() const;

	/// @brief �v���C���[�A�j���[�V�����������擾����
	/// @return ���݂̃A�j���[�V��������
	float GetCurrentAnimationRatio() const;

	/// @brief ���ۂɓ������{�[�����X�g���擾����
	/// @return �{�[�����X�g
	BoneList* GetMoveBoneList();

	/// @brief �{�[�������擾
	/// @return �{�[����
	u_int GetBoneCnt() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void Awake() override;
	void LateUpdate() override;
	void Draw() override;
	void OnAddComponent(Component& _comp);

	/// @brief �A�j���[�V�����R���g���[���[�̏���
	void SetupAnimCon();

	/// @brief Renderer����{�[�����R�s�[���A�������{�[�����쐬 
	void CopyBoneList(CP_MeshRenderer& _mr);

	/// @brief �Đ��ł����Ԃ��H
	/// @return �Đ��ł��邩
	bool IsCanPlay();

	/// @brief �R�s�[���ꂽ�p�����[�^�����݂��Ă��邩
	/// @return ���݂��Ă���
	bool IsExistCopyAnimParameter();

	/// @brief �{�[���R���r�l�[�V�����s����X�V
	void UpdateBoneCombMtx();

	/// @brief �q�m�[�h�̃R���r�l�[�V�����s����X�V����i�ċA�֐��j
	/// @param _treeNode �X�V����m�[�h
	/// @param _parentMtx ���[���h�ϊ����邽�߂̐e�܂ł̍s��
	void UpdateNodeHierarchy(TreeNode& _treeNode, const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief �{�[���̃R���r�l�[�V�����s����X�V
	void UpdateAnimationMtx();

	/// @brief �V�F�[�_�[�̃o�b�t�@���X�V����
	void UpdateBoneBuffer();

	void ImGuiDebug() override;
};

template<HashiTaku::AnimParam::AnimParamConcept T>
inline const T* CP_Animation::GetParameterPointer(const std::string& _paramName) const
{
	if (!pAnimConPlayer) return nullptr;

	// �p�����[�^����擾����
	auto pValue = pAnimConPlayer->GetCopyAnimParameters().GetValueAddress(_paramName);
	return std::get_if<T>(pValue);
}
