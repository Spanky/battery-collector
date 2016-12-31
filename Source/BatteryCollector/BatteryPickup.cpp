// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryCollector.h"
#include "BatteryPickup.h"

ABatteryPickup::ABatteryPickup()
{
	BatteryPower = 150.0f;
}

// Called when the game starts or when spawned
void ABatteryPickup::BeginPlay()
{
	Super::BeginPlay();

	GetMeshComponent()->SetSimulatePhysics(true);
}

void ABatteryPickup::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();

	Destroy();
}