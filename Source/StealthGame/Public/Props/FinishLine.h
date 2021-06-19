// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/MyPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "FinishLine.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerEntersFinishLine);
UCLASS()
class STEALTHGAME_API AFinishLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinishLine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AMyPlayerCharacter* PlayerCharacter = nullptr;
	UBoxComponent* TriggerBox = nullptr;

public:	

	FPlayerEntersFinishLine PlayerEntersFinishLine;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
