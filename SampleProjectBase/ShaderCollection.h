#pragma once
#include "VertexShader.h"
#include "PixelShader.h"

namespace HashiTaku
{
	struct SceneLightsParam;

	// コンセプトで取得できるリソースの型を指定
	template<typename T>
	concept ShaderConcept = std::is_base_of_v<Shader, T>;

	// シェーダーリソースを保有するクラス
	class ShaderCollection : public Singleton_Base<ShaderCollection>
	{
		friend class Singleton_Base<ShaderCollection>;

		// シェーダー配列
		std::unordered_map<std::string, std::unique_ptr<Shader>> shaderList;

		// csoファイルが入っているフォルダまでのパス名
		static constexpr auto FILE_FOLDER = "assets/Shader/";
	public:
		// デフォルトのシェーダーの名前
		std::string defaultVS;	// 頂点シェーダー
		std::string defaultPS;	// ピクセルシェーダー

		void Init();

		/// @brief オブジェクトごとで変わらないバッファ情報を更新する(ライト、カメラなど)
		void UniqueUpdateBuffer();

		// 取得する
		VertexShader* GetVertexShader(const std::string& _shaderName);
		PixelShader* GetPixelShader(const std::string& _shaderName);

		/// @brief 頂点シェーダー名前リストを取得
		/// @return シェーダー名リスト
		std::vector<const std::string*> GetVSNameList();

		/// @brief ピクセルシェーダー名前リストを取得
		/// @return シェーダー名リスト
		std::vector<const std::string*> GetPSNameList();
	private:
		/// @brief CSOファイルから読み込む
		void LoadFromCSO();

		/// @brief シェーダーをロードして作成する
		/// @tparam T シェーダーのクラス
		/// @param _csoFileName csoオブジェクトのファイル名（パス名はなし）
		template<ShaderConcept T>
		void LoadShader(const std::string& _csoFileName);
	private:
		ShaderCollection() : defaultVS(""), defaultPS("") {};
		~ShaderCollection() {};
	};

	template<ShaderConcept T>
	inline void ShaderCollection::LoadShader(const std::string& _csoFileName)
	{
		namespace fs = std::filesystem;

		// 対応したシェーダー作成
		std::unique_ptr<Shader> pCreateShader = std::make_unique<T>();

		// ロードする全体のパス名
		std::string loadFilePath = FILE_FOLDER + _csoFileName;

		// シェーダー名を取得
		// 拡張子を外す
		fs::path noExtPath = _csoFileName;
		std::string shaderName = noExtPath.stem().string();

		// シェーダーをcsoからロード
		pCreateShader->LoadCsoFile(loadFilePath.c_str());
		pCreateShader->SetName(shaderName);

		// 配列に追加する
		shaderList.emplace(shaderName, std::move(pCreateShader));
	}
}