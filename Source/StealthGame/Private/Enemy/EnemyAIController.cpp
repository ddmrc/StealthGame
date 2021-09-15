// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "AIComponents/Tasks/MyBTTask_Searching.h"
#include <DrawDebugHelpers.h>

void AEnemyAIController::SetUpPointerVariables()
{

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("Player Character Not Found In EnemyAIController.cpp (17)."));
		}
	}


	if (SpawnTargetLocationHandler == nullptr)
	{
		TArray <AActor*> Arr;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnAITargetLocationHandler::StaticClass(), Arr);

		if (Arr.IsValidIndex(0))
		{
			SpawnTargetLocationHandler = Cast< ASpawnAITargetLocationHandler>(Arr[0]);
		}

		if (!SpawnTargetLocationHandler)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnTargetLocationHandler Not Detected In EnemyAIController.cpp (32)."));
		}
	}

	if (SpeechManager == nullptr)
	{
		SpeechManager = Cast< AStealthGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SpeechManager;

		if (!SpeechManager)
		{
			UE_LOG(LogTemp, Error, TEXT("Speech Manager Reference Not Detected In EnemyAIController.h (49)"));
		}
	}

}

void AEnemyAIController::BindingFunctions()
{
	if (UAIPerceptionComponent* ThisPerceptionComponent = GetPerceptionComponent())
	{
		ThisPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);

	}

	//setting up timerhandle for this function
	TimerSearchingToPatrol.BindUFunction(this, FName("SetAIState"), EAIStates::Patrol);
	TimerSearchToConfused.BindUFunction(this, FName("SetAIState"), EAIStates::Confused);
	TimerDetectedToChasing.BindUFunction(this, FName("SetAIState"), EAIStates::Chasing);
	TimerDetectedToLookAround.BindUFunction(this, FName("SetAIState"), EAIStates::LookingAround);
	TimerDetectAllies.BindUFunction(this, FName("ToggleDetectAllies"), false);
}

void AEnemyAIController::SettingUpBlackBoard()
{
	//SETTING UP AI BEHAVIOUR TREE & BLACKBOARD
	CurrentAIState = EAIStates::Patrol;
	GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName("CurrentState"), (uint8)CurrentAIState);
	GetBlackboardComponent()->SetValueAsObject("PlayerCharacter", PlayerCharacter);
}

/*   */

void AEnemyAIController::BeginPlay()
{

	Super::BeginPlay();

	SetUpPointerVariables();
	BindingFunctions();
	SettingUpBlackBoard();

}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (CurrentAIState == EAIStates::Patrol)
	{

		if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Sight")
		{
			if (ActorTriggeredStimulus && ActorTriggeredStimulus == PlayerCharacter)
			{
				GetWorld()->GetTimerManager().PauseTimer(LookAroundTimer);
				SetAIState(EAIStates::Detected);
				return;
			}
			else
			{
			

			}


		}
		
		else if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Hearing")
		{
			if (ActorTriggeredStimulus && ActorTriggeredStimulus == PlayerCharacter)
			{
				GetWorld()->GetTimerManager().PauseTimer(LookAroundTimer);
				StartSearch(AIStimulus, DebugLogText);
				return;
			}
			else
			{
				if (ActorTriggeredStimulus && ActorTriggeredStimulus->GetActorLocation().Equals(GetCharacter()->GetActorLocation(), 1000.f))
				{
					//SetAIState(EAIStates::Conversation);
					
				}
			}
		}

		if (!GetWorld()->GetTimerManager().IsTimerActive(LookAroundTimer))
		{
			float RandFloatLookAround = FMath::FRandRange(3.f, 5.f);
			GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimer, TimerDetectedToLookAround, RandFloatLookAround, false);
		}

	}

	else if (CurrentAIState == EAIStates::Searching)
	{

		if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Sight")
		{
			if (ActorTriggeredStimulus && ActorTriggeredStimulus == PlayerCharacter)
			{
				SetAIState(EAIStates::Detected);
			}
			else
			{

			}
	
		}

		else if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Hearing" && bSearchPointNeedsUpdate && ActorTriggeredStimulus == PlayerCharacter)
		{
			if (ActorTriggeredStimulus && ActorTriggeredStimulus == PlayerCharacter)
			{
				bSearchPointNeedsUpdate = false;
				SpawnTargetLocationHandler->RemoveRandomSearchPoints();
				bSearchPointsDeleted = true;
				StartSearch(AIStimulus, DebugLogText);
			}
			else
			{

			}
		}

		if (TargetMoveLocation && GetCharacter()->GetActorLocation().Equals(TargetMoveLocation->GetActorLocation(), 50.f))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Arrived"));
			//CurrentAIState = EAIStates::LookingAround;
			//bAIHasArrivedSearchLocation = false;
		}


	}

	else if (CurrentAIState == EAIStates::LookingAround)

	{

		if (!GetWorld()->GetTimerManager().IsTimerActive(LookAroundTimer) && bWantToSetPatrolTimer)
		{
			GetWorld()->GetTimerManager().ClearTimer(LookAroundTimer);
			GetWorld()->GetTimerManager().SetTimer(LookAroundTimer, TimerSearchingToPatrol, 2.f, false);
		}

		if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Sight" && ActorTriggeredStimulus == PlayerCharacter)
		{
			if (ActorTriggeredStimulus && ActorTriggeredStimulus == PlayerCharacter)
			{
				SetAIState(EAIStates::Detected);
			}
			else
			{

			}
			
			
		}

		else if (AIStimulus.WasSuccessfullySensed() && AIStimulus.Type.Name == "Default__AISense_Hearing" && ActorTriggeredStimulus == PlayerCharacter)
		{
			if (ActorTriggeredStimulus && ActorTriggeredStimulus == PlayerCharacter)
			{
				SetAIState(EAIStates::Detected);
			}
			else
			{

			}
				StartSearch(AIStimulus, DebugLogText);
				return;		
		}
		
	}

	else if (CurrentAIState == EAIStates::Chasing)
	{

		if (GetWorld()->GetTimerManager().IsTimerActive(PatrolTimer))
		{
			GetWorld()->GetTimerManager().PauseTimer(PatrolTimer);
		}
	
		if (!AIStimulus.IsActive() && AIStimulus.Type.Name == "Default__AISense_Sight")
		{

			if (SpawnTargetLocationHandler)
			{
				SpawnTargetLocationHandler->RemoveRandomSearchPoints();
			}

			StartSearch(AIStimulus, DebugLogText);
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
		
		if (GetWorld()->GetTimerManager().IsTimerActive(DetectAlliesTimer))
		{
			GetWorld()->GetTimerManager().PauseTimer(DetectAlliesTimer);
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
		}

		if (!bIgnoreSenseFromAllies)
		{
			float TimeToDetectAllies = 7.f;
			GetWorld()->GetTimerManager().ClearTimer(DetectAlliesTimer);
			GetWorld()->GetTimerManager().SetTimer(DetectAlliesTimer, TimerDetectAllies, TimeToDetectAllies, false);

		}

		if (SpawnTargetLocationHandler && !bSearchPointsDeleted)
		{
			SpawnTargetLocationHandler->RemoveRandomSearchPoints();
			bSearchPointsDeleted = true;
		}
	}

	else if (CurrentAIState == EAIStates::Conversation)
	{
		GetWorld()->GetTimerManager().PauseTimer(LookAroundTimer);
		GetWorld()->GetTimerManager().PauseTimer(DetectAlliesTimer);
	}
	
}

void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	AIStimulus = Stimulus;
	LastStimulusLocation = Stimulus.StimulusLocation;
	if (Stimulus.WasSuccessfullySensed())
	{

		ActorTriggeredStimulus = Actor;

		if (bIgnoreSenseFromAllies)
		{
			TArray<AActor*> HostileActors;
			PerceptionComponent->GetPerceivedHostileActors(HostileActors);

			if (HostileActors.IsValidIndex(0))
			{
				if (HostileActors[0] == PlayerCharacter && ActorTriggeredStimulus != PlayerCharacter)
				{
					ActorTriggeredStimulus = PlayerCharacter;
				}


			}
			else
			{
				ActorTriggeredStimulus = nullptr;
			}


		}

		if (ActorTriggeredStimulus && ActorTriggeredStimulus != PlayerCharacter && !bIgnoreSenseFromAllies)
		{
			bIgnoreSenseFromAllies = true;
			float TimeToDetectAllies = 7.f;
			GetWorld()->GetTimerManager().ClearTimer(DetectAlliesTimer);
			GetWorld()->GetTimerManager().SetTimer(DetectAlliesTimer, TimerDetectAllies, TimeToDetectAllies, false);

		}


	}

	if (CurrentAIState == EAIStates::Searching && Stimulus.WasSuccessfullySensed())
	{
		bSearchPointNeedsUpdate = true;

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
		case EAIStates::Conversation:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Conversation"));
			break;


		}
	}
		
}

EAIStates AEnemyAIController::RequestGetAIState()
{
	return CurrentAIState;
}

void AEnemyAIController::StartSearch(FAIStimulus Stimulus, bool DebugLog)
{

	FVector AILocation = Stimulus.ReceiverLocation;
	FVector StimulusLocation = Stimulus.StimulusLocation;

	float Distance = FMath::Abs(FVector::Distance(AILocation,StimulusLocation));
	float StimulusStrenght = Stimulus.Strength;

	float RadiusForPoints = 250.f;
	int32 NumberOfPointsToSpawn = 2;

	float TimerToConfusedSeconds = 5.f;

	if (Distance >= 1000.f)
	{
		RadiusForPoints = 550.f;
		NumberOfPointsToSpawn = 4;
		TimerToConfusedSeconds = 10.f;
	}
	else if (Distance < 999.f && Distance > 400.f)
	{
		RadiusForPoints = 350.f;
		NumberOfPointsToSpawn = 3;
		TimerToConfusedSeconds = 7.f;
	}

	RadiusForPoints = RadiusForPoints + (StimulusStrenght*200);

	if (DebugLog)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawningCubes"));
		UE_LOG(LogTemp, Warning, TEXT("Distance from AI: %scm"), *FString::SanitizeFloat(Distance));
		UE_LOG(LogTemp, Warning, TEXT("Stimulus Strenght: %s"), *FString::SanitizeFloat(StimulusStrenght));
		UE_LOG(LogTemp, Warning, TEXT("RadiusForPoints: %s"), *FString::SanitizeFloat(RadiusForPoints));
		UE_LOG(LogTemp, Warning, TEXT("NumberOfPointsToSpawn: %s"), *FString::FromInt(NumberOfPointsToSpawn));
		UE_LOG(LogTemp, Warning, TEXT("SecondsToConfused: %s"), *FString::SanitizeFloat(TimerToConfusedSeconds));
		DrawDebugSphere(GetWorld(), StimulusLocation, RadiusForPoints, 24, FColor::Blue, false, 5.f);
	}


	if (SpawnTargetLocationHandler)
	{
		SpawnTargetLocationHandler->SpawnRandomSearchPoints(LastStimulusLocation, RadiusForPoints, NumberOfPointsToSpawn);
		bSearchPointsDeleted = false;
	}

	GetWorld()->GetTimerManager().PauseTimer(PatrolTimer);
	SetAIState(EAIStates::Searching);

	if (!GetWorld()->GetTimerManager().IsTimerActive(PatrolTimer))
	{

		GetWorld()->GetTimerManager().ClearTimer(PatrolTimer);
		GetWorld()->GetTimerManager().SetTimer(PatrolTimer, TimerSearchToConfused, TimerToConfusedSeconds, false);
	}

}

void AEnemyAIController::CustomPauseTimer(bool bWantToPause, FTimerHandle Timer)
{
	if (bWantToPause)
	{
		GetWorld()->GetTimerManager().PauseTimer(Timer);
	}
}

void AEnemyAIController::ToggleDetectAllies(bool bState)
{
	bIgnoreSenseFromAllies = bState;

}

