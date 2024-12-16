#pragma once
#include "CharacterActionController.h"
#include "ChangeAnimObserver.h"

namespace HashiTaku
{
	class CP_Enemy;
	class CP_Animation;

	/// @brief �G�̃A�N�V�����Ǘ��N���X
	/// @tparam T �A�N�V�����̃X�e�[�g��\���񋓌^
	class EnemyActionController : public CharacterActionController
	{

	public:
		/// @brief �R���X�g���N�^
		/// @param _enemy �G�l�~�[�R���|�[�l���g
		/// @param _controllerName �A�N�V�����R���g���[���[��
		EnemyActionController(CP_Enemy& _enemy, const std::string& _controllerName);
		virtual ~EnemyActionController() {}

	protected:
		/// @brief �eState�̕�������擾����
		/// @param _stateId ��Ԃ�ID
		/// @return ������
		virtual std::string GetStateStr(int _stateId) = 0;

		/// @brief �eState��ID���擾����
		/// @param _stateName�@��Ԗ�
		/// @return ��Ԃ�ID
		virtual int GetStateId(const std::string& _stateName) = 0;
	};
}