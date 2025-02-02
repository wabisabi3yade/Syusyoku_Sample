#pragma once
#include "CP_Renderer.h"
#include "IApplyDepthShadow.h"
#include "IBoneSupplier.h"

// �`��ɕK�v
#include "Mesh_Group.h"

namespace HashiTaku
{
	class ShadowDrawer;

	// ���b�V���`��R���|�[�l���g
	class CP_MeshRenderer : public CP_Renderer, public IApplyShadowDepth
	{
		/// @brief �`�悷�郁�b�V���Q
		Mesh_Group* pRenderMesh{ nullptr };

		/// @brief �g�p����}�e���A���i�Ȃ��Ƃ��̓��b�V���̃}�e���A�����g�p����j
		std::vector<Material*> setMaterials;

		/// @brief ���b�V���̈ʒu�I�t�Z�b�g
		DXSimp::Vector3 offsetMeshPosition;

		/// @brief �e��`�悷��N���X
		ShadowDrawer* pShadowDrawer;

		/// @brief �{�[���o�b�t�@���������Ă����N���X
		IBoneBufferSupplier* pBoneBuffer;

		/// @brief �I�u�W�F�N�g�̌��_��\�����邩
		bool isOriginDisplay;

		/// @brief �e��`�悳���邩�H
		bool isShadow;
	public:
		CP_MeshRenderer();
		~CP_MeshRenderer() {}

		/// @brief ������
		void Init() override;

		/// @brief �폜������
		void OnDestroy() override;

		/// @brief �`�悷�郁�b�V�����Z�b�g
		/// @param _renderMesh ���b�V���Q
		void SetRenderMesh(Mesh_Group& _renderMesh);

		/// @brief �`�悷�郁�b�V�����Z�b�g
		/// @param _assetName ���b�V����
		void SetRenderMesh(const std::string& _assetName);

		// �V�F�[�_�[�̖��O����V�F�[�_�[��ύX����
		void SetVertexShader(const std::string& _vsName);
		void SetPixelShader(const std::string& _psName);

		/// @brief ���b�V���̃I�t�Z�b�g���Z�b�g
		/// @param _offsetPos �Z�b�g����I�t�Z�b�g���W
		void SetMeshOffset(const DXSimp::Vector3& _offsetPos);

		/// @brief �`�悷�郁�b�V�����擾
		/// @return ���b�V���Q
		Mesh_Group* GetRenderMesh();
		
		/// @brief ���b�V���̍��W�I�t�Z�b�g���擾
		/// @return ���b�V���̍��W�I�t�Z�b�g
		const DXSimp::Vector3& GetMeshOffsetPos() const;

		/// @brief �[�x�������݂��s��
		void WriteDepth() override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		// �R���|�[�l���g���ʊ֐�
		void Draw() override;

		/// @brief �`��ł���̂��Ԃ�
		/// @return �`��ł��邩�H
		bool IsCanDraw();

		/// @brief ���[�h���̃I�t�Z�b�g�s��v�Z
		/// @return ���[�h�s��
		DXSimp::Matrix CalcLoadMtx();

		/// @brief ���b�V����`��
		void DrawMesh(RenderParam::WVP& _wvp);

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
		DXSimp::Vector3 WorldOffset(const DXSimp::Vector3& _offset);

		/// @brief ���_�\��
		void OriginDisplay();

		/// @brief �}�e���A�����擾����
		/// @param meshIdx ���b�V��ID
		Material* GetMaterial(u_int meshIdx);

		void ImGuiDebug() override;
	};
}