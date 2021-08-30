// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "AIComponents/Tasks/MyBTTask_Searching.h"


void AEnemyAIController::BeginPlay()
{

	Super::BeginPlay();


	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}

	if (SpawnTargetLocationHandler == nullptr)
	{
		TArray <AActor*> Arr;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnAITargetLocationHandler::StaticClass(), Arr);

		if (Arr.IsValidIndex(0))
		{
			SpawnTargetLocationHandler = Cast< ASpawnAITargetLocationHandler>(Arr[0]);
		}
	}


	if (UAIPerceptionComponent* ThisPerceptionComponent = GetPerceptionComponent())
	{
		ThisPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);

	}

	//setting up timerhandle for this function
	TimerSearchingToPatrol.BindUFunction(this, FName("SetAIState"), EAIStates::Patrol);
	TimerConfusedToSearch.BindUFunction(this, FName("SetAIState"), EAIStates::Searching);
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



	if (Stimulus.WasSuccessfullySensed() && Stimulus.Type.Name == "Default__AISense_Sight")
	{
	
		LastStimulusLocation = Stimulus.StimulusLocation;

		if (CurrentAIState == EAIStates::Patrol || CurrentAIState == EAIStates::Confused || CurrentAIState == EAIStates::Searching)
		{
			SetAIState(EAIStates::Detected);
			GetWorld()->GetTimerManager().SetTimer(ConfusedTimer, TimerDetectedToChasing, 1.0f, false);
		}

	}
	else if (Stimulus.WasSuccessfullySensed() && Stimulus.Type.Name == "Default__AISense_Hearing")
	{
		LastStimulusLocation = Stimulus.StimulusLocation;

		if (CurrentAIState == EAIStates::Patrol || CurrentAIState == EAIStates::Confused)
		{
			SetAIState(EAIStates::Searching);
			GetWorld()->GetTimerManager().SetTimer(ConfusedTimer, TimerSearchingToPatrol, 5.0f, false);
		}
	}
	else if (CurrentAIState == EAIStates::Chasing && !Stimulus.IsActive() )
	{

		SetAIState(EAIStates::Confused);


		GetWorld()->GetTimerManager().SetTimer(ConfusedTimer, TimerConfusedToSearch, 2.0f, false);
	
		Stimulus.StimulusLocation; //THIS IS SEARCH POINT WHERE PLAYER WAS LOST
		// Stay within radius of this location when searching for player.

		//if you dont see player and timer is over then back to patrol
		//else if you find player go chase
		//MAKE SURE THE CHANGE FROM Search to Patrol is done within Tick since it wont work
		//SPAWN AI to Location within radious of stimulus.StimulusLocation so the AI searches


		//USE Stimulus.StimulusLocation ONLY when AI HEARS pleayer (on detection)


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
			if (SpawnTargetLocationHandler)
			{
				SpawnTargetLocationHandler->RemoveRandomSearchPoints();
			}
			break;
		case EAIStates::Chasing:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Player Being Chasing"));
			break;
		case EAIStates::Confused:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("AI Confused"));
			break;
		case EAIStates::Searching:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("AI Searching"));
			if (SpawnTargetLocationHandler)
			{
				SpawnTargetLocationHandler->SpawnRandomSearchPoints(LastStimulusLocation, 500.f, 2);
			}
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
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimer, TimerSearchingToPatrol, 2.f, false);
		}

		if (PerceptionComponent->HasAnyActiveStimulus(*PlayerCharacter))
		{
			SetAIState(EAIStates::Detected);
			GetWorld()->GetTimerManager().SetTimer(ConfusedTimer, TimerDetectedToChasing, 1.0f, false);
		}
	}
	else
	{

		GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
	}




}