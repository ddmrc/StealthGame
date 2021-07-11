// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"




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
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	UpdateMovementState();


}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemyCharacter::UpdateMovementState()
{
	AEnemyAIController* ThisController = Cast<AEnemyAIController>(GetController());
	if (ThisController->CurrentAIState == EAIStates::Idle && CurrentMovementState != EMovementState::Idle)
	{
		CurrentMovementState = EMovementState::Idle;
	}
	else if ((ThisController->CurrentAIState == EAIStates::Patrol || ThisController->CurrentAIState == EAIStates::Searching) && CurrentMovementState != EMovementState::Walking)
	{
		CurrentMovementState = EMovementState::Walking;
	}
	else if (ThisController->CurrentAIState == EAIStates::Chasing && CurrentMovementState != EMovementState::Running)
	{
		CurrentMovementState = EMovementState::Running;
	}
	else if ((ThisController->CurrentAIState == EAIStates::Detected || ThisController->CurrentAIState == EAIStates::Confused) && CurrentMovementState != EMovementState::Stopped)
	{
		CurrentMovementState = EMovementState::Stopped;
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