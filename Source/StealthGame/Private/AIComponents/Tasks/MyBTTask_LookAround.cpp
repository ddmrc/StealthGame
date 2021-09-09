// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Tasks/MyBTTask_LookAround.h"
#include "AIController.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"




UMyBTTask_LookAround::UMyBTTask_LookAround(const FObjectInitializer& objectInitializer)
{
	NodeName = "CustomLookAround";
	bNotifyTick = true;

}

EBTNodeResult::Type UMyBTTask_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;


	//SetWaitTime(2.f);
	FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));
	

	
	//put select animation here
	
	return NodeResult;

}

void UMyBTTask_LookAround::SetWaitTime(float WaitingTime)
{
	WaitTime = WaitingTime;
}

void UMyBTTask_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTWaitTaskMemory* MyMemory = (FBTWaitTaskMemory*)NodeMemory;
	MyMemory->RemainingWaitTime -= DeltaSeconds;

	if (MyMemory->RemainingWaitTime <= 0.0f)
	{

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);


	}
}




