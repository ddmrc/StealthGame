// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/PlayerInGameUIWidget.h"
#include "Enemy/EnemyCharacter.h"
#include "MyCustomHUD.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API AMyCustomHUD : public AHUD
{
	GENERATED_BODY()

public:

	AMyCustomHUD();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void ReadPlayerStealthState();

	UPROPERTY(EditAnywhere, Category = "Widgets")
		TSubclassOf<UUserWidget> PlayerInGameUI;

	UPlayerInGameUIWidget* PlayerInGameUIWidget;

	AEnemyCharacter* AICharacter = nullptr;


};
