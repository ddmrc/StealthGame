// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyAIController.h"
#include "GenericTeamAgentInterface.h"
#include "EnemyCharacter.generated.h"

UENUM()
enum class EMovementState : uint8
{
	Idle,
	Walking,
	Stopped,
	Running,
	Crouching,
	StaticSearch,

};


UCLASS()
class STEALTHGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	

	//Updates Movement State According to AI State
	void UpdateMovementState();

	EMovementState GetMovementState();

	EAIStates GetAICurrentState();

	void UpdateForceRun(bool bForce);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EMovementState CurrentMovementState;

	bool bForceRun = false;

	//virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID)override { TeamID = NewTeamID; }
	



};
