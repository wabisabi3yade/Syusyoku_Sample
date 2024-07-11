#pragma once

//ID3D11バッファの基底クラス
class Buffer_Base
{
protected:
	/// @brief バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;

public:
	Buffer_Base() {};
	virtual ~Buffer_Base() {};
	
	/// @brief バッファを作成
	/// @param _allSize 全体のサイズ
	/// @param _elementSize 要素のサイズ
	/// @param pInitData データ
	/// @return 成功フラグ
	virtual bool CreateBuffer(u_int _allSize, u_int _elementSize, void* pInitData) = 0;

	/// @brief バッファを取得する
	/// @return バッファ
	const ID3D11Buffer& GetBuffer()const;
};

