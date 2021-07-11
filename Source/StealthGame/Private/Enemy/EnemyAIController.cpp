// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

void AEnemyAIController::BeginPlay()
{

	Super::BeginPlay();


	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}


	if (UAIPerceptionComponent* ThisPerceptionComponent = GetPerceptionComponent())
	{
		ThisPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);

	}

	//setting up timerhandle for this function
	TimerConfusedToSearch.BindUFunction(this, FName("SetAIState"), EAIStates::Patrol);
	TimerDetectedToChasing.BindUFunction(this, FName("SetAIState"), EAIStates::Chasing);

	//SETTING UP AI BEHAVIOUR TREE & BLACKBOARD
	CurrentAIState = EAIStates::Patrol;
	GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName("CurrentState"), (uint8)CurrentAIState);
	GetBlackboardComponent()->SetValueAsObject("PlayerCharacter", PlayerCharacter);
}



void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{



	if (Stimulus.WasSuccessfullySensed() )
	{

		if (CurrentAIState == EAIStates::Patrol || CurrentAIState == EAIStates::Confused)
		{
			SetAIState(EAIStates::Detected);
			GetWorld()->GetTimerManager().SetTimer(ConfusedTimer, TimerDetectedToChasing, 1.0f, false);
		}

	}
	else if (!Stimulus.IsActive() && CurrentAIState == EAIStates::Chasing)
	{

		SetAIState(EAIStates::Confused);

		GetWorld()->GetTimerManager().SetTimer(ConfusedTimer, TimerConfusedToSearch, 2.0f, false);

	}
	else
	{

		SetAIState(EAIStates::Patrol);

	}



	
}

void AEnemyAIController::SetAIState(EAIStates NewState)
{
	CurrentAIState = NewState;
	GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName("CurrentState"), (uint8)CurrentAIState);

	//Debug Lines
	if (GEngine)
	{
		switch (NewState)
		{
		case EAIStates::Idle:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("AI Idle"));
			break;
		case EAIStates::Detected:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Player Detected"));
			break;
		case EAIStates::Patrol:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Hidden"));
			break;
		case EAIStates::Chasing:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Player Being Chasing"));
			break;
		case EAIStates::Confused:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("AI Confused"));
			break;
		case EAIStates::Searching:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("AI Searching"));
			break;


		}
	}
		
}

EAIStates AEnemyAIController::RequestGetAIState()
{
	return CurrentAIState;
}
