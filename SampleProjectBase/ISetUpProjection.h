#pragma once

namespace HashiTaku
{
	class IGetRenderResource;

	// ���e���@�̕ϊ��s������߂�C���^�[�t�F�[�X
	class ISetUpProjection
	{
	public:
		// ���e�s������߂āA�Ԃ�����
		virtual bool SetUpProjection(UINT screenWidth, UINT screenHeight, IGetRenderResource* _pRenderResource) = 0;
	};
}