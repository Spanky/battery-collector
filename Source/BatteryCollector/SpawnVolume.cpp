// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pickup.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	SpawnDelayMin = 1.0f;
	SpawnDelayMax = 5.0f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	StartNextSpawnTimer();
}

void ASpawnVolume::StartNextSpawnTimer()
{
	ActualSpawnDelay = FMath::FRandRange(SpawnDelayMin, SpawnDelayMax);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, ActualSpawnDelay, false);
}

FVector ASpawnVolume::GetRandomSpawnPoint() const
{
	return UKismetMathLibrary::RandomPointInBoundingBox(WhereToSpawn->Bounds.Origin, WhereToSpawn->Bounds.BoxExtent);
}

void ASpawnVolume::SpawnPickup()
{
	if(WhatToSpawn != nullptr)
	{
		UWorld* const world = GetWorld();
		if(world)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = Instigator;

			FVector spawnLocation = GetRandomSpawnPoint();
			FRotator spawnRotation;
			spawnRotation.Yaw = FMath::FRand() * 360.0f;
			spawnRotation.Pitch = FMath::FRand() * 360.0f;
			spawnRotation.Roll = FMath::FRand() * 360.0f;

			APickup* spawnedPickup = world->SpawnActor<APickup>(WhatToSpawn, spawnLocation, spawnRotation, spawnParams);
			StartNextSpawnTimer();
		}
	}
}