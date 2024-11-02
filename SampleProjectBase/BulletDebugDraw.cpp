#include "pch.h"
#include "BulletDebugDraw.h"

#include "ShaderCollection.h"
#include "VertexBuffer.h"

#include "Geometory.h"

namespace DX = DirectX::SimpleMath;

using namespace HashiTaku;

constexpr float CONTACT_DRAW_LENGTH(0.5f);	// �ڐG�_��`�悷��Ƃ��̐��̒���

BulletDebugDraw::BulletDebugDraw()
	: debugMode(DBG_DrawWireframe), isDisplay(true)
{
}

void BulletDebugDraw::drawLine(const btVector3& _from, const btVector3& _to, const btVector3& _color)
{
	DX::Vector3 start(Bullet::ToDXVector3(_from));
	DX::Vector3 end(Bullet::ToDXVector3(_to));
	DX::Color lineColor(
		Bullet::ToDxScalar(_color.x()),
		Bullet::ToDxScalar(_color.y()),
		Bullet::ToDxScalar(_color.z()),
		1.0f
	);

	// ���C����`�悷�邽�߂̃f�[�^���o�b�t�@�ɒǉ�
	Geometory::AddLine(start, end, lineColor);
}

void BulletDebugDraw::drawContactPoint(const btVector3& _pointOnB, const btVector3& _normalOnB, btScalar _distance, int _lifeTime, const btVector3& _color)
{
	// �ڐG�_�ɒZ�����C����`�悷��i�@���̕����Ɂj
	btVector3 to = _pointOnB + _normalOnB * _distance;

	drawLine(_pointOnB, to, _color);

	// �I�v�V�����ŁA�ڐG�_�ɓ_��`�悷��
	//drawLine(_pointOnB, _pointOnB + btVector3(0.1f, 0.1f, 0.1f), _color);
}

void BulletDebugDraw::reportErrorWarning(const char* _warningString)
{
	HASHI_DEBUG_LOG(_warningString);
}

void BulletDebugDraw::draw3dText(const btVector3& _location, const char* _textString)
{
}

void BulletDebugDraw::setDebugMode(int _debugMode)
{
	debugMode = _debugMode;
}

int BulletDebugDraw::getDebugMode() const
{
	return debugMode;
}

void BulletDebugDraw::SetIsDisplay(bool _isDisplay)
{
	isDisplay = _isDisplay;
}

bool BulletDebugDraw::GetIsDisplay() const
{
	return isDisplay;
}
