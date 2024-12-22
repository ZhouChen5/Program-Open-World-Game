// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "ChopInterface.h"
#include "MeleeWeapon.h"
#include "Components/PrimitiveComponent.h"
#include <Components/BoxComponent.h>
#include "toos_axe.generated.h"



/**
 * 
 */
UCLASS()
class PALUWORLD_API Atoos_axe : public AMeleeWeapon,public IChopInterface
{
	GENERATED_BODY()

public:
	Atoos_axe();

	// 这个是骨骼网格体组件
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	USkeletalMeshComponent* AxeMeshComponent;

	// Box 盒体碰撞组件
	UPROPERTY(VisibleAnywhere, blueprintReadWrite)
	UBoxComponent* BoxCollisionComponent;

	// Damage 盒体碰撞组件
	UPROPERTY(VisibleAnywhere, blueprintReadWrite)
	UBoxComponent* DamageCollision;

	UPROPERTY(VisibleAnywhere, blueprintReadWrite)
	class APlayerBaseCharacter* weaponowner;


	//与接口联系
	virtual void Chop_Implementation(float damage, FVector hitlocation) override;

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void EnableHit(bool enable);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float getdamage;
};
