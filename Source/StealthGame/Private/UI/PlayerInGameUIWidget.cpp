// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerInGameUIWidget.h"


void UPlayerInGameUIWidget::NativeConstruct()
{
	//Super::NativeConstruct();

	CurrentEyeState = EEyeState::ClosedEye;
	SearchEyeImage->SetVisibility(ESlateVisibility::Hidden);
	OpenEyeImage->SetVisibility(ESlateVisibility::Hidden);
	ClosedEyeImage->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerInGameUIWidget::UpdateEyeStateWidget(EEyeState EyeState)
{
	CurrentEyeState = EyeState;

	if (CurrentEyeState == EEyeState::ClosedEye)
	{
		SearchEyeImage->SetVisibility(ESlateVisibility::Hidden);
		OpenEyeImage->SetVisibility(ESlateVisibility::Hidden);
		ClosedEyeImage->SetVisibility(ESlateVisibility::Visible);
		
	}
	else if (CurrentEyeState == EEyeState::SearchEye)
	{
		ClosedEyeImage->SetVisibility(ESlateVisibility::Hidden);
		OpenEyeImage->SetVisibility(ESlateVisibility::Hidden);
		SearchEyeImage->SetVisibility(ESlateVisibility::Visible);
	}
	else if (CurrentEyeState == EEyeState::OpenEye)
	{
		ClosedEyeImage->SetVisibility(ESlateVisibility::Hidden);
		SearchEyeImage->SetVisibility(ESlateVisibility::Hidden);
		OpenEyeImage->SetVisibility(ESlateVisibility::Visible);
	}

}