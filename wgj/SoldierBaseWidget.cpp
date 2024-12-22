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
    //检查 HealthProgress 指针是否有效
    if (HealthProgress) {
        //百分比显示血条
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
