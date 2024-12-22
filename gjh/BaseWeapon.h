// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class PALUWORLD_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyComponent")
	UStaticMeshComponent* RootComp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MyComponent")
	USkeletalMeshComponent* MainMesh;
	
	virtual void ReceiveActorBeginOverlap(AActor* OtherActor) ;

};
