#include "pch.h"
#include "CP_Animation.h"

#include "GameObject.h"

// メッシュ
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"

// アセット取得
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

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

void CP_Animation::Start()
{
	// レンダラーに設定しているスケルタルメッシュを取得
	CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
	SkeletalMesh* pSetMesh = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());

	if (pSetMesh != nullptr)
	{
		SetSkeletalMesh(*pSetMesh);
	}
}

void CP_Animation::LateUpdate()
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

	if (ImGui::Button(TO_UTF8("ウィンドウ")))
		isWindowOpen = true;

	if (!isWindowOpen) return;

	std::string name = "アニメーション";
	ImGui::Begin(TO_UTF8(name), &isWindowOpen);

	if (pAnimController)
		pAnimController->ImGuiSetting();

	ImGui::End();
}

void CP_Animation::SetSkeletalMesh(SkeletalMesh& _skeletalMesh)
{
	pSkeletalMesh = &_skeletalMesh;
}

void CP_Animation::SetAnimationController(AnimationController& _controller)
{
	pAnimController = &_controller;
	pAnimController->SetBoneList(pSkeletalMesh->GetBoneList());
}

SkeletalMesh& CP_Animation::GetSkeletalMesh()
{
	return *pSkeletalMesh;
}

nlohmann::json CP_Animation::Save()
{
	auto data = Component::Save();

	return data;
}

void CP_Animation::Load(const nlohmann::json& _data)
{
	Component::Load(_data);
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

	Matrix m = Matrix::CreateScale(Vector3::One * 0.01f) *
		Matrix::CreateFromYawPitchRoll(
			180.f * Mathf::degToRad,
			0,
			0
		);


	// ノードを辿って全体のコンビネーション行列を更新していく
	UpdateNodeHierarchy(pRootNode, m/*Matrix::Identity*/);
}

void CP_Animation::UpdateNodeHierarchy(TreeNode& _treeNode, const Matrix& _parentMtx)
{
	Matrix nodeMatrix = Matrix::Identity;

	/*Matrix nodeMatrix = _treeNode.GetTransformMtx();*/
	/*Matrix rotationY = DirectX::XMMatrixRotationZ(DirectX::XM_PI);
	Matrix scaleZ = DirectX::XMMatrixScaling(1.0f, 1.0f, -1.0f);*/
	Matrix mi = nodeMatrix;


	// 対応したボーンがあるなら
	if (_treeNode.HasBone())
	{
		Bone& pBone = _treeNode.GetBone();

		// コンビネーション行列を求める
		pBone.CreateCombMtx(_parentMtx);
		mi = pBone.GetAnimMtx();
	}

	Matrix toWorldMtx = mi * _parentMtx;

	//if (_treeNode.GetName().find("AssimpFbx$_PreRotation") != std::string::npos)
	//{
	//	Matrix rotationY = DirectX::XMMatrixRotationY(DirectX::XM_PI);
	//	Matrix scaleZ = DirectX::XMMatrixScaling(1.0f, 1.0f, -1.0f);
	//	nodeMatrix = rotationY */* scaleZ * */nodeMatrix;

	//	Matrix toWorldMtx = /*nodeMatrix **/ _parentMtx;
	//}
	/*else
	{

		Matrix toWorldMtx = nodeMatrix * _parentMtx;
	}*/

	
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
	pAnimController = _other.pAnimController;
}
