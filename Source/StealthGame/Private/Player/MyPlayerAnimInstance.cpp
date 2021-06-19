// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/MyPlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


void UMyPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PlayerCharacter = Cast<AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

bool UMyPlayerAnimInstance::IsPlayerCrouching()
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetMovementComponent()->IsCrouching();
	}
	else
		return false;
}

float UMyPlayerAnimInstance::GetFrontAxisValue()
{
	if (PlayerCharacter)
	{
		
		float Temp_Axis_Val = PlayerCharacter->FrontAxisValue;
		float Adapted_Val = 50.f; //this is the idle crouch

		if (Temp_Axis_Val == 0.0)
		{
			Adapted_Val = 50.f;
		}
		else if (Temp_Axis_Val != 0.0)
		{
			if (Temp_Axis_Val > 0.0)
			{
				//needts to be max 0
				Adapted_Val = 50.f * (Temp_Axis_Val * 2.0f);
			}
			else if (Temp_Axis_Val < 0.0)
			{
				Adapted_Val = 50.f + (Temp_Axis_Val * 50.0f);
			}
		}
		
		
		m_FrontAxisValue = Adapted_Val;

	}
	

	return m_FrontAxisValue;
}

float UMyPlayerAnimInstance::GetSideAxisValue()
{
	if (PlayerCharacter)
	{
		float Temp_Axis_Val = PlayerCharacter->SideAxisValue;
		float Adapted_Val = 50.f; //this is the idle crouch

		if (Temp_Axis_Val == 0.0)
		{
			Adapted_Val = 50.f;
		}
		else if (Temp_Axis_Val != 0.0)
		{
			if (Temp_Axis_Val > 0.0)
			{
				//needts to be max 0
				Adapted_Val = 50.f * (Temp_Axis_Val * 2.0f);
			}
			else if (Temp_Axis_Val < 0.0)
			{
				Adapted_Val = 50.f + (Temp_Axis_Val * 50.0f);
			}
		}

		m_SideAxisValue = Adapted_Val;
	}



	return m_SideAxisValue;
}

