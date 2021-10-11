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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AEnemyAIController* EnemyController = nullptr;
	void DebugDialog();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
