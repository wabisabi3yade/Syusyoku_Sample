#pragma once
#include "Component.h"

namespace HashiTaku
{
	// �Q�[�����J�����N���X
	class CP_Camera : public Component
	{
		/// @brief �J�����̏�x�N�g��
		DXSimp::Vector3 camUp{ Vec3::Up };

		/// @brief �J�����̏�x�N�g��
		DXSimp::Vector3 target{ Vec3::Forward };

		/// @brief ����p
		float fov;

		/// @brief �`�拗��
		float distance;

		/// @brief ���s���e�ɂ��邩�H
		bool isOrthographic{ false };

		// �r���[�|�[�g�̔ԍ�
		u_int viewPortSlot;
	public:
		CP_Camera();
		virtual ~CP_Camera() {};

		virtual void Init() override;
		virtual void OnDestroy() override;

		/// @brief �r���[�ϊ��s��
		virtual void UpdateViewMatrix();

		/// @brief �������e�ɕύX
		virtual void SetPerspective();

		/// @brief ���s���e�ɕύX
		virtual void SetOrthographic();

		// fov�Z�b�g
		virtual void SetFov(float _setFov);

		// �r���[�|�[�g�ԍ����Z�b�g
		virtual void SetViewportSlot(u_int _slot);

		/// @brief �J�����̏�x�N�g��
		virtual const DXSimp::Vector3& GetCamUp() const;

		/// @brief �J�����̍��W
		virtual const DXSimp::Vector3& GetEyePos() const;

		/// @brief �J�����̒����_
		virtual const DXSimp::Vector3& GetTarget() const;

		/// @brief ����p���擾(deg)
		/// @return ����p
		virtual float GetFov() const;

		void ImGuiDebug() override;

		/// @brief �Z�[�u����
		/// @param _sceneData �Z�[�u�f�[�^
		json Save();

		/// @brief ���[�h����
		/// @param _sceneData ���[�h����f�[�^ 
		void Load(const json& _data);

	protected:
		/// @brief ���e�s����X�V����
		virtual void UpdateProjection();

		/// @brief �������e�s����X�V
		virtual void UpdatePerspective();

		/// @brief ���s���e���Z�b�g����
		virtual void UpdateOrthographic();
	};

	/// @brief CP_Camera��Null�I�u�W�F�N�g
	class CP_CameraNull : public CP_Camera
	{
	public:
		CP_CameraNull() {}
		~CP_CameraNull() {}

		void Init() override {}

		void LateUpdate() override {}

		void Draw() override {}

		void ImGuiDebug() override {}

		/// @brief �r���[�ϊ��s��
		void UpdateViewMatrix() {}

		/// @brief �������e�ɕύX
		void SetPerspective() {}

		/// @brief ���s���e�ɕύX
		void SetOrthographic() {}

		// fov�Z�b�g
		void SetFov(float _setFov) {}

		// �r���[�|�[�g�ԍ����Z�b�g
		void SetViewportSlot(u_int _slot) {}

		/// @brief �J�����̏�x�N�g��
		const DXSimp::Vector3& GetCamUp() const override { return Vec3::Up; }

		/// @brief �J�����̍��W
		const DXSimp::Vector3& GetEyePos() const override
		{
			return DXSimp::Vector3::Zero;
		}

		/// @brief �J�����̒����_
		const DXSimp::Vector3& GetTarget() const override
		{
			return Vec3::Forward;
		};

		float GetFov() const override { return 0.0f; }

		/// @brief �Z�[�u����
		/// @param _sceneData �Z�[�u�f�[�^
		json Save() { return json(); }

		/// @brief ���[�h����
		/// @param _sceneData ���[�h����f�[�^ 
		void Load(const json& _data) {}

	private:
		/// @brief ���e�s����X�V����
		virtual void UpdateProjection() {};

		/// @brief �������e�s����X�V
		virtual void UpdatePerspective() {};

		/// @brief ���s���e���Z�b�g����
		virtual void UpdateOrthographic() {};
	};
}