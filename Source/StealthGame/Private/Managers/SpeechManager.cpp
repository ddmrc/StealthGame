// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SpeechManager.h"

// Sets default values
ASpeechManager::ASpeechManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpeechManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpeechManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bProcessIsAlive)
	{
		if (bIsConversationOnGoing)
		{
			MediateConversation();
		}
		else if (SettingUpNetworkForConversation())
		{
			bIsConversationOnGoing = true;
			
		}
		
		if (!IsConversationOnGoing())
		{
			bProcessIsAlive = false;
			bHasNetworkForConversationBeenSet = true;
			bIsConversationOnGoing = false;

		}
	}


	

}

bool ASpeechManager::RequestJoinConversation(AActor* IncomingActor)
{

	if (IncomingActor && (SpeakerOne == nullptr || SpeakerTwo == nullptr))
	{
		
		if (SpeakerOne == nullptr)
		{
			SpeakerOne = IncomingActor;
			SpeakerOneComponent = SpeakerOne->FindComponentByClass<UCustomConversationGenerator>();
		}
		else if (SpeakerTwo == nullptr)
		{
			SpeakerTwo = IncomingActor;
			SpeakerTwoComponent = SpeakerTwo->FindComponentByClass<UCustomConversationGenerator>();
		}
		else
		{
			return false;
		}
		
		if (SpeakerOne && SpeakerTwo)
		{
			bHasNetworkForConversationBeenSet = false;
			bProcessIsAlive = true;
			return true;
		}

	}

	return false;
}


bool ASpeechManager::SettingUpNetworkForConversation()
{
	if (SpeakerOne && SpeakerTwo && SpeakerOneComponent && SpeakerTwoComponent)
	{

		if (!bHasNetworkForConversationBeenSet)
		{
			FString Category = RandomConversationCategorySelector();

			SpeakerOneComponent->LoadConversation(Category);
			SpeakerTwoComponent->LoadConversation(Category);

			SpeakerTwoComponent->bHasFinishedTalking = true;
			bForceSpeakerOneTalk = true;
			return true;
		}
		
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("Network For Conversation Not Created in SpeechManager.cpp (71)."))
	}

	return false;
}

FString ASpeechManager::RandomConversationCategorySelector()
{
	FString CategoryToLoad;
	int32 RandCategory = FMath::RandRange(0, 2);

	switch (RandCategory)
	{
	case 0:
		CategoryToLoad = "A";
		break;
	case 1:
		CategoryToLoad = "B";
		break;
	case 2:
		CategoryToLoad = "C";
		break;
	default:
		CategoryToLoad = "A";
		break;

	}

	return CategoryToLoad;
}

void ASpeechManager::MediateConversation()
{
	
	//S1 speaks
	//S1 Silence
	//S2 speaks
	//S2 Sielce
	//Check how much conversation left





	if (!bForceSpeakerOneTalk && SpeakerOneComponent->bHasFinishedTalking && !SpeakerOneComponent->AudioPlayer->IsPlaying())
	{

		SpeakerTwoIsSpeaking();
		bForceSpeakerOneTalk = true;
	}
	else if (bForceSpeakerOneTalk && SpeakerTwoComponent->bHasFinishedTalking && !SpeakerTwoComponent->AudioPlayer->IsPlaying())
	{

		SpeakerOneIsSpeaking();
		bForceSpeakerOneTalk = false;




	}

	

}

bool ASpeechManager::SpeakerOneIsSpeaking()
{


		SpeakerOneComponent->ToggleThroughPhrases();
		
		

	
	return IsConversationOnGoing();

}
bool ASpeechManager::SpeakerTwoIsSpeaking()
{


		SpeakerTwoComponent->ToggleThroughPhrases();




	return IsConversationOnGoing();
}
bool ASpeechManager::IsConversationOnGoing()
{
	if (SpeakerOneComponent->Conversation[2] == nullptr && SpeakerTwoComponent->Conversation[2] == nullptr)
	{
		SpeakerOne = nullptr;
		SpeakerTwo = nullptr;
		SpeakerOneComponent->AudioPlayer->Sound = nullptr;
		SpeakerTwoComponent->AudioPlayer->Sound = nullptr;
		bProcessIsAlive = false;
		bIsConversationOnGoing = false;
		UE_LOG(LogTemp, Warning, TEXT("Finished"));
		return false;

	}
	else
	{
		return true;
	}
}
void ASpeechManager::EmergencyKillAudio()
{
	if (SpeakerOne && SpeakerOneComponent)
	{
		SpeakerOneComponent->AudioPlayer->Stop();
		SpeakerOne = nullptr;
		SpeakerOneComponent = nullptr;
	}

	if (SpeakerTwo && SpeakerTwoComponent)
	{
		SpeakerTwoComponent->AudioPlayer->Stop();
		SpeakerTwo = nullptr;
		SpeakerTwoComponent = nullptr;
	}
	 
}