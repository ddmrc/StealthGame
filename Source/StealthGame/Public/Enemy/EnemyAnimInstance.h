// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/EnemyCharacter.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:


	AEnemyCharacter* ThisCharacter = nullptr;

	bool IsWalking = false;
	bool IsMoving = false;

	virtual void NativeBeginPlay()override;


	EAIStates GetCurrentAIState();

	UFUNCTION(BlueprintCallable)
	bool IsAIWalking();
	UFUNCTION(BlueprintCallable)
	bool IsAIMoving();
	UFUNCTION(BlueprintCallable)
	bool IsAIRunning();
	UFUNCTION(BlueprintCallable)
	bool IsPlayerDetected();
	UFUNCTION(BlueprintCallable)
	bool IsAIConfused();
};
