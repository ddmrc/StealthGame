// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "CustomAudioComponent.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UCustomAudioComponent : public UAudioComponent
{
	GENERATED_BODY()
public:

	void Play(float StartTime = 0.0f) override;
};
