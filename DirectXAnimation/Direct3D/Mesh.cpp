#include "Mesh.h"
#include "imgui\imgui.h"
#include<sstream>
#include <unordered_map>
#include "Pose.h"
#include <queue>
#include "Animation.h"
#include "CCDSolver.h"
// Mesh
Mesh::Mesh( Graphics& gfx,std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs )
{
	if( !IsStaticInitialized() )
	{
		AddStaticBind( std::make_unique<Bind::Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}

	for( auto& pb : bindPtrs )
	{
		if( auto pi = dynamic_cast<Bind::IndexBuffer*>(pb.get()) )
		{
			AddIndexBuffer( std::unique_ptr<Bind::IndexBuffer>{ pi } );
			pb.release();
		}
		else
		{
			AddBind( std::move( pb ) );
		}
	}
	AddBind( std::make_unique<Bind::TransformCbuf>( gfx,*this ) );
	this->bindablePtrs = std::move(bindPtrs);
}
void Mesh::Draw( Graphics& gfx,Transform accumulatedTransform ) noexcept(!IS_DEBUG)
{
	transform = accumulatedTransform;
	auto vbuf = std::make_unique<Bind::VertexBuffer>(gfx, skinnedvbuffer);
	AddBind(std::move(vbuf));
	Drawable::Draw( gfx );
}
DirectX::XMMATRIX Mesh::GetTransformXM() const noexcept
{
	mat4 tf = transformToMat4(transform);
	DirectX::XMFLOAT4X4 mat = *reinterpret_cast<DirectX::XMFLOAT4X4*>(&tf);
	return DirectX::XMLoadFloat4x4( &mat );
}

unsigned int Mesh::GetNumVertices() const noexcept
{
	return vbuffer.size();
}
void Mesh::SetBuffer(std::vector<Vertex>& buf)
{
	vbuffer = buf;
}
//#define noskin
void Mesh::Skin(Skeleton& skeleton, Pose& pose)
{
	unsigned int numVertices = vbuffer.size();
	skinnedvbuffer.resize(numVertices);
	Pose& bindPose = skeleton.GetBindPose();
	if(pose.Size()>0)
	{
#pragma omp parallel for
		for (int i = 0; i < numVertices; i++)
		{
#ifndef noskin
			vec3 pos = { 0.0f, 0.0f, 0.0f }, norm = { 0.0f, 0.0f, 0.0f };
			auto& vertex = vbuffer[i];
			for (int j = 0; j < vertex.jointID.size(); j++)
			{
				const auto bpindex = vertex.jointID[j];
				const auto poseindex = vertex.jointID[j];
				
				//The other way is DEFINITELY wrong, don't bother
				auto skin = combine(pose.GetGlobalTransform(poseindex), skeleton.GetInvTransform(bpindex)); //correct
				
				pos = pos + transformPoint(skin, vertex.pos) * vertex.weight[j];
				norm = norm + transformVector(skin, vertex.normals) * vertex.weight[j];
			}
			skinnedvbuffer[i].pos = pos;
			skinnedvbuffer[i].normals = norm;
#else
			skinnedvbuffer[i].pos = vbuffer[i].pos;
			skinnedvbuffer[i].normals = vbuffer[i].normals;
#endif
		}
	}
	else
	{
#pragma omp parallel for
		for (int i = 0; i < numVertices; i++)
		{
			skinnedvbuffer[i].pos = vbuffer[i].pos;
			skinnedvbuffer[i].normals = vbuffer[i].normals;
		}
	}
}

Transform& Mesh::GetTransform()
{
	return transform;
}
void Mesh::SetTransform(Transform t)
{
	transform = t;
}
// Node
Node::Node(const std::string& name_, std::vector<Mesh*> meshPtrs,const Transform& iptransform ) noxnd
	:
	meshPtrs( std::move( meshPtrs ))
{
	name = name_;
	transform = iptransform;
	appliedTransform = Transform();
}

void Node::UpdateTransforms(Transform accumulatedTransform)
{
	const auto temp = combine(transform, appliedTransform);
	const auto built = combine(temp, accumulatedTransform);
	for (auto& mesh : meshPtrs)
	{
		mesh->SetTransform(built);
	}
	for (const auto& pc : childPtrs)
	{
		pc->UpdateTransforms(built);
	}
}
void Node::Draw( Graphics& gfx,Transform accumulatedTransform , Skeleton& skeleton, Pose& curPose) const noxnd
{
	auto name_ = name;
	const auto temp = combine(transform, accumulatedTransform);
	const auto built = combine(appliedTransform, temp);
	for( const auto pm : meshPtrs )
	{
		pm->Draw( gfx,built);
	}
	for( const auto& pc : childPtrs )
	{
		pc->Draw( gfx,built , skeleton, curPose);
	}
}
void Node::AddChild( std::shared_ptr<Node> pChild) noxnd
{
	assert( pChild );
	childPtrs.push_back( std::move( pChild ) );
}

void Node::ShowTree(int& nodeIndexTracked, std::optional<int>& selectedIndex, Node*& pSelectedNode) const noexcept
{
	const int currentNodeIndex = nodeIndexTracked;
	nodeIndexTracked++;
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((currentNodeIndex == selectedIndex.value_or(-1)) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((childPtrs.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
	// render this node
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)currentNodeIndex, node_flags, name.c_str()
	);
	// processing for selecting node
	if (ImGui::IsItemClicked())
	{
		selectedIndex = currentNodeIndex;
		pSelectedNode = const_cast<Node*>(this);
	}
	// recursive rendering of open node's children
	if (expanded)
	{
		for (const auto& pChild : childPtrs)
		{
			pChild->ShowTree(nodeIndexTracked, selectedIndex, pSelectedNode);
		}
		ImGui::TreePop();
	}
}

void Node::SetAppliedTransform(Transform transform) noexcept
{
	appliedTransform = transform;
}

unsigned int Node::GetIndex(std::string nodeName) const
{
	if (0 == nodeName.compare(name))
		return index;
	for (const auto child : childPtrs)
	{
		const auto retval = child->GetIndex(nodeName);
		if (retval != -1) return retval;
	}
	return -1;
}
Transform Node::GetLocalTransform() const
{
	return transform;
}

Transform Node::GetGlobalTransform()
{
	auto cur = this;
	Transform t;
	while (cur)
	{
		t = combine(cur->transform, t);
		cur = cur->parent;
	}
	return t;
}

// Model
class ModelWindow
{
public:
	void Show(const char* windowName, const Node& root)
	{
		windowName = windowName ? windowName : "Model";
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(nodeIndexTracker, selectedIndex, pSelectedNode);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				auto& transform = transforms[*selectedIndex];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);

				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);
				ImGui::SliderFloat("Scale", &transform.scale, 0.1f, 50.0f);
				if (ImGui::Button("Reset"))
				{
					Reset(selectedIndex);
				}
			}
		}
		ImGui::End();
	}
	void Reset(const std::optional<int>& index)
	{
		 transforms[*index] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	}

	Transform GetTransform() const noexcept
	{
		const auto& transform = transforms.at(*selectedIndex);
		Transform t(
			{transform.x, transform.y, transform.z },
			{yawPitchRoll(transform.yaw, transform.pitch, transform.roll)},
			{transform.scale,transform.scale, transform.scale}
		);
		return t;
	}
	Node* GetSelectedNode() const noexcept
	{
		return pSelectedNode;
	}
private:
	std::optional<int> selectedIndex;
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float scale = 1.0f;
	};
	std::unordered_map<int, TransformParameters> transforms;
};


Model::Model( Graphics& gfx,const std::string fileName )
	:
	pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile( fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals
	);
	if (pScene == nullptr || pScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE)
	{
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
	}
	
	StoreBoneData(*pScene);
	curPose.Resize(boneNames.size());
	transforms.resize(boneNames.size());
	for( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		meshPtrs.push_back( ParseMesh( gfx,*pScene->mMeshes[i] ) );
	}

	

	pRoot = ParseNode(*pScene->mRootNode);
	pRoot->parent = nullptr;
	if (curPose.Size() > 0)
	{
		curPose.SetParent(0, -1);
	}
	skeleton = Skeleton(curPose, boneNames);
	skeleton.SetTransforms(transforms);
	curPose = skeleton.GetBindPose();
	//int i = 0;
	
	for (size_t i = 0; i < pScene->mNumAnimations; i++)
	{
		auto p = pScene->mAnimations[i];
		float totTime = p->mDuration / p->mTicksPerSecond;
		std::vector<JointAnimation> jointAnims;
		for(int j = 0; j<p->mNumChannels;j++)
		{
			auto ch = p->mChannels[j];
			JointAnimation jointAnim;
			std::vector<KeyFrame> kf;
			int index = GetBoneIndex(ch->mNodeName.C_Str());
			
			if (index != -1 )
			{
				aiVector3t<ai_real> aiScale, aiPos;
				aiQuaterniont<ai_real> aiQuat;
				float factor = totTime / ch->mNumPositionKeys;
				for (int k = 0; k < ch->mNumPositionKeys; k++)
				{
					aiPos = ch->mPositionKeys[k].mValue;
					aiQuat = ch->mRotationKeys[k].mValue;
					aiScale = ch->mScalingKeys[k].mValue;

					vec3 scale = { (float)aiScale.x, (float)aiScale.y, (float)aiScale.z };
					quat rot = { aiQuat.x, aiQuat.y , aiQuat.z , aiQuat.w };
					vec3 pos = { (float)aiPos.x, (float)aiPos.y, (float)aiPos.z };
					Transform t(pos, rot, scale);

					float timestamp = (float)k * factor;
					kf.push_back(KeyFrame(timestamp, t));
				}
				jointAnims.push_back(JointAnimation(index, kf));
			}
			else
			{
				int x = 0;
			}
		}
		anim = Animation(totTime, jointAnims);
	}
	
	
	const auto boneIDArmUp = GetBoneIndex("Armature_Upper_Arm_L");
	if (-1 != boneIDArmUp)
	{
		ikchain.AddJoint(Joint(boneIDArmUp, skeleton.GetBindPose().GetLocalTransform(boneIDArmUp)));
	}
	const auto boneIDArmLow = GetBoneIndex("Armature_Lower_Arm_L");
	if(-1 != boneIDArmLow)
	{
		ikchain.AddJoint(Joint(boneIDArmLow, skeleton.GetBindPose().GetLocalTransform(boneIDArmLow)));
	}
	const auto boneIDHand = GetBoneIndex("Armature_Hand_L");
	if(-1 != boneIDHand)
	{
		ikchain.AddJoint(Joint(boneIDHand, skeleton.GetBindPose().GetLocalTransform(boneIDHand)));
	}

	const auto chainBone1 = GetBoneIndex("Armature_Bone");
	const auto chainBone2 = GetBoneIndex("Armature_Bone_001");
	const auto chainBone3 = GetBoneIndex("Armature_Bone_002");
	const auto chainBone4 = GetBoneIndex("Armature_Bone_012");
	const auto chainBone5 = GetBoneIndex("Armature_Bone_013");
	if (chainBone1 != -1 && chainBone2 != -1 && chainBone3 != -1 && chainBone4 != -1 && chainBone5 != -1)
	{
		ikchain.AddJoint(Joint(chainBone1, skeleton.GetBindPose().GetLocalTransform(chainBone1)));
		ikchain.AddJoint(Joint(chainBone2, skeleton.GetBindPose().GetLocalTransform(chainBone2)));
		ikchain.AddJoint(Joint(chainBone3, skeleton.GetBindPose().GetLocalTransform(chainBone3)));
		ikchain.AddJoint(Joint(chainBone4, skeleton.GetBindPose().GetLocalTransform(chainBone4)));
		ikchain.AddJoint(Joint(chainBone5, skeleton.GetBindPose().GetLocalTransform(chainBone5)));
	}
}

bool Model::IKChainPresent()
{
	return ikchain.Size() > 0;
}
void Model::Draw(Graphics& gfx, float timestamp, DirectX::XMFLOAT3 lightPos, bool animate, bool ik)
{
	curPose = skeleton.GetBindPose();
	if (auto node = pWindow->GetSelectedNode())
	{
		Transform t = pWindow->GetTransform();
		node->SetAppliedTransform(t);
		if(node->isBone)
		{
			t = combine(skeleton.GetBindPose().GetLocalTransform(node->index), t);
			curPose.SetLocalTransform(node->index, t);
		}
	}
	curTime += timestamp;
	
	if (animate)
	{
		curPose = anim.GetCurPose(curTime, skeleton);
	}
	
	//***************** IK Part **********
	if (ik)
	{
		vec3 pt = { lightPos.x, lightPos.z, -lightPos.y };

		auto globaltransform = skeleton.GetBindPose().GetGlobalTransform(ikchain.GetIndex(0));
		const auto& meshTransform = meshPtrs[0]->GetTransform();
		//const auto& meshTransform = transforms[ikchain.GetIndex(ikchain.Size()-1)];
		//globaltransform = combine(meshTransform, globaltransform);
		globaltransform = combine(globaltransform, meshTransform);
		vec3 offset = { globaltransform.position.x , globaltransform.position.y ,globaltransform.position.z };

		pt = pt - offset;
		Transform target = Transform(pt, quat(), vec3(1.f, 1.f, 1.f));
		CCDSolver solver;
		solver.mIKChain = ikchain.GetTransforms();
		solver.Solve(target);
		ikchain.SetTransforms(solver.mIKChain);
		for (unsigned int i = 0; i < ikchain.Size(); i++)
		{
			curPose.SetLocalTransform(ikchain.GetIndex(i), ikchain.GetTransform(i));
		}
	}
	//************** IK Part - End ******************

	for (auto& mesh : meshPtrs)
	{
		mesh->Skin(skeleton, curPose);
	}
	pRoot->Draw(gfx, Transform(), skeleton, curPose);
}

int Model::AddBoneName(unsigned int boneID, std::string name)
{
	if (!IsBone(name))
	{
		curPose.Resize(curPose.Size() + 1);
		boneNames.push_back(name);
		boneIDs.push_back(boneID);
	}
	return GetBoneIndex(name);
}

void Model::AddIfAbsent(std::vector<std::string> &stringList, std::string stringToAdd)
{
	for (const auto str : stringList)
	{
		if (stringToAdd.compare(str) == 0)
			return;
	}
	stringList.push_back(stringToAdd);
}

bool Model::Present(const std::vector<std::string>& stringList, std::string stringToCheck)
{
	for (const auto str : stringList)
	{
		if (stringToCheck.compare(str) == 0)
			return true;
	}
	return false;
}
void Model::Fill(const aiNode* node, const std::vector<std::string> names)
{
	if (node!= nullptr)
	{
		std::string name = node->mName.C_Str();
		
		bool nodeIsRoot = name.find("root") != std::string::npos || name.find("ROOT") != std::string::npos || name.find("Root") != std::string::npos;
		if (Present(names, name) || nodeIsRoot)
			AddIfAbsent(boneNames, node->mName.C_Str());
		for (int i = 0; i < node->mNumChildren; i++)
		{
			const aiNode* child = node->mChildren[i];
			Fill(child, names);
		}
	}
}
void Model::StoreBoneData(const aiScene& pScene)
{
	std::vector<std::string> tempNames;
	for (int i = 0; i < pScene.mNumMeshes; i++)
	{
		const auto &mesh = pScene.mMeshes[i];
		for (int j = 0; j < mesh->mNumBones; j++)
		{
			const auto& bone = mesh->mBones[j];
			AddIfAbsent(tempNames, bone->mName.C_Str());
		}
	}
	Fill(pScene.mRootNode, tempNames);
}
std::unique_ptr<Mesh> Model::ParseMesh( Graphics& gfx,const aiMesh& mesh )
{
	namespace dx = DirectX;
	using Dvtx::VertexLayout;
	
	std::vector<Vertex> vbuffer;
	for (unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		vbuffer.push_back({
			vec3(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z),
			vec3(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z)
			});
	}
	//curPose.Resize(curPose.Size()+mesh.mNumBones);
	for (int boneID = 0; boneID < mesh.mNumBones; boneID++)
	{
		const auto bone = mesh.mBones[boneID];
		aiVector3t<ai_real> aiScale, aiPos;
		aiQuaterniont<ai_real> aiQuat;
		aiMatrix4x4t<ai_real> mat = bone->mOffsetMatrix;

		mat.Decompose(aiScale, aiQuat, aiPos);
		vec3 scale = { (float)aiScale.x, (float)aiScale.y, (float)aiScale.z };
		quat rot = { aiQuat.x, aiQuat.y , aiQuat.z , aiQuat.w };
		vec3 pos = { (float)aiPos.x, (float)aiPos.y, (float)aiPos.z };
		Transform transform(pos, rot, scale);
		//curPose.SetLocalTransform(boneID, transform);
		
		int index = AddBoneName(boneID, bone->mName.C_Str());
		transforms[index] = transform;
		for (int i = 0; i < bone->mNumWeights; i++)
		{
			vbuffer[bone->mWeights[i].mVertexId].jointID.push_back(index);
			vbuffer[bone->mWeights[i].mVertexId].weight.push_back(bone->mWeights[i].mWeight);
		}
	}

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Joint", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Weight", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	

	std::vector<unsigned short> indices;
	indices.reserve( mesh.mNumFaces * 3 );
	for( unsigned int i = 0; i < mesh.mNumFaces; i++ )
	{
		const auto& face = mesh.mFaces[i];
		assert( face.mNumIndices == 3 );
		indices.push_back( face.mIndices[0] );
		indices.push_back( face.mIndices[1] );
		indices.push_back( face.mIndices[2] );
	}

	std::vector<std::unique_ptr<Bind::Bindable>> bindablePtrs;

	bindablePtrs.push_back( std::make_unique<Bind::IndexBuffer>( gfx,indices ) );

	auto pvs = std::make_unique<Bind::VertexShader>( gfx,L"SkinnedVS.cso" );
	auto pvsbc = pvs->GetBytecode();
	bindablePtrs.push_back( std::move( pvs ) );

	bindablePtrs.push_back( std::make_unique<Bind::PixelShader>( gfx,L"LightPS.cso" ) );

	bindablePtrs.push_back( std::make_unique<Bind::InputLayout>( gfx,ied,pvsbc ) );

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color = { 0.6f,0.6f,0.8f };
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3];
	} pmc;
	bindablePtrs.push_back( std::make_unique<Bind::PixelConstantBuffer<PSMaterialConstant>>( gfx,pmc,1u ) );

	
	auto mesh1 = std::make_unique<Mesh>( gfx,std::move( bindablePtrs ) );
	mesh1->SetBuffer(vbuffer);
	return mesh1;
}



std::shared_ptr<Node> Model::ParseNode( const aiNode& node , Node* pParent) noexcept
{
	namespace dx = DirectX;
	aiVector3t<ai_real> aiScale, aiPos;
	aiQuaterniont<ai_real> aiQuat;
	aiMatrix4x4t<ai_real> mat = node.mTransformation;

	const auto name = node.mName;
	mat.Decompose(aiScale, aiQuat, aiPos);
	vec3 scale = { (float)aiScale.x, (float)aiScale.y, (float)aiScale.z };
	quat rot = {(float)aiQuat.x, (float)aiQuat.y , (float)aiQuat.z , (float)aiQuat.w };
	vec3 pos = { (float)aiPos.x, (float)aiPos.y, (float)aiPos.z };
	Transform transform(pos, rot, scale);
	
	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve( node.mNumMeshes );
	for( size_t i = 0; i < node.mNumMeshes; i++ )
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back( meshPtrs.at( meshIdx ).get() );
	}
	
	
	auto pNode = std::make_shared<Node>(node.mName.C_Str(), std::move( curMeshPtrs ),transform );
	pNode->parent = pParent;
	bool nodeIsBone = IsBone(pNode->name);
	static bool ROOT_PRESENT = false;
	if (nodeIsBone)
	{
		int curIndex = GetBoneIndex(pNode->name);
		pNode->index = curIndex;
		pNode->SetAsBone();
		curPose.SetLocalTransform(curIndex, transform);
		if (0 == curIndex)
		{
			ROOT_PRESENT = true;
			pNode->SetAsRoot(curIndex);
			curPose.SetParent(curIndex, -1);
		}
		auto temp = pNode->parent;
		Transform t = Transform();
		while (temp && !temp->isBone)
		{
			t = combine(temp->transform, t);
			temp = temp->parent;
		}
		if (temp)
		{
			pNode->transform = combine(transform, t);
			curPose.SetLocalTransform(pNode->index, pNode->transform);
		}//*/
	}
	
	
	for( size_t i = 0; i < node.mNumChildren; i++ )
	{
		pNode->AddChild( ParseNode( *node.mChildren[i] , pNode.get()));
		if (IsBone(pNode->childPtrs[i]->name))
		{
			if (nodeIsBone)
			{
				curPose.SetParent(pNode->childPtrs[i]->index, pNode->index);
			}
			else if(!pNode->childPtrs[i]->isRoot)
			{
				if (ROOT_PRESENT)
				{
					Node* temp = pNode.get();
					while (temp && !IsBone(temp->name))
					{
						temp = temp->parent;
					}
					if (temp)
					{
						curPose.SetParent(pNode->childPtrs[i]->index, temp->index);
					}

				}
			}
		}
	}
	return pNode;
}

void Model::ShowWindow(const char* windowName) noexcept
{
	pWindow->Show(windowName, *pRoot);
}



Model::~Model()
{}

void Model::GetPoseFromRoot()
{

}
unsigned int Model::GetBoneIndex(std::string name)
{
	unsigned int i = 0;
	for (auto bonename : boneNames)
	{
		if (0 == bonename.compare(name))
			return i;
		i++;
	}
	return -1;
}
bool Model:: IsBone(std::string name)
{
	for (auto bonename : boneNames)
	{
		if (0 == bonename.compare(name))
			return true;
	}
	return false;
}




ModelException::ModelException(int line, const char* file, std::string note) noexcept
	:
	CustomException(line, file),
	note(std::move(note))
{}

const char* ModelException::what() const noexcept
{
	std::ostringstream oss;
	oss << CustomException::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ModelException::GetType() const noexcept
{
	return "Chili Model Exception";
}

const std::string& ModelException::GetNote() const noexcept
{
	return note;
}

