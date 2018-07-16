// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* Left, UTankTrack* Right)
{
	if (Left == nullptr || Right == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No track to set!!!"))
		return;
	}

	LeftTrack = Left;
	RightTrack = Right;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	float Time = GetWorld()->GetTimeSeconds();
	UE_LOG(LogTemp, Warning, TEXT("%f: Intent move forward throw: %f"), Time, Throw)

	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

	// TODO Prevent double speed due to dual contorl use.
}
