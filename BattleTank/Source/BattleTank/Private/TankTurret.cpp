// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, 1.0f);
	float RotationChange = RelativeSpeed * MaxDegreesPerSec * GetWorld()->DeltaTimeSeconds;
	float NewRotation = RelativeRotation.Yaw + RotationChange;
	//NewElevation = FMath::Clamp<float>(NewElevation, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(0.0f, NewRotation, 0.0f));
}
