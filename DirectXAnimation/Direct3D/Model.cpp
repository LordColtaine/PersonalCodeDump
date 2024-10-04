#include "Model.h"

namespace Anim
{
	Scene::Scene(const char* filepath)
	{
		Assimp::Importer imp;
		pScene = imp.ReadFile(filepath,
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_ConvertToLeftHanded |
			aiProcess_GenNormals
		);
	}
	const aiScene* Scene::GetScene() const noexcept
	{
		return pScene;
	}
	Pose GetPose()
	{
		return Pose();
	}
}