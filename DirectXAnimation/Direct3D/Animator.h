#pragma once
#include "Animation.h"

class Animator
{
	void SetAnimation(Animation anim)
	{
		animation = anim;
	}

private:
	
	Animation animation;
	float curTime = 0;
};