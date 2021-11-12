// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/AIManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/KismetMathLibrary.h"



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

	DebugAIState();

	DialogMechanic();

	UpdatePlayerHeat();
}

void AAIManager::SetUpAIPointers()
{
	if (PatrolGuard1 == nullptr || PatrolGuard2 == nullptr)
	{

		TArray <AActor*> TempActorArr;
		UGameplayStatics::GetAllActorsOfClass(this, AEnemyCharacter::StaticClass(), TempActorArr);
		if (TempActorArr.Num() > 0)
		{
			for (int i = 0; i < TempActorArr.Num(); i++)
			{
				if (TempActorArr.IsValidIndex(i))
				{

					if (TempActorArr[i]->Tags.Num() > 0 && TempActorArr[i]->Tags[0].ToString() == "Guard1")
					{
						
						if (PatrolGuard1 == nullptr)
						{
							PatrolGuard1 = Cast<AEnemyCharacter>(TempActorArr[i]);
							ControllerPatrolGuard1 = Cast<AEnemyAIController>(PatrolGuard1->GetController());
							ControllerPatrolGuard1->ConversationIdentifier = "PatrolGuard1";
						}

					}
					else if (TempActorArr[i]->Tags.Num() == 0 && TempActorArr[i]->GetOwner()->GetName().EndsWith("0"))
					{
						if (PatrolGuard1 == nullptr)
						{
							PatrolGuard1 = Cast<AEnemyCharacter>(TempActorArr[i]);
							ControllerPatrolGuard1 = Cast<AEnemyAIController>(PatrolGuard1->GetController());
							ControllerPatrolGuard1->ConversationIdentifier = "PatrolGuard1";
							PatrolGuard1->Tags.Add("Guard1");
						}
					}

					if (TempActorArr[i]->Tags.Num() > 0 && TempActorArr[i]->Tags[0].ToString() == "Guard2")
					{
						if (PatrolGuard2 == nullptr)
						{
							
							PatrolGuard2 = Cast<AEnemyCharacter>(TempActorArr[i]);
							ControllerPatrolGuard2 = Cast<AEnemyAIController>(PatrolGuard2->GetController());
							ControllerPatrolGuard2->ConversationIdentifier = "PatrolGuard2";
						}
					}
					else if (TempActorArr[i]->Tags.Num() == 0 && TempActorArr[i]->GetOwner()->GetName().EndsWith("1"))
					{
						if (PatrolGuard2 == nullptr)
						{
							PatrolGuard2 = Cast<AEnemyCharacter>(TempActorArr[i]);
							ControllerPatrolGuard2 = Cast<AEnemyAIController>(PatrolGuard2->GetController());
							ControllerPatrolGuard2->ConversationIdentifier = "PatrolGuard2";
							PatrolGuard2->Tags.Add("Guard2");
						}

					}
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

	if (ControllerPatrolGuard2 && ControllerPatrolGuard2->GetWantsToStartConversation() && !ControllerPatrolGuard2->TargetMoveLocation)
	{
		bGPatrolGuard2WantsConversation = true;
	}

	if (bGPatrolGuard1WantsConversation && bGPatrolGuard2WantsConversation)
	{

		ControllerPatrolGuard1->bConversationPointNeedsCreating = true;

		return true;
	}

	return false;
}

void AAIManager::DialogMechanic()
{
	CheckIfAnyAIWantsConversation();

		if (MakeAIFaceEachOther())
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

		if (DialogManager)
		{
			if (DialogManager->HasAnyAIHaveConversationLeft())
			{
				DialogManager->RunThroughConversation(3);
			}

			DialogManager->CheckIfConversationHasEnded();

			if (DialogManager->NotifyConversationHasEnded())
			{
				if (ControllerPatrolGuard1->bConversationPointNeedsDeleting)
				{
					ControllerPatrolGuard1->SpawnTargetLocationHandler->RemoveRandomSearchPoints();

					ControllerPatrolGuard1->bConversationPointNeedsDeleting = false;
				}

				if (ControllerPatrolGuard2->bHasReachedConversationLocation)
				{
					ControllerPatrolGuard2->bHasReachedConversationLocation = false;

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

				if (ControllerPatrolGuard2->TargetMoveLocation)
				{
					ControllerPatrolGuard2->TargetMoveLocation = nullptr;
				}
				

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

		if (ControllerPatrolGuard1->bConversationPointNeedsCreating)
		{
			
			PatrolGuard1->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PatrolGuard1->GetActorLocation(), PatrolGuard2->GetActorLocation()));
			
			FVector Guard1Location = PatrolGuard1->GetActorLocation();
			FVector Offset = PatrolGuard1->GetActorForwardVector() * 100.f;
			FVector NewLocation = Guard1Location + Offset;

			ControllerPatrolGuard1->SpawnTargetLocationHandler->ToggleAllPatrolMoveToPoints(false);

			ControllerPatrolGuard1->SpawnTargetLocationHandler->SpawnRandomSearchPoints(NewLocation, 0.f, 0);

			ControllerPatrolGuard1->bConversationPointNeedsCreating = false;
			ControllerPatrolGuard1->bConversationPointNeedsDeleting = true;			

		}


		if (ControllerPatrolGuard2->TargetMoveLocation && 
			PatrolGuard2 && 
			PatrolGuard2->GetActorLocation().Equals(ControllerPatrolGuard2->TargetMoveLocation->GetActorLocation(), 50.f))
		{


			PatrolGuard1->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PatrolGuard1->GetActorLocation(), PatrolGuard2->GetActorLocation()));
			PatrolGuard2->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(ControllerPatrolGuard2->TargetMoveLocation->GetActorLocation(), PatrolGuard1->GetActorLocation()));

			if (!ControllerPatrolGuard2->bHasReachedConversationLocation)
			{
				ControllerPatrolGuard1->SpawnTargetLocationHandler->RemoveRandomSearchPoints();
				ControllerPatrolGuard1->SpawnTargetLocationHandler->ToggleAllPatrolMoveToPoints(false);

				ControllerPatrolGuard2->bHasReachedConversationLocation = true;
				//ControllerPatrolGuard2->TargetMoveLocation = nullptr;
				return true;
			}

				
		}

	
	}

	return false;
}