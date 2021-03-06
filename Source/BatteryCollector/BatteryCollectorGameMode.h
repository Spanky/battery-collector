// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

class ASpawnVolume;
class UUserWidget;

UENUM(BlueprintType)
enum class BatteryPlayState
{
	Playing,
	GameOver,
	Won,
	Unknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;


public:
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const { return PowerRequiredToWin; }

	UFUNCTION(BlueprintPure, Category = "Power")
	BatteryPlayState GetCurrentPlayState() const { return CurrentPlayState; }

	UFUNCTION(BlueprintCallable, Category = "Power", meta = (BlueprintProtected = "true"))
	void SetCurrentPlayState(BatteryPlayState NewState);

private:
	void HandleNewState(BatteryPlayState NewState);

	void ActivateSpawnVolumes();
	void DeactivateSpawnVolumes();

	void RagdollCharacter();
	void DisablePlayerInput();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float PlayerPowerDecayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float PowerRequiredToWin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UUserWidget* CurrentWidget;

private:
	BatteryPlayState CurrentPlayState;

	TArray<ASpawnVolume*> SpawnVolumeActors;
};