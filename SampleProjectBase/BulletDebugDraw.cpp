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
       /* Bullet::ToDxScalar(_color.x()), 
        Bullet::ToDxScalar(_color.y()),
        Bullet::ToDxScalar(_color.z())*/

        Bullet::ToDxScalar(0),
        Bullet::ToDxScalar(0),
        Bullet::ToDxScalar(0)
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

    Microsoft::WRL::ComPtr<ID3D11Buffer> pBuffer;

    u_int lineCnt = static_cast<u_int>(lines.size());
    u_int allSize = sizeof(LineVertex) * lineCnt;

    auto pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

    D3D11_BUFFER_DESC desc;

    // ���_�o�b�t�@�̏������ݒ�
    desc.ByteWidth = allSize;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA data;

    // ���������ɐݒ肷��o�b�t�@�f�[�^
    data.pSysMem = lines.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    // ���_�o�b�t�@�̐���
    HRESULT hr = pDevice->CreateBuffer(&desc, &data, &pBuffer);

    if (FAILED(hr)) return;

    D3D11_Renderer* pRenderer = Direct3D11::GetInstance()->GetRenderer();
    ID3D11DeviceContext* pContext = pRenderer->GetDeviceContext();

    RenderParam::WVP wvp = pRenderer->GetParameter().GetWVP(
        DX::Vector3::Zero,
        DX::Vector3::One,
        DX::Quaternion::Identity
        );

    wvp.world.Transpose(wvp.world);

    pVxShader->UpdateSubResource(0, &wvp);

    pVxShader->SetGPU();
    pPxShader->SetGPU();

    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    // �f�o�C�X�R���e�L�X�g�擾
    ID3D11DeviceContext* pDevicecontext =
        Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

    // ���_�o�b�t�@���Z�b�g����
    unsigned int stride = sizeof(LineVertex);
    unsigned  offset = 0;
    pDevicecontext->IASetVertexBuffers(0, 1, pBuffer.GetAddressOf(), &stride, &offset);

    // ���C���̕`��
    pContext->Draw(lineCnt, 0);

    // ���
    lines.clear();
}
