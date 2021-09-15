// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Conversation/CustomConversationGenerator.h"
#include "CustomSound/CustomSoundWrapper.h"


// Sets default values for this component's properties
UCustomConversationGenerator::UCustomConversationGenerator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCustomConversationGenerator::BeginPlay()
{
	Super::BeginPlay();

	

	
	if (AudioPlayer == nullptr)
	{
		AudioPlayer = GetOwner()->FindComponentByClass<UCustomAudioComponent>();

	}


	if (AudioPlayer)
	{
		AudioPlayer->OnAudioFinished.AddDynamic(this, &UCustomConversationGenerator::WaitForAnswer);
	}

	ToggleHasFinishedSpeaking.BindUFunction(this, FName("ToggleHasFinishedTalkingToTrue"));
	
}

// Called every frame
void UCustomConversationGenerator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCustomConversationGenerator::LoadConversation(FString Category)
{
	Conversation.Empty();

	USoundCue* Stitch1 = ReturnSoundCue(Category, 1);
	USoundCue* Stitch2 = ReturnSoundCue(Category, 2);
	USoundCue* Stitch3 = ReturnSoundCue(Category, 3);

	Conversation.Add(Stitch1);
	Conversation.Add(Stitch2);
	Conversation.Add(Stitch3);

}

USoundCue* UCustomConversationGenerator::ReturnSoundCue(FString Category, int32 SubCategoryNumber)
{

	if (Category == "A")
	{
		switch (SubCategoryNumber)
		{
		case 1:
			if (A1.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, A1.Num()-1);
				if (A1.IsValidIndex(RandomChoiceIndex))
				{
					return A1[RandomChoiceIndex];
				}
			}
			break;

		case 2:
			if (A2.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, A2.Num()-1);
				if (A2.IsValidIndex(RandomChoiceIndex))
				{
					return A2[RandomChoiceIndex];
				}
			}
			break;

		case 3:
			if (A3.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, A3.Num()-1);
				if (A3.IsValidIndex(RandomChoiceIndex))
				{
					return A3[RandomChoiceIndex];
				}
			}
			break;

		default:
			return nullptr;
		}
	}

	else if (Category == "B")
	{
		switch (SubCategoryNumber)
		{
		case 1:
			if (B1.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, B1.Num()-1);
				if (B1.IsValidIndex(RandomChoiceIndex))
				{
					return B1[RandomChoiceIndex];
				}
			}
			break;

		case 2:
			if (B2.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, B2.Num()-1);
				if (B2.IsValidIndex(RandomChoiceIndex))
				{
					return B2[RandomChoiceIndex];
				}
			}
			break;

		case 3:
			if (B3.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, B3.Num()-1);
				if (B3.IsValidIndex(RandomChoiceIndex))
				{
					return B3[RandomChoiceIndex];
				}
			}
			break;

		default:
			return nullptr;
		}
	}

	else if (Category == "C")
	{
		switch (SubCategoryNumber)
		{
		case 1:
			if (C1.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, C1.Num()-1);
				if (C1.IsValidIndex(RandomChoiceIndex))
				{
					return C1[RandomChoiceIndex];
				}
			}
			break;

		case 2:
			if (C2.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, C2.Num()-1);
				if (C2.IsValidIndex(RandomChoiceIndex))
				{
					return C2[RandomChoiceIndex];
				}
			}
			break;

		case 3:
			if (C3.Num() > 0)
			{
				int32 RandomChoiceIndex = FMath::RandRange(0, C3.Num()-1);
				if (C3.IsValidIndex(RandomChoiceIndex))
				{
					return C3[RandomChoiceIndex];
				}
			}
			break;

		default:
			return nullptr;
		}
	}

	return nullptr;
	
	
}

void UCustomConversationGenerator::WaitForAnswer()
{
	if (AudioPlayer->Sound != Silence)
	{
		AudioPlayer->SetSound(Silence);
		AudioPlayer->Play();
		FTimerHandle HasFinishedSpeaking;
		GetWorld()->GetTimerManager().ClearTimer(HasFinishedSpeaking);
		GetWorld()->GetTimerManager().SetTimer(HasFinishedSpeaking, ToggleHasFinishedSpeaking, AudioPlayer->Sound->Duration, false);
	}
	

}

void UCustomConversationGenerator::ToggleHasFinishedTalkingToTrue()
{
	
	bHasFinishedTalking = true;


}

void UCustomConversationGenerator::ToggleThroughPhrases()
{
	if (AudioPlayer && !AudioPlayer->IsPlaying())
	{
		if (Conversation.IsValidIndex(0) && Conversation[0] != nullptr)
		{
			AudioPlayer->SetSound(Conversation[0]);
			Conversation[0] = nullptr;


		}
		else if (Conversation.IsValidIndex(1) && Conversation[1] != nullptr)
		{
			AudioPlayer->SetSound(Conversation[1]);
			Conversation[1] = nullptr;

		}
		else if (Conversation.IsValidIndex(2) && Conversation[2] != nullptr)
		{
			AudioPlayer->SetSound(Conversation[2]);
			Conversation[2] = nullptr;
			

		}



		if (AudioPlayer->Sound && AudioPlayer->Sound->IsPlayable() && !AudioPlayer->IsPlaying())//PLAY AUDIO
		{
			bHasFinishedTalking = false;
			AudioPlayer->Play(0.0f, DialogSoundIntensity, true, true);

			

		}


	}
}