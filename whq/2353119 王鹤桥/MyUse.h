// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "ChopInterface.h"
#include "Components/PrimitiveComponent.h"
#include <Components/BoxComponent.h>
#include "MyUse.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API UMyUse : public UStaticMesh,public IChopInterface
{
	GENERATED_BODY()
	
public:
	//构造函数创建
	UMyUse();

	// 这个是网格体组件
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	UStaticMeshComponent* AxeMeshComponent;

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


	//是否可以碰撞
	UFUNCTION(BlueprintCallable, Category = "Function")
	void EnableHit(bool enable);


	//伤害检验
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float getdamage;
};
