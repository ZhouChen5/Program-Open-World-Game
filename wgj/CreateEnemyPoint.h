// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateEnemyPoint.generated.h"

UCLASS()
class PALUWORLD_API ACreateEnemyPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreateEnemyPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="IsBoss")
	bool IsBoss = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
