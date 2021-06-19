// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FindComponentByClass<UPawnSensingComponent>()->CouldSeePawn(PlayerCharacter))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Red, TEXT("Player Detected!"));
		}

	}
	else
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Yellow, TEXT("Player Hidden!"));
		}




}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

