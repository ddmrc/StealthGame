// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Notifies/Custom_FootstepSound.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/Character.h"
#include "CustomSound/CustomSoundWrapper.h"

UCustom_FootstepSound::UCustom_FootstepSound()
	: Super()
{
	VolumeMultiplier = 1.f;
	PitchMultiplier = 1.f;

#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(196, 142, 255, 255);
	bPreviewIgnoreAttenuation = false;
#endif // WITH_EDITORONLY_DATA
}

void UCustom_FootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	

	if (Sound)
	{
		if (Sound->IsPlayable())
		{
			FVector SoundLocation = MeshComp->GetBoneLocation("pelvis", EBoneSpaces::WorldSpace);

			CustomSoundWrapper::PlaySoundAtLocation(MeshComp->GetWorld(), Sound, SoundLocation, VolumeMultiplier, MeshComp->GetOwner(), MaxSoundDistanceDetection * VolumeMultiplier);

		}
	}
}