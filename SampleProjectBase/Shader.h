#pragma once

namespace HashiTaku
{
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

		// バッファの種類
		enum class BufferType
		{
			WVP,	// WVP
			Material,	// マテリアル
			Light,	// ライト
			CameraPos,	// カメラの座標
			Color,	// ジオメトリ使用時の色
			BoneMatricies,	// ボーン行列
			LightWVP	// ライトから見たWVP行列
		};

	private:
		/// @brief シェーダーの種類
		Type type;

	protected:
		/// @brief 名前
		std::string shaderName;

		/// @brief このシェーダーで利用するGPUに送るバッファの配列
		std::vector<ID3D11Buffer*> pBuffers;

		/// @brief シェーダに送るテクスチャ
		std::vector<ID3D11ShaderResourceView*> pTextures;

		/// @brief バッファ種類
		std::vector<BufferType> bufferTypes;

		/// @brief 各シェーダーでの作成処理
		/// @param _pData シェーダーのデータ
		/// @param _dataSize データのサイズ
		virtual void MakeShader(const char* _pData, u_int _dataSize) = 0;

		/// @brief シェーダーから定数バッファを作成する
		/// @param _pData シェーダの文字列
		/// @param _dataSize サイズ
		virtual void MakeBuffer(const char* _pData, u_int _dataSize);

	public:
		Shader(Type _type) : type(_type), shaderName("") {}
		virtual ~Shader() {};

		/// @brief 使用するテクスチャなどの初期化を行う
		virtual void InitAssets() {};

		/// @brief csoからシェーダを作成
		/// @param _filePath csoのパス
		void LoadCsoFile(const char* _filePath);

		/// @brief バッファに情報を入れる
		/// @param _slot スロットの番号
		/// @param _pData バッファに入れる情報
		void UpdateSubResource(u_int _slot, void* _pData);

		/// @brief バッファに情報を入れる(高速だが、扱い注意)
		/// @param _slot スロットの番号
		/// @param _pData バッファに入れる情報
		void Map(u_int _slot, void* _pData, size_t _dataSize);

		/// @brief 1ループに一回しか更新しなくてもよいバッファはここで更新する
		virtual void OnceUpdateBuffer() {}

		/// @brief GPUに所持しているバッファを送る
		virtual void SetGPU() = 0;

		/// @brief シェーダーに送るための配列に追加
		/// @param _slot スロットの番号
		/// @param _pTexture テクスチャ
		void SetTexture(u_int _slot, const Texture* _pTexture);

		/// @brief シェーダに名前をつける
		/// @param _name 名前
		void SetName(const std::string& _name);

		/// @brief シェーダーの種類を取得
		/// @return シェーダータイプ
		Type GetShaderType() const;

		/// @brief バッファの種類
		/// @param _slot スロット番号
		/// @return バッファの種類
		BufferType GetBufferType(u_int _slot);

		/// @brief バッファの数を取得
		/// @return バッファの数
		u_int GetBufferNum();

		// シェーダー名を取得
		std::string GetShaderName() const;
	};
}