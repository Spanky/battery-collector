// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

public:
	ABatteryPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void WasCollected_Implementation() override;

	float GetPower()
	{
		return BatteryPower;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float BatteryPower;
};
