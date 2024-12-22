// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "BasePalCharacter.h"

bool UHealthBarWidget::Initialize()
{
	//观察父类的初始化是否正确
	if (Super::Initialize()) {
		//百分比显示血条
		HealthProgress->PercentDelegate.BindUFunction(this, FName("GetHealthProgress"));
		return true;
	}
	return false;
}

float UHealthBarWidget::GetHealthProgress()
{
	return ownerCharacter->GetHealthProgress();
}

