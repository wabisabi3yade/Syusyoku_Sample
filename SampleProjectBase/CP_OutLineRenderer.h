#pragma once
#include "CP_Renderer.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace HashiTaku
{
	class Mesh_Group;
	class IBoneBufferSupplier;

	/// @brief �A�E�g���C���`��R���|�[�l���g(MeshRenderer�̌�ɂ���)
	class CP_OutLineRenderer : public CP_Renderer
	{
		struct OutLineParameter
		{
			/// @brief ���̐F
			DXSimp::Color lineColor;

			/// @brief ���̑���
			float lineScale{ 0.07f };
		};

		/// @brief �A�E�g���C���̃p�����[�^
		OutLineParameter lineParameter;

		/// @brief �A�E�g���C�����_�V�F�[�_�[
		static VertexShader* pOutLineVS;

		/// @brief �A�j���[�V�����𔽉f�����钸�_�V�F�[�_�[
		static VertexShader* pAnimationOutLineVS;

		/// @brief �A�E�g���C���s�N�Z���V�F�[�_�[
		static PixelShader* pOutLinePS;

		/// @brief �g�p���Ă��钸�_�V�F�[�_�[
		VertexShader* pUseVetrexShader;

		/// @brief �{�[���̃o�b�t�@�����N���X
		IBoneBufferSupplier* pBoneBuffer;

		/// @brief �`�悷�郁�b�V��
		Mesh_Group* pRenderMesh;
	public:
		CP_OutLineRenderer();
		~CP_OutLineRenderer() {}

		/// @brief ������
		void Init() override;

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;

	private:
		void Start() override;

		/// @brief �`��
		void Draw() override;
		void DrawSetup();

		/// @brief  �A�E�g���C���V�F�[�_�[���擾����
		void GetShader();

		/// @brief MeshRenderer���烁�b�V�����擾����
		void GetRenderMesh();

		/// @brief �g�p���钸�_�V�F�[�_�[���Z�b�g����
		void SetUseShader();

		/// @brief �A�j���[�V�������Ă��邩�擾
		/// @return �A�j���[�V�������Ă��邩�H
		bool IsAnimation() const;

		/// @brief ���[�h�s��쐬
		/// @return ���[�h�s��
		DXSimp::Matrix MakeLoadMatrix();

		void ImGuiDebug() override;
	};
}