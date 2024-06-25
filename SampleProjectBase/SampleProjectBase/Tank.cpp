#include "pch.h"
#include "Tank.h"

#include "PlayerMove.h"

Tank::Tank()
{
	AddComponent<PlayerMove>();

}

Tank::~Tank()
{
}
