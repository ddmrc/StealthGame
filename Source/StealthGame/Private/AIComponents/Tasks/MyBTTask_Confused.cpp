// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Tasks/MyBTTask_Confused.h"

UMyBTTask_Confused::UMyBTTask_Confused(const FObjectInitializer& objectInitializer)
{
	NodeName = "CustomChaseAction";
	bNotifyTick = true;

}

EBTNodeResult::Type UMyBTTask_Confused::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	SetWaitTime(1.f);

	return NodeResult;

}

void UMyBTTask_Confused::SetWaitTime(float WaitingTime)
{
	WaitTime = WaitingTime;
}

void UMyBTTask_Confused::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if (MyMemory->RemainingWaitTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}