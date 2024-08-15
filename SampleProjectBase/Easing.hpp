#pragma once
// �C�[�W���O���N���X
class IEasing;

// ��������C����N���X
class SceneManager;

// �C�[�W���O�֐��`�[�g�V�[�g�Fhttps://easings.net/ja
namespace HashiTaku
{
	/// @brief �C�[�W���O�̎��
	enum class EaseKind
	{
		Linear,

		InSine,
		OutSine,
		InOutSine,

		InQuad,
		OutQuad,
		InOutQuad,

		InCubic,
		OutCubic,
		InOutCubic,

		InQuart,
		OutQuart,
		InOutQuart,

		InQuint,
		OutQuint,
		InOutQuint,

		InExpo,
		OutExpo,
		InOutExpo,

		InCirc,
		OutCirc,
		InOutCirc,

		InBack,
		OutBack,
		InOutBack,

		InElastic,
		OutElastic,
		InOutElastic,

		InBounce,
		OutBounce,
		InOutBounce,

		MaxNum
	};

	/// @brief �C�[�W���O�֐�
	class Easing
	{
		friend class SceneManager;

		/// @brief �C�[�W���O�̃N���X���܂Ƃ߂����X�g
		static std::unordered_map<EaseKind, std::unique_ptr<IEasing>> easeList;

		/// @brief �C�[�W���O�̗񋓌^�Ɩ��O�Ƃ̑Ή��\
		static std::vector<std::string> easeNames;
	public:
		/// @brief �C�[�W���O�̒l���擾
		/// @param _ratio ����(0.0�`1.0)
		/// @param _easeType �C�[�W���O���
		/// @return �C�[�W���O�̌v�Z�l
		static float EaseValue(float _ratio, EaseKind _easeType);

		/// @brief ImGui�ݒ�
		/// @param _easeFunc �֐��I�u�W�F�N�g
		/// @param _nowEase ���݂̃C�[�W���O
		static void ImGuiSelect(EaseKind& _nowEase);

	private:
		/// @brief �������@�V�[���}�l�[�W���[��
		static void Init();

		/// @brief �C�[�W���O�쐬�i�������j
		/// @param _easeId �쐬����C�[�W���O
		static void MakeEase(u_int _easeId);

		/// @brief string�^����񋓌^�ɕϊ�
		/// @param _easeName �C�[�W���O��
		/// @return �C�[�W���O�̎��
		static EaseKind StrToEase(const std::string& _easeName);
	};
}