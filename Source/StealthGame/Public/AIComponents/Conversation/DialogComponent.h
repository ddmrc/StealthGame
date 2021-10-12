// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "CustomSound/CustomAudioComponent.h"
#include "DialogComponent.generated.h"


USTRUCT()
struct FDialogLine
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		USoundCue* AudioCue;
		UPROPERTY(EditAnywhere)
		FString LineTag;
		UPROPERTY(EditAnywhere)
		int32 PlayerHeat;

};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STEALTHGAME_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDialogComponent();
	UPROPERTY(EditAnywhere)
	TArray<FDialogLine> DefaultBank;

	bool PlayDialogLine(USoundCue* DialogLineAudio);
	bool GetIsCurrentlyTalking() { return bIsCurrentlyTalking; }
	USoundCue* GetRandSoundBasedOnTag(TArray<FDialogLine>Bank,FString Tag);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void NotifyLineIsFinishedPlaying();


	UCustomAudioComponent* AudioComponent = nullptr;

	bool bIsCurrentlyTalking = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
