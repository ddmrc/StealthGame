// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Tasks/MyBTTask_Searching.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/Character.h"



UMyBTTask_Searching::UMyBTTask_Searching(const FObjectInitializer& objectInitializer)
{
	NodeName = "CustomMoveToLocation";
	bNotifyTick = true;
	bStopOnOverlap = false;
}

EBTNodeResult::Type UMyBTTask_Searching::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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



void UMyBTTask_Searching::SelectTarget(UBehaviorTreeComponent& OwnerComp)
{


	//OwnerComp.GetBlackboardComponent()->SetValueAsObject("MoveToLocation",)


	//if (MoveToLocationList.Num() == 0)
	//{
	//	UGameplayStatics::GetAllActorsOfClass(this, AMoveToLocationActor::StaticClass(), MoveToLocationList);

	//}



	//if (MoveToLocationList.Num() > 0 && bIsMoveToLocationReached)
	//{
	//	int RandomNumber = FMath::RandRange(0, MoveToLocationList.Num() - 1);

	//	if (MoveToLocationList.IsValidIndex(RandomNumber))
	//	{

	//		CurrentMoveToTarget = MoveToLocationList[RandomNumber];
	//		OwnerComp.GetBlackboardComponent()->SetValueAsObject("MoveToLocation", MoveToLocationList[RandomNumber]);
	//		bIsMoveToLocationReached = false;
	//	}
	//}

	//if (CurrentMoveToTarget)
	//{


	//	if (OwnerComp.GetAIOwner()->GetCharacter()->GetActorLocation().Equals(CurrentMoveToTarget->GetActorLocation(), 50.f))
	//	{


	//		bIsMoveToLocationReached = true;
	//	}
	//}
}

