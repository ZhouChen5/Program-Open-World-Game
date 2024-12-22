// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "BasePalCharacter.h"

/**
 * 
 */
class PALWORLD_API RayCast
{
public:
	RayCast();
	~RayCast();
};


namespace RayCastDetection {
	UFUNCTION(BlueprintCallable, Category = "RayCast")
	//创建一个静态函数来通过射线检测来获取到目标帕鲁
	static ABasePalCharacter* GetPalByRayCast(FVector StartLocation, FVector Direction, float MaxDistance) {
		UWorld* world = GEngine->GetWorldFromContextObject(nullptr, EGetWorldErrorMode::LogAndReturnNull);
		//判断世界是否存在
		if (!world) {
			return nullptr;
		}
		//定义射线检测结果命中结果
		FHitResult HitResult;
		//进行射线检测
		bool bHit = world->LineTraceSingleByChannel(HitResult, StartLocation, StartLocation + Direction * MaxDistance, ECC_Pawn);
		//判断是否检测成功
		if (!bHit) {
			return nullptr;
		}
		//获取目标体
		AActor* HitActor = HitResult.GetActor();
		//检测命中的物体为什么类
		ABasePalCharacter* Pal = Cast<ABasePalCharacter>(HitActor);
		//判断是否检测为BasePalCharacter类
		if(Pal){
			#ifdef WITH_EDITOR
			DrawDebugLine(world, StartLocation, HitResult.Location, FColor::Green, false, 0.5f, 0, 5.f);
			#endif // WITH_EDITOR
			return Pal;
		}
		//未检测到
		return nullptr;
	}

}