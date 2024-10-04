#include "Clip.h"

Clip::Clip() 
{
	mName = "No name given";
	mStartTime = 0.0f;
	mEndTime = 0.0f;
	mLooping = true;
}

/*To implement the Sample function, make
sure the clip is validand that the time is in
the range of the clip.Then, loop through
all of the tracks.Get the joint ID of the
track, sample the track, and assign the
sampled value back to the Pose reference.
If a component of a transform isn't
animated, the reference components are
used to provide default values.The
function then returns the adjusted time */
float Clip::Sample(Pose& outPose, float time) 
{
	if (GetDuration() == 0.0f)
	{
		return 0.0f;
	}
	time = AdjustTimeToFitRange(time);
	unsigned int size = mTracks.size();
	for (unsigned int i = 0; i < size; ++i) 
	{
		unsigned int j = mTracks[i].GetId(); // Joint
		Transform local = outPose.GetLocalTransform(j);
		Transform animated = mTracks[i].Sample(local, time, mLooping);
		outPose.SetLocalTransform(j, animated);
	}
	return time;
}
/*The AdjustTimeToFitRange function
that should loop has the same logic as the
AdjustTimeToFitTrack function you
implemented for the templated Track
class :*/
float Clip::AdjustTimeToFitRange(float inTime) 
{
	if (mLooping) 
	{
		float duration = mEndTime - mStartTime;
		if (duration <= 0) 
		{
			0.0f;
		}
		inTime = fmodf(inTime - mStartTime, mEndTime - mStartTime);
		if (inTime < 0.0f) 
		{
			inTime += mEndTime - mStartTime;
		}
		inTime = inTime + mStartTime;
	}
	else 
	{
		if (inTime < mStartTime) 
		{
			inTime = mStartTime;
		}
		if (inTime > mEndTime)
		{
			inTime = mEndTime;
		}
	}
	return inTime;
}
/*The RecalculateDuration function
sets mStartTime and mEndTime to
default values of 0. Next, these functions
loop through every TransformTrack
object in the animation clip.If the track is
valid, the startand end times of the track
are retrieved.The smallest start timeand
the largest end time are stored.The start
time of a clip might not be 0; it's possible
to have a clip that starts at an arbitrary
point in time :*/
void Clip::RecalculateDuration() 
{
	mStartTime = 0.0f;
	mEndTime = 0.0f;
	bool startSet = false;
	bool endSet = false;
	unsigned int tracksSize = mTracks.size();
	for (unsigned int i = 0; i< tracksSize; ++i) 
	{
		if(mTracks[i].IsValid()) 
		{
			float startTime = mTracks[i].GetStartTime();
			float endTime = mTracks[i].GetEndTime();
			if (startTime < mStartTime || !startSet) 
			{
				mStartTime = startTime;
				startSet = true;
			}
			if (endTime > mEndTime || !endSet) 
			{
				mEndTime = endTime;
				endSet = true;
			}
		}
	}
}
/*[] operator is meant to retrieve the
TransformTrack object for a specific
joint in the clip.This function is mainly
used by whatever code loads the
animation clip from a file.The function
performs a linear search through all of the
tracks to see whether any of them targets
the specified joint.If a qualifying track is
found, a reference to it is returned.If no
qualifying track is found, a new one is
created and returned:*/
TransformTrack& Clip::operator[](unsigned int joint) 
{
	for (int i = 0, s = mTracks.size(); i < s; ++i) 
	{
		if (mTracks[i].GetId() == joint) 
		{
			return mTracks[i];
		}
	}
	mTracks.push_back(TransformTrack());
	mTracks[mTracks.size() - 1].SetId(joint);
	return mTracks[mTracks.size() - 1];
}
/*The remaining getter functions of the
Clip class are simple :*/

std::string & Clip::GetName() 
{
	return mName;
}
unsigned int Clip::GetIdAtIndex(unsigned int index) 
{
	return mTracks[index].GetId();
}

unsigned int Clip::Size() 
{
	return (unsigned int)mTracks.size();
}

float Clip::GetDuration() 
{
	return mEndTime - mStartTime;
}

float Clip::GetStartTime() 
{
	return mStartTime;
}

float Clip::GetEndTime() 
{
	return mEndTime;
}

bool Clip::GetLooping() 
{
	return mLooping;
}

/*Similarly, the remaining setter functions
of the Clip class are simple :*/

void Clip::SetName(const std::string & inNewName) 
{
	mName = inNewName;
}

void Clip::SetIdAtIndex(unsigned int index, unsigned int id) 
{
	return mTracks[index].SetId(id);
}

void Clip::SetLooping(bool inLooping) 
{
	mLooping = inLooping;
}

