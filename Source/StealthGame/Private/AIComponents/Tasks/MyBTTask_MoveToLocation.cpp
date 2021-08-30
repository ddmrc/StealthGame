// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Tasks/MyBTTask_MoveToLocation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/Character.h"



UMyBTTask_MoveToLocation::UMyBTTask_MoveToLocation(const FObjectInitializer& objectInitializer)
{
	NodeName = "CustomMoveToLocation";
	bNotifyTick = true;
	bStopOnOverlap = false;
}

EBTNodeResult::Type UMyBTTask_MoveToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	SelectTarget(OwnerComp);

	//COPY PASTE FROM ORIGINAL SOURCE CODE
	FBTMoveToTaskMemory* MyMemory = CastInstanceNodeMemory<FBTMoveToTaskMemory>(NodeMemory);
	MyMemory->PreviousGoalLocation = FAISystem::InvalidLocation;
	MyMemory->MoveRequestID = FAIRequestID::InvalidRequest;

	AAIController* MyController = OwnerComp.GetAIOwner();
	MyMemory->bWaitingForPath = bUseGameplayTasks ? false : MyController->ShouldPostponePathUpdates();
	if (!MyMemory->bWaitingForPath)
	{
		NodeResult = PerformMoveTask(OwnerComp, NodeMemory);
	}


	if (NodeResult == EBTNodeResult::InProgress && bObserveBlackboardValue)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (ensure(BlackboardComp))
		{
			if (MyMemory->BBObserverDelegateHandle.IsValid())
			{
				BlackboardComp->UnregisterObserver(BlackboardKey.GetSelectedKeyID(), MyMemory->BBObserverDelegateHandle);
			}
			MyMemory->BBObserverDelegateHandle = BlackboardComp->RegisterObserver(BlackboardKey.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_MoveTo::OnBlackboardValueChange));
		}
	}

	return NodeResult;

}


void UMyBTTask_MoveToLocation::SelectTarget(UBehaviorTreeComponent& OwnerComp)
{
	if (!bMoveToScanned)
	{
		MoveToLocationList.Empty();
	}
	if (MoveToLocationList.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(this, AMoveToLocationActor::StaticClass(), FName("Activated"), MoveToLocationList);
		bMoveToScanned = true;
	}



	if (MoveToLocationList.Num() > 0 && bIsMoveToLocationReached)
	{
		int RandomNumber = FMath::RandRange(0, MoveToLocationList.Num() - 1);

		if (MoveToLocationList.IsValidIndex(RandomNumber))
		{
			CurrentMoveToTarget = MoveToLocationList[RandomNumber];
			OwnerComp.GetBlackboardComponent()->SetValueAsObject("MoveToLocation", MoveToLocationList[RandomNumber]);
			bIsMoveToLocationReached = false;
		}
	}

	if (CurrentMoveToTarget)
	{


		if (OwnerComp.GetAIOwner()->GetCharacter()->GetActorLocation().Equals(CurrentMoveToTarget->GetActorLocation(), 50.f))
		{


			bIsMoveToLocationReached = true;
		}
	}
}

//EBTNodeResult::Type UMyBTTask_MoveToLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
//{
//	Super::AbortTask(OwnerComp, NodeMemory);
//
//	UE_LOG(LogTemp, Warning, TEXT("ABORT"));
//
//	return EBTNodeResult::Aborted;
//}
//
//void UMyBTTask_MoveToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
//{
//	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
//	UE_LOG(LogTemp, Warning, TEXT("TICK"));
//}


