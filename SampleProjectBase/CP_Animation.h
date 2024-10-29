#pragma once
#include "Component.h"
#include "IAnimationObserver.h"
#include "AnimControllPlayer.h"

// ���f��
class TreeNode;

constexpr u_int MAX_BONEMTX(400);	// �V�F�[�_�[�̓n���{�[���s��̍ő吔

/// @brief �A�j���[�V�����R���|�[�l���g
class CP_Animation : public Component
{
	// �V�F�[�_�[�ɓn���{�[���s��\����
	struct BoneCombMtricies
	{
		DirectX::SimpleMath::Matrix matrix[MAX_BONEMTX];
	};

	static BoneCombMtricies boneComb;

	/// @brief �X�P���^�����b�V��
	SkeletalMesh* pSkeletalMesh;

	/// @brief ���f���̃I�t�Z�b�g�s��
	DirectX::SimpleMath::Matrix offsetMtx;

	/// @brief �A�j���[�V�����R���g���[���[
	AnimationController* pAnimController;

	/// @brief �A�j���[�V�����R���g���[���Đ��@�\
	std::unique_ptr<AnimControllPlayer> pAnimConPlayer;
public:
	CP_Animation();
	CP_Animation(const CP_Animation& _other);
	virtual ~CP_Animation() {}

	CP_Animation& operator=(const CP_Animation& _other);

	void Init() override {}

	void Awake() override;

	void Update() override;

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

	/// @brief Renderer����X�P���^�����b�V�����Z�b�g 
	void SetupSkeletalMesh();

	// �A�j���[�V�����R���g���[���[���Z�b�g
	void SetAnimationController(AnimationController& _controller);

	/// @brief �v���C���[�̍Đ����x���Z�b�g����
	/// @param _setSpeed �X�s�[�h���Z�b�g
	void SetCurPlayerSpeed(float _setSpeed);

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[��ǉ�
	/// @param _observer �I�u�U�[�o�[
	void AddChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	/// @brief �A�j���[�V�����ύX�I�u�U�[�o�[���폜
	/// @param _observer �I�u�U�[�o�[
	void RemoveChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	// �X�P���^�����b�V�����擾
	SkeletalMesh& GetSkeletalMesh();

	AnimationController* GetAnimationController();

	/// @brief ���ݍĐ����Ă���A�j���[�V�����̃��[�g���[�V�����̍��W�ړ����x��n��
	/// @return ���W�ړ����x(s)
	const DirectX::SimpleMath::Vector3& GetMotionPosSpeedPerSec() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �A�j���[�V�����R���g���[���[�̏���
	void SetupAnimCon();

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

	void Copy(const CP_Animation& _other);

	void ImGuiSetting() override;
};

