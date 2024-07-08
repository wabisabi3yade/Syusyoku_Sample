#include "SceneMoveInfo.h"

constexpr int SUBSCENE_TYPE_MIN = 0;	// �T�u�V�[���^�C�v�̍Œ�l
constexpr int SUBSCENE_TYPE_MAX = 256;	// �T�u�V�[���^�C�v�̍ő�l
										// (�����邩�͊e��ǃN���X�ɂ���ĕς��)

SceneMoveInfo::SceneMoveInfo(int _initSub, BroadType::Type _initBroad)
{
	// �����̃V�[��������
	broadType = _initBroad;
	subType = _initSub;

	pRunningState = new SceneRunningState();
}

SceneMoveInfo::~SceneMoveInfo()
{
	CLASS_DELETE(pRunningState);
}

void SceneMoveInfo::OnChangeScene(int _nextSub, BroadType::Type _nextBroad)
{
	// �J�ڐ�̑�ǁE�T�u�V�[���̃^�C�v����
	subType = _nextSub;
	broadType = _nextBroad;

	// �V�[���̉ғ���Ԃ��I���ɂ���
	pRunningState->SetFinshState();

}


int SceneMoveInfo::GetRunningState() const
{
	return pRunningState->GetRunningState();
}

void SceneMoveInfo::Init()
{
	pRunningState->Init();
}

