// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Player/MyPlayerCharacter.h"
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
	Searching,
	Confused,
};


UCLASS()
class STEALTHGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:


	AMyPlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	EAIStates CurrentAIState;

	FTimerHandle ConfusedTimer;
	FTimerDelegate TimerConfusedToSearch;
	FTimerDelegate TimerDetectedToChasing;

	virtual void BeginPlay() override;
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorPerceptionUpdatedDelegate, AActor*, Actor, FAIStimulus, Stimulus);
	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void SetAIState(EAIStates NewState);


};
