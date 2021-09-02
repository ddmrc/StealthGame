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
	TimerSearchToConfused.BindUFunction(this, FName("SetAIState"), EAIStates::Confused);
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

	//AILookAroundMechanic();



	if (CurrentAIState == EAIStates::Patrol)
	{


		LastState = EAIStates::Patrol;
		


		if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Sight")
		{
			//LastStimulusLocation = AIStimulus->StimulusLocation;
			GetWorld()->GetTimerManager().PauseTimer(LookAroundTimer);
			SetAIState(EAIStates::Detected);
			UE_LOG(LogTemp, Warning, TEXT("Detected"));
			return;

		}

		else if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Hearing")
		{
			//LastStimulusLocation = AIStimulus->StimulusLocation;
			GetWorld()->GetTimerManager().PauseTimer(LookAroundTimer);

			if (SpawnTargetLocationHandler)
			{
				UE_LOG(LogTemp, Warning, TEXT("SpawningCubes"));
				float RadiusForPoints = 550.f;
				int32 NumberOfPointsToSpawn = 2;
				SpawnTargetLocationHandler->SpawnRandomSearchPoints(LastStimulusLocation, RadiusForPoints, NumberOfPointsToSpawn);
				bSearchPointsDeleted = false;
			}
			GetWorld()->GetTimerManager().PauseTimer(PatrolTimer);
			SetAIState(EAIStates::Searching);
			return;

		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(LookAroundTimer))
		{
			UE_LOG(LogTemp, Warning, TEXT("PATROLLING"));
			float RandFloatLookAround = FMath::FRandRange(3.f, 5.f);
			GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimer, TimerDetectedToLookAround, RandFloatLookAround, false);
		}

	}

	else if (CurrentAIState == EAIStates::Searching)
	{

		//IF Noise Strength is 1 then radius is more specifc if it's weaker radius is wider
		//Also keep into account
		if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolTimer))
		{


			GetWorld()->GetTimerManager().ClearTimer(PatrolTimer);
			GetWorld()->GetTimerManager().SetTimer(PatrolTimer, TimerSearchToConfused, 5.0f, false);
		}

		if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Sight")
		{
			//LastStimulusLocation = AIStimulus->StimulusLocation;
			SetAIState(EAIStates::Detected);
			UE_LOG(LogTemp, Warning, TEXT("SENSED"));
			
			
		}

		else if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Hearing")
		{

			//if (SpawnTargetLocationHandler)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("SpawningCubes"));
			//	float RadiusForPoints = 550.f;
			//	int32 NumberOfPointsToSpawn = 2;
			//	SpawnTargetLocationHandler->SpawnRandomSearchPoints(LastStimulusLocation, RadiusForPoints, NumberOfPointsToSpawn);
			//	bSearchPointsDeleted = false;
			//}
			
		

		}


	}

	else if (CurrentAIState == EAIStates::LookingAround)

	{


		if (!GetWorld()->GetTimerManager().IsTimerActive(LookAroundTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimer, TimerSearchingToPatrol, 2.f, false);
		}


		if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Sight")
		{

			SetAIState(EAIStates::Detected);

		}
		else if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Hearing")
		{

			if (SpawnTargetLocationHandler)
			{
				UE_LOG(LogTemp, Warning, TEXT("SpawningCubes"));
				float RadiusForPoints = 550.f;
				int32 NumberOfPointsToSpawn = 2;
				SpawnTargetLocationHandler->SpawnRandomSearchPoints(LastStimulusLocation, RadiusForPoints, NumberOfPointsToSpawn);
				bSearchPointsDeleted = false;
			}

			GetWorld()->GetTimerManager().PauseTimer(PatrolTimer);
			SetAIState(EAIStates::Searching);

		}



		
	}

	else if (CurrentAIState == EAIStates::Chasing)
	{
		if (!AIStimulus.IsActive())
		{


			if (SpawnTargetLocationHandler)
			{
				UE_LOG(LogTemp, Warning, TEXT("SpawningCubes"));
				float RadiusForPoints = 550.f;
				int32 NumberOfPointsToSpawn = 2;
				SpawnTargetLocationHandler->SpawnRandomSearchPoints(LastStimulusLocation, RadiusForPoints, NumberOfPointsToSpawn);
				bSearchPointsDeleted = false;
			}
			GetWorld()->GetTimerManager().PauseTimer(PatrolTimer);
			SetAIState(EAIStates::Searching);

		}



	}

	else if (CurrentAIState == EAIStates::Detected)
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(SearchTimer))
		{
			GetWorld()->GetTimerManager().PauseTimer(SearchTimer);
		}

		if (GetWorld()->GetTimerManager().IsTimerActive(LookAroundTimer))
		{
			GetWorld()->GetTimerManager().PauseTimer(LookAroundTimer);
		}
		

		if (!GetWorld()->GetTimerManager().IsTimerActive(DetectedTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(DetectedTimer);
			GetWorld()->GetTimerManager().SetTimer(DetectedTimer, TimerDetectedToChasing, 1.0f, false);
		}
	}

	else if (CurrentAIState == EAIStates::Confused)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(SearchTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(SearchTimer);
			GetWorld()->GetTimerManager().SetTimer(SearchTimer, TimerSearchingToPatrol, 2.0f, false, - 1.f);
			UE_LOG(LogTemp, Warning, TEXT("CONSFUSED"));
		}

		if (SpawnTargetLocationHandler && !bSearchPointsDeleted)
		{
			SpawnTargetLocationHandler->RemoveRandomSearchPoints();
			bSearchPointsDeleted = true;
			UE_LOG(LogTemp, Warning, TEXT("Deleting Cubes"));
		}
	}
	
	
}

void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

		AIStimulus = Stimulus;


	if (Stimulus.WasSuccessfullySensed())
	{
		LastStimulusLocation = Stimulus.StimulusLocation;
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
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Patrolling"));
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

