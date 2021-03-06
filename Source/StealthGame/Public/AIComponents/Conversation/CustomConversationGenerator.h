// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "CustomSound/CustomAudioComponent.h"
#include "CustomConversationGenerator.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHGAME_API UCustomConversationGenerator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCustomConversationGenerator();

	TArray<USoundCue*> Conversation;
	UCustomAudioComponent* AudioPlayer = nullptr;

	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> A1;
	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> A2;
	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> A3;


	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> B1;
	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> B2;
	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> B3;


	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> C1;
	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> C2;
	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	TArray<USoundCue*> C3;

	UPROPERTY(EditAnywhere, Category = "SoundHandlers")
	USoundCue* Silence;

	UPROPERTY(EditAnywhere, Category = "Dialog Volume")
		float DialogSoundIntensity = 750.f;

	bool bHasConversationBeenLoaded = false;
	bool bHasFinishedTalking = false;
	bool bStartConversation = false;

	UCustomConversationGenerator* OtherConversationGenerator;
	FTimerDelegate ToggleHasFinishedSpeaking;
	AActor* OtherActorInConversation;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//void Play(float StartTime) override;


	UFUNCTION()
	void LoadConversation(FString Category);

	USoundCue* ReturnSoundCue(FString Category, int32 SubCategoryNumber);

	UFUNCTION()
	void WaitForAnswer();


	UFUNCTION()
	void ToggleThroughPhrases();


	UFUNCTION()
		void ToggleHasFinishedTalkingToTrue();
};
