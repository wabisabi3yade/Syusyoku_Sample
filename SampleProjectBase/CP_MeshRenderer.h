#pragma once
#include "CP_Renderer.h"

// �`��ɕK�v
#include "Mesh_Base.h"

// ���b�V���`��R���|�[�l���g
class CP_MeshRenderer : public CP_Renderer
{
	Mesh_Base* pRenderMesh{ nullptr };

	// ���[���h�ϊ��s������߂�
	const RenderParam::WVP WVPSetup();

	// �}�e���A���̕`�揀��
	/*void MeshDraw(u_int _meshIdx, RenderParam::WVP& _wvp);*/
public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	// ���b�V����ݒ�
	void SetRenderMesh(Mesh_Base& _renderMesh);

	void ImGuiSetting() override;
};

