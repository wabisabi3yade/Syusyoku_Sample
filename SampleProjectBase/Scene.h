#pragma once

class InSceneSystemManager;

// 1�V�[���N���X
class Scene
{
	/// @brief �V�[����
	std::string sceneName;

	/// @brief �V�[�����V�X�e���}�l�W���[
	InSceneSystemManager* pInSceneSystem;

public:
	Scene(const std::string& _name);
	~Scene();

	/// @brief ���s�֐�
	void Exec();

	/// @brief �Z�[�u�@�\
	void Save();

	/// @brief �V�[�����ݒ�
	/// @param _name ���O
	void SetName(const std::string& _name);

	/// @brief �V�[�������擾
	std::string GetName() const;
private:
	void Load();

	/// @brief �`�揀��
	void DrawSetup();

	// �Z�[�u�t�@�C���̃p�X��
	std::string SaveFilePath();

	/// @brief �V�[���I�u�W�F�N�g�����[�h
	/// @param _objectData �V�[���I�u�W�F�N�g�f�[�^
	void LoadSceneObject(const nlohmann::json& _objectData);
};

