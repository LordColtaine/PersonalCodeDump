#include "Track.h"


template Track<float, 1>;
template Track<vec3, 3>;
template Track<quat, 4>;

namespace TrackHelpers 
{
	inline float Interpolate(float a, float b, float t) 
	{
		return a + (b - a) * t;
	}
	inline vec3 Interpolate(const vec3& a, const vec3& b, float t) 
	{
		return lerp(a, b, t);
	}
	inline quat Interpolate(const quat& a, const quat& b, float t) 
	{
		quat result = mix(a, b, t);
		if (dot(a, b) < 0) 
		{ // Neighborhood
			result = mix(a, -b, t);
		}
		return normalized(result); //NLerp, not slerp
	}

	inline float AdjustHermiteResult(float f) 
	{
		return f;
	}

	inline vec3 AdjustHermiteResult(const vec3& v) 
	{
		return v;
	}

	inline quat AdjustHermiteResult(const quat& q) 
	{
		return normalized(q);
	}

	inline void Neighborhood(const float& a, float& b) {}
	inline void Neighborhood(const vec3& a, vec3& b) {}
	inline void Neighborhood(const quat& a, quat& b) 
	{
		if (dot(a, b) < 0) 
		{
			b = -b;
		}
	}
}; // End Track Helpers namespace

template<typename T, int N>
Track<T, N>::Track() 
{
	mInterpolation = Interpolation::Linear;
}
template<typename T, int N>
float Track<T,N>::GetStartTime() 
{
	return mFrames[0].mTime;
}
template<typename T, int N>
float Track<T, N>::GetEndTime()
{
	return mFrames[mFrames.size() - 1].mTime;
}

template<typename T, int N>
T Track<T, N>::Sample(float time, bool looping) {
	if (mInterpolation == Interpolation::Constant) 
	{
		return SampleConstant(time, looping);
	}
	else if (mInterpolation == Interpolation::Linear) 
	{
		return SampleLinear(time, looping);
	}
	return SampleCubic(time, looping);
}

template<typename T, int N>
Frame<N>& Track<T, N>::operator[](unsigned int index) 
{
	return mFrames[index];
}

template<typename T, int N>
void Track<T, N>::Resize(unsigned int size) 
{
	mFrames.resize(size);
}

template<typename T, int N>
unsigned int Track<T, N>::Size() 
{
	return mFrames.size();
}

template<typename T, int N>
Interpolation Track<T, N>::GetInterpolation() 
{
	return mInterpolation;
}
template<typename T, int N>
void Track<T, N>::SetInterpolation(Interpolation interpolation) 
{
	mInterpolation = interpolation;
}

/*The Hermite function implements the
basic functions covered in the
Understanding cubic Hermite splines
section of this chapter. The second point
might need to be negated by the
Neighborhood helper function.
Quaternions also need to be normalized.
Both neighborhooding and normalization
are carried out by helper functions:*/
template<typename T, int N>
T Track<T, N>::Hermite(float t, const T& p1, const T& s1, const T& _p2, const T& s2) 
{
	float tt = t * t;
	float ttt = tt * t;
	T p2 = _p2;
	TrackHelpers::Neighborhood(p1, p2);
	float h1 = 2.0f * ttt - 3.0f * tt + 1.0f;
	float h2 = -2.0f * ttt + 3.0f * tt;
	float h3 = ttt - 2.0f * tt + t;
	float h4 = ttt - tt;
	T result = p1 * h1 + p2 * h2 + s1 * h3 + s2 * h4;
	return TrackHelpers::AdjustHermiteResult(result);
}

/*The FrameIndex function takes time as an argument; it should
return the frame immediately before that time(on the left).This
behavior changes depending on whether the track is intended to
be sampled looping or not.Follow these steps to implement the
FrameIndex function :*/

template<typename T, int N> 
int Track<T, N>::FrameIndex(float time, bool looping) 
{
	unsigned int size = (unsigned int)mFrames.size();
	/*If the track has one frame or less, it is
	invalid.If an invalid track is encountered,
	return -1 :*/
	if (size <= 1) 
	{
		return -1;
	}
	/*If the track is sampled as looping, the
		input time needs to be adjusted so that it
		falls between the startand end frames.
		This means you need to know the time at
		the first frame of the track, the time at the
		frame of the track, and the duration of the
		track :*/
	if (looping) 
	{
		float startTime = mFrames[0].mTime;
		float endTime = mFrames[size - 1].mTime;
		float duration = endTime - startTime;
		/* Since the track is looping, time needs to
			be adjusted so that it is within a valid
			range.To do this, make time relative to
			duration by subtracting the start time
			from itand modulo the result with
			duration.If time is negative, add the
			duration.Don't forget to add the start
			time back into time :*/
		time = fmodf(time - startTime, endTime - startTime);
		if (time < 0.0f) 
		{
			time += endTime - startTime;
		}
		time = time + startTime;
	}
	/*If the track doesn't loop, any time value
		that is less than the start frame should
		clamp to 0 and any time value that is
		greater than the second - to - last frame
		should clamp to the second - to - last frame's
		index:*/
	else 
	{
		if (time <= mFrames[0].mTime) 
		{
			return 0;
		}
		if (time >= mFrames[size - 2].mTime) 
		{
			return (int)size - 2;
		}
	}
	/* Now that the time is in a valid range, loop
		through every frame.The frame that is
		closest to the time(but still less) is the
		frame whose index should be returned.
		This frame can be found by looping
		through the frames of the track backward
		and returning the first index whose time is
		less than the time that is looked up :*/
	for (int i = (int)size - 1; i >= 0; --i) 
	{
		if (time >= mFrames[i].mTime) 
		{
			return i;
		}
	}
	// Invalid code, we should not reach here!
		return -1;
} // End of FrameIndex

/*The next function to implement is AdjustTimeToFitTrack.
When given a time, this function needs to adjust the time to be
in the range of the start / end frames of the track.This is, of
course, different depending on whether the track loops.Take
the following steps to implement the AdjustTimeToFitTrack
function :
1. If a track has less than one frame, the
track is invalid.If an invalid track is used,
return 0 :*/
	template<typename T, int N>
float Track<T, N>::AdjustTimeToFitTrack(float time, bool looping) 
{
	unsigned int size = (unsigned int)mFrames.size();
	if (size <= 1) 
	{
		return 0.0f;
	}
	/*Find the start time, end time, and
		duration of the track.The start time is the
		time of the first frame, the end time is the
		time of the last frame, and the duration is
		the difference between the two.If the
		track has a 0 duration, it is invalid—return 0:*/
	float startTime = mFrames[0].mTime;
	float endTime = mFrames[size - 1].mTime;
	float duration = endTime - startTime;
	if (duration <= 0.0f) 
	{
		return 0.0f;
	}
	/*If the track loops, adjust the time by the
		duration of the track :*/
	if (looping) 
	{
		time = fmodf(time - startTime, endTime - startTime);
		if (time < 0.0f) 
		{
			time += endTime - startTime;
		}
		time = time + startTime;
	}
	/*If the track does not loop, clamp the time
		to the first or last frame.Return the
		adjusted time :*/
	else 
	{
		if (time <= mFrames[0].mTime) 
		{
			time = startTime;
		}
		if (time >=	mFrames[size - 1].mTime) 
		{
			time = endTime;
		}
	}
	return time;
}

template<> float Track<float, 1>::Cast(float* value) 
{
	return value[0];
}
template<> vec3 Track<vec3, 3>::Cast(float* value) 
{
	return vec3(value[0], value[1], value[2]);
}
template<> quat Track<quat, 4>::Cast(float* value) 
{
	quat r = quat(value[0], value[1], value[2], value[3]);
	return normalized(r);
}
template<typename T, int N>
T Track<T, N>::SampleConstant(float t, bool loop) 
{
	int frame = FrameIndex(t, loop);
	if (frame < 0 || frame >= (int)mFrames.size()) 
	{
		return T();
	}
	return Cast(&mFrames[frame].mValue[0]);
}

template<typename T, int N>
T Track<T, N>::SampleLinear(float time, bool looping) 
{
	int thisFrame = FrameIndex(time, looping);
	if (thisFrame < 0 || thisFrame >= mFrames.size() - 1) 
	{
		return T();
	}
	int nextFrame = thisFrame + 1;
	float trackTime = AdjustTimeToFitTrack(time, looping);
	float thisTime = mFrames[thisFrame].mTime;
	float frameDelta = mFrames[nextFrame].mTime - thisTime;
	if (frameDelta <= 0.0f) 
	{
		return T();
	}
	float t = (trackTime - thisTime) / frameDelta;
	T start = Cast(&mFrames[thisFrame].mValue[0]);
	T end = Cast(&mFrames[nextFrame].mValue[0]);
	return TrackHelpers::Interpolate(start, end, t);
}

template<typename T, int N>
T Track<T, N>::SampleCubic(float time, bool looping) 
{
	int thisFrame = FrameIndex(time, looping);
	if (thisFrame < 0 || thisFrame >= mFrames.size() - 1) 
	{
		return T();
	}
	int nextFrame = thisFrame + 1;
	float trackTime = AdjustTimeToFitTrack(time, looping);
	float thisTime = mFrames[thisFrame].mTime;
	float frameDelta = mFrames[nextFrame].mTime - thisTime;
	if (frameDelta <= 0.0f) 
	{
		return T();
	}
	float t = (trackTime - thisTime) / frameDelta;
	size_t fltSize = sizeof(float);
	T point1 = Cast(&mFrames[thisFrame].mValue[0]);
	T slope1;// = mFrames[thisFrame].mOut * frameDelta;
	memcpy(&slope1, mFrames[thisFrame].mOut, N * fltSize);
	slope1 = slope1 * frameDelta;
	T point2 = Cast(&mFrames[nextFrame].mValue[0]);
	T slope2;// = mFrames[nextFrame].mIn[0] * frameDelta;
	memcpy(&slope2, mFrames[nextFrame].mIn, N * fltSize);
	slope2 = slope2 * frameDelta;
	return Hermite(t, point1, slope1, point2, slope2);
}
