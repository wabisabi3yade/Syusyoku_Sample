#pragma once
#include "VertexShader.h"
#include "PixelShader.h"

namespace HashiTaku
{
	struct SceneLightsParam;

	// �R���Z�v�g�Ŏ擾�ł��郊�\�[�X�̌^���w��
	template<typename T>
	concept ShaderConcept = std::is_base_of_v<Shader, T>;

	// �V�F�[�_�[���\�[�X��ۗL����N���X
	class ShaderCollection : public Singleton_Base<ShaderCollection>
	{
		friend class Singleton_Base<ShaderCollection>;

		// �V�F�[�_�[�z��
		std::unordered_map<std::string, std::unique_ptr<Shader>> shaderList;

		// cso�t�@�C���������Ă���t�H���_�܂ł̃p�X��
		static constexpr auto FILE_FOLDER = "assets/Shader/";
	public:
		// �f�t�H���g�̃V�F�[�_�[�̖��O
		std::string defaultVS;	// ���_�V�F�[�_�[
		std::string defaultPS;	// �s�N�Z���V�F�[�_�[

		void Init();

		/// @brief �I�u�W�F�N�g���Ƃŕς��Ȃ��o�b�t�@�����X�V����(���C�g�A�J�����Ȃ�)
		void UniqueUpdateBuffer();

		// �擾����
		VertexShader* GetVertexShader(const std::string& _shaderName);
		PixelShader* GetPixelShader(const std::string& _shaderName);

		/// @brief ���_�V�F�[�_�[���O���X�g���擾
		/// @return �V�F�[�_�[�����X�g
		std::vector<const std::string*> GetVSNameList();

		/// @brief �s�N�Z���V�F�[�_�[���O���X�g���擾
		/// @return �V�F�[�_�[�����X�g
		std::vector<const std::string*> GetPSNameList();
	private:
		/// @brief CSO�t�@�C������ǂݍ���
		void LoadFromCSO();

		/// @brief �V�F�[�_�[�����[�h���č쐬����
		/// @tparam T �V�F�[�_�[�̃N���X
		/// @param _csoFileName cso�I�u�W�F�N�g�̃t�@�C�����i�p�X���͂Ȃ��j
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

		// �Ή������V�F�[�_�[�쐬
		std::unique_ptr<Shader> pCreateShader = std::make_unique<T>();

		// ���[�h����S�̂̃p�X��
		std::string loadFilePath = FILE_FOLDER + _csoFileName;

		// �V�F�[�_�[�����擾
		// �g���q���O��
		fs::path noExtPath = _csoFileName;
		std::string shaderName = noExtPath.stem().string();

		// �V�F�[�_�[��cso���烍�[�h
		pCreateShader->LoadCsoFile(loadFilePath.c_str());
		pCreateShader->SetName(shaderName);

		// �z��ɒǉ�����
		shaderList.emplace(shaderName, std::move(pCreateShader));
	}
}