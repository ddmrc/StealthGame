// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyCustomHUD.h"
#include "Kismet/GameplayStatics.h"

AMyCustomHUD::AMyCustomHUD()
{

}

void AMyCustomHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerInGameUI)
	{
		PlayerInGameUIWidget = CreateWidget<UPlayerInGameUIWidget>(GetWorld(), PlayerInGameUI);
		
		if (PlayerInGameUIWidget)
		{
			PlayerInGameUIWidget->AddToViewport();
		}
	}

	if (AICharacter == nullptr)
	{
		TArray<AActor*> ActorArray;
		UGameplayStatics::GetAllActorsOfClass(this, AEnemyCharacter::StaticClass(), ActorArray);

		if (ActorArray.IsValidIndex(0))
		{
			AICharacter = Cast<AEnemyCharacter>(ActorArray[0]);
		}
	}
}

void AMyCustomHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ReadPlayerStealthState();

}

void AMyCustomHUD::ReadPlayerStealthState()
{
	if (AICharacter && PlayerInGameUIWidget)
	{
		EAIStates PlayerDetectetStatus = AICharacter->GetAICurrentState();

		if (PlayerDetectetStatus == EAIStates::Patrol || PlayerDetectetStatus == EAIStates::Confused)
		{
			PlayerInGameUIWidget->UpdateEyeStateWidget(EEyeState::ClosedEye);
		}
		else if (PlayerDetectetStatus == EAIStates::Searching)
		{
			PlayerInGameUIWidget->UpdateEyeStateWidget(EEyeState::SearchEye);
		}
		else if (PlayerDetectetStatus == EAIStates::Detected)
		{
			PlayerInGameUIWidget->UpdateEyeStateWidget(EEyeState::OpenEye);
		}
	}
	
}