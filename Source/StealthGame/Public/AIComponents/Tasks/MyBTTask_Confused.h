// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "MyBTTask_Confused.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UMyBTTask_Confused : public UBTTask_Wait
{
	GENERATED_BODY()
public:
	UMyBTTask_Confused(const FObjectInitializer& objectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void SetWaitTime(float WaitingTime);

	//Create another Function for Animation + Sound

protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
