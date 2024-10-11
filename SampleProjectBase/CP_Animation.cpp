#include "pch.h"
#include "CP_Animation.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"
#include "AssetGetter.h"
#include "AnimationController.h"

using namespace DirectX::SimpleMath;

CP_Animation::BoneCombMtricies CP_Animation::boneComb;

CP_Animation::CP_Animation() : pSkeletalMesh(nullptr), pAnimController(nullptr)
{
}

CP_Animation::CP_Animation(const CP_Animation& _other)
{
	Copy(_other);
}

CP_Animation& CP_Animation::operator=(const CP_Animation& _other)
{
	Copy(_other);

	return *this;
}

void CP_Animation::Awake()
{
	// スケルタルメッシュ準備
	SetupSkeletalMesh();

	// アニメーションコントローラー準備
	SetupAnimCon();
}

void CP_Animation::Update()
{
	if (!IsCanPlay()) return;

	// アニメーション行列を更新
	UpdateAnimationMtx();

	// コンビネーション行列を更新
	UpdateBoneCombMtx();

	// ボーン行列をバッファとして送信
	UpdateBoneBuffer();
}

void CP_Animation::ImGuiSetting()
{
	// コントローラー名表示
	std::string text = "controllerName:";
	std::string controllerName = "Null";
	if (pAnimController)
		controllerName = pAnimController->GetAssetName();
	text += controllerName;
	ImGui::Text(text.c_str());

	// コントローラー変更
	if (AssetGetter::ImGuiGetCombobox<AnimationController>("animationController", controllerName))
	{
		AnimationController* pGetAnimCon = AssetGetter::GetAsset<AnimationController>(controllerName);
		if (pGetAnimCon)
			SetAnimationController(*pGetAnimCon);
	}

	// コントローラー再生
	if (pAnimConPlayer)
		pAnimConPlayer->ImGuiCall();
}

void CP_Animation::SetBool(const std::string& _paramName, bool _isBool)
{
#ifdef EDIT
	// コピーアニメーションパラメータがあるか確認
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetBool(_paramName, _isBool);
}

void CP_Animation::SetInt(const std::string& _paramName, int _intVal)
{
#ifdef EDIT
	// コピーアニメーションパラメータがあるか確認
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetInt(_paramName, _intVal);
}

void CP_Animation::SetFloat(const std::string& _paramName, float _floatVal)
{
#ifdef EDIT
	// コピーアニメーションパラメータがあるか確認
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetFloat(_paramName, _floatVal);
}

void CP_Animation::SetTrigger(const std::string& _paramName)
{
#ifdef EDIT
	// コピーアニメーションパラメータがあるか確認
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetTrigger(_paramName);
}

bool CP_Animation::GetBool(const std::string& _paramName)
{
#ifdef EDIT
	// コピーアニメーションパラメータがあるか確認
	if (!IsExistCopyAnimParameter()) return false;
#endif // EDIT

	return pAnimConPlayer->GetCopyAnimParameters().GetBool(_paramName);
}

int CP_Animation::GetInt(const std::string& _paramName)
{
#ifdef EDIT
	// コピーアニメーションパラメータがあるか確認
	if (!IsExistCopyAnimParameter()) return 0;
#endif // EDIT

	return pAnimConPlayer->GetCopyAnimParameters().GetInt(_paramName);
}

float CP_Animation::GetFloat(const std::string& _paramName)
{
#ifdef EDIT
	// コピーアニメーションパラメータがあるか確認
	if (!IsExistCopyAnimParameter()) return 0.0f;
#endif // EDIT

	return pAnimConPlayer->GetCopyAnimParameters().GetFloat(_paramName);
}

void CP_Animation::SetupSkeletalMesh()
{
	// レンダラーに設定しているスケルタルメッシュを取得
	CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
	pSkeletalMesh = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());
}

void CP_Animation::SetAnimationController(AnimationController& _controller)
{
	pAnimController = &_controller;
}

void CP_Animation::SetCurPlayerSpeed(float _setSpeed)
{
	pAnimConPlayer->GetCurNodePlayer().SetPlaySpeedTimes(_setSpeed);
}

SkeletalMesh& CP_Animation::GetSkeletalMesh()
{
	return *pSkeletalMesh;
}

AnimationController* CP_Animation::GetAnimationController()
{
	return pAnimController;
}

const DirectX::SimpleMath::Vector3& CP_Animation::GetMotionPosSpeedPerSec() const
{
#ifdef EDIT
	if (!pAnimConPlayer)
	{
		HASHI_DEBUG_LOG("アニメーション再生が作成されていません");
		return Vector3::Zero;
	}
#endif // EDIT

	return pAnimConPlayer->GetCurNodePlayer().GetRootMotionSpeed();
}

nlohmann::json CP_Animation::Save()
{
	auto data = Component::Save();

	if (pAnimController)
	{
		data["animConName"] = pAnimController->GetAssetName();
	}

	return data;
}

void CP_Animation::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	std::string animConName = "";
	if (HashiTaku::LoadJsonString("animConName", animConName, _data))
	{
		AnimationController* pLoadController = AssetGetter::GetAsset<AnimationController>(animConName);
		if (pLoadController)
			SetAnimationController(*pLoadController);
	}
}

void CP_Animation::SetupAnimCon()
{
	if (!pSkeletalMesh) return;

	pAnimConPlayer = std::make_unique<AnimControllPlayer>(*pAnimController, pSkeletalMesh->GetBoneList(), GetTransform());
}

void CP_Animation::UpdateAnimationMtx()
{
	// アニメーションプレイヤーで更新する
	pAnimConPlayer->Update();
}

bool CP_Animation::IsCanPlay()
{
	// アニメーションコントローラー非設定
	if (pAnimController == nullptr) return false;

	assert(pSkeletalMesh != nullptr && "スケルタルメッシュ非設定");

	return true;
}

bool CP_Animation::IsExistCopyAnimParameter()
{
	if (!pAnimConPlayer)
	{
		HASHI_DEBUG_LOG("シーンを開始後呼び出してください");
		return false;
	}

	return true;
}

void CP_Animation::UpdateBoneCombMtx()
{
	TreeNode& pRootNode = pSkeletalMesh->GetRootNode();

	// ルートポジション座標を引いて、メッシュを移動させないようにする(y座標は反映しない)
	Vector3 rootPos;
	pAnimConPlayer->GetCurrentRootPos(rootPos);
	rootPos.y = 0.0f;
	rootPos *= -1.0f;
	Matrix posMtx = Matrix::CreateTranslation(rootPos);


	// ノードを辿って全体のコンビネーション行列を更新していく
	UpdateNodeHierarchy(pRootNode, posMtx);
}

void CP_Animation::UpdateNodeHierarchy(TreeNode& _treeNode, const Matrix& _parentMtx)
{
	Matrix nodeMatrix = Matrix::Identity;

	// 対応したボーンがあるなら
	if (_treeNode.HasBone())
	{
		Bone& pBone = _treeNode.GetBone();

		nodeMatrix = pBone.GetAnimMtx();

		// ローカル空間内のボーン座標を求める
		pBone.CreateGlobalMtx(_parentMtx, offsetMtx);

		// コンビネーション行列を求める
		pBone.CreateCombMtx(_parentMtx);
	}

	Matrix toWorldMtx = nodeMatrix * _parentMtx;

	// 再帰的にボーンを更新していく
	for (u_int c_i = 0; c_i < _treeNode.GetChildNum(); c_i++)
	{
		UpdateNodeHierarchy(*_treeNode.GetChild(c_i), toWorldMtx);
	}
}

void CP_Animation::UpdateBoneBuffer()
{
	u_int boneCnt = pSkeletalMesh->GetBoneNum();

	// ボーン数ループ
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		const Bone& bone = *pSkeletalMesh->GetBone(b_i);

		// ボーンのID番目に行列を入れる
		boneComb.matrix[bone.GetIndex()] = bone.GetCombMtx();
		boneComb.matrix[bone.GetIndex()] = boneComb.matrix[bone.GetIndex()].Transpose();
	}

	// シェーダーにボーン行列を渡す
	u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
	for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
	{
		Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
		/*pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));*/
		pMaterial->GetVertexShader().UpdateSubResource(1, &boneComb);
	}
}

void CP_Animation::Copy(const CP_Animation& _other)
{
	if (&_other == this) return;

	Component::operator=(_other);

	pSkeletalMesh = _other.pSkeletalMesh;
	offsetMtx = _other.offsetMtx;
	pAnimController = _other.pAnimController;
}
