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



	if (AICharacter1 == nullptr || AICharacter2 == nullptr)
	{
		TArray<AActor*> ActorArray;
		UGameplayStatics::GetAllActorsOfClass(this, AEnemyCharacter::StaticClass(), ActorArray);

		NumberOfAIEnemies = ActorArray.Num();


		if (ActorArray.IsValidIndex(0))
		{
			AICharacter1 = Cast<AEnemyCharacter>(ActorArray[0]);
		}

		if (ActorArray.IsValidIndex(1))
		{
			AICharacter2 = Cast<AEnemyCharacter>(ActorArray[1]);
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

	if (NumberOfAIEnemies == 1)
	{
		if (AICharacter1 && PlayerInGameUIWidget)
		{
			EAIStates PlayerDetectetStatus = AICharacter1->GetAICurrentState();

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
	else if (NumberOfAIEnemies == 2)
	{
		if (AICharacter1 &&AICharacter2 && PlayerInGameUIWidget)
		{
			EAIStates PlayerDetectetStatus = EAIStates::Patrol; //Default

			if (AICharacter1->GetAICurrentState() == EAIStates::Chasing || AICharacter2->GetAICurrentState() == EAIStates::Chasing)
			{
				PlayerDetectetStatus = EAIStates::Chasing;
			}
			else if (AICharacter1->GetAICurrentState() == EAIStates::Detected || AICharacter2->GetAICurrentState() == EAIStates::Detected)
			{
				PlayerDetectetStatus = EAIStates::Detected;
			}
			else if (AICharacter1->GetAICurrentState() == EAIStates::Searching || AICharacter2->GetAICurrentState() == EAIStates::Searching)
			{
				PlayerDetectetStatus = EAIStates::Searching;
			}


			if (PlayerDetectetStatus == EAIStates::Patrol || PlayerDetectetStatus == EAIStates::Confused)
			{
				PlayerInGameUIWidget->UpdateEyeStateWidget(EEyeState::ClosedEye);
			}
			else if (PlayerDetectetStatus == EAIStates::Searching)
			{
				PlayerInGameUIWidget->UpdateEyeStateWidget(EEyeState::SearchEye);
			}
			else if (PlayerDetectetStatus == EAIStates::Detected || PlayerDetectetStatus == EAIStates::Chasing)
			{
				PlayerInGameUIWidget->UpdateEyeStateWidget(EEyeState::OpenEye);
			}
		}
	}
	
}