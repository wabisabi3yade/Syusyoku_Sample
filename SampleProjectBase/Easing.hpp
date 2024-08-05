#pragma once
// �C�[�W���O���N���X
class Ease_Base;

// ��������C����N���X
class SceneManager;

// �C�[�W���O�֐��`�[�g�V�[�g�Fhttps://easings.net/ja
namespace HashiTaku
{
	// �C�[�W���O�֐��̖߂�l�Ƃ���
	using EaseFunction = std::function<float(float)>;

	/// @brief �C�[�W���O�̎��
	enum class EaseKind
	{
		Linear,
		Sine,
		Quad,
		Cubic,
		Quart,
		Back,
		Elastic,
		Bounce,
		MaxNum
	};

	enum class EaseFuncKind
	{
		In,
		Out,
		InOut
	};

	struct EaseType
	{
		EaseKind ease;
		EaseFuncKind funcType;
	};

	/// @brief �C�[�W���O�֐�
	class Easing
	{
		friend class SceneManager;

		/// @brief �C�[�W���O�̃N���X���܂Ƃ߂����X�g
		static std::vector<std::unique_ptr<Ease_Base>> easeList;
		/// @brief string�Ƒ��ݕϊ��������̂Ŏ������X�g(�C�[�W���O���)
		static std::unordered_map<std::string, EaseKind> easeDictionary;
		/// @brief string�Ƒ��ݕϊ��������̂Ŏ������X�g(�C�[�W���O�֐�)
		static std::unordered_map<std::string, EaseFuncKind> funcDictionary;

	public:
		/// @brief �w�肵���C�[�W���O�֐���Ԃ�
		/// @param _easeFunc �i�[����֐��I�u�W�F�N�g�ϐ�
		/// @param _getEase �擾�������C�[�W���O��
		/// @param _funcType �֐��̎��
		static void GetEase(EaseFunction& _easeFunc, EaseType _easeType);

		/// @brief ImGui�ݒ�
		/// @param _easeFunc �֐��I�u�W�F�N�g
		/// @param _nowEase ���݂̃C�[�W���O
		static void ImGuiSelect(EaseFunction& _easeFunc, EaseType& _nowEase);

	private:
		/// @brief �������@�V�[���}�l�[�W���[��
		static void Init();

		/// @brief �C�[�W���O�쐬�i�������j
		/// @param _easeId �쐬����C�[�W���O
		static void MakeEase(u_int _easeId);

		// �C�[�W���O�̎�ނ���string�̕ϊ�
		static std::string EaseToString(EaseKind _type);

		// �֐��̎�ނ���string�̕ϊ�
		static std::string FuncToString(EaseFuncKind _type);

		// ���O�z����擾
		static std::vector<std::string> GetEaseNames();
		static std::vector<std::string> GetFuncNames();
	};
}