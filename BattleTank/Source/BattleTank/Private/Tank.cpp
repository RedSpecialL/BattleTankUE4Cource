// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "Projectile.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::AimAt(const FVector& HitLocation) const
{
	if (ensure(TankAimingComponent != nullptr))
	{
		TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
	}
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankAimingComponent = FindComponentByClass<UTankAimingComponent>();
}

void ATank::Fire()
{
	if (ensure(Barrel == nullptr))
	{
		return;
	}

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTime;
	if (!isReloaded)
	{
		return;
	}

	FVector SpawnLocation = Barrel->GetSocketLocation(FName("Projectile"));
	FRotator SpawnRotation = Barrel->GetSocketRotation(FName("Projectile"));
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(
		ProjectileBP, SpawnLocation, SpawnRotation);
	
	if (Projectile != nullptr)
	{
		Projectile->Launch(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}
