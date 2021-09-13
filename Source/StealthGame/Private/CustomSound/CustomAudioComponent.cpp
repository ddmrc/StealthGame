// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSound/CustomAudioComponent.h"
#include "Perception/AISense_Hearing.h"
#include <DrawDebugHelpers.h>

void UCustomAudioComponent::Play(float StartTime)
{
	Super::Play(StartTime);



}

void UCustomAudioComponent::Play(float StartTime, float SoundRange, bool ReportSound, bool Debug)
{
	Super::Play(StartTime);

	if (ReportSound)
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetOwner()->GetActorLocation(), Sound->GetVolumeMultiplier()* SoundRange, GetOwner());

	if (Debug)
	DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), VolumeMultiplier* SoundRange, 24, FColor::Purple, false, 1.f);
	
}