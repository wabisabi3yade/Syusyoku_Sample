#include "pch.h"
#include "LayerdAnimationNode.h"
#include "AssetGetter.h"
#include "SkeletalMesh.h"

namespace HashiTaku
{
	constexpr int NOT_SET_BONEID(-1);	// ボーンを指定していない時のボーンID
	constexpr u_int START_BLEND_DEPTH(9999);	// 開始時のブレンドの深さ

	LayerdAnimationNode::LayerdAnimationNode(const BoneList& _boneList, const std::string& _nodeName) :
		AnimationNode_Base(_nodeName, NodeType::Layerd),
		pBoneList(&_boneList),
		pBaseAnimation(nullptr),
		pBlendAnimation(nullptr),
		beginBlendPlayFrame(0),
		beginBlendBoneId(NOT_SET_BONEID),
		isMeshSpaceRotationBlend(false),
		blendDepth(0)
	{
		blendSpeedCurve.SetCurveName("Blend Speed");
	}

	void LayerdAnimationNode::GetAnimTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const
	{
		// 機能できていないなら
		if (!CanWarking()) return;

		if (isMeshSpaceRotationBlend)	// メッシュ空間で回転量のブレンドを行う
		{
			GetMeshSpaceBlendTransform(_outTransforms,
				pBoneList->GetRootNode(),
				DXSimp::Quaternion::Identity,
				DXSimp::Quaternion::Identity,
				DXSimp::Quaternion::Identity,
				_baseRatio,
				_blendRatio,
				START_BLEND_DEPTH);
		}
		else 	// ローカル空間で回転量のブレンドを行う
		{
			GetLocalSpaceBlendTransform(_outTransforms,
				_baseRatio,
				_blendRatio);
		}

	}

	void LayerdAnimationNode::GetMeshSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms,
		const TreeNode* _pCurrentNode,
		const DXSimp::Quaternion& _parenBaseRotation,
		const DXSimp::Quaternion& _parentBlendRotation,
		const DXSimp::Quaternion& _parentCalculateRotation,
		float _baseRatio,
		float _blendRatio,
		u_int _curBlendDepth) const
	{
		// ボーンがないなら次に進む
		if (!_pCurrentNode->HasBone())
		{
			u_int childCnt = _pCurrentNode->GetChildNum();
			for (u_int c_i = 0; c_i < childCnt; c_i++)
				GetMeshSpaceBlendTransform(_outTransforms,
					_pCurrentNode->GetChild(c_i),
					_parenBaseRotation,
					_parentBlendRotation,
					_parentCalculateRotation,
					_baseRatio,
					_blendRatio,
					_curBlendDepth + 1
				);

			return;
		}

		// ブレンド開始のノードなら深さを0にする
		if (_pCurrentNode->GetBoneIdx() == beginBlendBoneId)
			_curBlendDepth = 1;

		u_int uBoneId = static_cast<u_int>(_pCurrentNode->GetBoneIdx());
		const Bone& bone = *_pCurrentNode->GetBone();

		// メッシュ空間の回転量(次回への引数に)
		DXSimp::Quaternion baseMeshSpaceRotation;
		DXSimp::Quaternion blendMeshSpaceRotation;
		DXSimp::Quaternion calcMeshSpaceRotation;

		// ブレンド側のアニメーションなら(開始時の深さよりも下、すなわちブレンド開始ボーンよりも子ノード)
		if (_curBlendDepth < START_BLEND_DEPTH)
		{
			// ブレンド深さ以内なら
			if (_curBlendDepth <= blendDepth)
			{
				// ベース側のアニメーショントランスフォーム
				BoneTransform baseAnimTransform;
				bool hasAnimation = pBaseAnimation->GetTransformByRatio(uBoneId, _baseRatio, baseAnimTransform);
				if (!hasAnimation)	// 初期姿勢アニメーション
				{
					baseAnimTransform = bone.GetLocalNodeTransform();
				}

				// ブレンド側も計算する
				BoneTransform blendAnimTransform;
				hasAnimation = pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, blendAnimTransform);
				if (!hasAnimation)	// 初期姿勢アニメーション
				{
					blendAnimTransform = bone.GetLocalNodeTransform();
				}

				// ブレンドする割合を求める(深さ0なら1)
				float blendFactor = 1.0f / blendDepth * _curBlendDepth;

				// ベースとブレンドのメッシュ空間の回転量を求める
				baseMeshSpaceRotation = Quat::Multiply(baseAnimTransform.rotation, _parenBaseRotation);
				blendMeshSpaceRotation = Quat::Multiply(blendAnimTransform.rotation, _parentBlendRotation);

				// ブレンドとベース側でブレンドを行う(メッシュ空間)
				calcMeshSpaceRotation = DXSimp::Quaternion::Slerp(baseMeshSpaceRotation, blendMeshSpaceRotation, blendFactor);

				// ローカル空間に戻す
				_outTransforms[uBoneId].rotation = Quat::RotationDifference(calcMeshSpaceRotation, _parentCalculateRotation);

				// トランスフォームと、スケールもブレンド(ローカル空間)
				_outTransforms[uBoneId].position =
					DXSimp::Vector3::Lerp(baseAnimTransform.position, blendAnimTransform.position, blendFactor);
				_outTransforms[uBoneId].scale =
					DXSimp::Vector3::Lerp(baseAnimTransform.scale, blendAnimTransform.scale, blendFactor);
			}
			else // ブレンド側のアニメーションで深さの範囲外
			{
				// ブレンドアニメーションを取得
				BoneTransform blendAnimTransform;
				if (!pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, blendAnimTransform))
				{
					// アニメーションないなら初期姿勢
					blendAnimTransform = bone.GetLocalNodeTransform();
				}

				// メッシュ空間の回転量
				blendMeshSpaceRotation = Quat::Multiply(blendAnimTransform.rotation, _parentBlendRotation);

				// 親ボーンとのローカル空間に戻す
				blendAnimTransform.rotation = Quat::RotationDifference(blendMeshSpaceRotation, _parentCalculateRotation);

				// 結果を代入
				_outTransforms[uBoneId] = blendAnimTransform;

				// 計算したメッシュ空間の回転量を求める(次回の為に)
				calcMeshSpaceRotation = blendMeshSpaceRotation;
			}
		}
		else // ベース側のアニメーション
		{
			pBaseAnimation->GetTransformByRatio(uBoneId, _baseRatio, _outTransforms[uBoneId]);

			// 次のために計算しておく
			baseMeshSpaceRotation = Quat::Multiply(_outTransforms[uBoneId].rotation, _parenBaseRotation);

			// ブレンド側も計算する
			DXSimp::Quaternion blendRotation;
			pBlendAnimation->GetQuaternionByRatio(uBoneId, _blendRatio, blendRotation);
			blendMeshSpaceRotation = Quat::Multiply(blendRotation, _parentBlendRotation);

			// ベース側を反映しているのでベースと同じ
			calcMeshSpaceRotation = baseMeshSpaceRotation;
		}

		// 子ノードに渡す
		u_int childCnt = _pCurrentNode->GetChildNum();
		for (u_int c_i = 0; c_i < childCnt; c_i++)
			GetMeshSpaceBlendTransform(_outTransforms,
				_pCurrentNode->GetChild(c_i),
				baseMeshSpaceRotation,
				blendMeshSpaceRotation,
				calcMeshSpaceRotation,
				_baseRatio,
				_blendRatio,
				_curBlendDepth + 1	// 深度を増やしていく
			);
	}

	void LayerdAnimationNode::GetLocalSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms,
		float _baseRatio,
		float _blendRatio) const
	{
		// ベースのボーングループにのみアニメーションを反映させる
		// （ベース側アニメーション）
		u_int baseBoneCnt = static_cast<u_int>(baseAnimBoneIds.size());
		for (u_int b_i = 0; b_i < baseBoneCnt; b_i++)
		{
			u_int uBoneId = static_cast<u_int>(baseAnimBoneIds[b_i]);
			pBaseAnimation->GetTransformByRatio(uBoneId, 
				_baseRatio,
				_outTransforms[uBoneId]);
		}

		// ブレンドのボーングループにのみアニメーションを反映させる
		// （ブレンド側アニメーション）
		u_int blendBoneCnt = static_cast<u_int>(blendAnimBoneIds.size());
		for (u_int b_i = 0; b_i < blendBoneCnt; b_i++)
		{
			u_int uBoneId = static_cast<u_int>(blendAnimBoneIds[b_i]);
			pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, _outTransforms[uBoneId]);
		}
	}

	u_int LayerdAnimationNode::GetAllKeyFrame() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 1;
#endif // EDIT

		return pBlendAnimation->GetAllAnimationFrame();
	}

	u_int LayerdAnimationNode::GetBlendAllKeyFrame() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 1;
#endif // EDIT

		return pBlendAnimation->GetAllAnimationFrame();
	}

	float LayerdAnimationNode::GetBlendCurveSpeed(float _playRatio) const
	{
		return blendSpeedCurve.GetValue(_playRatio);
	}

	float LayerdAnimationNode::GetBlendAnimationTime() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 1.0f;
#endif // EDIT

		return pBlendAnimation->GetAnimationTime();
	}

	float LayerdAnimationNode::GetBeginBlendPlayRatio() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 0.0f;
#endif // EDIT

		return static_cast<float>(beginBlendPlayFrame) / pBlendAnimation->GetAllAnimationFrame();
	}

	int LayerdAnimationNode::GetBeginBlendBoneId() const
	{
		return beginBlendBoneId;
	}

	DXSimp::Vector3 LayerdAnimationNode::GetRootMotionPos(float _ratio) const
	{
#ifdef EDIT
		if (!pBaseAnimation) return  DXSimp::Vector3::Zero;
#endif // EDIT

		return pBaseAnimation->GetRootMotionPos(_ratio);
	}

	const DXSimp::Vector3& LayerdAnimationNode::GetRootMotionPerSpeed() const
	{
#ifdef EDIT
		if (!pBaseAnimation) return  DXSimp::Vector3::Zero;
#endif // EDIT

		return pBaseAnimation->GetRootMotionPosSpeedPerSec();
	}

	bool LayerdAnimationNode::GetMotionSpaceRotationBlend() const
	{
		return isMeshSpaceRotationBlend;
	}

	json LayerdAnimationNode::Save()
	{
		auto data = AnimationNode_Base::Save();

		data["blendBeginFrame"] = beginBlendPlayFrame;
		data["blendCurve"] = blendSpeedCurve.Save();

		if (pBaseAnimation)
			data["baseAnim"] = pBaseAnimation->GetAssetName();
		if (pBlendAnimation)
			data["blendAnim"] = pBlendAnimation->GetAssetName();

		data["beginBoneId"] = beginBlendBoneId;
		data["beginBlendFlame"] = beginBlendPlayFrame;
		data["meshSpaceBlend"] = isMeshSpaceRotationBlend;
		data["blendDepth"] = blendDepth;

		return data;
	}

	void LayerdAnimationNode::Load(const json& _data)
	{
		AnimationNode_Base::Load(_data);

		LoadJsonUnsigned("beginBlendFlame", beginBlendPlayFrame, _data);
		LoadJsonBoolean("meshSpaceBlend", isMeshSpaceRotationBlend, _data);
		LoadJsonUnsigned("blendDepth", blendDepth, _data);
		json blendCurveData;
		if (LoadJsonData("blendCurve", blendCurveData, _data))
		{
			blendSpeedCurve.Load(blendCurveData);
		}

		std::string loadStr;
		if (LoadJsonString("baseAnim", loadStr, _data))
		{
			pBaseAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
			if (pBaseAnimation)
				SetAnimationTime(pBaseAnimation->GetAnimationTime());
		}
		if (LoadJsonString("blendAnim", loadStr, _data))
		{
			pBlendAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
			beginBlendPlayFrame = 0;
		}
		if (LoadJsonInteger("beginBoneId", beginBlendBoneId, _data))
		{
			SetBlendBeginBone(beginBlendBoneId);
		}
	}

	bool LayerdAnimationNode::CanWarking() const
	{
#ifdef EDIT
		// 基礎となるアニメーションがなかったら機能しない
		if (!pBaseAnimation)
		{
			assert(!"基礎アニメーションをセットしてね");
			return false;
		}

		if (!pBlendAnimation)
		{
			assert(!"ブレンドアニメーションをセットしてね");
			return false;
		}
#endif // EDIT

		return true;
	}

	void LayerdAnimationNode::SetBlendBeginBone(int _beginBoneId)
	{
		// ボーンの数より大きい　または　0以下なら処理しない
		if (_beginBoneId >= static_cast<int>(pBoneList->GetBoneCnt()) ||
			_beginBoneId < 0) return;

		beginBlendBoneId = _beginBoneId;

		// ベース側とブレンド側でボーンを分けていく
		baseAnimBoneIds.clear();
		blendAnimBoneIds.clear();
		SeparateBoneGroup(*pBoneList->GetRootNode());
	}

	void LayerdAnimationNode::SeparateBoneGroup(const TreeNode& _treeNode)
	{
		// ツリーノードに対応したボーンがあるか確認
		if (_treeNode.HasBone())
		{
			// ボーンのインデックスを取得
			u_int boneId = _treeNode.GetBoneIdx();

			if (boneId == beginBlendBoneId) // ブレンド開始のボーンだったら
			{
				// ブレンドグループを作成する
				CreateBlendBoneGroup(_treeNode);
				return; // 以降のボーンは処理しない
			}

			baseAnimBoneIds.push_back(boneId);	// ベース側に追加
		}

		u_int childCnt = _treeNode.GetChildNum();
		// 再帰で分けていく
		for (u_int c_i = 0; c_i < childCnt; c_i++)
			SeparateBoneGroup(*_treeNode.GetChild(c_i));
	}

	void LayerdAnimationNode::CreateBlendBoneGroup(const TreeNode& _blendBoneNode)
	{
		if (_blendBoneNode.HasBone())
			blendAnimBoneIds.push_back(_blendBoneNode.GetBoneIdx());	// ブレンドに追加

		// 再帰で分けていく
		u_int childCnt = _blendBoneNode.GetChildNum();
		for (u_int c_i = 0; c_i < childCnt; c_i++)
			CreateBlendBoneGroup(*_blendBoneNode.GetChild(c_i));
	}

	void LayerdAnimationNode::ImGuiDebug()
	{
#ifdef EDIT
		AnimationNode_Base::ImGuiDebug();

		ImGui::Checkbox("MeshSpaceBlend", &isMeshSpaceRotationBlend);

		// ブレンド開始するボーンのIdをセット
		if (ImGui::DragInt("BlendBeginBoneId", &beginBlendBoneId, 1, 0, (int)pBoneList->GetBoneCnt()))
		{
			SetBlendBeginBone(beginBlendBoneId);
		}

		// ブレンドする深さ
		ImGui::DragScalar("BlendDepth", ImGuiDataType_U32, &blendDepth);

		// ブレンド側の開始フレーム
		u_int maxFrame = 0;
		if (pBlendAnimation) maxFrame = pBlendAnimation->GetAllAnimationFrame();
		ImGui::DragScalar("BlendBeginFrame", ImGuiDataType_U32, &beginBlendPlayFrame, 1, 0, &maxFrame);

		// ブレンド側の速度カーブ
		blendSpeedCurve.ImGuiCall();

		// アニメーションをセットする
		static std::string str;
		AssetGetter::ImGuiGetCombobox<AnimationData>("Animation", str);
		if (ImGui::Button("Set Base"))
		{
			AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
			if (pGetData)
			{
				pBaseAnimation = pGetData;
				SetAnimationTime(pBaseAnimation->GetAnimationTime());
			}
		}
		ImGui::SameLine();

		// アニメーション名表示
		std::string animName = "Null";
		if (pBaseAnimation)
			animName = pBaseAnimation->GetAssetName();
		ImGui::Text(animName.c_str());

		if (ImGui::Button("Set Blend"))
		{
			AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
			if (pGetData) pBlendAnimation = pGetData;
		}
		ImGui::SameLine();
		// アニメーション名表示
		animName = "Null";
		if (pBlendAnimation)
			animName = pBlendAnimation->GetAssetName();
		ImGui::Text(animName.c_str());
#endif // EDIT
	}
}