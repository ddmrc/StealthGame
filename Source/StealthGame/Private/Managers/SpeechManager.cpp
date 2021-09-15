// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SpeechManager.h"

// Sets default values
ASpeechManager::ASpeechManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpeechManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpeechManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// IDEA HAVE A BACTIVE BOOL THAT TRIGGERS WITH FIRST INSTANTIATION 
	//THAT WILL ENSURE IT DOESN'T TRIGGER TWICE 
	//CAN EVEN HAVE STORE THE LAST ALLY
	//
	//IDEA 2: CREATE AN INSTANCE OF THIS ACTOR
	//POPULATE IT
	//USE IT
	//DESTROY IT
	//THIS ENSURES ONE ACTOR PER CONVERSATION


	

}
