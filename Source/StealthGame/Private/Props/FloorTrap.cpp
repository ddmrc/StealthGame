// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/FloorTrap.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/Character.h"
#include "CustomSound/CustomSoundWrapper.h"

// Sets default values
AFloorTrap::AFloorTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorTrap::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerActor == nullptr)
	{
		PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	}

	bIsActive = true;
	
	
}

// Called every frame
void AFloorTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorTrap::NotifyActorBeginOverlap(AActor* OtherActor)
{

	Super::NotifyActorBeginOverlap(OtherActor);


	if (SoundEffect->IsPlayable()&& PlayerActor && OtherActor == PlayerActor && bIsActive)
	{
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation());
		//UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.f, OtherActor, MaxSoundDistanceDetection);
		
		CustomSoundWrapper::PlaySoundAtLocation(GetWorld(), SoundEffect, GetActorLocation(), SoundVolumeMultiplier, OtherActor, MaxSoundDistanceDetection);
		bIsActive = false;
	}

}

