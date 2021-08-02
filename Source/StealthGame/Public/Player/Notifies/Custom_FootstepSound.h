// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "Custom_FootstepSound.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UCustom_FootstepSound : public UAnimNotify_PlaySound
{
	GENERATED_BODY()
	

public:

	UCustom_FootstepSound();

	//UPROPERTY(EditAnywhere)
	float MaxSoundDistanceDetection = 1000.f;
	UPROPERTY(EditAnywhere)
	float CustomVolumeMultiplier = 1.f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
