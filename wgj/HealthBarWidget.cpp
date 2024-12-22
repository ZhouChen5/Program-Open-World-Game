// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "BasePalCharacter.h"

bool UHealthBarWidget::Initialize()
{
	//�۲츸��ĳ�ʼ���Ƿ���ȷ
	if (Super::Initialize()) {
		//�ٷֱ���ʾѪ��
		HealthProgress->PercentDelegate.BindUFunction(this, FName("GetHealthProgress"));
		return true;
	}
	return false;
}

float UHealthBarWidget::GetHealthProgress()
{
	return ownerCharacter->GetHealthProgress();
}

