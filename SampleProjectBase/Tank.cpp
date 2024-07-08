#include "pch.h"
#include "Tank.h"

#include "PlayerMove.h"
#include "PlayerShot.h"
#include "PlayerInput.h"
Tank::Tank()
{
	AddComponent<PlayerMove>();
	AddComponent<PlayerShot>();
	AddComponent<PlayerInput>();
}

Tank::~Tank()
{
}
