// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AIManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"



static int32 GForceAIState = 0;

static FAutoConsoleVariableRef CVarShowSoundEvents(
	TEXT("Stealth.Debug.ForceAIState"),
	GForceAIState,
	TEXT("0.Default, 1.Idle, 2.Patr, 3.Chase, 4.LookAr, 5.Search, 6.Confu, 7.Det, 8.Conv, Disable: 9. Sight 10.Hearing 11.All, Heat: 12.+ 13.-"),
	ECVF_Cheat
);


// Sets default values
AAIManager::AAIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/StealhGame/Blueprints/Managers/DialogManager_BP.DialogManager_BP'"));
	if (ItemBlueprint.Object)
	{

		MyItemBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}

	ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint2(TEXT("Blueprint'/Game/StealhGame/Blueprints/Characters/AICharacter_BP.AICharacter_BP'"));
	if (ItemBlueprint2.Object)
	{

		MyItemBlueprint2 = (UClass*)ItemBlueprint2.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AAIManager::BeginPlay()
{
	Super::BeginPlay();
	
	GForceAIState = 0;
	LocalPlayerHeat = 0;

}

// Called every frame
void AAIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetUpAIPointers();
	//SetUpDialogManager();
	DebugAIState();

	DialogMechanic();




	UpdatePlayerHeat();


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
					ControllerPatrolGuard1->ConversationIdentifier = "PatrolGuard1";
				}
			}
			else if (TempActorArr.IsValidIndex(i) && TempActorArr[i]->Tags[0] == "Guard2")
			{
				if (PatrolGuard2 == nullptr)
				{
					PatrolGuard2 = Cast<AEnemyCharacter>(TempActorArr[i]);
					ControllerPatrolGuard2 = Cast<AEnemyAIController>(PatrolGuard2->GetController());
					ControllerPatrolGuard2->ConversationIdentifier = "PatrolGuard2";
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
		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Confused)
		{
			ControllerPatrolGuard1->SetAIState(EAIStates::Confused);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Confused)
		{
			ControllerPatrolGuard2->SetAIState(EAIStates::Confused);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		bDebugNeedsReset = true;
		break;
	case 7:
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
	case 8:


		if (ControllerPatrolGuard1 && ControllerPatrolGuard1->CurrentAIState != EAIStates::Conversation)
		{
			ControllerPatrolGuard1->SetWantsToStartConversation(true);
			ControllerPatrolGuard1->SetAIState(EAIStates::Conversation);
			ControllerPatrolGuard1->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();
		}
		if (ControllerPatrolGuard2 && ControllerPatrolGuard2->CurrentAIState != EAIStates::Conversation)
		{
			ControllerPatrolGuard2->SetWantsToStartConversation(true);
			ControllerPatrolGuard2->SetAIState(EAIStates::Conversation);
			ControllerPatrolGuard2->FindComponentByClass<UAIPerceptionComponent>()->Deactivate();

		}
		GForceAIState = 99;
		bDebugNeedsReset = true;
		break;
	case 9:
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
	case 10:
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
	case 11:
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
	case 12:
		if (LocalPlayerHeat < 3)
		{
			LocalPlayerHeat++;
			GForceAIState = 99;
		}
		break;
	case 13:
		if (LocalPlayerHeat > 0)
		{
			LocalPlayerHeat--;
			GForceAIState = 99;
		}
		break;

	case 99:

		break;
	}
}

bool AAIManager::CheckIfAnyAIWantsConversation()
{
	bool bGPatrolGuard1WantsConversation = false;
	bool bGPatrolGuard2WantsConversation = false;
	if (ControllerPatrolGuard1 && ControllerPatrolGuard1->GetWantsToStartConversation())
	{
		bGPatrolGuard1WantsConversation = true;
		
	}

	if (ControllerPatrolGuard2 && ControllerPatrolGuard2->GetWantsToStartConversation())
	{
		bGPatrolGuard2WantsConversation = true;
	}

	if (bGPatrolGuard1WantsConversation && bGPatrolGuard2WantsConversation)
	{
		return true;
	}

	return false;
}

void AAIManager::DialogMechanic()
{

		if (CheckIfAnyAIWantsConversation() && MakeAIFaceEachOther())
		{
					
			ControllerPatrolGuard1->SetAIState(EAIStates::Conversation);
			ControllerPatrolGuard2->SetAIState(EAIStates::Conversation);

			if (DialogManager == nullptr)
			{
				SpawnDialogManager();
			}

			if (DialogManager)
			{
				DialogManager->SetUpConversationForController(1, "Default");
				DialogManager->SetUpConversationForController(2, "Default");
			}




			ControllerPatrolGuard1->SetWantsToStartConversation(false);
			ControllerPatrolGuard2->SetWantsToStartConversation(false);
		}

		if (DialogManager && DialogManager->HasAnyAIHaveConversationLeft())
		{
			DialogManager->RunThroughConversation(3);
		}
		if (DialogManager)
		{
			DialogManager->CheckIfConversationHasEnded();
		}
		

		if (DialogManager && DialogManager->NotifyConversationHasEnded())
		{

			if (ControllerPatrolGuard1->bConversationPointNeedsDeleting)
			{
				ControllerPatrolGuard1->SpawnTargetLocationHandler->RemoveRandomSearchPoints();
				ControllerPatrolGuard1->SpawnTargetLocationHandler->ToggleAllPatrolMoveToPoints(true);

				ControllerPatrolGuard1->bConversationPointNeedsCreating = true;
				ControllerPatrolGuard1->bConversationPointNeedsDeleting = false;
			}
			


			if (ControllerPatrolGuard1->CurrentAIState == EAIStates::Conversation)
			{
				ControllerPatrolGuard1->SetAIState(EAIStates::Patrol);

			}

			if (ControllerPatrolGuard2->CurrentAIState == EAIStates::Conversation)
			{
				ControllerPatrolGuard2->SetAIState(EAIStates::Patrol);
			}

			DialogManager->Destroy();
			if (DialogManager != nullptr)
			{
				DialogManager = nullptr;
			}
		}
	
}

void AAIManager::SpawnDialogManager()
{
	if (MyItemBlueprint)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		AActor* NewActor = GetWorld()->SpawnActor(MyItemBlueprint, &SpawnLocation, &SpawnRotation);

		DialogManager = Cast<ADialogManager>(NewActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Class Not Detected to Spawn"));
	}

	if (DialogManager && !DialogManager->CheckAIControllerReferenceByIndex(1) && !DialogManager->CheckAIControllerReferenceByIndex(2))
	{
		DialogManager->SetUpAIPointerReference(ControllerPatrolGuard1, 1);
		DialogManager->SetUpAIPointerReference(ControllerPatrolGuard2, 2);
	}
}

void AAIManager::SpawnPatrolGuard(FVector Location, FRotator Rotation)
{
	if (MyItemBlueprint2)
	{

		AActor* NewActor = GetWorld()->SpawnActor(MyItemBlueprint2, &Location, &Rotation);


	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Class Not Detected to Spawn"));
	}
}

void AAIManager::UpdatePlayerHeat()
{
	if (ControllerPatrolGuard1 && ControllerPatrolGuard2)
	{
		if ((ControllerPatrolGuard1->PlayerDetectionTag == EPlayerDetectionStatus::Detected ||
			ControllerPatrolGuard2->PlayerDetectionTag == EPlayerDetectionStatus::Detected) &&
			!bHasPlayerHeatBeenIncreased)
		{
			if (LocalPlayerHeat < 3)
			{
				LocalPlayerHeat++;
				bHasPlayerHeatBeenIncreased = true;
			}

			
		}
		else if (ControllerPatrolGuard1->PlayerDetectionTag == EPlayerDetectionStatus::NotDetected &&
			ControllerPatrolGuard2->PlayerDetectionTag == EPlayerDetectionStatus::NotDetected &&
			bHasPlayerHeatBeenIncreased)
		{
			bHasPlayerHeatBeenIncreased = false;
		}

		ControllerPatrolGuard1->PlayerHeat = LocalPlayerHeat;
		ControllerPatrolGuard2->PlayerHeat = LocalPlayerHeat;

	}
}

bool AAIManager::MakeAIFaceEachOther()
{
	if (PatrolGuard1 && PatrolGuard2)
	{

		
		FVector Guard1Location = PatrolGuard1->GetActorLocation();
		FVector Offset = PatrolGuard1->GetActorForwardVector() * 100.f;
		FVector NewLocation = Guard1Location + Offset;

		if (ControllerPatrolGuard1->bConversationPointNeedsCreating)
		{
			ControllerPatrolGuard1->SpawnTargetLocationHandler->ToggleAllPatrolMoveToPoints(false);


			ControllerPatrolGuard1->SpawnTargetLocationHandler->SpawnRandomSearchPoints(NewLocation, 0.f, 1);

			ControllerPatrolGuard1->bConversationPointNeedsCreating = false;
			ControllerPatrolGuard1->bConversationPointNeedsDeleting = true;
		}


		//if (!PatrolGuard2->GetActorLocation().Equals(NewLocation), 50.f)
		//{
		//	if (ControllerPatrolGuard2->GetSpawnTargetLocationHandler())
		//	{
		//		//ControllerPatrolGuard2->GetSpawnTargetLocationHandler()->SpawnRandomSearchPoints(NewLocation, 0.f, 1);
		//		ControllerPatrolGuard2->MoveToLocation(NewLocation);
		//		PatrolGuard2->SetActorRotation(PatrolGuard1->GetViewRotation() + 
		//			FRotator(PatrolGuard1->GetActorForwardVector().X , GetActorForwardVector().Y + 180.f,
		//				0.f));
		//	}
		//	
		//	
		//	//PatrolGuard2->SetActorLocation(NewLocation);
		//}
	

		if (ControllerPatrolGuard2->TargetMoveLocation)
		{
			if (ControllerPatrolGuard2->GetCharacter()->GetActorLocation().Equals(ControllerPatrolGuard2->TargetMoveLocation->GetActorLocation(), 50.f))
			{
				ControllerPatrolGuard2->bHasReachedConversationLocation = true;
				return true;
			}
		}
		//if (PatrolGuard2->GetActorLocation().Equals(NewLocation,100.f))
		//{
		//	return true;
		//}
	}

	return false;
}