// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/FinishLine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AFinishLine::AFinishLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFinishLine::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	if (TriggerBox == nullptr)
	{
		TriggerBox = FindComponentByClass<UBoxComponent>();
	}
}

// Called every frame
void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void AFinishLine::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor == PlayerCharacter)
	{
		PlayerEntersFinishLine.Broadcast();
	}
}