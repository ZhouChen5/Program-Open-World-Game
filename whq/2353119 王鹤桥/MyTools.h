// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "ChopInterface.h"
#include "Components/PrimitiveComponent.h"
#include <Components/BoxComponent.h>
#include "MyTools.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API AMyTools : public AStaticMeshActor, public IChopInterface
{
	GENERATED_BODY()
public:
	AMyTools();

	// ��������������
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	UStaticMeshComponent* AxeMeshComponent;

	// Box ������ײ���
	UPROPERTY(VisibleAnywhere, blueprintReadWrite)
	UBoxComponent* BoxCollisionComponent;

	// Damage ������ײ���
	UPROPERTY(VisibleAnywhere, blueprintReadWrite)
	UBoxComponent* DamageCollision;

	UPROPERTY(VisibleAnywhere, blueprintReadWrite)
	class APlayerBaseCharacter* weaponowner;


	//��ӿ���ϵ
	virtual void Chop_Implementation(float damage, FVector hitlocation) override;

protected:

	//��Ϸ��ʼʱ�Ĵ���
	virtual void BeginPlay() override;

	//�Ƿ������ײ
	UFUNCTION(BlueprintCallable, Category = "Function")
	void EnableHit(bool enable);


	//�˺�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float getdamage;
};
