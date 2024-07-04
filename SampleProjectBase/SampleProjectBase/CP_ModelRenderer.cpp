#include "pch.h"
#include "CP_ModelRenderer.h"

void CP_ModelRenderer::Draw()
{
	if (pModel == nullptr) return;
	pModel->Draw(GetTransform());
}
