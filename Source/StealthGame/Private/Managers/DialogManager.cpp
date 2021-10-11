// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/DialogManager.h"
#include "Kismet/GameplayStatics.h"

static int32 GDialogOption = 0;

static FAutoConsoleVariableRef CVarShowSoundEvents(
	TEXT("Stealth.Debug.ConversationEvents"),
	GDialogOption,
	TEXT("0 = stop, 1 = Greeting, 2 = Inquire, 3 = Goodbye"),
	ECVF_Cheat
);


// Sets default values
ADialogManager::ADialogManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADialogManager::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyController == nullptr)
	{
		TArray<AActor*> TempArr;
		UGameplayStatics::GetAllActorsOfClass(this, AEnemyAIController::StaticClass(), TempArr);
		if (TempArr.IsValidIndex(0))
		{
			EnemyController = Cast<AEnemyAIController>(TempArr[0]);
		}

		GDialogOption = 0;
	}



	
}

// Called every frame
void ADialogManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DebugDialog();

}

void ADialogManager::DebugDialog()
{

	
	UDialogComponent* DialogComponent = EnemyController->GetDialogComponent();
	

	USoundCue* Line1 = DialogComponent->GetRandSoundBasedOnTag(DialogComponent->DefaultBank, "Greeting");
	USoundCue* Line2 = DialogComponent->GetRandSoundBasedOnTag(DialogComponent->DefaultBank, "Inquire");
	USoundCue* Line3 = DialogComponent->GetRandSoundBasedOnTag(DialogComponent->DefaultBank, "Goodbye");
	switch (GDialogOption)
	{
	case 0:
		break;
	case 1:
		if (DialogComponent)
		{
			DialogComponent->PlayDialogLine(Line1);
			UE_LOG(LogTemp, Warning, TEXT("PLAYING Dialog"));
			GDialogOption = 0;
		}
		break;
	case 2:
		if (DialogComponent)
		{
			DialogComponent->PlayDialogLine(Line2);
			UE_LOG(LogTemp, Warning, TEXT("PLAYING Dialog"));
			GDialogOption = 0;
		}
		break;
	case 3:
		if (DialogComponent)
		{
			DialogComponent->PlayDialogLine(Line3);
			UE_LOG(LogTemp, Warning, TEXT("PLAYING Dialog"));
			GDialogOption = 0;
		}
		break;
	}
}