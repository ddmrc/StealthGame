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

		AActor* NewActor = GetWorld()->SpawnActor(MyItemBlueprint, &SpawnCentralPoint, &Rotation);

		
		WroldSearchMovingToList.Add(NewActor);

		if (SurrondingNumberOfSpawnLocations > 0)
		{
			for (int i = 1; i < SurrondingNumberOfSpawnLocations+1; i++)
			{

				float LocationX = 0.f;
				float LocationY = 0.f;

				if (i > 1)
				{
					LocationX = FMath::RandRange(5.f, Radius);
					LocationY = FMath::RandRange(5.f, Radius);

				}

				const FVector SpawnLocation = FVector(SpawnCentralPoint.X + LocationX, SpawnCentralPoint.Y + LocationY, SpawnCentralPoint.Z);

				NewActor = GetWorld()->SpawnActor(MyItemBlueprint, &SpawnLocation, &Rotation);
				WroldSearchMovingToList.Add(NewActor);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Class Not Detected to Spawn"));
	}


	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AMoveToLocationActor::StaticClass(),FName("Activated"), WroldSearchMovingToList);


}

void ASpawnAITargetLocationHandler::RemoveRandomSearchPoints()
{

	if (WroldSearchMovingToList.Num() > 0)
	{
		for (int i = 0; i < WroldSearchMovingToList.Num(); i++)
		{
			if (WroldSearchMovingToList.IsValidIndex(i))
			{
				WroldSearchMovingToList[i]->Destroy();

			}
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
				WroldPatrolMovingToList[i]->Tags.Remove(FName("Activated"));
				WroldPatrolMovingToList[i]->Tags.Add(FName("Deactivated"));

				if (WroldPatrolMovingToList[i]->Tags.IsValidIndex(1))
				{
					WroldPatrolMovingToList[i]->Tags.RemoveAt(1);
				}
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
				WroldPatrolMovingToList[i]->Tags.Remove(FName("Deactivated"));
				WroldPatrolMovingToList[i]->Tags.Add(FName("Activated"));

				if (WroldPatrolMovingToList[i]->Tags.IsValidIndex(1))
				{
					WroldPatrolMovingToList[i]->Tags.RemoveAt(1);
				}
			}
		}
	}


}
