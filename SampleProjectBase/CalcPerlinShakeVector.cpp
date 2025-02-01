#include "pch.h"
#include "CalcPerlinShakeVector.h"

namespace HashiTaku
{
	constexpr float MAX_SHAKE_ELAPSED(10000.0f);	// 揺れの最大時間
	constexpr float MAX_RAND_PERLINOFFSET(50.0f); // パーリンノイズのオフセット値の最大値

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

		// 乱数でパーリンノイズのオフセット値を決める
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

		// オフセットをリセット
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

		// 時間経過でフェードアウトさせるフェードアウト割合を求める
		float fadeOutRatio = 1.0f;
		if (curShakeParameter.isFadeOut)
		{
			fadeOutRatio -= std::clamp(shakeElapsedTime / curShakeParameter.time, 0.0f, 1.0f);
		}

		// X軸
		if (curShakeParameter.shakeVec.x > Mathf::epsilon)
		{
			// パーリンノイズから取得
			_outVector.x = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.x * speed * shakeElapsedTime
				)));
			_outVector.x -= 0.5f;
			_outVector.x *= 2.0f;

			// 揺れの力の倍率を求める
			_outVector.x *= curShakeParameter.power * fadeOutRatio;

			// ベクトルの大きさも掛ける
			_outVector.x *= curShakeParameter.shakeVec.x;
		}
		// Y軸
		if (curShakeParameter.shakeVec.y > Mathf::epsilon)
		{
			// パーリンノイズから取得
			_outVector.y = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.y * speed * shakeElapsedTime
				)));
			_outVector.y -= 0.5f;
			_outVector.y *= 2.0f;

			// 揺れの力の倍率を求める
			_outVector.y *= curShakeParameter.power * fadeOutRatio;

			// ベクトルの大きさも掛ける
			_outVector.y *= curShakeParameter.shakeVec.y;
		}
		// Z軸
		if (curShakeParameter.shakeVec.z > Mathf::epsilon)
		{
			// パーリンノイズから取得
			_outVector.z = static_cast<float>(Random::GetPerlinNoise(static_cast<double>(
				randPerlinOffset.z * speed * shakeElapsedTime
				)));
			_outVector.z -= 0.5f;
			_outVector.z *= 2.0f;

			// 揺れの力の倍率を求める
			_outVector.z *= curShakeParameter.power * fadeOutRatio;

			// ベクトルの大きさも掛ける
			_outVector.z *= curShakeParameter.shakeVec.z;
		}
	}

	void PerlinShake::Update(float _deltaTime)
	{
		shakeOffset = DXSimp::Vector3::Zero;

		if (!isShaking) return;

		/// 経過時間を加算
		shakeElapsedTime += _deltaTime;
		// 永続のときにオーバーフローを起こさないようにするために
		shakeElapsedTime = Mathf::Repeat(shakeElapsedTime, MAX_SHAKE_ELAPSED);

		// オフセット値を求める
		CalcShakeOffset(shakeOffset);

		// 時間が0以上　かつ　全体時間を過ぎたら終わる
		if (curShakeParameter.time > 0.0f &&
			shakeElapsedTime > curShakeParameter.time)
			EndShake();
	}
}
