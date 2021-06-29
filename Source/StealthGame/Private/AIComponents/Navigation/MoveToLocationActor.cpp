// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Navigation/MoveToLocationActor.h"

// Sets default values
AMoveToLocationActor::AMoveToLocationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMoveToLocationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoveToLocationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

