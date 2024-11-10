#pragma once
#include "CP_Renderer.h"
#include "Polygon2D.h"

class VertexShader;
class PixelShader;
class Texture;

class CP_UIRenderer : public CP_Renderer
{
	/// @brief UI�`��Ɏg�p���钸�_�V�F�[�_�[
	static VertexShader* pVertexShader;

	/// @brief UI�`��Ɏg�p����s�N�Z���V�F�[�_�[
	static PixelShader* pPixelShader;

	/// @brief �V�F�[�_�[�����ɃZ�b�g����Ă��邩�H
	static bool isSetShader;

	/// @brief �e�N�X�`����\��|���S��
	std::unique_ptr<Polygon2D> pPolygon;

	/// @brief �\������e�N�X�`��
	const Texture* pTexture;

public:
	CP_UIRenderer();
	~CP_UIRenderer(){}

	void Init() override;
	void OnChangeScale();
	void OnChangePosition();
	void OnChangeRotation();

	/// @brief �e�N�X�`�����Z�b�g
	/// @param _setTexture �Z�b�g����e�N�X�`��
	void SetTexture(const Texture& _setTexture);

	/// @brief UV���W���Z�b�g
	/// @param _startUV �J�nUV
	/// @param _endUV �I�_UV
	void SetUV(const DirectX::SimpleMath::Vector2& _startUV, const DirectX::SimpleMath::Vector2& _endUV);

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief �`��
	void Draw() override;

	/// @brief �`�揀��
	void DrawSetup();

	/// @brief �T�C�Y�ȂǕύX���邽�߂Ƀ|���S���Đ���
	void ReCreatePolygon();

	void ImGuiDebug() override;
};

