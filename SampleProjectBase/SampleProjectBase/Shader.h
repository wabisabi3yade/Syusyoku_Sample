#pragma once

class Texture;
// シェーダーの基底クラス
class Shader
{
public:
	// シェーダーの種類
	enum class Type
	{
		Vertex,	// 頂点
		Pixel,	// ピクセル
		Num
	};

private:
	Type type;	// シェーダーの種類
	std::string name;	// 名前
protected:	
	// このシェーダーで利用するGPUに送るバッファのリスト
	std::vector<ID3D11Buffer*> pBuffers;	
	std::vector<ID3D11ShaderResourceView*> pTextures;	// シェーダーへ送るテクスチャ情報
	
	/// <summary>
	//	各シェーダーでの作成処理
	/// </summary>
	/// <param name="pData">シェーダーのデータ</param>
	/// <param name="_dataSize">データのサイズ</param>
	virtual void MakeShader(const char* _pData, u_int _dataSize) = 0;

	// シェーダーから定数バッファを作成する
	virtual void MakeBuffer(const char* _pData, u_int _dataSize);

public:
	Shader(Type _type) : type(_type), name("") {}
	virtual ~Shader() {};
	
	// csoファイルからシェーダーを作成する
	void LoadCsoFile(const char* _filePath);	

	/// <summary>
	/// バッファに情報を入れる
	/// </summary>
	/// <param name="_slot">スロットの番号</param>
	/// <param name="_pData">バッファに入れる情報</param>
	void UpdateBuffer(u_int _slot, void* _pData);	

	/// <summary>
	/// シェーダーに送るための配列に追加
	/// </summary>
	/// <param name="_slot">スロットの番号</param>
	/// <param name="_texture"></param>
	void SetTexture(u_int _slot, Texture* _texture);

	void SetName(std::string _name);

	// GPUに所持しているバッファを送る
	virtual void Bind() = 0;
};

