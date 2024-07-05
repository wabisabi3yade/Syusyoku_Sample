#include "pch.h"
#include "ShaderSphere.h"


#include "CP_MeshRenderer.h"
#include "SphereCollider.h"

ShaderSphere::ShaderSphere()
{
	AddComponent<SphereCollider>();
	AddComponent<CP_MeshRenderer>();
}
