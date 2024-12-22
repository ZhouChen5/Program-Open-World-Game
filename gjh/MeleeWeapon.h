// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite, Category = "AnimValue")
	int hurtvalue=30;
	
};
