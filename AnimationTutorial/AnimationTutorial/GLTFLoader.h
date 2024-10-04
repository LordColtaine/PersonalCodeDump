#pragma once
#include<iostream>
#include<vector>
#include"Track.h"
#include "cgltf.h"
#include "Transform.h"
#include "Pose.h"
#include "Clip.h"
cgltf_data* LoadGLTFFile(const char* path);
void FreeGLTFFile(cgltf_data* handle);
Pose LoadRestPose(cgltf_data* data);
std::vector<std::string> LoadJointNames(cgltf_data* data);
std::vector<Clip> LoadAnimationClips(cgltf_data* data);
