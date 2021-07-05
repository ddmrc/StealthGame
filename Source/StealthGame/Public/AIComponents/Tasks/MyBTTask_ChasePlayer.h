// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "MyBTTask_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UMyBTTask_ChasePlayer : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:

	UMyBTTask_ChasePlayer(const FObjectInitializer& objectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void LocatePlayer(UBehaviorTreeComponent& OwnerComp);

	ACharacter* PlayerCharacter = nullptr;
};
