// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyPlayerCharacter::AMyPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	
	
}

// Called every frame
void AMyPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyPlayerCharacter::MoveForward(float AxisVal)
{
	if (AxisVal != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), AxisVal);
	}

	FrontAxisValue = AxisVal;

}


void AMyPlayerCharacter::MoveSideways(float AxisVal)
{
	if (AxisVal != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), AxisVal);
	}

	SideAxisValue = AxisVal;
}

void AMyPlayerCharacter::LookUp(float AxisVal)
{
	
	UGameplayStatics::GetPlayerController(GetWorld(),0)->AddPitchInput(AxisVal * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyPlayerCharacter::LookSide(float AxisVal)
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->AddYawInput(AxisVal * BaseLookRightRate * GetWorld()->GetDeltaSeconds());
}

void AMyPlayerCharacter::PlayerCrouch()
{

	 Crouch(false);
	 
}
void AMyPlayerCharacter::PlayerUnCrouch()
{
	UnCrouch();
}