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
	//����һ����̬������ͨ�����߼������ȡ��Ŀ����³
	static ABasePalCharacter* GetPalByRayCast(FVector StartLocation, FVector Direction, float MaxDistance) {
		UWorld* world = GEngine->GetWorldFromContextObject(nullptr, EGetWorldErrorMode::LogAndReturnNull);
		//�ж������Ƿ����
		if (!world) {
			return nullptr;
		}
		//�������߼�������н��
		FHitResult HitResult;
		//�������߼��
		bool bHit = world->LineTraceSingleByChannel(HitResult, StartLocation, StartLocation + Direction * MaxDistance, ECC_Pawn);
		//�ж��Ƿ���ɹ�
		if (!bHit) {
			return nullptr;
		}
		//��ȡĿ����
		AActor* HitActor = HitResult.GetActor();
		//������е�����Ϊʲô��
		ABasePalCharacter* Pal = Cast<ABasePalCharacter>(HitActor);
		//�ж��Ƿ���ΪBasePalCharacter��
		if(Pal){
			#ifdef WITH_EDITOR
			DrawDebugLine(world, StartLocation, HitResult.Location, FColor::Green, false, 0.5f, 0, 5.f);
			#endif // WITH_EDITOR
			return Pal;
		}
		//δ��⵽
		return nullptr;
	}

}