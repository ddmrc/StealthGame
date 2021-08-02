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
	TimerConfusedToPatrol.BindUFunction(this, FName("SetAIState"), EAIStates::Patrol);
	TimerDetectedToChasing.BindUFunction(this, FName("SetAIState"), EAIStates::Chasing);
	TimerDetectedToLookAround.BindUFunction(this, FName("SetAIState"), EAIStates::LookingAround);

	//SETTING UP AI BEHAVIOUR TREE & BLACKBOARD
	CurrentAIState = EAIStates::Patrol;
	GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName("CurrentState"), (uint8)CurrentAIState);
	GetBlackboardComponent()->SetValueAsObject("PlayerCharacter", PlayerCharacter); 

	SetGenericTeamId(2);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AILookAroundMechanic();

		
	
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

		GetWorld()->GetTimerManager().SetTimer(ConfusedTimer, TimerConfusedToPatrol, 2.0f, false);

	}
	else
	{
		if (CurrentAIState != EAIStates::LookingAround)
		{
			SetAIState(EAIStates::Patrol);
		}
			




		
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
		case EAIStates::LookingAround:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("AI Looking Around"));
			break;


		}
	}
		
}

EAIStates AEnemyAIController::RequestGetAIState()
{
	return CurrentAIState;
}

void AEnemyAIController::AILookAroundMechanic()
{
	if (CurrentAIState == EAIStates::Patrol || CurrentAIState == EAIStates::LookingAround)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(LookAroundTimer) && CurrentAIState == EAIStates::Patrol)
		{

			GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
			float RandFloatLookAround = FMath::FRandRange(3.f, 5.f);
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimer, TimerDetectedToLookAround, RandFloatLookAround, false);

		}
		else if (!GetWorld()->GetTimerManager().IsTimerActive(LookAroundTimer) && CurrentAIState == EAIStates::LookingAround)
		{
			GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimer, TimerConfusedToPatrol, 2.f, false);
		}
	}
	else
	{

		GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
	}

}