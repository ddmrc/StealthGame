// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Props/FinishLine.h"
#include "Managers/SpeechManager.h"
#include "StealthGameModeBase.generated.h"

/**
 * 
 */

UENUM()
enum class EGamePlayState
{
	EPlaying,
	EPlayerLost,
	EPlayerWon,
};
UCLASS()
class STEALTHGAME_API AStealthGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	AFinishLine* FinishLine = nullptr;
	ASpeechManager* SpeechManager = nullptr;

protected:

	void SetNewState(EGamePlayState NewState);
	UFUNCTION()
	void PlayerWinsGame();



};
