// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyAIController.h"
#include "Managers/DialogManager.h"
#include "AIManager.generated.h"

UCLASS()
class STEALTHGAME_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIManager();

	AEnemyCharacter* PatrolGuard1 = nullptr;
	AEnemyCharacter* PatrolGuard2 = nullptr;
	AEnemyCharacter* PatrolGuard3 = nullptr;

	AEnemyAIController* ControllerPatrolGuard1 = nullptr;
	AEnemyAIController* ControllerPatrolGuard2 = nullptr;
	AEnemyAIController* ControllerPatrolGuard3 = nullptr;

	ADialogManager* DialogManager = nullptr;

	int32 LocalPlayerHeat;
	int32 NumberOfLinesForConversation = 3;
protected:

	bool bDebugNeedsReset = false;

	bool bConversationHasBeenSet = false;

	TSubclassOf<ADialogManager> MyItemBlueprint;

	TSubclassOf<AEnemyCharacter> MyItemBlueprint2;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SetUpAIPointers();

	void SetUpDialogManager();

	bool CheckIfAnyAIWantsConversation();

	void DialogMechanic();

	void DebugAIState();

	void SpawnDialogManager();

	void SpawnPatrolGuard(FVector Location,FRotator Rotation);

	void UpdatePlayerHeat();

	bool bHasPlayerHeatBeenIncreased = false;

	bool MakeAIFaceEachOther();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
