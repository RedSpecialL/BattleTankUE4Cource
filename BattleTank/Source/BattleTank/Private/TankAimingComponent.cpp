// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankAimingComponent::AimAt(const FVector& AimLocation) const
{
	if (ensure(Barrel != nullptr))
	{
		FVector LaunchVelocity;
		FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

		if (UGameplayStatics::SuggestProjectileVelocity(
			this,
			LaunchVelocity,
			StartLocation,
			AimLocation,
			LaunchSpeed,
			false,
			0.0f,
			0.0f,
			ESuggestProjVelocityTraceOption::DoNotTrace
		))
		{
			FVector AimDirection = LaunchVelocity.GetSafeNormal();
			MoveBarrel(AimDirection);
		}
	}
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel != nullptr))
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

void UTankAimingComponent::MoveBarrel(const FVector& AimDirection) const
{
	if (!ensure(Barrel) || !ensure(Turret))
	{
		return;
	}

	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator Diff = AimAsRotator - BarrelRotator;

	Barrel->Elevate(Diff.Pitch);
	Turret->Rotate(Diff.GetNormalized().Yaw);
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	if (ensure(BarrelToSet != nullptr) || ensure(TurretToSet != nullptr))
	{
		Barrel = BarrelToSet;
		Turret = TurretToSet;
	}
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

