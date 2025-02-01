#include "pch.h"
#include "CalcPerlinShakeVector.h"

namespace HashiTaku
{
	constexpr float MAX_SHAKE_ELAPSED(10000.0f);	// �h��̍ő厞��
	constexpr float MAX_RAND_PERLINOFFSET(50.0f); // �p�[�����m�C�Y�̃I�t�Z�b�g�l�̍ő�l

	PerlinShake::PerlinShake() :
		shakeElapsedTime(0.0f),
		isShaking(false)
	{
	}

	void PerlinShake::BeginShake(const DXSimp::Vector3& _vector, 
		float _power,
		float _time,
		float _speed,
		bool _isFadeOut)
	{
		isShaking = true;
		shakeElapsedTime = 0.0f;

		curShakeParameter.isFadeOut = _isFadeOut;
		curShakeParameter.power = _power;
		curShakeParameter.time = _time;
		curShakeParameter.speed = _speed;
		curShakeParameter.shakeVec = _vector;

		// �����Ńp�[�����m�C�Y�̃I�t�Z�b�g�l�����߂�
		randPerlinOffset.x = Random::Range<float>(0.0f, MAX_RAND_PERLINOFFSET);
		randPerlinOffset.y = Random::Range<float>(0.0f, MAX_RAND_PERLINOFFSET);
		randPerlinOffset.z = Random::Range<float>(0.0f, MAX_RAND_PERLINOFFSET);
	}

	void PerlinShake::BeginShake(const PerlinShakeParameter& _shakeParam)
	{
		BeginShake(_shakeParam.shakeVec,
			_shakeParam.power,
			_shakeParam.time,
			_shakeParam.speed,
			_shakeParam.isFadeOut);
	}

	void PerlinShake::EndShake()
	{
		isShaking = false;

		// �I�t�Z�b�g�����Z�b�g
		shakeOffset = DXSimp::Vector3::Zero;
	}

	bool PerlinShake::GetIsShaking() const
	{
		return isShaking;
	}

	const DXSimp::Vector3& PerlinShake::GetShakeOffset() const
	{
		return shakeOffset;
	}

	void PerlinShake::CalcShakeOffset(DXSimp::Vector3& _outVector)
	{
		float speed = curShakeParameter.speed;

		// ���Ԍo�߂Ńt�F�[�h�A�E�g������t�F�[�h�A�E�g���������߂�
		float fadeOutRatio = 1.0f;
		if (curShakeParameter.isFadeOut)
		{
			fadeOutRatio -= std::clamp(shakeElapsedTime / curShakeParameter.time, 0.0f, 1.0f);
		}

		// X��
		if (curShakeParameter.shakeVec.x > Mathf::epsilon)
		{
			// �p�[�����m�C�Y����擾
			_outVector.x = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.x * speed * shakeElapsedTime
				)));
			_outVector.x -= 0.5f;
			_outVector.x *= 2.0f;

			// �h��̗͂̔{�������߂�
			_outVector.x *= curShakeParameter.power * fadeOutRatio;

			// �x�N�g���̑傫�����|����
			_outVector.x *= curShakeParameter.shakeVec.x;
		}
		// Y��
		if (curShakeParameter.shakeVec.y > Mathf::epsilon)
		{
			// �p�[�����m�C�Y����擾
			_outVector.y = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.y * speed * shakeElapsedTime
				)));
			_outVector.y -= 0.5f;
			_outVector.y *= 2.0f;

			// �h��̗͂̔{�������߂�
			_outVector.y *= curShakeParameter.power * fadeOutRatio;

			// �x�N�g���̑傫�����|����
			_outVector.y *= curShakeParameter.shakeVec.y;
		}
		// Z��
		if (curShakeParameter.shakeVec.z > Mathf::epsilon)
		{
			// �p�[�����m�C�Y����擾
			_outVector.z = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.z * speed * shakeElapsedTime
				)));
			_outVector.z -= 0.5f;
			_outVector.z *= 2.0f;

			// �h��̗͂̔{�������߂�
			_outVector.z *= curShakeParameter.power * fadeOutRatio;

			// �x�N�g���̑傫�����|����
			_outVector.z *= curShakeParameter.shakeVec.z;
		}
	}

	void PerlinShake::Update(float _deltaTime)
	{
		shakeOffset = DXSimp::Vector3::Zero;

		if (!isShaking) return;

		/// �o�ߎ��Ԃ����Z
		shakeElapsedTime += _deltaTime;
		// �i���̂Ƃ��ɃI�[�o�[�t���[���N�����Ȃ��悤�ɂ��邽�߂�
		shakeElapsedTime = Mathf::Repeat(shakeElapsedTime, MAX_SHAKE_ELAPSED);

		// �I�t�Z�b�g�l�����߂�
		CalcShakeOffset(shakeOffset);

		// ���Ԃ�0�ȏ�@���@�S�̎��Ԃ��߂�����I���
		if (curShakeParameter.time > 0.0f &&
			shakeElapsedTime > curShakeParameter.time)
			EndShake();
	}
}
