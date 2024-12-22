// Fill out your copyright notice in the Description page of Project Settings.


#include "DayNightController.h"

// Sets default values
ADayNightController::ADayNightController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADayNightController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADayNightController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

