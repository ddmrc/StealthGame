// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DialogManager.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ADialogManager::ADialogManager()
{

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

void ADialogManager::SetUpConversationForController(int32 PatrolGuardID, int32 NumberOfLines, FString Topic)
{

	if (NumberOfLines == 3)
	{
		USoundCue* Line1;
		USoundCue* Line2;
		USoundCue* Line3;
		
		FString Line1Tag;
		FString Line2Tag;
		FString Line3Tag;

		if (Topic == "Default")
		{
			Line1Tag = "Greeting";
			Line2Tag = "Inquire";
			Line3Tag = "Goodbye";
		}

		switch (PatrolGuardID)
		{
		case 1:
			if (DialogComponentPatrolGuard1)
			{
				Line1 = DialogComponentPatrolGuard1->GetRandSoundBasedOnTag
				(DialogComponentPatrolGuard1->DefaultBank, Line1Tag, ControllerPatrolGuard1->PlayerHeat);

				Line2 = DialogComponentPatrolGuard1->GetRandSoundBasedOnTag
				(DialogComponentPatrolGuard1->DefaultBank, Line2Tag, ControllerPatrolGuard1->PlayerHeat);

				Line3 = DialogComponentPatrolGuard1->GetRandSoundBasedOnTag
				(DialogComponentPatrolGuard1->DefaultBank, Line3Tag, ControllerPatrolGuard1->PlayerHeat);
			}

			ConversationPatrolGuard1.Add(Line1);
			ConversationPatrolGuard1.Add(Line2);
			ConversationPatrolGuard1.Add(Line3);

			PatrolGuard1HasLinesLeft = true;
			break;

		case 2:
			if (DialogComponentPatrolGuard2)
			{
				Line1 = DialogComponentPatrolGuard2->GetRandSoundBasedOnTag
				(DialogComponentPatrolGuard2->DefaultBank, Line1Tag, ControllerPatrolGuard2->PlayerHeat);

				Line2 = DialogComponentPatrolGuard2->GetRandSoundBasedOnTag
				(DialogComponentPatrolGuard2->DefaultBank, Line2Tag, ControllerPatrolGuard2->PlayerHeat);

				Line3 = DialogComponentPatrolGuard2->GetRandSoundBasedOnTag
				(DialogComponentPatrolGuard2->DefaultBank, Line3Tag, ControllerPatrolGuard2->PlayerHeat);
			}

			ConversationPatrolGuard2.Add(Line1);
			ConversationPatrolGuard2.Add(Line2);
			ConversationPatrolGuard2.Add(Line3);

			PatrolGuard2HasLinesLeft = true;
			break;
		}
	}

}

bool ADialogManager::HasAnyAIHaveConversationLeft()
{
	if (PatrolGuard1HasLinesLeft || PatrolGuard2HasLinesLeft)
	{
		return true;
	}

	return false;
}

void ADialogManager::RunThroughConversation()
{

	if (ConversationPatrolGuard1.Num() > 0 && PatrolGuardIDToTalk == 1 && !DialogComponentPatrolGuard2->GetIsCurrentlyTalking())
	{
		DialogComponentPatrolGuard1->PlayDialogLine(ConversationPatrolGuard1[0]);

		ConversationPatrolGuard1.RemoveAt(0,1, true);

		if (ConversationPatrolGuard1.Num() == 0)
		{
			PatrolGuard1HasLinesLeft = false;
		}

		PatrolGuardIDToTalk = 2;

	}

	else if (ConversationPatrolGuard2.Num() > 0 && PatrolGuardIDToTalk == 2 && !DialogComponentPatrolGuard1->GetIsCurrentlyTalking())
	{
		DialogComponentPatrolGuard2->PlayDialogLine(ConversationPatrolGuard2[0]);

		ConversationPatrolGuard2.RemoveAt(0, 1, true);

		if (ConversationPatrolGuard2.Num() == 0)
		{
			PatrolGuard2HasLinesLeft = false;
		}

		PatrolGuardIDToTalk = 1;

	}
	

}

bool ADialogManager::NotifyConversationHasEnded() 
{ 
	if (!PatrolGuard1HasLinesLeft && !PatrolGuard2HasLinesLeft)
	{
		if (!DialogComponentPatrolGuard1->GetIsCurrentlyTalking() && !DialogComponentPatrolGuard2->GetIsCurrentlyTalking())
		{
			bHasConversationEnded = true;
		}
	}

	return bHasConversationEnded; 
}





