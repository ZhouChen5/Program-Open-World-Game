// Fill out your copyright notice in the Description page of Project Settings.


#include "FarSoldierCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerBaseCharacter.h"
#include "BaseCharacter.h"

void AFarSoldierCharacter::AttackDamage()
{
	FVector start = GetActorLocation();
	FVector end = start + GetActorForwardVector() * 800;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	TArray<AActor*> ignoreList;

	TArray<FHitResult> hitList;
	bool isResult = UKismetSystemLibrary::LineTraceMultiForObjects(
		GetWorld(),                   // 获取当前世界
		start,                         // 起始点（远程小兵的位置）
		end,                           // 结束点（远程小兵的攻击目标）
		ObjectTypes,                   // 物体类型过滤，您可以根据需求修改
		true,                          // 是否忽略所有已经标记的物体（true 表示忽略）
		ignoreList,                    // 忽略的物体列表（例如已经击中的物体）
		EDrawDebugTrace::ForDuration,  // 调试模式，显示碰撞检测的持续时间
		hitList,                       // 存储击中的物体列表
		true                           // 是否在碰撞时阻止移动（可以根据需求调整）
	);

	if (isResult)
	{
		APlayerBaseCharacter* player = GetAttackRangePlayer(hitList);
		if (player)
		{
			player->DiaoXie(20.0f);
		}
	}

}
