#pragma once

class InSceneSystemManager;

// 1�V�[���N���X
class Scene : public HashiTaku::IImGuiUser
{
	/// @brief �V�[����
	std::string sceneName;

	/// @brief �V�[�����V�X�e���}�l�W���[
	InSceneSystemManager* pInSceneSystem;

	/// @brief �V�[���Đ����邩�H
	bool isPlay;

public:
	Scene(const std::string& _name);
	~Scene();

	/// @brief ���s�֐�
	void Exec();

	/// @brief �Z�[�u�@�\
	void Save();

	// �V�[�����ݒ�
	void SetName(const std::string& _name);

	// ���O���擾
	std::string GetName() const;

	// �X�V�����Ă��邩�擾
	bool GetIsUpdatePlay() const;
private:
	void ImGuiSetting() override;

	void Load();

	/// @brief �`�揀��
	void DrawSetup();

	// �Z�[�u�t�@�C���̃p�X��
	std::string SaveFilePath();

	/// @brief �V�[���Đ����邩�H
	/// @return �Đ����邩�H
	bool IsUpdatePlay();

	/// @brief �Đ����̕\��
	void ImGuiPlaying();

	/// @brief ��~���̕\��
	void ImGuiStop();

	/// @brief �Đ����n�߂�
	void PlayStart();

	/// @brief �Đ����~�߂�
	void PlayStop();
};

