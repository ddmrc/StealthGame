// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "AIComponents/Navigation/MoveToLocationActor.h"
#include "MyBTTask_Searching.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UMyBTTask_Searching : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:
	UMyBTTask_Searching(const FObjectInitializer& objectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//IF I WANT TO DO SOMETHING WHEN IT ABORTS
	//virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//IF I WANT TO DO SOMETHING WHILE IT TICKS (AFTER EXECUTE)
	//virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//CustomTask Setup Target
	void SetSearchLocation(FVector SearchLocation) 
	{
		SearchPointLocation = SearchPointLocation;
	}

protected:
	TArray <AActor*> MoveToLocationList;

	AActor* CurrentMoveToTarget;
	FVector SearchPointLocation = FVector::ZeroVector;
	bool bIsMoveToLocationReached = true;
	bool bMoveToScanned = false;


	void SelectTarget(UBehaviorTreeComponent& OwnerComp);

};

