// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Player/MyPlayerCharacter.h"
#include "AIComponents/Navigation/SpawnAITargetLocationHandler.h"
#include "EnemyAIController.generated.h"


/**
 * 
 */
UENUM()
enum class EAIStates : uint8
{
	Idle,
	Detected,
	Chasing,
	Patrol,
	Confused,
	Searching,
	LookingAround,
};


UCLASS()
class STEALTHGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:

	bool bSearchPointsDeleted = true;
	bool bSearchPointNeedsUpdate= false;
	bool bWantToSetPatrolTimer = true;



	UPROPERTY(EditAnywhere)
	bool DebugLogText = false;


	AMyPlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	EAIStates CurrentAIState;

	FTimerHandle DetectedTimer;
	FTimerHandle SearchTimer;
	FTimerHandle PatrolTimer;
	FTimerHandle LookAroundTimer;

	FTimerDelegate TimerSearchingToPatrol;
	FTimerDelegate TimerDetectedToChasing;
	FTimerDelegate TimerDetectedToLookAround;
	FTimerDelegate TimerSearchToConfused;

	FVector LastStimulusLocation = FVector::ZeroVector;

	ASpawnAITargetLocationHandler* SpawnTargetLocationHandler = nullptr;

	FAIStimulus AIStimulus;
	EAIStates LastState;

	AActor* TargetMoveLocation = nullptr;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorPerceptionUpdatedDelegate, AActor*, Actor, FAIStimulus, Stimulus);
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void SetAIState(EAIStates NewState);

	UFUNCTION()
	void CustomPauseTimer(bool bWantToPause, FTimerHandle Timer);



	UFUNCTION()
	EAIStates RequestGetAIState();

protected:
	UFUNCTION()
	void StartSearch(FAIStimulus Stimulus, bool DebugLog);

};
