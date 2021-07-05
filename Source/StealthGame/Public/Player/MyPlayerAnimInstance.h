// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Player/MyPlayerCharacter.h"
#include "MyPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UMyPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	AMyPlayerCharacter* PlayerCharacter = nullptr;

		float m_SideAxisValue = 50.f;
		float m_FrontAxisValue = 50.f;

	virtual void NativeBeginPlay()override;


	UFUNCTION(BlueprintCallable)
	bool IsPlayerCrouching();
	UFUNCTION(BlueprintCallable)
	bool IsPlayerMoving();
	UFUNCTION(BlueprintCallable)
	bool IsPlayerSprinting();
	UFUNCTION(BlueprintCallable)
	float GetFrontAxisValue();	
	UFUNCTION(BlueprintCallable)
	float GetSideAxisValue();

	
};
