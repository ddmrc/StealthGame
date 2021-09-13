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


	if (SettingUpNetworkForConversation())
	{
		MediateConversation();
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
		

		bHasNetworkForConversationBeenSet = false;
		return true;
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

			SpeakerOneComponent->LoadConversation("A");
			SpeakerTwoComponent->LoadConversation("A");

			SpeakerTwoComponent->bHasFinishedTalking = true;
			bIsConversationOnGoing = true;
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

	if (SpeakerOneIsSpeaking() && SpeakerTwoComponent->bHasFinishedTalking)
	{
		if (SpeakerOneComponent->bHasFinishedTalking)
		{
			SpeakerTwoIsSpeaking();
		}
	}
	else
	{
		bIsConversationOnGoing = false;
	}

}

bool ASpeechManager::SpeakerOneIsSpeaking()
{
	if (!SpeakerOneComponent->AudioPlayer->IsPlaying() && IsConversationOnGoing())
	{

		SpeakerOneComponent->ToggleThroughPhrases();
		
		
	}
	
	return IsConversationOnGoing();

}
bool ASpeechManager::SpeakerTwoIsSpeaking()
{
	if (!SpeakerTwoComponent->AudioPlayer->IsPlaying() && IsConversationOnGoing())
	{

		SpeakerTwoComponent->ToggleThroughPhrases();


	}
	UE_LOG(LogTemp, Warning, TEXT("LOL"));
	return IsConversationOnGoing();
}
bool ASpeechManager::IsConversationOnGoing()
{
	if (SpeakerOneComponent->Conversation.IsValidIndex(2) && SpeakerOneComponent->Conversation[2] == nullptr)
	{

		SpeakerOneComponent->AudioPlayer->Sound = nullptr;
		return false;

	}
	else
	{
		return true;
	}
}