// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AIManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"



static int32 GForceAIState = 0;

static FAutoConsoleVariableRef CVarShowSoundEvents(
	TEXT("Stealth.Debug.ForceAIState"),
	GForceAIState,
	TEXT("0 = Default Behaviour, 1 = Idle, 2 = Patrol, 3 = Chase, 4= LookAround,5= Search,6= Detected, 7= Conversation, 8= Disable Sight,9= Disable Hearing, 10= Disable All Senses"),
	ECVF_Cheat
);


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


	DebugAIState();

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

void AAIManager::DebugAIState()
{
	switch (GForceAIState)
	{
	case 0:
		if (ControllerPatrolGuard1 && bDebugNeedsReset)
		{
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Activate();
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Hearing::StaticClass(), true);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
		}
		if (ControllerPatrolGuard2 && bDebugNeedsReset)
		{
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Activate();
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Hearing::StaticClass(), true);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Sight::StaticClass(), true);
		}
		if (bDebugNeedsReset)
		{
			bDebugNeedsReset = false;
		}
		break;
	case 1:
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Idle)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::Idle);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
			
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Idle)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::Idle);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 2:
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Patrol)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::Patrol);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Patrol)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::Patrol);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 3:
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Chasing)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::Chasing);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Chasing)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::Chasing);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 4:
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::LookingAround)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::LookingAround);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::LookingAround)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::LookingAround);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 5:
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Searching)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::Searching);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Searching)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::Searching);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 6:
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Detected)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::Detected);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Detected)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::Detected);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 7:
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Conversation)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::Conversation);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Conversation)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::Conversation);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 8:
		if (ControllerPatrolGuard1)
		{	
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Sight::StaticClass(),false);
		}
		if (ControllerPatrolGuard2)
		{
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
		}
		bDebugNeedsReset = true;
		break;
	case 9:
		if (ControllerPatrolGuard1)
		{
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
		}
		if (ControllerPatrolGuard2)
		{
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
		}
		bDebugNeedsReset = true;
		break;
	case 10:
		if (ControllerPatrolGuard1)
		{
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
		}
		if (ControllerPatrolGuard2)
		{
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->SetSenseEnabled(UAISense_Hearing::StaticClass(), false);
		}
		bDebugNeedsReset = true;
		break;
	}
}