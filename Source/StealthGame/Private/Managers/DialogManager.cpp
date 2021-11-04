// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DialogManager.h"
#include "Kismet/GameplayStatics.h"

static int32 GDialogOption = 0;

static FAutoConsoleVariableRef CVarShowSoundEvents(
	TEXT("Stealth.Debug.ConversationEvents"),
	GDialogOption,
	TEXT("0 = stop, 1 = Greeting, 2 = Inquire, 3 = Goodbye"),
	ECVF_Cheat
);


// Sets default values
ADialogManager::ADialogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADialogManager::BeginPlay()
{
	Super::BeginPlay();



		GDialogOption = 0;




	
}

// Called every frame
void ADialogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DebugDialog();





	//if (GDialogOption == 4)
	//{
	//	FString Topic = "Default";
	//	SetUpConversationForController(1, Topic);
	//	SetUpConversationForController(2, Topic);
	//	GDialogOption = 0;

	//}
	//else if (GDialogOption == 5)
	//{
	//	int32 NumberOfLinesToSay = 3;
	//	RunThroughConversation(NumberOfLinesToSay);
	//}
}

void ADialogManager::DebugDialog()
{

	if (ControllerPatrolGuard1)
	{
		UDialogComponent* DialogComponent = ControllerPatrolGuard1->GetDialogComponent();


		USoundCue* Line1 = DialogComponent->GetRandSoundBasedOnTag(DialogComponent->DefaultBank, "Greeting");
		USoundCue* Line2 = DialogComponent->GetRandSoundBasedOnTag(DialogComponent->DefaultBank, "Inquire");
		USoundCue* Line3 = DialogComponent->GetRandSoundBasedOnTag(DialogComponent->DefaultBank, "Goodbye");
		switch (GDialogOption)
		{
		case 0:
			break;
		case 1:
			if (DialogComponent)
			{
				DialogComponent->PlayDialogLine(Line1);
				GDialogOption = 0;
			}
			break;
		case 2:
			if (DialogComponent)
			{
				DialogComponent->PlayDialogLine(Line2);
				GDialogOption = 0;
			}
			break;
		case 3:
			if (DialogComponent)
			{
				DialogComponent->PlayDialogLine(Line3);
				UE_LOG(LogTemp, Warning, TEXT("PLAYING Dialog"));
				GDialogOption = 0;
			}
			break;
		}
	}
}

void ADialogManager::SetUpAIPointerReference(AEnemyAIController* Controller, int32 PatrolGuardID)
{
	switch (PatrolGuardID)
	{
	case 1:
		ControllerPatrolGuard1 = Controller;
		DialogComponentPatrolGuard1 = Controller->GetDialogComponent();
		break;
	case 2:
		ControllerPatrolGuard2 = Controller;
		DialogComponentPatrolGuard2 = Controller->GetDialogComponent();
		break;
	}
}

bool ADialogManager::CheckAIControllerReferenceByIndex(int32 PatrolGuardID)
{
	switch (PatrolGuardID)
	{
	case 1:
		if (ControllerPatrolGuard1) { return true; }

		break;
	case 2:
		if (ControllerPatrolGuard2) { return true; }
		break;

	}

	return false;
}

void ADialogManager::SetUpConversationForController(int32 PatrolGuardID, FString Topic)
{
	bHasConversationEnded = false;

	USoundCue* Line1;
	USoundCue* Line2;
	USoundCue* Line3;
	switch (PatrolGuardID)
	{
	case 1:
		if (DialogComponentPatrolGuard1 && Topic == "Default")
		{
			Line1 = DialogComponentPatrolGuard1->GetRandSoundBasedOnTag(DialogComponentPatrolGuard1->DefaultBank, "Greeting");
			Line2 = DialogComponentPatrolGuard1->GetRandSoundBasedOnTag(DialogComponentPatrolGuard1->DefaultBank, "Inquire");
			Line3 = DialogComponentPatrolGuard1->GetRandSoundBasedOnTag(DialogComponentPatrolGuard1->DefaultBank, "Goodbye");
		}



		
		ConversationPatrolGuard1.Empty();

		ConversationPatrolGuard1.Add(Line1);
		ConversationPatrolGuard1.Add(Line2);
		ConversationPatrolGuard1.Add(Line3);

		PatrolGuard1HasLinesLeft = true;
		break;

	case 2:
		if (DialogComponentPatrolGuard2)
		{
			Line1 = DialogComponentPatrolGuard2->GetRandSoundBasedOnTag(DialogComponentPatrolGuard2->DefaultBank, "Greeting");
			Line2 = DialogComponentPatrolGuard2->GetRandSoundBasedOnTag(DialogComponentPatrolGuard2->DefaultBank, "Inquire");
			Line3 = DialogComponentPatrolGuard2->GetRandSoundBasedOnTag(DialogComponentPatrolGuard2->DefaultBank, "Goodbye");
		}




		ConversationPatrolGuard2.Empty();

		ConversationPatrolGuard2.Add(Line1);
		ConversationPatrolGuard2.Add(Line2);
		ConversationPatrolGuard2.Add(Line3);

		PatrolGuard2HasLinesLeft = true;
		break;
	}

}

void ADialogManager::RunThroughConversation(int32 NumberOfLinesToSpeak)
{
	if (NumberOfLinesToSpeak == 3)
	{
		if (ConversationPatrolGuard1.Num() > 0 && PatrolGuardIDToTalk == 1)
		{
			if (ConversationPatrolGuard1[0] != nullptr && !DialogComponentPatrolGuard2->GetIsCurrentlyTalking())
			{
				DialogComponentPatrolGuard1->PlayDialogLine(ConversationPatrolGuard1[0]);
				ConversationPatrolGuard1[0] = nullptr;
				PatrolGuardIDToTalk = 2;
				ControllerPatrolGuard2->bIsTalking = false;
				ControllerPatrolGuard1->bIsTalking = true;
			}
			else if (ConversationPatrolGuard1[1] != nullptr && !DialogComponentPatrolGuard2->GetIsCurrentlyTalking())
			{
				DialogComponentPatrolGuard1->PlayDialogLine(ConversationPatrolGuard1[1]);
				ConversationPatrolGuard1[1] = nullptr;
				PatrolGuardIDToTalk = 2;
				ControllerPatrolGuard2->bIsTalking = false;
				ControllerPatrolGuard1->bIsTalking = true;
			}
			else if (ConversationPatrolGuard1[2] != nullptr && !DialogComponentPatrolGuard2->GetIsCurrentlyTalking())
			{
				DialogComponentPatrolGuard1->PlayDialogLine(ConversationPatrolGuard1[2]);
				ConversationPatrolGuard1[2] = nullptr;
				PatrolGuardIDToTalk = 2;
				PatrolGuard1HasLinesLeft = false;
				ControllerPatrolGuard2->bIsTalking = false;
				ControllerPatrolGuard1->bIsTalking = true;

			}
		}
		else if (ConversationPatrolGuard2.Num() > 0 && PatrolGuardIDToTalk == 2)
		{

			if (ConversationPatrolGuard2[0] != nullptr && !DialogComponentPatrolGuard1->GetIsCurrentlyTalking())
			{
				DialogComponentPatrolGuard2->PlayDialogLine(ConversationPatrolGuard2[0]);
				ConversationPatrolGuard2[0] = nullptr;
				PatrolGuardIDToTalk = 1;
				ControllerPatrolGuard1->bIsTalking = false;
				ControllerPatrolGuard2->bIsTalking = true;
			}
			else if (ConversationPatrolGuard2[1] != nullptr && !DialogComponentPatrolGuard1->GetIsCurrentlyTalking())
			{
				DialogComponentPatrolGuard2->PlayDialogLine(ConversationPatrolGuard2[1]);
				ConversationPatrolGuard2[1] = nullptr;
				PatrolGuardIDToTalk = 1;
				ControllerPatrolGuard1->bIsTalking = false;
				ControllerPatrolGuard2->bIsTalking = true;
			}
			else if (ConversationPatrolGuard2[2] != nullptr && !DialogComponentPatrolGuard1->GetIsCurrentlyTalking())
			{
				DialogComponentPatrolGuard2->PlayDialogLine(ConversationPatrolGuard2[2]);
				ConversationPatrolGuard2[2] = nullptr;
				PatrolGuardIDToTalk = 1;

				PatrolGuard2HasLinesLeft = false;
				ControllerPatrolGuard1->bIsTalking = false;
				ControllerPatrolGuard2->bIsTalking = true;

			}
		}
	}

	if (!PatrolGuard1HasLinesLeft && !PatrolGuard2HasLinesLeft)
	{
		bHasConversationEnded = true;
	}
}

bool ADialogManager::GetIfAConversationIsReadyToStart() 
{
	if (PatrolGuard1ConversationReady && PatrolGuard2ConversationReady)
	{
		return true;
	}
	return false;
}

bool ADialogManager::HasAnyAIHaveConversationLeft()
{
	if (PatrolGuard1HasLinesLeft || PatrolGuard2HasLinesLeft)
	{
		return true;
	}

	return false;
}

