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
	//���캯������
	UMyUse();

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


	//�Ƿ������ײ
	UFUNCTION(BlueprintCallable, Category = "Function")
	void EnableHit(bool enable);


	//�˺�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "health")
	float getdamage;
};
