// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class APickup;
class UBoxComponent;

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UBoxComponent* GetWhereToSpawn() const
	{
		return WhereToSpawn;
	}

	FVector GetRandomSpawnPoint() const;

private:
	void StartNextSpawnTimer();
	void SpawnPickup();


protected:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<APickup> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayMin;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayMax;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* WhereToSpawn;

	float ActualSpawnDelay;
};