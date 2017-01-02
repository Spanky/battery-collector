// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerPowerDecayRate = 0.01f;
}

void ABatteryCollectorGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(MyCharacter)
	{
		if(MyCharacter->GetCurrentPower() >= PowerRequiredToWin)
		{
			SetCurrentPlayState(BatteryPlayState::Won);
		}
		else if(MyCharacter->GetCurrentPower() > 0.0f)
		{
			MyCharacter->UpdatePower(-PlayerPowerDecayRate * DeltaSeconds * MyCharacter->GetInitialPower());
		}
		else
		{
			SetCurrentPlayState(BatteryPlayState::GameOver);
		}
	}
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
	for (AActor* currentActor : FoundActors)
	{
		SpawnVolumeActors.Add(CastChecked<ASpawnVolume>(currentActor));
	}

	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(MyCharacter)
	{
		PowerRequiredToWin = MyCharacter->GetInitialPower() * 1.25f;
	}

	if (HUDWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	SetCurrentPlayState(BatteryPlayState::Playing);
}

void ABatteryCollectorGameMode::SetCurrentPlayState(BatteryPlayState NewState)
{
	CurrentPlayState = NewState;
	HandleNewState(CurrentPlayState);
}

void ABatteryCollectorGameMode::HandleNewState(BatteryPlayState NewState)
{
	switch(NewState)
	{
	case BatteryPlayState::Playing:
		ActivateSpawnVolumes();
		break;
	case BatteryPlayState::Won:
		DeactivateSpawnVolumes();
		break;
	case BatteryPlayState::GameOver:
		DeactivateSpawnVolumes();
		DisablePlayerInput();
		RagdollCharacter();
		break;
	case BatteryPlayState::Unknown:
		UE_LOG(LogTemp, Error, TEXT("Play state is 'Unknown'"));
		break;
	}
}

void ABatteryCollectorGameMode::ActivateSpawnVolumes()
{
	for(ASpawnVolume* currentSpawnVolume : SpawnVolumeActors)
	{
		currentSpawnVolume->SetSpawningEnabled(true);
	}
}

void ABatteryCollectorGameMode::DeactivateSpawnVolumes()
{
	for (ASpawnVolume* currentSpawnVolume : SpawnVolumeActors)
	{
		currentSpawnVolume->SetSpawningEnabled(false);
	}
}

void ABatteryCollectorGameMode::RagdollCharacter()
{
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if(MyCharacter)
	{
		MyCharacter->GetMesh()->SetSimulatePhysics(true);
		MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
	}
}

void ABatteryCollectorGameMode::DisablePlayerInput()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if(PlayerController)
	{
		PlayerController->SetCinematicMode(true, false, false, true, true);
	}
}