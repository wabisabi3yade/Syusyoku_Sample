#pragma once
#include "CP_Renderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

class Mesh_Group;

/// @brief �A�E�g���C���`��R���|�[�l���g(MeshRenderer�̌�ɂ���)
class CP_OutLineRenderer : public CP_Renderer
{
	/// @brief �A�E�g���C�����_�V�F�[�_�[
	static VertexShader* pOutLineVS;

	/// @brief �A�E�g���C���s�N�Z���V�F�[�_�[
	static PixelShader* pOutLinePS;

	/// @brief �`�悷�郁�b�V��
	Mesh_Group* pRenderMesh;
public:
	CP_OutLineRenderer();
	~CP_OutLineRenderer() {}

	/// @brief ������
	void Init() override;
private:	
	void Start() override;

	/// @brief �`��
	void Draw() override;
	void DrawSetup();

	/// @brief  �A�E�g���C���V�F�[�_�[���擾����
	void GetShader();

	/// @brief MeshRenderer���烁�b�V�����擾����
	void GetRenderMesh();

	/// @brief ���[�h�s��쐬
	/// @return ���[�h�s��
	DirectX::SimpleMath::Matrix MakeLoadMatrix();

	void ImGuiDebug() override;
};

