#pragma once
#include "CP_Collider.h"

namespace HashiTaku
{
	class CP_BoxCollider : public CP_Collider
	{
		/// @brief ����
		DirectX::SimpleMath::Vector3 length;

		// AABB�t���O
		bool isAABB = false;

	public:
		CP_BoxCollider();
		~CP_BoxCollider() {}

		/// @brief �������Z�b�g����
		/// @param _length 
		void SetLength(const DirectX::SimpleMath::Vector3& _length);

		/// @brief �������擾����
		/// @return �{�b�N�X�̒���
		const DirectX::SimpleMath::Vector3& GetLength()const;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void Init();

		/// @brief �{�b�N�X�̌`����쐬
		void CreateShape() override;

		/// @brief ���f���̃T�C�Y���瓖���蔻��̑傫�������߂�
		void SizeFromModelSize();

		/// @brief �����X�V
		void LengthUpdate();

		/// @brief ���[���h���W�Ɍv�Z
		/// @param ���[���h���W�ł̒���
		void CalcWorldLength(DirectX::SimpleMath::Vector3& _out);

		void ImGuiDebug() override;
	};
}