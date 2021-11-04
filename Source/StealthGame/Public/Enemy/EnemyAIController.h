// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Player/MyPlayerCharacter.h"
#include "GenericTeamAgentInterface.h"
#include "AIComponents/Navigation/SpawnAITargetLocationHandler.h"
#include "AIComponents/Conversation/DialogComponent.h"
#include "GameMode/StealthGameModeBase.h"
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
	Conversation,
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActorPerceptionUpdatedDelegate, AActor*, Actor, FAIStimulus, Stimulus);
UCLASS()
class STEALTHGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:

	/*VARIABLES*/
	AActor* TargetMoveLocation = nullptr;

	FAIStimulus AIStimulus;

	FVector LastStimulusLocation = FVector::ZeroVector;

	UPROPERTY()
	EAIStates CurrentAIState;

	UPROPERTY()
	bool bIsTalking;

	/*FUNCTIONS*/

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetAIState(EAIStates NewState);

	UFUNCTION()
	EAIStates RequestGetAIState();


	UFUNCTION()
	UDialogComponent* GetDialogComponent();

	bool GetWantsToStartConversation() { return bWantsToStartConversation; }
	void SetWantsToStartConversation(bool bWantToStartConversation) {  bWantsToStartConversation = bWantToStartConversation; }

protected:

	/*VARIABLES*/
	AActor* ActorTriggeredStimulus = nullptr;
	AMyPlayerCharacter* PlayerCharacter = nullptr;
	ASpawnAITargetLocationHandler* SpawnTargetLocationHandler = nullptr;
	UDialogComponent* DialogComponent = nullptr;


	UPROPERTY(EditAnywhere)
	bool DebugLogText = false;
	bool bSearchPointsDeleted = true;
	bool bSearchPointNeedsUpdate = false;
	bool bWantToSetPatrolTimer = true;
	bool bIgnoreSenseFromAllies = false;
	bool bWantsToStartConversation = false;
	FTimerHandle DetectedTimer;
	FTimerHandle SearchTimer;
	FTimerHandle PatrolTimer;
	FTimerHandle LookAroundTimer;
	FTimerHandle DetectAlliesTimer;

	FTimerDelegate TimerSearchingToPatrol;
	FTimerDelegate TimerDetectedToChasing;
	FTimerDelegate TimerDetectedToLookAround;
	FTimerDelegate TimerSearchToConfused;
	FTimerDelegate TimerDetectAllies;



	/*FUNCTIONS*/

	virtual void BeginPlay() override;

	void BindingFunctions();

	UFUNCTION()
	void CustomPauseTimer(bool bWantToPause, FTimerHandle Timer);

	UFUNCTION()
	void StartSearch(FAIStimulus Stimulus, bool DebugLog);

	void SetUpPointerVariables();

	void SettingUpBlackBoard();

	UFUNCTION()
	void ToggleDetectAllies(bool bState);

	//DEBUG
	bool DebugSpeech = true;

	void CheckAIStateToSetBehaviour();

	void PatrolBehaviour();

	void SearchingBehaviour();

	void LookingAroundBehaviour();

	void ChasingBehaviour();

	void DetectedBehaviour();

	void ConfusedBehaviour();

	void ConversationBehaviour();
};
