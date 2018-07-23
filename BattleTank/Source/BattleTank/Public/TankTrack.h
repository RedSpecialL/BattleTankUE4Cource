// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UTankTrack();

public:
	UFUNCTION(BlueprintCallable, Category = Input)
	virtual void BeginPlay() override;
	void SetThrottle(float Throttle);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	void ApplySidewaysForce();
	void DriveTrack();

private:
	/*
	* Max force per track in Newtons.
	*/
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float MaxDrivingForce = 10000000;

	float CurrentThrottle = 0.0f;
};
