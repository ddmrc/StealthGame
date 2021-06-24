// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "MyPlayerCharacter.generated.h"

UCLASS()
class STEALTHGAME_API AMyPlayerCharacter : public ACharacter, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//Called to determine where AI can detect player
	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation,
		int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor) const override;

	void MoveForward(float AxisVal);
	void MoveSideways(float AxisVal);
	void LookUp(float AxisVal);
	void LookSide(float AxisVal);

	void PlayerCrouch();
	void PlayerUnCrouch();




	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
		float BaseLookUpRate = 90.0f;

	/** Base lookright rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(EditAnywhere, Category = "Look")
		float BaseLookRightRate = 90.0f;

	float FrontAxisValue = 0.0f;
	float SideAxisValue = 0.0f;
};
