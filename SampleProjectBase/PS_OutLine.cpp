#include "pch.h"
#include "PS_OutLine.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	PS_OutLine::PS_OutLine()
	{
		bufferTypes.resize(1);
		bufferTypes[0] = BufferType::CameraPos;
	}
	void PS_OutLine::OnceUpdateBuffer()
	{
		InSceneSystemManager* pSceneSystem = InSceneSystemManager::GetInstance();
#ifdef EDIT
		if (pSceneSystem->GetIsNullCamera()) return;
#endif // EDIT

		CP_Camera& cam = pSceneSystem->GetMainCamera();
		DXSimp::Vector3 camPos = cam.GetTransform().GetPosition();
		UpdateSubResource(0, &camPos);
	}
}

