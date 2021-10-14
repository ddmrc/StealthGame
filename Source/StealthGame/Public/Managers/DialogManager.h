// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIComponents/Conversation/DialogComponent.h"
#include "Enemy/EnemyAIController.h"
#include "DialogManager.generated.h"

UCLASS()
class STEALTHGAME_API ADialogManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogManager();
	void SetUpAIPointerReference(AEnemyAIController* Controller, int32 PatrolGuardID);

	bool CheckAIControllerReferenceByIndex(int32 PatrolGuardID);

	void SetUpConversationForController(int32 PatrolGuardID, FString Topic);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AEnemyAIController* ControllerPatrolGuard1 = nullptr;
	AEnemyAIController* ControllerPatrolGuard2 = nullptr;

	UDialogComponent* DialogComponentPatrolGuard1 = nullptr;
	UDialogComponent* DialogComponentPatrolGuard2 = nullptr;

	TArray<USoundCue*> ConversationPatrolGuard1;
	TArray<USoundCue*> ConversationPatrolGuard2;

	void DebugDialog();

	int32 PatrolGuardIDToTalk = 1;

	void RunThroughConversation(int32 NumberOfLinesToSpeak);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
