// Fill out your copyright notice in the Description page of Project Settings.


#include "AIComponents/Navigation/SpawnAITargetLocationHandler.h"

// Sets default values
ASpawnAITargetLocationHandler::ASpawnAITargetLocationHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/StealhGame/Blueprints/AI/MoToLocation_BP.MoToLocation_BP'"));
	if (ItemBlueprint.Object)
	{

		MyItemBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void ASpawnAITargetLocationHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnAITargetLocationHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASpawnAITargetLocationHandler::SpawnRandomSearchPoints(FVector CentralPoint, float Radius, int32 SurrondingNumberOfSpawnLocations)
{

	ToggleAllPatrolMoveToPoints(false);

	const FVector SpawnCentralPoint = CentralPoint;
	const FRotator Rotation = FRotator(0, 0, 0);

	if (MyItemBlueprint)
	{
		UE_LOG(LogTemp, Warning, TEXT("HELLO"));
		AActor* NewActor = GetWorld()->SpawnActor(MyItemBlueprint, &SpawnCentralPoint, &Rotation);




		if (SurrondingNumberOfSpawnLocations > 0)
		{
			for (int i = 0; i < SurrondingNumberOfSpawnLocations; i++)
			{
				float LocationX = FMath::RandRange(0.f, Radius);
				float LocationY = FMath::RandRange(0.f, Radius);


				const FVector SpawnLocation = FVector(SpawnCentralPoint.X + LocationX, SpawnCentralPoint.Y + LocationY, SpawnCentralPoint.Z);

				NewActor = GetWorld()->SpawnActor(MyItemBlueprint, &SpawnLocation, &Rotation);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Class Not Detected to Spawn"));
	}


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMoveToLocationActor::StaticClass(), WroldSearchMovingToList);

	TArray <int32> IndexToRemove;

	for (int i = 0; i < WroldSearchMovingToList.Num(); i++)
	{
		if (WroldSearchMovingToList.IsValidIndex(i) && WroldSearchMovingToList[i]->ActorHasTag(FName("Desactivated")))
		{
			IndexToRemove.Add(i);

			WroldSearchMovingToList.RemoveAt(i,1, false);
			
		}
	}
	for (int i = 0; i < IndexToRemove.Num();i++)
	{
		WroldSearchMovingToList.RemoveAt(IndexToRemove[i], 1, true);
	}

}

void ASpawnAITargetLocationHandler::RemoveRandomSearchPoints()
{
	for (int i = 0; i < WroldSearchMovingToList.Num() - 1; i++)
	{
		if (WroldSearchMovingToList.IsValidIndex(i))
		{
			WroldSearchMovingToList[i]->Destroy();

		}
	}

	ToggleAllPatrolMoveToPoints(true);
}
void ASpawnAITargetLocationHandler::ToggleAllPatrolMoveToPoints(bool bAreActive)
{

	if (!WroldPatrolMovingToList.IsValidIndex(0))
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMoveToLocationActor::StaticClass(), WroldPatrolMovingToList);
	}

	

	if (bAreActive == false)
	{
		for (int i = 0; i < WroldPatrolMovingToList.Num(); i++)
		{
			if (WroldPatrolMovingToList.IsValidIndex(i))
			{
				WroldPatrolMovingToList[i]->SetActorTickEnabled(false);
				WroldPatrolMovingToList[i]->SetActorEnableCollision(false);
				WroldPatrolMovingToList[i]->Tags.Add(FName("Deactivated"));
			}
		}
	}
	else
	{
		for (int i = 0; i < WroldPatrolMovingToList.Num(); i++)
		{
			if (WroldPatrolMovingToList.IsValidIndex(i))
			{
				WroldPatrolMovingToList[i]->SetActorTickEnabled(true);
				WroldPatrolMovingToList[i]->SetActorEnableCollision(true);
				WroldPatrolMovingToList[i]->Tags.Add(FName("Activated"));
			}
		}
	}
}
