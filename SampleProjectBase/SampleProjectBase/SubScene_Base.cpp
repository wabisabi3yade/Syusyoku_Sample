#include "SubScene_Base.h"
#include "SceneMoveInfo.h"

#include "Direct3D11.h"
#include "Camera.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"
#include "ImGuiMethod.h"

/// <summary>
/// �V�[���ړ�����ƂȂ�����Ăяo��
/// </summary>
/// <param name="_nextSubType">���̃T�u�V�[���̃^�C�v</param>
/// <param name="_nextBroadType">���̑�ǃV�[���̃^�C�v</param>
void SubScene_Base::OnMoveScene(int _nextSubType, BROAD_TYPE::TYPE _nextBroadType)
{
	// �V�[���J�ڏ���J�ڐ�ɕς���
	pSceneMoveInfo->OnChangeScene(_nextSubType, _nextBroadType);
}

SubScene_Base::SubScene_Base(SceneMoveInfo* _pSceneMoveInfo)
{
	// BroadScene���玝���Ă���
	pSceneMoveInfo = _pSceneMoveInfo;
	// ���\�[�X�Ǘ��|�C���^�擾
	resourceCollection = ResourceCollection::GetInstance();
	// �V�[���I�u�W�F�N�g�Ǘ��N���X�쐬
	sceneObjects = std::make_unique<SceneObjects>();
	// �J��������
	mainCamera = std::make_unique<Camera>();
}

SubScene_Base::~SubScene_Base()
{
}


void SubScene_Base::Exec()
{
	// �V�[�����̍X�V����
	Update();
	// Update�̌�ɍs���X�V����(�J�����ɏ����Ƃ�)
	LateUpdate();

	// ��ʃN���A
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();
	// �r���[�ϊ��s������߂�
	DirectX::XMMATRIX viewMatrix = mainCamera->GetViewMatrix();
	Direct3D11::GetInstance()->GetRenderer()->SetUpViewTransform(viewMatrix);
	// �V�[�����̕`�揈��
	Draw();

	ImGuiMethod::Draw();

	// �X���b�v�`�F�C��
	Direct3D11::GetInstance()->GetRenderer()->Swap();
}
