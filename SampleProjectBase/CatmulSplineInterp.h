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
		static DXSimp::Vector3 CalcVector3(const DXSimp::Vector3& _from, const DXSimp::Vector3& _to, const DXSimp::Vector3& _prev, const DXSimp::Vector3& _next, float _ratio);


		static DXSimp::Quaternion CalcQuaternion(const DXSimp::Quaternion& _from, const DXSimp::Quaternion& _to, const DXSimp::Quaternion& _prev, const DXSimp::Quaternion& next, float _ratio);

	private:

		/// @brief Catmull-Rom �X�v���C���ɂ�鐧��_�Ɛڐ��x�N�g���̌v�Z
		/// @param _from ��Ԃ����p�����[�^��
		/// @param _to ��Ԃ����p�����[�^��
		/// @param _prev ��Ԍ��̈�O�̃p�����[�^
		/// @param _next ��Ԑ�̈��̃p�����[�^
		/// @param _point1 ���o��������_1
		/// @param _point2 ���o��������_2
		/*static void CalcControlPoints(const DXSimp::Vector3& _from, const DXSimp::Vector3& _to, const DXSimp::Vector3& _prev, const DXSimp::Vector3& _next, DXSimp::Vector3& _point1, DXSimp::Vector3& _point2);*/
	};
}