// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIComponents/Conversation/CustomConversationGenerator.h"
#include "SpeechManager.generated.h"

UCLASS()
class STEALTHGAME_API ASpeechManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpeechManager();





protected:

	AActor* SpeakerOne = nullptr;
	UCustomConversationGenerator* SpeakerOneComponent;
	AActor* SpeakerTwo = nullptr;
	UCustomConversationGenerator* SpeakerTwoComponent;

	bool bHasNetworkForConversationBeenSet = false;
	bool bIsConversationOnGoing = false;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FString RandomConversationCategorySelector();

	void MediateConversation();
	bool SpeakerOneIsSpeaking();
	bool SpeakerTwoIsSpeaking();
	bool IsConversationOnGoing();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool RequestJoinConversation(AActor* IncomingActor);

	bool SettingUpNetworkForConversation();



};
