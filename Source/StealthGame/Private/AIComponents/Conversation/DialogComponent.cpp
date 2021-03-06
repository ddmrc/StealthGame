// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Conversation/DialogComponent.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/Character.h"

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

USoundCue* UDialogComponent::GetRandSoundBasedOnTag(TArray<FDialogLine>Bank, FString Tag, int32 PlayerHeat)
{
	TArray<USoundCue*> ValidSounds;

	for (int i = 0; i < Bank.Num();i++)
	{
		if (Bank.IsValidIndex(i) && Bank[i].LineTag == Tag && Bank[i].PlayerHeat == PlayerHeat)
		{
			ValidSounds.Add(Bank[i].AudioCue);
		}
	}

	int32 RandomSelection = FMath::RandRange(0, ValidSounds.Num() - 1);

	if (ValidSounds.IsValidIndex(RandomSelection))
	{
		return ValidSounds[RandomSelection];
	}

	return NULL;

}

bool UDialogComponent::PlayDialogLine(USoundCue* DialogLineAudio)
{
	if (AudioComponent)
	{
		if (!AudioComponent->IsPlaying())
		{
			AudioComponent->Sound = DialogLineAudio;
			AudioComponent->Play(0.f, ReportSoundRange, bReportSoundEnable, bDebugDraw);
			bIsCurrentlyTalking = true;
			Cast<AEnemyAIController>(GetOwner()->GetInstigatorController())->bIsTalking = bIsCurrentlyTalking;
			return true;
		}
	}

	return false;
}

void UDialogComponent::NotifyLineIsFinishedPlaying()
{
	bIsCurrentlyTalking = false;
	Cast<AEnemyAIController>(GetOwner()->GetInstigatorController())->bIsTalking = bIsCurrentlyTalking;
}