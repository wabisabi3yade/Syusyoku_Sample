#pragma once
#include "ISetUpProjection.h"
#include "IGetRenderResource.h"

namespace HashiTaku
{
	// �v���W�F�N�V�����ϊ��s����쐬����i�������e������j�N���X
	class SetUpPerspectiveProj : public ISetUpProjection
	{
		float fov;	// ����p
		float nearZ;	// �`�拗���i�ŒZ�j
		float farZ;	// �`�拗��(�Œ�)


	public:
		SetUpPerspectiveProj();
		~SetUpPerspectiveProj();

		/// <summary>
		/// �r���[�ϊ��s����쐬����
		/// </summary>
		/// <param name="_screenWidth"></param>
		/// <param name="_screenHeight"></param>
		/// <returns></returns>
		bool SetUpProjection(UINT _screenWidth, UINT _screenHeight, IGetRenderResource* _pRenderResource) override;
	};
}