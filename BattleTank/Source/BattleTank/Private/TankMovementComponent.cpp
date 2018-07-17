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
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

	// TODO Prevent double speed due to dual control use.
}

void UTankMovementComponent::IntendRotate(float Throw)
{
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FString Name = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s velosity: %s"), *Name, *MoveVelocity.GetSafeNormal().ToString());
}
