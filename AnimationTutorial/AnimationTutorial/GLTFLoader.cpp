#include "GLTFLoader.h"

cgltf_data* LoadGLTFFile(const char* path) 
{
	cgltf_options options;
	memset(&options, 0, sizeof(cgltf_options));
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, path, &data);
	if (result != cgltf_result_success) 
	{
		std::cout << "Could not load: " << path << "\n";
			return 0;
	}
	result = cgltf_load_buffers(&options, data, path);
	if (result != cgltf_result_success) 
	{
		cgltf_free(data);
		std::cout << "Could not load: " << path << "\n";
			return 0;
	}
	result = cgltf_validate(data);
	if (result != cgltf_result_success) 
	{
		cgltf_free(data);
		std::cout << "Invalid file: " << path << "\n";
			return 0;
	}
	return data;
}

void FreeGLTFFile(cgltf_data* data) 
{
	if (data == 0) 
	{
		std::cout << "WARNING: Can't free null data\n";
	}
	else 
	{
		cgltf_free(data);
	}
}


/*First, implement a helper function to get
	the local transform of cgltf_node.A
	node can store its transform either as a
	matrix or as a separate position, rotation,
	and scale components.If the node stores
	its transformation as a matrix, use the
	mat4ToTransform decomposition
	function; otherwise, create the
	components as needed :*/
	// Inside the GLTFHelpers namespace
namespace GLTFHelpers
{
	Transform GetLocalTransform(cgltf_node& n)
	{
		Transform result;
		if (n.has_matrix)
		{
			mat4 mat(&n.matrix[0]);
			result = mat4ToTransform(mat);
		}
		if (n.has_translation)
		{
			result.position = vec3(n.translation[0], n.translation[1], n.translation[2]);
		}
		if (n.has_rotation)
		{
			result.rotation = quat(n.rotation[0], n.rotation[1], n.rotation[2], n.rotation[3]);
		}
		if (n.has_scale)
		{
			result.scale = vec3(n.scale[0], n.scale[1], n.scale[2]);
		}
		return result;
	}
	/*Next, implement a helper function to get
	the index of cgltf_node from an array.
		The GLTFNodeIndex function can
		perform a simple linear lookup by looping
		through all the nodes in a.gltf fileand
		returning the index of the node that you
		are searching for.If the index is not found,
		return -1 to signal an invalid index :*/
		// Inside the GLTFHelper namespace
	int GetNodeIndex(cgltf_node* target, cgltf_node* allNodes, unsigned int numNodes)
	{
		if (target == 0)
		{
			return -1;
		}
		for (unsigned int i = 0; i < numNodes; ++i)
		{
			if (target == &allNodes[i])
			{
				return (int)i;
			}
		}
		return -1;
	}
	void GetScalarValues(std::vector<float>& out, unsigned int compCount, const cgltf_accessor& inAccessor)
	{
		out.resize(inAccessor.count * compCount);
		for (cgltf_size i = 0; i < inAccessor.count; ++i)
		{
			cgltf_accessor_read_float(&inAccessor, i, &out[i * compCount], compCount);
		}
	}
	/*Implement the TrackFromChannel
		helper function in GLTFLoader.cpp.
		Start the function implementation by
		setting the Track interpolation.To do
		this, make sure the Interpolation
		type of the track matches the
		cgltf_interpolation_type type of
		the sampler :*/
	// Inside the GLTFHelpers namespace
	template<typename T, int N>
	void TrackFromChannel(Track<T, N>&result, const cgltf_animation_channel &channel) 
	{
		cgltf_animation_sampler& sampler = *channel.sampler;
		Interpolation interpolation = Interpolation::Constant;
		if (sampler.interpolation == cgltf_interpolation_type_linear) 
		{
			interpolation = Interpolation::Linear;
		}
		else if (sampler.interpolation == cgltf_interpolation_type_cubic_spline) 
		{
			interpolation = Interpolation::Cubic;
		}
		bool isSamplerCubic = interpolation == Interpolation::Cubic;
		result.SetInterpolation(interpolation);
		/*The sampler input is an accessor to the
			animation timeline.The sampler output is
			an accessor to the animation values.Use
			GetScalarValues to convert these
			accessors into linear arrays of floatingpoint
			numbers.The number of frames the
			number of elements in the sampler input.
			The number of components per frame
			(vec3 or quat) is the number of value
			elements divided by the number of
			timeline elements.Resize the track to
			have enough room to store all the frames :*/
		std::vector<float> time; //times
		GetScalarValues(time, 1, *sampler.input);
		std::vector<float> val; //values
		GetScalarValues(val, N, *sampler.output);
		unsigned int numFrames = sampler.input->count;
		unsigned int compCount = val.size() / time.size();
		result.Resize(numFrames);
		/* To parse the time and value arrays into
			frame structures, loop through every
			frame in the sampler.For each frame, set
			the time, then read the input tangent, the
			value, then the output tangent.Inputand
			output tangents are only available if the
			sampler is cubic; if it is not, these should
			default to 0. A local offset variable
			needs to be used to deal with cubic tracks
			since the input and output tangents are as
			large as the number of components :*/
		for (unsigned int i = 0; i < numFrames; ++i) 
		{
			int baseIndex = i * compCount;
			Frame<N>& frame = result[i];
			int offset = 0;
			frame.mTime = time[i];
			for (int comp = 0; comp < N; ++comp) 
			{
				frame.mIn[comp] = isSamplerCubic ? val[baseIndex + offset++] : 0.0f;
			}
			for (int comp = 0; comp < N; ++comp) 
			{
				frame.mValue[comp] = val[baseIndex + offset++];
			}
			for (int comp = 0; comp < N; ++comp) 
			{
				frame.mOut[comp] = isSamplerCubic ? val[baseIndex + offset++] : 0.0f;
			}
		}
	} // End of TrackFromChannel function
		
}
/* With these helper functions, loading the
	rest pose takes very little work.Loop
	through all the nodes in the current glTF
	file.For each node, assign the local
	transform to the pose that will be
	returned.You can find the parent of a
	node using the GetNodeIndex helper
	function, which returns - 1 if a node has
	no parent :*/
Pose LoadRestPose(cgltf_data* data) 
{
	unsigned int boneCount = data->nodes_count;
	Pose result(boneCount);
	for (unsigned int i = 0; i < boneCount; ++i) 
	{
		cgltf_node* node = &(data->nodes[i]);
		Transform transform =GLTFHelpers::GetLocalTransform(data->nodes[i]);
		result.SetLocalTransform(i, transform);
		int parent = GLTFHelpers::GetNodeIndex( node -> parent, data->nodes, boneCount);
		result.SetParent(i, parent);
	}
	return result;
}

std::vector<std::string> LoadJointNames(cgltf_data* data) 
{
	unsigned int boneCount = (unsigned int)data->nodes_count;
	std::vector<std::string> result(boneCount, "Not Set");
	for (unsigned int i = 0; i < boneCount; ++i) 
	{
		cgltf_node* node = &(data->nodes[i]);
		if (node->name == 0) 
		{
			result[i] = "EMPTY NODE";
		}
		else 
		{ 
			result[i] = node->name;
		}
	}
	return result;
}

/*Implement the LoadAnimationClips
function in GLTFLoader.cpp; don't
forget to add the declaration of the
function to GLTFLoader.h.Loop
through all the clips in the provided
gltf_data.For every clip, set its name.
Loop through all of the channels in the
clipand find the index of the node that the
current channel affects :*/
std::vector<Clip> LoadAnimationClips(cgltf_data* data) 
{
	unsigned int numClips = data->animations_count;
	unsigned int numNodes = data->nodes_count;
	std::vector<Clip> result;
	result.resize(numClips);
	for (unsigned int i = 0; i < numClips; ++i) 
	{
		result[i].SetName(data -> animations[i].name);
		unsigned int numChannels = data -> animations[i].channels_count;
		for (unsigned int j = 0; j < numChannels; ++j) 
		{
			cgltf_animation_channel& channel =data -> animations[i].channels[j];
			cgltf_node* target = channel.target_node;
			int nodeId = GLTFHelpers::GetNodeIndex(target, data->nodes, numNodes);
			/*Each channel of a glTF file is an animation
				track.Some nodes might only animate
				their position, while others might animate
				the position, rotation, and scale.Check the
				type of channel that is parsedand call the
				TrackFromChannel helper function to
				convert it into an animation track.The[]
				operator of the Track class either
				retrieves the current track or creates a
				new one.This means the
				TransformTrack function for the node
				that you are parsing is always valid :*/
			if(channel.target_path ==cgltf_animation_path_type_translation) 
			{
				VectorTrack&track = result[i][nodeId].GetPositionTrack();
				GLTFHelpers::TrackFromChannel<vec3, 3>(track, channel);
			}
			else if(channel.target_path == cgltf_animation_path_type_scale) 
			{
				VectorTrack& track = result[i][nodeId].GetScaleTrack();
				GLTFHelpers::TrackFromChannel<vec3, 3>(track, channel);
			}
			else if (channel.target_path ==cgltf_animation_path_type_rotation) 
			{
				QuaternionTrack& track = result[i][nodeId].GetRotationTrack();
				GLTFHelpers::TrackFromChannel<quat, 4>(track, channel);
			}
		} // End num channels loop
			/*After all the tracks in a clip have been
			populated, call the
			ReclaculateDuration function of the
			clip.This ensures that the playback
			happens in the proper time range :*/
		result[i].RecalculateDuration();
	} // End num clips loop
	return result;
} // End of LoadAnimationClips function



