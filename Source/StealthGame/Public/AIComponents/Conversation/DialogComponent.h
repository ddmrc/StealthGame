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

	UCustomAudioComponent* AudioComponent = nullptr;
	bool bIsCurrentlyTalking = false;

	UPROPERTY(EditAnywhere, Category = "Sound Cues")
	TArray<FDialogLine> DefaultBank;

	UPROPERTY(EditAnywhere, Category = "AI Report")
	bool bReportSoundEnable = true;
	UPROPERTY(EditAnywhere, Category = "AI Report")
	float ReportSoundRange = 500.f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebugDraw = true;

	bool PlayDialogLine(USoundCue* DialogLineAudio);
	bool GetIsCurrentlyTalking() { return bIsCurrentlyTalking; }

	USoundCue* GetRandSoundBasedOnTag(TArray<FDialogLine>Bank, FString Tag, int32 PlayerHeat);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void NotifyLineIsFinishedPlaying();		
};
