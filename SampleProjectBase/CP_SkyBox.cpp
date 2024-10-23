#include "pch.h"
#include "CP_SkyBox.h"
#include "Texture.h"
#include "MaterialCreater.h"
#include "AssetGetter.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

constexpr u_int FACE_CNT = static_cast<u_int>(CP_SkyBox::FaceKind::MaxNum);	// 面の数
constexpr float DEFAULT_SIZE(1000.0f);	// デフォルトのスカイボックスサイズ
constexpr float CENTER_ANGLE(90.0f);	// 面の単位角度
constexpr auto MATERIAL_NAME = "M_SkyBox";	// マテリアル名
constexpr auto TEX_NULL = "Null";	// テクスチャが設定されていない時の名前

const std::vector<std::string> g_faceKindName =
{
	"X_Right",
	"X_Left",
	"Y_Top",
	"Y_Bottom",
	"Z_Front",
	"Z_Back"
};

CP_SkyBox::CP_SkyBox() : pMaterial(nullptr), size(DEFAULT_SIZE)
{
	for (u_int t_i = 0; t_i < FACE_CNT; t_i++)
	{
		pFaces[t_i] = std::make_unique<Sprite>();
	}

	MaterialSetting();
}

CP_SkyBox::CP_SkyBox(const CP_SkyBox& _other)
{
	Copy(_other);
}

CP_SkyBox& CP_SkyBox::operator=(const CP_SkyBox& _other)
{
	Copy(_other);
	return *this;
}

void CP_SkyBox::Init()
{
	pSpriteDrawer = std::make_unique<SpriteDrawer>();
}

void CP_SkyBox::Draw()
{
	if (!IsCanDraw()) return;

	for (u_int f_i = 0; f_i < FACE_CNT; f_i++)
	{
		FaceDraw(f_i);
	}
}

void CP_SkyBox::ImGuiSetting()
{
	ImGui::DragFloat("size", &size);

	for (u_int t_i = 0; t_i < FACE_CNT; t_i++)
	{
		Sprite& sprite = *pFaces[t_i];

		std::string texName = "Null";
		if (sprite.GetIsTexEnable())
			texName = sprite.GetTexture()->GetAssetName();
		ImGui::Text(texName.c_str());
	}

	// テクスチャ設定
	static char str[IM_INPUT_BUF] = "\0";
	ImGui::InputText("texName", str, IM_INPUT_BUF);
	static u_int faceId = 0;
	ImGuiMethod::ComboBox("Face", faceId, g_faceKindName);
	if (ImGui::Button("Set"))
	{
		Texture* pTex = AssetGetter::GetAsset<Texture>(str);
		if (!pTex) return;

		SetTexture(*pTex, static_cast<FaceKind>(faceId));
	}
}

void CP_SkyBox::SetTexture(Texture& _pTex, FaceKind _setFace)
{
#ifdef _DEBUG
	if (_setFace >= FaceKind::MaxNum) return;
#endif // _DEBUG

	u_int num = static_cast<u_int>(_setFace);
	pFaces[num]->SetTexture(_pTex);
}

nlohmann::json CP_SkyBox::Save()
{
	auto data = Component::Save();

	// テクスチャ
	SaveTexture(data, FaceKind::X_Right);
	SaveTexture(data, FaceKind::X_Left);
	SaveTexture(data, FaceKind::Y_Top);
	SaveTexture(data, FaceKind::Y_Bottom);
	SaveTexture(data, FaceKind::Z_Front);
	SaveTexture(data, FaceKind::Z_Back);

	return data;
}

void CP_SkyBox::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadTexture(_data, FaceKind::X_Right);
	LoadTexture(_data, FaceKind::X_Left);
	LoadTexture(_data, FaceKind::Y_Top);
	LoadTexture(_data, FaceKind::Y_Bottom);
	LoadTexture(_data, FaceKind::Z_Front);
	LoadTexture(_data, FaceKind::Z_Back);
}

void CP_SkyBox::MaterialSetting()
{
	if (AssetGetter::CheckImport<Material>(MATERIAL_NAME))
	{
		pMaterial = AssetGetter::GetAsset<Material>(MATERIAL_NAME);
	}
	else
	{
		Material& createMaterial = MaterialCreater::CraeteAsset(MATERIAL_NAME);
		pMaterial = &createMaterial;
		pMaterial->SetVertexShader("VS_Sprite");
		pMaterial->SetVertexShader("PS_TexColor");
	}
}

bool CP_SkyBox::IsCanDraw()
{
	if (!pMaterial) return false;

	return true;
}

void CP_SkyBox::FaceDraw(u_int _faceNum)
{
	FaceKind facePlace = static_cast<FaceKind>(_faceNum);

	Vector3 pos, scale, angles;

	// 面ごとのパラメータを取得
	FaceTransform(facePlace, pos, scale, angles);

	// 描画する
	pSpriteDrawer->DrawSprite(*pFaces[_faceNum], *pMaterial, pos, scale, Quat::ToQuaternion(angles));
}

void CP_SkyBox::FaceTransform(FaceKind _facePlace, DirectX::SimpleMath::Vector3& _outPos, DirectX::SimpleMath::Vector3& _outScale, DirectX::SimpleMath::Vector3& _outAngles)
{
	Transform& transform = GetTransform();

	Vector3 faceVec;

	_outAngles.x = -CENTER_ANGLE;

	switch (_facePlace)
	{
	case CP_SkyBox::FaceKind::X_Right:
		faceVec.x = -1.0f;
		_outAngles.y = -CENTER_ANGLE;
		break;
	case CP_SkyBox::FaceKind::X_Left:
		faceVec.x = 1.0f;
		_outAngles.y = CENTER_ANGLE;
		break;
	case CP_SkyBox::FaceKind::Y_Top:
		faceVec.y = 1.0f;
		_outAngles.x = -CENTER_ANGLE * 2;
		break;
	case CP_SkyBox::FaceKind::Y_Bottom:
		faceVec.y = -1.0f;
		_outAngles.x = 0.0f;
		break;
	case CP_SkyBox::FaceKind::Z_Front:
		faceVec.z = 1.0f;
		break;
	case CP_SkyBox::FaceKind::Z_Back:
		faceVec.z = -1.0f;
		_outAngles.y = CENTER_ANGLE * 2;
		break;
	default:
		assert(!"指定した面の場所が不正です");
		break;
	}

	_outPos = faceVec * size * 0.5f;
	_outScale = Vector3::One * size;
}

void CP_SkyBox::Copy(const CP_SkyBox& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);

	for (u_int t_i = 0; t_i < FACE_CNT; t_i++)
	{
		pFaces[t_i] = std::make_unique<Sprite>();
		*pFaces[t_i] = *_other.pFaces[t_i];
	}

	pMaterial = _other.pMaterial;
	size = _other.size;
}

void CP_SkyBox::SaveTexture(nlohmann::json& _data, FaceKind _kind)
{
	int faceId = static_cast<int>(_kind);
	std::string arrayName = g_faceKindName[faceId];
	auto& texData = _data[arrayName];
	if (!pFaces[faceId]->GetIsTexEnable())
	{
		texData = TEX_NULL;
		return;
	}

	texData = pFaces[faceId]->GetTexture()->GetAssetName();
}

void CP_SkyBox::LoadTexture(const nlohmann::json& _data, FaceKind _kind)
{
	int faceId = static_cast<int>(_kind);
	std::string arrayName = g_faceKindName[faceId];
	std::string texName = TEX_NULL;
	HashiTaku::LoadJsonString(arrayName, texName, _data);
	if (texName == TEX_NULL) return;	// 設定されていないなら

	Texture* pTex = AssetGetter::GetAsset<Texture>(texName);
	if (!pTex) return;
	pFaces[faceId]->SetTexture(*pTex);
}
