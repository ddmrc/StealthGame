// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

void AEnemyAIController::BeginPlay()
{

	Super::BeginPlay();


	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMyPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	}


	if (UAIPerceptionComponent* ThisPerceptionComponent = GetPerceptionComponent())
	{
		ThisPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);

	}

}



void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{

		CurrentAIState = EAIStates::Detected;
		GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName("CurrentState"), (uint8)CurrentAIState);
		//uint8 ThisCurrentState = GetBlackboardComponent()->GetValue<UBlackboardKeyType_Enum>("CurrentState");
		

			
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Player Detected"));
			
		}
	}
	else
	{

		CurrentAIState = EAIStates::Idle;
		GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(FName("CurrentState"), (uint8)CurrentAIState);


		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Undetected"));
		}
	}
	
}

