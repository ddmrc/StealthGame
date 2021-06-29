// Fill out your copyright notice in the Description page of Project Settings.


#include "AICompontents/Tasks/MyBTTask_MoveToLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"



UMyBTTask_MoveToLocation::UMyBTTask_MoveToLocation(const FObjectInitializer& objectInitializer)
{
	NodeName = "CustomMoveToLocation";
	bNotifyTick = true;
	bStopOnOverlap = false;
}

EBTNodeResult::Type UMyBTTask_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (MoveToLocationList.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(this, AMoveToLocationActor::StaticClass(), MoveToLocationList);

	}

	

	if (MoveToLocationList.Num() > 0 && bIsMoveToLocationReached)
	{
		int RandomNumber = FMath::RandRange(0, MoveToLocationList.Num()-1);

		if (MoveToLocationList.IsValidIndex(RandomNumber))
		{

			CurrentMoveToTarget = MoveToLocationList[RandomNumber];
			OwnerComp.GetBlackboardComponent()->SetValueAsObject("MoveToLocation", MoveToLocationList[RandomNumber]);
			bIsMoveToLocationReached = false;
		}
	}

	if (CurrentMoveToTarget)
	{


		if (OwnerComp.GetAIOwner()->GetCharacter()->GetActorLocation().Equals(CurrentMoveToTarget->GetActorLocation(),50.f))
		{
			

			bIsMoveToLocationReached = true;
		}
	}

	//Set Target to
	
	return EBTNodeResult::Succeeded;
}

void UMyBTTask_MoveToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	

}