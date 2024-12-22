// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PALUWORLD_API Factions
{
public:
	Factions();
	~Factions();
};


//����һ����ʾ��Ӫ��ö����
UENUM(BlueprintType)
enum class Faction : uint8{
	FRIEND UMETA(DisplayName = "Friend"),
	ENEMY UMETA(DisplayName = "Enemy"),
	NEUTRAL UMETA(DisplayName = "Nutral"),
	BOSS UMETA(DisplayName = "Boss")
};                     //�ѷ����з���������boss

//��ǰ��Ӫ��Ĭ�϶Եд�����ƣ�Ϊ��Եз��ͽ��з���true��Ϊ����ѷ��ͷ���false
inline bool IsEnemy(Faction& MyFaction, Faction& OtherFaction) {
	switch (MyFaction) {
	case Faction::FRIEND:                         //������³��Ĭ�����л���
		switch (OtherFaction) {
		case Faction::BOSS:
		case Faction::ENEMY:                      //�з���bossĬ��Ϊ�з�
			return true;
		default:
			return false;
		}
	case Faction::BOSS:                           //boss�����л���
	case Faction::ENEMY:                          //�з���³�����л���
		switch (OtherFaction) {
		case Faction::FRIEND:
			return true;
		default:
			return false;                         //����Ϊ����
		}
	default:                                      //����Ĭ��û�е���
		return true;
	}
}

