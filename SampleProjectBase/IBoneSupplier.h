#pragma once
namespace HashiTaku
{
	// �V�F�[�_�[�ɓn���{�[���s��\����
	struct BoneCombMtricies
	{
		static constexpr u_int MAX_BONEMTX{ 100 };	// �V�F�[�_�[�̓n���{�[���s��̍ő吔
		DXSimp::Matrix matrix[MAX_BONEMTX];
	};


	/// @brief �{�[���s��̃o�b�t�@����������C���^�[�t�F�[�X
	class IBoneBufferSupplier
	{

	public:
		IBoneBufferSupplier() = default;
		virtual ~IBoneBufferSupplier() = default;

		/// @brief �{�[���s��̃o�b�t�@���擾����
		/// @return �{�[���s��̃o�b�t�@(�]�u�ς�)
		virtual BoneCombMtricies* GetBoneBuffer() = 0;
	};
}


