#include "pch.h"

namespace HashiTaku
{
	void IImGuiUser::ImGuiCall()
	{
#ifdef EDIT
		ImGuiDebug();
#endif // EDIT
	}
}