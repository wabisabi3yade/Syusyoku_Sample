#pragma once
#include "EnemyActionController.h"
#include "BossActState_Base.h"

class CP_Player;
class CP_Boss;

/// @brief �{�X�̃A�N�V�����R���g���[���[
class BossActionController : public EnemyActionController
{
public:
	// �s���̋����^�C�v
	enum class ActDistance
	{
		Short,	// �ߋ���
		Mid,	// ������
		Far,	// ������
		MaxNum
	};

private:
	/// @brief �v���C���[�I�u�W�F�N�g
	CP_Player* pPlayerObject;

	/// @brief �̂����邱�Ƃ͂ł���̂��H(�A�j���[�V�����p�����[�^)
	const bool* pCanKnock;

	/// @brief �f�t�H���g��Ԑݒ�
	BossActState_Base::BossState defaultState;

	/// @brief ���ꂼ��̋������w�肷��
	std::array<float, static_cast<u_int>(ActDistance::MaxNum)> disLengthList;

	/// @brief �ߋ����s���̃A�N�V�������X�g
	std::vector<BossActState_Base::BossState> shortRangeActions;
	
	/// @brief �������s���̃A�N�V�������X�g
	std::vector<BossActState_Base::BossState> midRangeActions;

	/// @brief �������s���̃A�N�V�������X�g
	std::vector<BossActState_Base::BossState> farRangeActions;
public:
	BossActionController(CP_Boss& _boss);
	~BossActionController() {}

	/// @brief ����������
	/// @param _pAnimation �A�j���[�V����
	/// @param _pRigidBody ���W�b�h�{�f�B
	void Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody) override;

	/// @brief �X�e�[�g��ύX
	/// @param _nextState ���̃X�e�[�g
	/// @param _isForce�@�����؂�ւ�
	/// @return �����������H
	bool ChangeState(BossActState_Base::BossState _nextState, bool _isForce = false);

	/// @brief �_���[�W������
	void OnDamage(const HashiTaku::AttackInformation& _atkInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos) override;

	/// @brief �f�o�b�O�`��
	void DebugDisplay() override;

	/// @brief �����ŋ����̃X�e�[�g���擾
	/// @param _actDistance ����
	/// @return �{�X�̃X�e�[�g
	BossActState_Base::BossState RandState(ActDistance _actDistance);

	/// @brief �v���C���[�I�u�W�F�N�g���Z�b�g
	/// @param _playerObj �v���C���[�I�u�W�F�N�g
	void SetPlayer(CP_Player& _playerObj);

	/// @brief �U�������Z�b�g
	/// @param _atkInfo �U�����
	void SetAttackInfo(const HashiTaku::AttackInformation& _atkInfo);

	/// @brief �{�X�R���|�[�l���g���擾
	CP_Boss& GetBoss();

	/// @brief �v���C���[���擾����
	/// @return �v���C���[�I�u�W�F�N�g
	CP_Player& GetPlayer();

	/// @brief �̂����邱�Ƃ��ł��邩�擾
	/// @return �̂�����ł��邩�H
	const bool GetCanKnock() const;

	/// @brief �s���̋������擾
	/// @param _actDistance �擾�����������̎��
	/// @return �����񂨒���
	float GetActDistanceLength(ActDistance _actDistance);

	/// @brief �ǂ̋��������f����
	/// @param _disLength �����̒���
	/// @return �ǂ̋������H
	ActDistance JudgeActDistance(float _disLength) const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �X�V����
	void Update() override;

	/// @brief �{�X�̃X�e�[�g���쐬
	/// @tparam T �{�X�̍s���N���X
	/// @param _createState �쐬�������X�e�[�g
	template<class T>
	void CreateState(BossActState_Base::BossState _createState,
		std::vector<ActDistance> _actDistance);

	/// @brief �ŏ��ɃA�j���[�V�����p�����[�^����擾
	void GetAnimationParam();

	/// @brief �{�X�̍X�V�������s���邩
	/// @return �s���邩?
	bool IsCanBossUpdate();

	/// @brief �{�X�s���ɕϊ�
	/// @param _pBaseNode �x�[�X�m�[�h�N���X
	/// @return �{�X�u��
	BossActState_Base* CastBossAct(HashiTaku::StateNode_Base* _pBaseNode);

	/// @brief �f�t�H���g��Ԃ��Z�b�g
	/// @param _defaultState �f�t�H���g���
	void SetDefaultState(BossActState_Base::BossState _defaultState);

	/// @brief �eState�̕�������擾����
	/// @param _stateId ��Ԃ�ID
	/// @return ������
	std::string GetStateStr(int _stateId) override;

	/// @brief �eState��ID���擾����
	/// @param _stateName�@��Ԗ�
	/// @return ��Ԃ�ID
	int GetStateId(const std::string& _stateName) override;

	void ImGuiDebug() override;
};

template<class T>
inline void BossActionController::CreateState(BossActState_Base::BossState _createState, std::vector<ActDistance> _actDistance)
{
	// �X�e�[�g���쐬���āA���X�g�ɒǉ�����
	std::unique_ptr<BossActState_Base> createState = std::make_unique<T>();
	createState->Init(_createState, *this);
	AddNode(static_cast<int>(_createState), std::move(createState));

	// �����̃��X�g�ɂ��ǉ�����
	std::vector<ActDistance> dupplicateCheck;
	u_int disCnt = static_cast<u_int>(_actDistance.size());
	for (u_int d_i = 0; d_i < disCnt; d_i++)
	{
		ActDistance actDis = _actDistance[d_i];

		// �d���`�F�b�N
		bool isDupplicate = false;
		for (auto& dupp : dupplicateCheck)
		{
			if (actDis != dupp) continue;

			HASHI_DEBUG_LOG("�d�����Ă���v�f������܂�");
			isDupplicate = true; break;
		}
		if (isDupplicate) break;

		// �����̃��X�g�ɒǉ�����
		switch (actDis)
		{
		case ActDistance::Short:
			shortRangeActions.push_back(_createState);
			break;

		case ActDistance::Mid:
			midRangeActions.push_back(_createState);
			break;

		case ActDistance::Far:
			farRangeActions.push_back(_createState);
			break;

		default:
			break;
		}

		// �d���`�F�b�N���X�g�ɓ����
		dupplicateCheck.push_back(actDis);
	}
}
