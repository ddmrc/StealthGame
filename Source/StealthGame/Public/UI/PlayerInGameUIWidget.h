// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "PlayerInGameUIWidget.generated.h"

/**
 * 
 */
UENUM()
enum class EEyeState : uint8
{
	ClosedEye,
	SearchEye,
	OpenEye,

};

UCLASS()
class STEALTHGAME_API UPlayerInGameUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	virtual void NativeConstruct() override;
	void UpdateEyeStateWidget(EEyeState EyeState);


protected:

	EEyeState CurrentEyeState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* ClosedEyeImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* SearchEyeImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* OpenEyeImage;
};
