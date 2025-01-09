#pragma once

// �C�[�W���O
#include "Easing.hpp"

namespace HashiTaku
{
	// ���w�I�ȕ֗��֐��i�撣���ĕ׋����I�j
	namespace Mathf
	{
		constexpr float roundDeg = 360.0f;	// 1���̊p�x
		constexpr float radToDeg = 57.2958f;	//�@���W�A�����p�x
		constexpr float degToRad = 0.0174533f;	//�@�p�x�����W�A��
		constexpr float epsilon = std::numeric_limits<float>::epsilon();	// �ŏ�����
		constexpr float smallValue = 0.000001f;	// �������l
		constexpr float PI = std::numbers::pi_v<float>;	// 

		/// @brief �l��0����ő�l�܂łŐ��K������
		/// @param _t ���K������l
		/// @param _length �ő�l
		/// @return ���K�����ꂽ�l
		float Repeat(float _t, float _length);

		/// @brief �����ɂ����`���
		/// @param _from ��
		/// @param _to ��
		/// @param _ratio ���� 
		/// @return �v�Z�l
		float Lerp(float _from, float _to, float _ratio);
	}

	namespace Vec2
	{
		using namespace DXSimp;
		/// @brief ��Βl�����߂�
		/// @param _v �ϐ�
		/// @return ��Βl
		Vector2 Abs(const Vector2& _v);

		/// @brief ������ɕϊ�����
		/// @param _v Vector2
		/// @return ������
		std::string ToStr(const Vector2& _v);
	}

	namespace Vec3
	{
		using namespace DXSimp;

		/// <summary>
		/// ���ʐ��`��Ԃ�����֐�
		/// </summary>
		/// <param name="_startPos">�J�n�l</param>
		/// <param name="_endPos">�I�_�l</param>
		/// <param name="_ratio">����(0�`1)</param>
		/// <returns></returns>
		Vector3 Slerp(
			Vector3 _startPos,
			Vector3 _endPos,
			float _ratio
		);
		// ���[���h���W�n3���P�ʃx�N�g��
		constexpr Vector3 Up = { 0.0f, 1.0f, 0.0f };
		constexpr Vector3 Right = { 1.0f, 0.0f, 0.0f };
		constexpr Vector3 Forward = { 0.0f, 0.0f, 1.0f };

		// ����
		float Dot(Vector3 _v1, Vector3 _v2);

		// �O��
		Vector3 Cross(Vector3 _v1,
			Vector3 _v2);

		/// @brief float�^�Ƃ̍ő�l�����߂�
		Vector3 Max(const Vector3& _v3, float _floatVal);

		/// @brief ��Βl�����߂�
		/// @param _v �ϐ�
		/// @return ��Βl
		Vector3 Abs(const Vector3& _v);

		/// @brief ���[���h�s�񂩂�X�P�[�������߂�
		/// @param _mtx ���[���h�s��
		/// @return �X�P�[���l
		DXSimp::Vector3 WorldMtxToScale(const DXSimp::Matrix& _mtx);

		/// @brief �l��0����ő�l�܂łŐ��K������
		/// @param _v ���K������l
		/// @param _length �ő�l
		/// @return ���K�����ꂽ�l
		Vector3 Repeat(const Vector3& _v, float _length);

		/// @brief ������ɕϊ�����
		/// @param _v Vector3
		/// @return ������
		std::string ToStr(const Vector3& _v);
	}

	namespace Vec4
	{
		/// @brief float�^�Ƃ̍ő�l�����߂�
		DXSimp::Vector4 Max(const DXSimp::Vector4& _v4, float _floatVal);
	}

	namespace Quat
	{
		using namespace DXSimp;

		/// @brief �N�H�[�^�j�I�������]���Ɖ�]�p�x�����߂�
		/// @param _q �N�H�[�^�j�I��
		/// @param _axis ��]���̎Q��
		/// @param _angle ��]�p�x�̎Q��(rad)
		void ToAxisAngle(const Quaternion& _q, Vector3& _axis, float& _angle);

		/// @brief �N�H�[�^�j�I���̊|���Z������
		/// @param _q1 �N�H�[�^�j�I��1
		/// @param _q2 �N�H�[�^�j�I��2
		/// @return �|���Z�����N�H�[�^�j�I��
		Quaternion Multiply(const Quaternion& _q1, const Quaternion& _q2);

		/// @brief �N�H�[�^�j�I���̍��������߂�
		/// @param _origin ��
		/// @param _sub �����Z
		/// @return ����
		Quaternion RotationDifference(const Quaternion& _origin, const Quaternion& _sub);

		/// @brief �^�[�Q�b�g�̕����Ɍ�����N�H�[�^�j�I���𐶐�����
		/// @param _vector ���������x�N�g��
		/// @param _up ������x�N�g��
		/// @return ������N�H�[�^�j�I��
		Quaternion RotateToVector(const Vector3& _vector, const Vector3& _up = Vec3::Up);

		/// @brief �N�H�[�^�j�I������I�C���[�p�ɕϊ�
		/// @param _q �N�H�[�^�j�I��
		/// @return �I�C���[�p
		Vector3 ToEulerAngles(const Quaternion& _q);

		/// @brief �p�x����N�H�[�^�j�I������
		/// @param _degrees ��]�p�x(deg)
		/// @return �N�H�[�^�j�I��
		Quaternion ToQuaternion(const Vector3& _degrees);

		/// @brief �قړ������N�H�[�^�j�I�������f����
		/// @return ���������H
		bool CheckEqual(const Quaternion& _q1, const Quaternion& _q2);

		/// @brief ������ɕϊ�����
		/// @param _v Quaternion
		/// @return ������
		std::string ToStr(const Quaternion& _q);
	}

	namespace Mtx
	{
		using namespace DXSimp;

		// �^�E�x�N�g���擾
		Vector3 GetRightVector(const Matrix& _matrix);

		// �^��x�N�g���擾
		Vector3 GetUpVector(const Matrix& _matrix);

		// ���ʃx�N�g���擾
		Vector3 GetForwardVector(const Matrix& _matrix);

		/// @brief �����x�N�g�������]�s��쐬
		/// @param _right �E
		/// @param _up ��
		/// @param _forward �O
		/// @return ��]�s��
		Matrix RotateMatrixByVector(const Vector3& _right, const Vector3& _up, const Vector3& _forward);

		/// @brief �p�x�����]�s������߂�
		/// @param _degrees �p�x(deg) 
		/// @return ��]�s��
		Matrix CreateRoratateMtx(const Vector3& _degrees);

		/// @brief �g�����X�t�H�[������s����쐬����
		/// @param _transform �g�����X�t�H�[��
		/// @param _outtMatrix ���ʍs��
		void CreateTransformMatrix(const Vector3& _pos, const Vector3& _scale, const Quaternion& _rotation, Matrix& _outMatrix);

		/// @brief ���[���h�s�񂩂���W�E�X�P�[���E��]�ʂ��擾
		/// @param _worldMtx ���[���h�s��
		/// @param _pos �ړ����W
		/// @param _scele �X�P�[��
		/// @param _rot ��]��		
		void GetTransformFromWldMtx(const Matrix& _worldMtx, Vector3& _pos, Vector3& _scele, Quaternion& _rot);

		/// @brief Vector3�Ɗ|���Z������
		/// @param _mtx �s��
		/// @param _v Vector3
		/// @return �v�Z����Vector3
		Vector3 MultiplyVector3(const Matrix& _mtx, const Vector3& _v);
	}
}