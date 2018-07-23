// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Drive the tracks
	DriveTrack();
	ApplySidewaysForce();
	
	CurrentThrottle = 0.0f;
}

void UTankTrack::SetThrottle(float Throttle)
{
	// TODO: clamp throttle value.
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1.0f, 1.0f);
}

void UTankTrack::DriveTrack()
{
	FVector ForceToApply = GetForwardVector() * CurrentThrottle * MaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceToApply, ForceLocation);
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
void UTankTrack::ApplySidewaysForce()
{
	if (!GetComponentVelocity().Equals(FVector(0.0f), 0.1f))
	{
		FVector RightVector = GetRightVector();
		RightVector.Z = 0;
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float SlippageSpeed = FVector::DotProduct(RightVector, GetComponentVelocity());
		FVector CorrectionAcceleration = -SlippageSpeed / DeltaTime * RightVector;
		UStaticMeshComponent* TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
		FVector CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // Two tracks.
		TankRoot->AddForce(CorrectionForce);
	}
}

