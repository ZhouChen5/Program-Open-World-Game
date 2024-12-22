// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierBaseWidget.h"
#include "Components/ProgressBar.h"
#include "BaseCharacter.h"
#include "DebugHelper.h"

bool USoldierBaseWidget::Initialize()
{
    if (!Super::Initialize()) {
        return false;
    }
    //��� HealthProgress ָ���Ƿ���Ч
    if (HealthProgress) {
        //�ٷֱ���ʾѪ��
        HealthProgress->PercentDelegate.BindUFunction(this, FName("GetHealthProgress"));
    }
    else {
        Debug::print("HealthProgress is nullptr in USoldierBaseWidget::Initialize()");
        return false;
    }
    return true;
}

float USoldierBaseWidget::GetHealthProgress()
{
	return ownerCharacter->GetHealthProgress();
}
