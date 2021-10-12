// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Conversation/DialogComponent.h"

// Sets default values for this component's properties
UDialogComponent::UDialogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AudioComponent == nullptr)
	{
		AudioComponent = GetOwner()->FindComponentByClass<UCustomAudioComponent>();
	}

	if (AudioComponent)
	{
		AudioComponent->OnAudioFinished.AddDynamic(this, &UDialogComponent::NotifyLineIsFinishedPlaying);

	}
	
}


// Called every frame
void UDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDialogComponent::PlayDialogLine(USoundCue* DialogLineAudio)
{
	if (AudioComponent)
	{
		if (!AudioComponent->IsPlaying())
		{
			AudioComponent->Sound = DialogLineAudio;
			AudioComponent->Play(0, 500, true, true);
			bIsCurrentlyTalking = true;
			return true;
		}
	}

	return false;
}

USoundCue* UDialogComponent::GetRandSoundBasedOnTag(TArray<FDialogLine>Bank, FString Tag)
{
	TArray<USoundCue*> ValidSounds;
	for (int i = 0; i < Bank.Num();i++)
	{
		if (Bank.IsValidIndex(i) && Bank[i].LineTag == Tag)
		{
			ValidSounds.Add(Bank[i].AudioCue);
		}
	}

	int32 RandomSelection = FMath::RandRange(0, ValidSounds.Num() - 1);

	return ValidSounds[RandomSelection];
}

void UDialogComponent::NotifyLineIsFinishedPlaying()
{
	bIsCurrentlyTalking = false;
}