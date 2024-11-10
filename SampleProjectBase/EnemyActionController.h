#pragma once
#include "CharacterActionController.h"
#include "ChangeAnimObserver.h"

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
};
