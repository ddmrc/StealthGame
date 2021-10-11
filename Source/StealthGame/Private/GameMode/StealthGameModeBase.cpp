// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/StealthGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AStealthGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (FinishLine == nullptr)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFinishLine::StaticClass(), FoundActors);

		if (FoundActors.IsValidIndex(0))
		{
			FinishLine = Cast<AFinishLine>(FoundActors[0]);
		}
	}

	if (FinishLine)
	{
		FinishLine->PlayerEntersFinishLine.AddDynamic(this,&AStealthGameModeBase::PlayerWinsGame);
	}


	

	

	
}

void AStealthGameModeBase::SetNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
		case EGamePlayState::EPlaying:
			break;

		case EGamePlayState::EPlayerLost:
			UE_LOG(LogTemp, Warning, TEXT("You LOST!"));
			UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
			break;
		
		case EGamePlayState::EPlayerWon:
			UE_LOG(LogTemp, Warning, TEXT("You WON!"));
			UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
			break;

	}
}


void AStealthGameModeBase::PlayerWinsGame()
{
	SetNewState(EGamePlayState::EPlayerWon);
}