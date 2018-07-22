// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class UTankAimingComponent;
class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

public:
	void AimAt(const FVector& HitLocaiton) const;

	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	UTankAimingComponent* TankAimingComponent;
public:
	//TODO : Remomve once firing is moved to aiming component
	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 10000; // TODO: Find sensible default.

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBP;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTime = 3.0f;

	double LastFireTime = 0.0f;

	UTankBarrel* Barrel = nullptr; // TODO Remove
};
