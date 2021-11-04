// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();


	if (TryGetPawnOwner() && ThisCharacter == nullptr)
	{
		ThisCharacter = Cast<AEnemyCharacter>(this->TryGetPawnOwner());
	}


}

EAIStates UEnemyAnimInstance::GetCurrentAIState()
{
	if (ThisCharacter)
	{
		return ThisCharacter->GetAICurrentState();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ThisCharacter not found in EnemyAnimInstance.cpp"));
		return EAIStates::Idle;
	}
}

bool UEnemyAnimInstance::IsAIWalking()
{

	if (ThisCharacter)
	{

		if (ThisCharacter->GetMovementState() == EMovementState::Walking)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UEnemyAnimInstance::IsAIMoving()
{
	if (ThisCharacter)
	{
		if (ThisCharacter->GetMovementState() == EMovementState::Walking || ThisCharacter->GetMovementState() == EMovementState::Running || ThisCharacter->GetMovementState() == EMovementState::Crouching)
		{
			return true;
		}

		else if (ThisCharacter->GetMovementState() == EMovementState::Idle || ThisCharacter->GetMovementState() == EMovementState::Stopped)
		{
			return false;
		}
		else
		{
			return false;
		}

	}
	else
	{	
		return false;
	}
	
}

bool UEnemyAnimInstance::IsAIRunning()
{
	if (ThisCharacter)
	{
		if (ThisCharacter->GetMovementState() == EMovementState::Running)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UEnemyAnimInstance::IsPlayerDetected()
{
	if (ThisCharacter)
	{
		if (GetCurrentAIState() == EAIStates::Detected)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool UEnemyAnimInstance::IsAIConfused()
{
	if (ThisCharacter)
	{
		if (GetCurrentAIState() == EAIStates::Confused)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UEnemyAnimInstance::IsAILookingAround()
{
	if (ThisCharacter)
	{
		if (GetCurrentAIState() == EAIStates::LookingAround)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool UEnemyAnimInstance::IsAITalking()
{
	if (ThisCharacter)
	{
		if (ThisCharacter->GetIsCharacterTalking() == true)
		{
			return true;
		}

	}

	return false;
}
bool UEnemyAnimInstance::IsAIHavingConversation()
{
	if (ThisCharacter)
	{
		if (GetCurrentAIState() == EAIStates::Conversation)
		{
			return true;
		}
	}
	return false;
}