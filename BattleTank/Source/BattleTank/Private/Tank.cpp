// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

#include "Projectile.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankMovementComponent.h"
#include "TankTurret.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	UE_LOG(LogTemp, Warning, TEXT("DBG: %s constructor"), *GetName())


}

void ATank::AimAt(const FVector& HitLocation) const
{
	if (TankAimingComponent)
	{
		TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
	}
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("DBG: %s begin play"), *GetName())
}

void ATank::Fire()
{
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTime;
	if (!Barrel || !isReloaded)
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
