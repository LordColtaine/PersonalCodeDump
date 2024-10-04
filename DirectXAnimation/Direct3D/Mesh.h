#pragma once
#include "DrawableBase.h"
#include "BindableBase.h"
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <optional>
#include "Skeleton.h"
#include "ConditionalNoexcept.h"
#include "Pose.h"
#include "Animation.h"
#include "JointData.h"


class ModelException : public CustomException
{
public:
	ModelException(int line, const char* file, std::string note) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	const std::string& GetNote() const noexcept;
private:
	std::string note;
};
struct Vertex
{
	vec3 pos;
	vec3 normals;
	std::vector<int> jointID;
	std::vector<float> weight;
};

class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh( Graphics& gfx,std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs);
	void Draw( Graphics& gfx,Transform accumulatedTransform ) noxnd;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	unsigned int GetNumVertices() const noexcept;
	void SetBuffer(std::vector<Vertex>& buf);
	void Skin(Skeleton& skeleton, Pose& poses);
	Transform& GetTransform();
	void SetTransform(Transform t);
private:
	mutable Transform transform;
	std::vector<Vertex> vbuffer;
	std::vector<Vertex> skinnedvbuffer;
	std::vector<std::unique_ptr<Bind::Bindable>> bindablePtrs;
};

class Node
{
	friend class Model;
	friend class ModelWindow;
public:
	Node( const std::string& name,std::vector<Mesh*> meshPtrs,const Transform& transform ) noxnd;
	void Draw( Graphics& gfx,Transform accumulatedTransform, Skeleton& skeleton, Pose& pose ) const noxnd;
	void SetAppliedTransform(Transform transform)noexcept;
	Transform GetLocalTransform() const;
	Transform GetGlobalTransform();
	void UpdateTransforms(Transform accumulated);
	unsigned int GetIndex(std::string nodeName) const;
	void SetAsRoot(const int id)
	{
		isRoot = true;
		boneID = id;
	}
	bool NodeIsRoot() const
	{
		return isRoot;
	}
	bool NodeIsBone() const
	{
		return isBone;
	}
	void SetAsBone()
	{
		isBone = true;
	}
	int GetBoneID() const
	{
		return boneID;
	}
private:
	void ShowTree(int& nodeIndex, std::optional<int>& selectedIndex, Node*&p) const noexcept;
	void AddChild( std::shared_ptr<Node> pChild) noxnd;
private:
	std::string name;
	Node* parent;
	std::vector<std::shared_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	Transform transform;
	Transform appliedTransform;
	unsigned int index = -599;
	unsigned int numNodes;
	bool isRoot = false;
	bool isBone = false;
	int boneID = -1;
};

class Model
{
public:
	Model( Graphics& gfx,const std::string fileName );
	void Draw( Graphics& gfx, float timestamp, DirectX::XMFLOAT3 lightPos, bool Animate = false, bool Ik = false);
	void ShowWindow(const char* windowName) noexcept;
	~Model() noexcept;
	unsigned int GetBoneIndex(std::string name);
	bool IKChainPresent();
	Pose GetPose()
	{
		return curPose;
	}
private:
	void StoreBoneData(const aiScene& pScene);
	std::unique_ptr<Mesh> ParseMesh( Graphics& gfx,const aiMesh& mesh );
	std::shared_ptr<Node> ParseNode( const aiNode& node , Node* pParent = nullptr) noexcept;
	void Reset();
	void GetPoseFromRoot();
	bool IsBone(std::string name);
	int AddBoneName(unsigned int boneID, std::string name);
	//Node* FindNode(std::string bone, Node* node);
	void Fill(const aiNode* node, const std::vector<std::string> names);
	bool Present(const std::vector<std::string>& stringList, std::string stringToCheck);
	void AddIfAbsent(std::vector<std::string>& stringList, std::string stringToCheck);

private:
	std::shared_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
	std::unique_ptr<class ModelWindow> pWindow;
	std::vector<std::string> boneNames;
	std::vector<unsigned int> boneIDs;
	std::vector<Transform> transforms;
	Skeleton skeleton;
	Pose curPose;
	Pose appliedPose;
	Animation anim;
	Joints ikchain;
	float curTime = 0.0f;
};