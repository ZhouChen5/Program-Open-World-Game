// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Soldier1Character.h"
#include "FarSoldierCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PALUWORLD_API AFarSoldierCharacter : public ASoldier1Character
{
	GENERATED_BODY()
public:
	virtual void AttackDamage() override;
};
