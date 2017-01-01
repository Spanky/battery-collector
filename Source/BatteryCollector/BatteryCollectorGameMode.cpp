// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollector.h"
#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

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

	SetCurrentPlayState(BatteryPlayState::Playing);

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
}