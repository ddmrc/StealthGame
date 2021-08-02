// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player/MyPlayerCharacter.h"
#include "MyPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class STEALTHGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();


	//VARIABLES
	AMyPlayerCharacter* PlayerCharacter = nullptr;

	//Walking & Sprint Speeds


protected:

	

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void RequestMoveForward(float AxisVal);
	void RequestMoveSideways(float AxisVal);
	void RequestLookUp(float AxisVal);
	void RequestLookSide(float AxisVal);

	void RequestCrouch();
	void RequestSprint();
	void RequestSprintEnd();

	

};
