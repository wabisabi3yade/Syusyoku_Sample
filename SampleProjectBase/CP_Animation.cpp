#include "pch.h"
#include "CP_Animation.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"
#include "AssetGetter.h"
#include "AnimationController.h"

namespace HashiTaku
{
	CP_Animation::BoneCombMtricies CP_Animation::boneCombBuffer;

	// ボーン行列のスロット番号
	constexpr int SHBUFFER_BONE_SLOT(1);

	CP_Animation::CP_Animation() :
		pSkeletalMesh(nullptr), pAnimController(nullptr), boneCnt(0)
	{
	}

	void CP_Animation::Init()
	{
		for (u_int i = 0; i < MAX_BONEMTX; i++)
		{
			boneCombBuffer.matrix[i] = DXSimp::Matrix::Identity.Transpose();
		}

		// ボーンのコピーを行う
		CopyBoneList();
	}

	void CP_Animation::Awake()
	{
		if (!pSkeletalMesh)
			CopyBoneList();

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
	}

	void CP_Animation::Draw()
	{
		// ボーン行列をバッファとして送信
		UpdateBoneBuffer();
	}

	void CP_Animation::ImGuiDebug()
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

	void CP_Animation::SetAnimationController(AnimationController& _controller)
	{
		pAnimController = &_controller;
	}

	void CP_Animation::SetControllerPlaySpeed(float _setSpeed)
	{
		pAnimConPlayer->SetCurrentPlaySpeed(_setSpeed);
	}

	void CP_Animation::SetCurNodePlayerSpeed(float _setSpeed)
	{
		pAnimConPlayer->GetCurNodePlayer().SetPlaySpeedTimes(_setSpeed);
	}

	void CP_Animation::SetPlayRatio(float _playRatio)
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("アニメーション再生が作成されていません");
			return;
		}
#endif // EDIT

		pAnimConPlayer->GetCurNodePlayer().SetCurPlayRatio(_playRatio);
	}

	void CP_Animation::AddChangeAnimObserver(ChangeAnimObserver& _observer)
	{
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("アニメーション再生クラスが生成していないので追加できません");
			return;
		}

		pAnimConPlayer->AddChangeAnimObserver(_observer);
	}

	void CP_Animation::RemoveChangeAnimObserver(ChangeAnimObserver& _observer)
	{
		pAnimConPlayer->RemoveChangeAnimObserver(_observer);
	}

	const DirectX::SimpleMath::Vector3& CP_Animation::GetMotionPosSpeedPerSec() const
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("アニメーション再生が作成されていません");
			return  DXSimp::Vector3::Zero;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurNodePlayer().GetRootMotionSpeed();
	}

	DirectX::SimpleMath::Vector3 CP_Animation::GetCurAnimRMPos(float _ratio)
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("アニメーション再生が作成されていません");
			return  DXSimp::Vector3::Zero;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurNodePlayer().GetRootMotionPos(_ratio, true);
	}

	float CP_Animation::GetControllerPlaySpeed() const
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("アニメーション再生が作成されていません");
			return 0.0f;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurrentPlaySpeed();
	}

	float CP_Animation::GetCurrentPlayRatio() const
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("アニメーション再生が作成されていません");
			return 0.0f;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurNodePlayer().GetCurPlayRatio();
	}

	BoneList* CP_Animation::GetMoveBoneList()
	{
		return pMoveBoneList.get();
	}

	u_int CP_Animation::GetBoneCnt() const
	{
		if (!pSkeletalMesh) return 0;

		return pSkeletalMesh->GetBoneCnt();
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
		if (LoadJsonString("animConName", animConName, _data))
		{
			AnimationController* pLoadController = AssetGetter::GetAsset<AnimationController>(animConName);
			if (pLoadController)
				SetAnimationController(*pLoadController);
		}
	}

	void CP_Animation::SetupAnimCon()
	{
		if (!pMoveBoneList) return;

		pAnimConPlayer = std::make_unique<AnimControllPlayer>(*pAnimController, *pMoveBoneList, GetTransform());
	}

	void CP_Animation::CopyBoneList()
	{
		CP_MeshRenderer* pMr = gameObject->GetComponent<CP_MeshRenderer>();
		if (!pMr) return;

		// レンダラーに設定しているスケルタルメッシュを取得
		pSkeletalMesh = dynamic_cast<SkeletalMesh*>(pMr->GetRenderMesh());
		if (!pSkeletalMesh) return;

		// アニメーションするボーンをコピーし、作成
		pMoveBoneList = std::make_unique<BoneList>(pSkeletalMesh->GetBoneList());

		// モデル関係
		pMr->SetVertexShader("VS_SkinAnimation");
	}

	void CP_Animation::UpdateAnimationMtx()
	{
		// アニメーションプレイヤーで更新する
		pAnimConPlayer->Update(DeltaTime());
	}

	bool CP_Animation::IsCanPlay()
	{
#ifdef EDIT
		// アニメーションコントローラー非設定
		if (pAnimController == nullptr) return false;
		assert(pMoveBoneList && "アニメーションするボーンが作成されていません");
#endif // EDIT

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
		using namespace DXSimp;

		const TreeNode* pRootNode = pSkeletalMesh->GetRootNode();

		// ルートポジション座標を引いて、メッシュを移動させないようにする(y座標は反映しない)
		Vector3 rootPos;
		pAnimConPlayer->GetCurrentRootPos(rootPos);
		rootPos.y = 0.0f;
		rootPos *= -1.0f;
		Matrix posMtx = Matrix::CreateTranslation(rootPos);

		// ロードの回転、スケール
		Vector3 loadScales = Vector3::One * pSkeletalMesh->GetLoadOffsetScale();
		Vector3 loadAngles = pSkeletalMesh->GetLoadOffsetAngles();
		rootOffsetMtx =
			Matrix::CreateScale(Vector3::One * loadScales) * Mtx::CreateRoratateMtx(loadAngles);

		// ノードを辿って全体のコンビネーション行列を更新していく
		UpdateNodeHierarchy(*pRootNode, posMtx);
	}

	void CP_Animation::UpdateNodeHierarchy(const TreeNode& _treeNode, 
		const DXSimp::Matrix& _parentMtx)
	{
		DXSimp::Matrix nodeMatrix = DXSimp::Matrix::Identity;

		// 対応したボーンがあるなら
		if (_treeNode.HasBone())
		{
			u_int boneId = _treeNode.GetBoneIdx();
			Bone& moveBone = *pMoveBoneList->GetBone(boneId);
			nodeMatrix = moveBone.GetAnimMtx();

			// ローカル空間内のボーン座標を求める
			moveBone.CreateGlobalMtx(_parentMtx, rootOffsetMtx);

			// コンビネーション行列を求める
			moveBone.CreateCombMtx(_parentMtx);
		}

		DXSimp::Matrix toWorldMtx = nodeMatrix * _parentMtx;

		// 再帰的にボーンを更新していく
		for (u_int c_i = 0; c_i < _treeNode.GetChildNum(); c_i++)
		{
			UpdateNodeHierarchy(*_treeNode.GetChild(c_i), toWorldMtx);
		}
	}

	void CP_Animation::UpdateBoneBuffer()
	{
		if (!pSkeletalMesh) return;

		if (pMoveBoneList)	// 動かすボーンがあるか
		{
			u_int bufferCnt = static_cast<u_int>(pMoveBoneList->GetBoneCnt());
			// ボーン数ループ
			for (u_int b_i = 0; b_i < bufferCnt; b_i++)
			{
				const Bone& bone = *pMoveBoneList->GetBone(b_i);

				// ボーンのID番目に行列を入れる
				boneCombBuffer.matrix[bone.GetIndex()] = bone.GetCombMtx();

				// シェーダーに渡すので転置行列を作成
				boneCombBuffer.matrix[bone.GetIndex()] =
					boneCombBuffer.matrix[bone.GetIndex()].Transpose();
			}
		}

		// シェーダーにボーン行列を渡す
		u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
		for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
		{
			Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
			/*pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));*/
			pMaterial->GetVertexShader().UpdateSubResource(1, &boneCombBuffer);
		}
	}
}