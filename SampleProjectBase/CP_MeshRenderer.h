#pragma once
#include "CP_Renderer.h"

// �`��ɕK�v
#include "Mesh.h"
#include "Model.h"
#include "Material.h"

// �`�悷�郁�b�V���̏��
class RenderMesh
{
	friend class CP_MeshRenderer;

	// ���b�V��
	const Mesh* pMesh{ nullptr };

	// �}�e���A���̔ԍ�
	u_int materialID{ 0 };

public:
	RenderMesh() {}
	RenderMesh(const Mesh* _pMesh, u_int _materialID) : pMesh(_pMesh), materialID(_materialID) {};
	~RenderMesh() {}
};

// ���b�V���`��R���|�[�l���g
class CP_MeshRenderer : public CP_Renderer
{
	std::vector<RenderMesh> renderMeshes;

	std::vector<Material*> pMaterials;

	// ���[���h�ϊ��s������߂�
	const RenderParam::WVP WVPSetup();

	// �}�e���A���̕`�揀��
	void MeshDraw(u_int _meshIdx, RenderParam::WVP& _wvp);
public:
	using CP_Renderer::CP_Renderer;
	void Init();

	void Draw() override;

	// ���f�����Z�b�g
	void SetModel(Model& _model);

	// ���b�V����ݒ�
	void SetRenderMesh(const RenderMesh& _renderMesh);

	void ImGuiSetting() override;
};

