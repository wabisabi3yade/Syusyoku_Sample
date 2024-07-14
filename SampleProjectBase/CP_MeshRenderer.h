#pragma once
#include "CP_Renderer.h"

// �`��ɕK�v
#include "Mesh_Base.h"

// ���b�V���`��R���|�[�l���g
class CP_MeshRenderer : public CP_Renderer
{
	Mesh_Base* pRenderMesh{ nullptr };

public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	// ���b�V����ݒ�
	void SetRenderMesh(Mesh_Base& _renderMesh);

	void ImGuiSetting() override;

private:

	/// @brief ���[���h�ϊ��s������߂�
	/// @return 
	const RenderParam::WVP WVPSetup();

	/// @brief ���b�V����`��
	void DrawMesh(RenderParam::WVP _wvp);
};

