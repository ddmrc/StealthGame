// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UWorld;
class AActor;
class USoundBase;
class STEALTHGAME_API CustomSoundWrapper
{
public:
	CustomSoundWrapper();
	~CustomSoundWrapper();

	static void PlaySound(UWorld* World, USoundBase* SoundCue, FVector NoiseLocation, float Loudness, AActor* Instigator, float MaxRange);
};
