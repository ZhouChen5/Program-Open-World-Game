// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ChopInterface.generated.h"




// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChopInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class PALUWORLD_API IChopInterface
{
    GENERATED_BODY()

public:
    // 使用 BlueprintNativeEvent 来声明接口函数
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Chop")
    void Chop(float damage,FVector hitlocation);
    virtual void Chop_Implementation(float damage, FVector hitlocation) = 0;


};
