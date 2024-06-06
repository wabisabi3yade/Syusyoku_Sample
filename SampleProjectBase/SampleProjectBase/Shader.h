#pragma once

// シェーダーの基底クラス
class Shader
{
protected:
	virtual void MakeBuffer() = 0;	// バッファを作成する

public:
	Shader();
	virtual ~Shader();

	void WriteBuffer();	// バッファに情報を書き込む
	virtual void Bind();	// GPUに送る
};

