// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIComponents/Navigation/MoveToLocationActor.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnAITargetLocationHandler.generated.h"

UCLASS()
class STEALTHGAME_API ASpawnAITargetLocationHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnAITargetLocationHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnRandomSearchPoints(FVector CentralPoint, float Radius, int32 SurrondingNumberOfSpawnLocations);

	void RemoveRandomSearchPoints();

	void ToggleAllPatrolMoveToPoints(bool bAreActive);

	void SpawnConversationLocationPoint(FVector CentralPoint);


protected:
	TArray<AActor*> WroldPatrolMovingToList;
	TArray<AActor*> WroldSearchMovingToList;

	TSubclassOf<AMoveToLocationActor> MyItemBlueprint;

};

