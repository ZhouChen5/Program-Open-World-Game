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
		GetWorld(),                   // ��ȡ��ǰ����
		start,                         // ��ʼ�㣨Զ��С����λ�ã�
		end,                           // �����㣨Զ��С���Ĺ���Ŀ�꣩
		ObjectTypes,                   // �������͹��ˣ������Ը��������޸�
		true,                          // �Ƿ���������Ѿ���ǵ����壨true ��ʾ���ԣ�
		ignoreList,                    // ���Ե������б������Ѿ����е����壩
		EDrawDebugTrace::ForDuration,  // ����ģʽ����ʾ��ײ���ĳ���ʱ��
		hitList,                       // �洢���е������б�
		true                           // �Ƿ�����ײʱ��ֹ�ƶ������Ը������������
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
