// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "FloorTrap.generated.h"

UCLASS()
class STEALTHGAME_API AFloorTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTrap();

	UPROPERTY(EditAnywhere)
	USoundCue* SoundEffect;

	UPROPERTY(EditAnywhere)
	float MaxSoundDistanceDetection = 1000.f;
	UPROPERTY(EditAnywhere)
	float SoundVolumeMultiplier = 1.f;
protected:



	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ACharacter* PlayerActor = nullptr;

	bool bIsActive = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
