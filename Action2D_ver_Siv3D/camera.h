#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_
struct CAMERA
{
	Float2 Position;
	Float2 Offset;
	float ShakeOffset;
	int TimeCount;
};

void InitCamera(void);
void UpdateCamera(void);

CAMERA *GetCamera(void);
void ShakeCamera(float Offset);

#endif
