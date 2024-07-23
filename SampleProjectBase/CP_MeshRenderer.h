#pragma once
#include "CP_Renderer.h"

// �`��ɕK�v
#include "Mesh_Group.h"

// ���b�V���`��R���|�[�l���g
class CP_MeshRenderer : public CP_Renderer
{
	Mesh_Group* pRenderMesh{ nullptr };

public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	// ���b�V����ݒ�
	void SetRenderMesh(Mesh_Group& _renderMesh);

	void ImGuiSetting() override;

	Mesh_Group* GetRenderMesh();

private:

	/// @brief ���b�V����`��
	void DrawMesh(RenderParam::WVP _wvp);

	/// @brief �}�e���A���̏���
	/// @param _wvp wvp�s��
	/// @param _mtrlIdx�@�}�e���A��ID
	void MaterialSetup(RenderParam::WVP& _wvp, u_int _mtrlIdx);

	/// @brief �V�F�[�_�[�̏���
	/// @param _shader �V�F�[�_�[
	/// @param _wvp wvp�s��
	/// @param _material �}�e���A��
	void ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp, Material& _material);
};

