#pragma once

namespace HashiTaku
{
	/// @brief �A�j���[�V�����Ŏg�p����x�W�F���
	class CatmulSplineInterp
	{
		CatmulSplineInterp() {}
		~CatmulSplineInterp() {}
	public:
		/// @brief Catmull-Rom �X�v���C���ɂ�鐧��_�Ɛڐ��x�N�g���̌v�Z
		/// @param _from ��Ԃ����p�����[�^��
		/// @param _to ��Ԃ����p�����[�^��
		/// @param _prev ��Ԍ��̈�O�̃p�����[�^
		/// @param _next ��Ԑ�̈��̃p�����[�^

		/// @brief  Catmull�X�v���C���ɂ����
		/// @param _from �p�����[�^��
		/// @param _to �p�����[�^��
		/// @param _prev �p�����[�^�������O�̃p�����[�^
		/// @param _next �p�����[�^��̈��̃p�����[�^
		/// @param _ratio ����
		/// @return �v�Z�l
		static DirectX::SimpleMath::Vector3 CalcVector3(const DirectX::SimpleMath::Vector3& _from, const DirectX::SimpleMath::Vector3& _to, const DirectX::SimpleMath::Vector3& _prev, const DirectX::SimpleMath::Vector3& _next, float _ratio);


		static DirectX::SimpleMath::Quaternion CalcQuaternion(const DirectX::SimpleMath::Quaternion& _from, const DirectX::SimpleMath::Quaternion& _to, const DirectX::SimpleMath::Quaternion& _prev, const DirectX::SimpleMath::Quaternion& next, float _ratio);

	private:

		/// @brief Catmull-Rom �X�v���C���ɂ�鐧��_�Ɛڐ��x�N�g���̌v�Z
		/// @param _from ��Ԃ����p�����[�^��
		/// @param _to ��Ԃ����p�����[�^��
		/// @param _prev ��Ԍ��̈�O�̃p�����[�^
		/// @param _next ��Ԑ�̈��̃p�����[�^
		/// @param _point1 ���o��������_1
		/// @param _point2 ���o��������_2
		/*static void CalcControlPoints(const DirectX::SimpleMath::Vector3& _from, const DirectX::SimpleMath::Vector3& _to, const DirectX::SimpleMath::Vector3& _prev, const DirectX::SimpleMath::Vector3& _next, DirectX::SimpleMath::Vector3& _point1, DirectX::SimpleMath::Vector3& _point2);*/
	};
}