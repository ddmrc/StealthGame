// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSound/CustomSoundWrapper.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include <DrawDebugHelpers.h>


static int32 GShowSoundEvents = 0;

static FAutoConsoleVariableRef CVarShowSoundEvents(
	TEXT("Stealth.Debug.ShowNoiseEvents"),
	GShowSoundEvents,
	TEXT("0 = hide, 1 = show, 2 = loud"),
	ECVF_Cheat
);


CustomSoundWrapper::CustomSoundWrapper()
{
}

CustomSoundWrapper::~CustomSoundWrapper()
{
}


void CustomSoundWrapper::PlaySoundAtLocation(UWorld* World, USoundBase* SoundCue, FVector NoiseLocation, float Loudness, AActor* Instigator, float MaxRange)
{
	UGameplayStatics::PlaySoundAtLocation(World, SoundCue, NoiseLocation, Loudness);
	UAISense_Hearing::ReportNoiseEvent(World, NoiseLocation, Loudness, Instigator, MaxRange);

	if (GShowSoundEvents == 0)
	{
		return;
	}

	
	DrawDebugSphere(World, NoiseLocation, MaxRange, 24, FColor::Red,false,1.f);
}