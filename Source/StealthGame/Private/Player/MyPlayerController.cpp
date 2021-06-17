// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "Kismet/GameplayStatics.h"

AMyPlayerController::AMyPlayerController()
{

}



void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast< AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		/*InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayerController::RequestJump);
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AMyPlayerController::RequestStopJump);


		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyPlayerController::RequestSprintStart);
		InputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyPlayerController::RequestSprintEnd);*/

		InputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerController::RequestMoveForward);
		InputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerController::RequestMoveSideways);
		InputComponent->BindAxis(TEXT("LookUp"), this, &AMyPlayerController::RequestLookUp);
		InputComponent->BindAxis(TEXT("LookRight"), this, &AMyPlayerController::RequestLookSide);

		InputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AMyPlayerController::RequestCrouch);
		
	}
}


void AMyPlayerController::RequestMoveForward(float AxisVal)
{
	PlayerCharacter->MoveForward(AxisVal);
	
}


void AMyPlayerController::RequestMoveSideways(float AxisVal)
{
	PlayerCharacter->MoveSideways(AxisVal);
}

void AMyPlayerController::RequestLookUp(float AxisVal)
{
	PlayerCharacter->LookUp(AxisVal);
}

void AMyPlayerController::RequestLookSide(float AxisVal)
{
	PlayerCharacter->LookSide(AxisVal);
}

void AMyPlayerController::RequestCrouch()
{

	if (!PlayerCharacter->bIsCrouched)
	{
		PlayerCharacter->PlayerCrouch();
	}
	else
	{
		PlayerCharacter->PlayerUnCrouch();
	}
}


