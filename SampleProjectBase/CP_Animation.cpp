#include "pch.h"
#include "CP_Animation.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"
#include "AssetGetter.h"
#include "AnimationController.h"

using namespace DirectX::SimpleMath;

Vector3 g_rootOffset;

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
	static bool isWindowOpen = true;

	// コントローラー名取得
	std::string controllerName;
	if (pAnimController)
		controllerName = pAnimController->GetAssetName();

	// コントローラー名表示
	std::string text = "controllerName:";
	text += controllerName;
	ImGui::Text(text.c_str());

	// コントローラー変更
	if (AssetGetter::ImGuiGetCombobox<AnimationController>("animationController", controllerName))
	{
		AnimationController* pGetAnimCon = AssetGetter::GetAsset<AnimationController>(controllerName);
		if (pGetAnimCon)
			SetAnimationController(*pGetAnimCon);
	}

	ImGui::Begin("Animation", &isWindowOpen);
	if (pAnimController)
		pAnimController->ImGuiCall();
	ImGui::End();
}

void CP_Animation::SetupSkeletalMesh()
{
	// レンダラーに設定しているスケルタルメッシュを取得
	CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
	SkeletalMesh* pSetMesh = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());

	if (pSetMesh)	// スケルタルメッシュがあるなら
		pSkeletalMesh = pSetMesh;
}

void CP_Animation::SetAnimationController(AnimationController& _controller)
{
	pAnimController = &_controller;
}

SkeletalMesh& CP_Animation::GetSkeletalMesh()
{
	return *pSkeletalMesh;
}

AnimationController* CP_Animation::GetAnimationController()
{
	return pAnimController;
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
	if (pSkeletalMesh)
		pAnimController->Begin(pSkeletalMesh->GetBoneList());
}

void CP_Animation::UpdateAnimationMtx()
{
	// アニメーションコントローラーで更新する
	pAnimController->Update(pSkeletalMesh->GetBoneList());
}

bool CP_Animation::IsCanPlay()
{
	// アニメーションコントローラー非設定
	if (pAnimController == nullptr) return false;

	assert(pSkeletalMesh != nullptr && "スケルタルメッシュ非設定");

	return true;
}

void CP_Animation::UpdateBoneCombMtx()
{
	TreeNode& pRootNode = pSkeletalMesh->GetRootNode();

	Vector3 loadScales = Vector3::One * pSkeletalMesh->GetLoadOffsetScale();
	Vector3 loadAngles = pSkeletalMesh->GetLoadOffsetAngles();
	offsetMtx =
		Matrix::CreateScale(Vector3::One * pSkeletalMesh->GetLoadOffsetScale()) * Mtx::CreateRoratateMtx(loadAngles);

	Matrix transformMtx = pRootNode.GetTransformMtx();

	// ノードを辿って全体のコンビネーション行列を更新していく
	UpdateNodeHierarchy(pRootNode, Matrix::Identity);
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
	/*pAnimController = _other.pAnimController;*/
}
