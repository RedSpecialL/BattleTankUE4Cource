// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* Left, UTankTrack* Right)
{
	if (ensure(Left != nullptr && Right != nullptr))
	{
		LeftTrack = Left;
		RightTrack = Right;
		return;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No track to set!!!"))
	}
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (ensure(LeftTrack && RightTrack))
	{
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(Throw);
	}
}

void UTankMovementComponent::IntendRotate(float Throw)
{
	if (ensure(LeftTrack && RightTrack))
	{
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(-Throw);
	}
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FVector TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	FVector AIForwardIntention = MoveVelocity.GetSafeNormal();

	IntendMoveForward(FVector::DotProduct(AIForwardIntention, TankForward));
	IntendRotate(FVector::CrossProduct(AIForwardIntention, TankForward).Z);
}
