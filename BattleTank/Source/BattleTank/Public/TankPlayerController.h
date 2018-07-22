// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATankPlayerController();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UFUNCTION(BlueprintCallable, Category = Setup)
	class ATank* GetControlledTank() const;

private:
	void AimTowardsCrosshair();
	bool GetLookDirection(const FVector2D& ScreenLocation, FVector& LookDirection) const;
	bool GetLookVectorHitLocation(const FVector& LookDirection, FHitResult& HitResult) const;
	FTwoVectors GetReachLineEnds(const FVector& WorldDirection) const;
	bool GetSightRayHitLocation(FVector& HitLocation) const;

private:
	UPROPERTY(EditDefaultsOnly)
	FVector2D CrossHairPosition;
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange;
};
