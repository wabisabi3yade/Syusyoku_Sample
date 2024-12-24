#pragma once
#include "CP_Renderer.h"

// �`��ɕK�v
#include "Mesh_Group.h"

namespace HashiTaku
{
	// ���b�V���`��R���|�[�l���g
	class CP_MeshRenderer : public CP_Renderer
	{
		/// @brief �`�悷�郁�b�V���Q
		Mesh_Group* pRenderMesh{ nullptr };

		/// @brief �g�p����}�e���A���i�Ȃ��Ƃ��̓��b�V���̃}�e���A�����g�p����j
		std::vector<Material*> setMaterials;

		/// @brief �I�u�W�F�N�g�̌��_��\�����邩
		bool isOriginDisplay;
	public:
		CP_MeshRenderer();
		~CP_MeshRenderer() {}

		/// @brief �`�悷�郁�b�V�����Z�b�g
		/// @param _renderMesh ���b�V���Q
		void SetRenderMesh(Mesh_Group& _renderMesh);

		/// @brief �`�悷�郁�b�V�����Z�b�g
		/// @param _assetName ���b�V����
		void SetRenderMesh(const std::string& _assetName);

		// �V�F�[�_�[�̖��O����V�F�[�_�[��ύX����
		void SetVertexShader(const std::string& _vsName);
		void SetPixelShader(const std::string& _psName);

		/// @brief �`�悷�郁�b�V�����擾
		/// @return ���b�V���Q
		Mesh_Group* GetRenderMesh();

		json Save() override;
		void Load(const json& _data) override;
	private:
		// �R���|�[�l���g���ʊ֐�
		void Start() override;
		void Draw() override;

		/// @brief �`��ł���̂��Ԃ�
		/// @return �`��ł��邩�H
		bool IsCanDraw();

		/// @brief ���[�h���̃I�t�Z�b�g�s��v�Z
		/// @return ���[�h�s��
		DirectX::SimpleMath::Matrix CalcLoadMtx();

		/// @brief ���b�V����`��
		void DrawMesh(RenderParam::WVP& _wvp);

		/// @brief �e�`��
		void DrawShadow();

		/// @brief �}�e���A���̏���
		/// @param _wvp wvp�s��
		/// @param _pMaterial�@�}�e���A��
		void MaterialSetup(RenderParam::WVP& _wvp, Material* _pMaterial);

		/// @brief �V�F�[�_�[�̏���
		/// @param _shader �V�F�[�_�[
		/// @param _wvp wvp�s��
		/// @param _material �}�e���A��
		void ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp, Material& _material);

		/// @brief ���[�J�������l�������I�t�Z�b�g�ɕϊ�
		/// @param _worldOffset �ϊ��������I�t�Z�b�g
		DirectX::SimpleMath::Vector3 WorldOffset(const DirectX::SimpleMath::Vector3& _offset);

		/// @brief ���_�\��
		void OriginDisplay();

		void ImGuiDebug() override;
	};
}