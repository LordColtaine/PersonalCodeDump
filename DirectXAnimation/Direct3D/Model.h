#pragma once
#include "Pose.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace Anim
{
	class Scene
	{
	public:
		Scene(const char* path);
		const aiScene* GetScene() const noexcept;
		Pose GetPose();
	private:
		const aiScene* pScene;
	};

	
}
