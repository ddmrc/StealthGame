// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Tasks/MyBTTask_Detected.h"

UMyBTTask_Detected::UMyBTTask_Detected(const FObjectInitializer& objectInitializer)
{
	NodeName = "CustomDetectedAction";
	bNotifyTick = true;

}

EBTNodeResult::Type UMyBTTask_Detected::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	SetWaitTime(1.f);

	return NodeResult;

}

void UMyBTTask_Detected::SetWaitTime(float WaitingTime)
{
	WaitTime = WaitingTime;
}

void UMyBTTask_Detected::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if (MyMemory->RemainingWaitTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}