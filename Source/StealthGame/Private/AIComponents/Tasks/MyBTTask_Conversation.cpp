// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Tasks/MyBTTask_Conversation.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyCharacter.h"
#include "GameFramework/Character.h"

UMyBTTask_Conversation::UMyBTTask_Conversation(const FObjectInitializer& objectInitializer)
{
	NodeName = "CustomConversation";
	bNotifyTick = true;
	bStopOnOverlap = false;
}

EBTNodeResult::Type UMyBTTask_Conversation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Super::ExecuteTask(OwnerComp, NodeMemory);
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;


	//Put Code Here
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()->GetCharacter()->GetController());
	if (AIController->ConversationIdentifier != "PatrolGuard1")
	{
		if (!AIController->bHasReachedConversationLocation && !bMoveToScanned)
		{
			SelectTarget(OwnerComp);
		}
		
	}




	//COPY PASTE FROM ORIGINAL SOURCE CODE
	FBTMoveToTaskMemory* MyMemory = CastInstanceNodeMemory<FBTMoveToTaskMemory>(NodeMemory);
	MyMemory->PreviousGoalLocation = FAISystem::InvalidLocation;
	MyMemory->MoveRequestID = FAIRequestID::InvalidRequest;

	AAIController* MyController = OwnerComp.GetAIOwner();

	MyMemory->bWaitingForPath = bUseGameplayTasks ? false : MyController->ShouldPostponePathUpdates();
	if (!MyMemory->bWaitingForPath && AIController->ConversationIdentifier != "PatrolGuard1")
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


void UMyBTTask_Conversation::SelectTarget(UBehaviorTreeComponent& OwnerComp)
{

	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()->GetCharacter()->GetController());

	if (!AIController->bHasReachedConversationLocation && !bMoveToScanned)
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveTo Located"));
		TArray <AActor*> TempArr;

		UGameplayStatics::GetAllActorsOfClassWithTag(this, AMoveToLocationActor::StaticClass(), FName("Activated"), TempArr);
		bMoveToScanned = true;

		if (TempArr.IsValidIndex(0))
		{
			CurrentMoveToTarget = TempArr[0];
			OwnerComp.GetBlackboardComponent()->SetValueAsObject("MoveToLocation", CurrentMoveToTarget);
			bIsMoveToLocationReached = false;

			
			if (AIController)
			{
				AIController->TargetMoveLocation = CurrentMoveToTarget;

			}
		}

	}



	if (CurrentMoveToTarget)
	{
	
		if (OwnerComp.GetAIOwner()->GetCharacter()->GetActorLocation().Equals(CurrentMoveToTarget->GetActorLocation(), 50.f))
		{

			if (AIController)
			{
				AEnemyCharacter* EnemyCharacter = Cast< AEnemyCharacter>(AIController->GetCharacter());
				if (EnemyCharacter)
				{
					EnemyCharacter->UpdateForceRun(false);
				}
				//UE_LOG(LogTemp, Warning, TEXT("MoveTo Located"));
			}

			bIsMoveToLocationReached = true;
			AIController->bHasReachedConversationLocation = true;
		}
	}
}


