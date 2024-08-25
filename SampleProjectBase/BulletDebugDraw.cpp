#include "pch.h"
#include "BulletDebugDraw.h"

#include "ShaderCollection.h"
#include "VertexBuffer.h"

namespace DX = DirectX::SimpleMath;

using namespace HashiTaku;

BulletDebugDraw::BulletDebugDraw()
    : debugMode(DBG_DrawWireframe), pVxShader(nullptr), pPxShader(nullptr)
{
    ShaderCollection* shCol = ShaderCollection::GetInstance();

    pVxShader = shCol->GetVertexShader("VS_Line");
    pPxShader = shCol->GetPixelShader("PS_Line");

    assert(pVxShader && pPxShader && "�V�F�[�_�[������Ɏ擾�ł��܂���");
}

void BulletDebugDraw::drawLine(const btVector3& _from, const btVector3& _to, const btVector3& _color)
{
    DX::Vector3 start(Bullet::ToDXVector3(_from));
    DX::Vector3 end(Bullet::ToDXVector3(_to));
    DX::Vector3 lineColor(
        Bullet::ToDxScalar(_color.x()), 
        Bullet::ToDxScalar(_color.y()),
        Bullet::ToDxScalar(_color.z())
    );

    // ���C����`�悷�邽�߂̃f�[�^���o�b�t�@�ɒǉ�
    lines.push_back({ start, lineColor });
    lines.push_back({ end, lineColor });
}

void BulletDebugDraw::drawContactPoint(const btVector3& _pointOnB, const btVector3& _normalOnB, btScalar _distance, int _lifeTime, const btVector3& _color)
{
    // �ڐG�_�ɒZ�����C����`�悷��i�@���̕����Ɂj
    btVector3 to = _pointOnB + _normalOnB * _distance;

    drawLine(_pointOnB, to, _color);

    // �I�v�V�����ŁA�ڐG�_�ɓ_��`�悷��
    drawLine(_pointOnB, _pointOnB + btVector3(0.1f, 0.1f, 0.1f), _color);
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

void BulletDebugDraw::Draw()
{
    if (lines.empty()) return;

    VertexBuffer* pVertexBuf = new VertexBuffer();

    u_int lineCnt = static_cast<u_int>(lines.size());
    u_int allSize = sizeof(LineVertex) * lineCnt;

    pVertexBuf->CreateBuffer(allSize, 0, lines.data());
    
    D3D11_Renderer* pRenderer = Direct3D11::GetInstance()->GetRenderer();
    ID3D11DeviceContext* pContext = pRenderer->GetDeviceContext();

    RenderParam::WVP wvp = pRenderer->GetParameter().GetWVP(
        DX::Vector3::Zero,
        DX::Vector3::One,
        DX::Quaternion::Identity
        );

    pVxShader->UpdateSubResource(0, &wvp);

    pVxShader->SetGPU();
    pPxShader->SetGPU();

    pVertexBuf->SetGPU();

    // ���C���̕`��
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    pContext->Draw(lineCnt, 0);

    // ���
    delete pVertexBuf;
    lines.clear();
}
