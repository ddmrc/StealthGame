// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AIManager.h"

// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	




	//if (ControllerPatrolGuard1 && PatrolGuard1)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Guard1"));
	//}

	//if (ControllerPatrolGuard2 && PatrolGuard2)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Guard2"));
	//}
	//if (DialogManager)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("DialogManager Set"));
	//}

}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetUpAIPointers();
	SetUpDialogManager();


	if (ControllerPatrolGuard1->CurrentAIState != EAIStates::Idle  || ControllerPatrolGuard2->CurrentAIState != EAIStates::Idle)
	{
		ControllerPatrolGuard1->SetAIState(EAIStates::Idle);
		ControllerPatrolGuard2->SetAIState(EAIStates::Idle);
	}

}

void AAIManager::SetUpAIPointers()
{
	TArray <AActor*> TempActorArr;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemyCharacter::StaticClass(), TempActorArr);
	if (TempActorArr.Num() > 0)
	{
		for (int i = 0; i < TempActorArr.Num(); i++)
		{
			if (TempActorArr.IsValidIndex(i) && TempActorArr[i]->Tags[0] == "Guard1")
			{
				if (PatrolGuard1 == nullptr)
				{
					PatrolGuard1 = Cast<AEnemyCharacter>(TempActorArr[i]);
					ControllerPatrolGuard1 = Cast<AEnemyAIController>(PatrolGuard1->GetController());
				}
			}
			else if (TempActorArr.IsValidIndex(i) && TempActorArr[i]->Tags[0] == "Guard2")
			{
				if (PatrolGuard2 == nullptr)
				{
					PatrolGuard2 = Cast<AEnemyCharacter>(TempActorArr[i]);
					ControllerPatrolGuard2 = Cast<AEnemyAIController>(PatrolGuard2->GetController());
				}
			}
		}
	}
}

void AAIManager::SetUpDialogManager()
{
	TArray <AActor*> TempArr;
	UGameplayStatics::GetAllActorsOfClass(this, ADialogManager::StaticClass(), TempArr);
	if (TempArr.IsValidIndex(0))
	{
		DialogManager = Cast<ADialogManager>(TempArr[0]);
	}


	if (DialogManager && !DialogManager->CheckAIControllerReferenceByIndex(1) && !DialogManager->CheckAIControllerReferenceByIndex(2))
	{
		DialogManager->SetUpAIPointerReference(ControllerPatrolGuard1, 1);
		DialogManager->SetUpAIPointerReference(ControllerPatrolGuard2, 2);
	}
}