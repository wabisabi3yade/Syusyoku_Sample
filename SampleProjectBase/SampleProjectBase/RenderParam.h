#pragma once
#include "CbTransformSet.h"
#include "CbProjectionSet.h"
#include "CbViewSet.h"

// �`��ɕK�v�ȃp�����[�^�N���X
class RenderParam
{
public:
	struct ConstBuffer
	{

	};
private:
	// GPU�ɓn���萔�o�b�t�@������������
	bool InitConstBuffer(ID3D11Device* _device);

public:
	CbTransformSet cbTransformSet;	// GPU�ɓn�����W�f�[�^
	CbProjectionSet cbProjectionSet;	// GPU�ɓn�����e�f�[�^
	CbViewSet cbViewSet;	// GPU�ɓn���r���[�ϊ��f�[�^

	RenderParam() {};
	~RenderParam();

	/// <summary>
	/// �p�����[�^������������(�o�b�t�@�̃T�C�Y�m��)
	/// </summary>
	/// <param name="_device">�f�o�C�X</param>
	/// <returns>���Ȃ��ł������ǂ����t���O</returns>
	bool Init(ID3D11Device* _device);
};

