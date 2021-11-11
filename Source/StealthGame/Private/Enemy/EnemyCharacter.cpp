// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"




// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentMovementState = EMovementState::Idle;
	//SetGenericTeamId(2);

	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ThisController == nullptr)
	{
		ThisController = Cast<AEnemyAIController>(GetController());
		CurrentMovementState = EMovementState::Idle;
	}
	UpdateMovementState();

	/*FString NM;
	GetName(NM);
	UE_LOG(LogTemp, Warning, TEXT("This Character: %s"), *NM);*/
	
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemyCharacter::UpdateMovementState()
{
	
	
	//if (ThisController->CurrentAIState == EAIStates::Searching && GetVelocity() == FVector::ZeroVector)
	//{
	//	
	//	CurrentMovementState = EMovementState::StaticSearch;
	//	return;
	//}
	if (ThisController)
	{
		
		if (ThisController->CurrentAIState == EAIStates::Idle && CurrentMovementState != EMovementState::Idle)
		{
			

			if (GetVelocity() == FVector::ZeroVector)
			{
				CurrentMovementState = EMovementState::Idle;
			}
			else
			{
				
				CurrentMovementState = EMovementState::Walking;
			}
			

			
		}
		else if (ThisController->CurrentAIState == EAIStates::Patrol && CurrentMovementState != EMovementState::Walking)
		{

			CurrentMovementState = EMovementState::Walking;


		}
		else if (ThisController->CurrentAIState == EAIStates::Searching)
		{
			if (GetVelocity() == FVector::ZeroVector)
			{
				CurrentMovementState = EMovementState::StaticSearch;
				return;
			}
			else if (bForceRun)
			{
				//Nothing, keep Running
				CurrentMovementState = EMovementState::Running;
				return;
			}
			else if (CurrentMovementState == EMovementState::Walking && !bForceRun)
			{
				//Nothing, keep Walking
				CurrentMovementState = EMovementState::Walking;
			}

			else
			{
				//Emergency change state to walking
				CurrentMovementState = EMovementState::Walking;
			}
		}
		else if (ThisController->CurrentAIState == EAIStates::Chasing && CurrentMovementState != EMovementState::Running)
		{
			CurrentMovementState = EMovementState::Running;
			bForceRun = true;
		}
		else if ((ThisController->CurrentAIState == EAIStates::Detected || ThisController->CurrentAIState == EAIStates::Confused) && CurrentMovementState != EMovementState::Stopped)
		{
			CurrentMovementState = EMovementState::Stopped;
			if (ThisController->CurrentAIState == EAIStates::Confused)
			{
				bForceRun = false;
			}
		}
		else if (ThisController->CurrentAIState == EAIStates::LookingAround)
		{
			CurrentMovementState = EMovementState::StaticSearch;
		}
		else if (ThisController->ConversationIdentifier != "PatrolGuard1" && ThisController->CurrentAIState == EAIStates::Conversation)
		{
			

			if (!ThisController->bHasReachedConversationLocation)
			{
				
				CurrentMovementState = EMovementState::Walking;
			}

		}

	}

}

EMovementState AEnemyCharacter::GetMovementState()
{

	return CurrentMovementState;
}

EAIStates AEnemyCharacter::GetAICurrentState()
{
	if (Cast<AEnemyAIController>(GetController()))
	{
		return Cast<AEnemyAIController>(GetController())->RequestGetAIState();
	}
	else
	{
		return EAIStates::Idle;
	}
}

void AEnemyCharacter::UpdateForceRun(bool bForce)
{
	//Used in Search Task to stop running when arrive
	//to location
	bForceRun = bForce;
};

bool AEnemyCharacter::GetIsCharacterTalking()
{
	return ThisController->bIsTalking;
}