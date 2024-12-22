// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateSoldierPoint.h"
#include "CombatGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACreateSoldierPoint::ACreateSoldierPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACreateSoldierPoint::BeginPlay()
{
	Super::BeginPlay();
	
	ACombatGameMode* gameMode = Cast<ACombatGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (gameMode)
	{
		gameMode->AddPointToSoldierList(this);
	}
}

// Called every frame
void ACreateSoldierPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

