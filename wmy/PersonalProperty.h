// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PALUWORLD_API PersonalProperty
{
public:
	PersonalProperty();
	~PersonalProperty();
};

//��ʾ���Ե�ö����
UENUM(BlueprintType)
enum class PersonProperty : uint8 {
	NONE UMETA(DisplayName = "None"),
	FIRE UMETA(DisplayName = "Fire"),
	WATER UMETA(DisplayName = "Water"),
	GRASS UMETA(DisplayName = "Grass"),
	THUNDER UMETA(DisplayName = "Thunder"),
	ICE UMETA(DisplayName = "Ice"),
	GROUND UMETA(DisplayName = "Ground"),
	DARK UMETA(DisplayName = "Dark")
};

//��ȡʵ���˺�
inline float RealInjury(float Injury, PersonProperty ownerProperty, PersonProperty enemyProperty) {

	//�˺�ϵ��
	float coe = 1.0f; 
	//�ж����Կ��ƻ򱻿���
	switch (ownerProperty) {
		//��ϵ
	case PersonProperty::FIRE:
		switch (enemyProperty) {
		case PersonProperty::WATER:
			coe = 0.5f;
			break;
		case PersonProperty::ICE:
		case PersonProperty::GRASS:
			coe = 2.0f;
			break;
		}
		break;
		//ˮϵ
	case PersonProperty::WATER:
		switch (enemyProperty) {
		case PersonProperty::THUNDER:
			coe = 0.5f;
			break;
		case PersonProperty::FIRE:
			coe = 2.0f;
			break;
		}
		break;
		//��ϵ
	case PersonProperty::GRASS:
		switch (enemyProperty) {
		case PersonProperty::FIRE:
			coe = 0.5f;
			break;
		case PersonProperty::GROUND:
			coe = 2.0f;
			break;
		}
		break;
		//������
	case PersonProperty::NONE:
		switch (enemyProperty) {
		case PersonProperty::DARK:
			coe = 0.5f;
			break;
		}
		break;
		//��ϵ
	case PersonProperty::THUNDER:
		switch (enemyProperty) {
		case PersonProperty::GROUND:
			coe = 0.5f;
			break;
		case PersonProperty::WATER:
			coe = 2.0f;
			break;
		}
		break;
		//��ϵ
	case PersonProperty::ICE:
		switch (enemyProperty) {
		case PersonProperty::FIRE:
			coe = 0.5f;
			break;
		}
		break;
		//����ϵ
	case PersonProperty::GROUND:
		switch (enemyProperty) {
		case PersonProperty::GRASS:
			coe = 0.5f;
			break;
		case PersonProperty::THUNDER:
			coe = 2.0f;
			break;
		}
		break;
		//��ϵ
	default:
		switch (enemyProperty) {
		case PersonProperty::NONE:
			coe = 2.0f;
			break;
		}
		break;
	}
	return Injury * coe;
}