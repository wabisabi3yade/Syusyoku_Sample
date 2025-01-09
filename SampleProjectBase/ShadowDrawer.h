#pragma once
#include "RenderTarget.h"
#include "DepthStencil.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IApplyDepthShadow.h"

namespace HashiTaku
{
	class DirectionLight;

	/// @brief �e�`��N���X
	class ShadowDrawer : public IImGuiUser
	{
		/// @brief �[�x�l���������ޕ`�惊�X�g
		std::list<IApplyShadowDepth*> depthWriters;

		/// @brief �[�x���������ނƂ��̃��C�g����̍s��
		RenderParam::WVP lightMatrix;

		/// @brief �[�x���������ރ����_�[�^�[�Q�b�g
		std::unique_ptr<RenderTarget> pRenderTarget;

		/// @brief �[�x�X�e���V��
		std::unique_ptr<DepthStencil> pDepthStencil;

		/// @brief �[�x�������݂Ɏg�p����s�N�Z���V�F�[�_�[
		PixelShader* pDepthWritePS;

		/// @brief �`��N���X
		D3D11_Renderer* pRenderer;

		/// @brief �e�𗎂Ƃ�����
		const DirectionLight* pLight;
	public:
		/// @brief �R���X�g���N�^
		/// @param _light ����
		ShadowDrawer(const DirectionLight& _light);
		~ShadowDrawer() {}

		/// @brief �[�x�e�N�X�`�����쐬����
		void CreateDepthTexture();

		/// @brief �[�x���������ރI�u�W�F�N�g�̃��[���h�ϊ��s����Z�b�g
		/// @param _worldMtx ���[���h�ϊ��s��
		/// @param _pVs �`�悷�钸�_�V�F�[�_
		void SetWorldMatrix(const DXSimp::Matrix& _worldMtx, VertexShader* _pVs);

		/// @brief ���C�g����̍s����擾
		RenderParam::WVP& GetLightMatrix();

		/// @brief �[�x���������񂾃����_�[�^�[�Q�b�g�r���[���擾
		/// @return �[�x���������񂾃����_�[�^�[�Q�b�g�r���[
		RenderTarget& GetDepthRTV();

		/// @brief �[�x�������݂��s���N���X��ǉ�
		/// @param _pRenderer �������݂��s���N���X�ϐ�
		void AddDepthWriteRenderer(IApplyShadowDepth& _renderer);

		/// @brief �[�x�������݂��s���N���X���폜
		/// @param _pRenderer �������݂��s���N���X�ϐ�
		void RemoveDepthWriteRenderer(IApplyShadowDepth& _renderer);

	private:
		/// @brief �[�x�������݂ł��邩�H
		/// @return �[�x�������݂ł��邩�H
		bool GetCanDepthWrite() const;

		/// @brief ���C�g���猩���s����v�Z����
		void CalcLightMatrix();

		void ImGuiDebug() override;
	};
}