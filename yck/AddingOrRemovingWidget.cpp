#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺������ݴ����С��������ҿ�������ִ����ӻ��Ƴ�С�����Ĳ���
void UAC_Inventory::AddingOrRemovingWidget(UUserWidget* Widget, APlayerController* PlayerController) {
    if (Widget && PlayerController) {
        if (Widget->IsInViewport()) {
            // ���С���������ӿ��У�������ʾ�������Ƴ���
            Widget->RemoveFromViewport();
        }
        else {
            // ���С���������ӿ��У�����������ӿ�����ʾ
            PlayerController->AddWidget(Widget);
        }
    }
}

void UAC_Inventory::Consume_Item(FStruct_Inventory_Slot_Item InventoryItemToConsume,
    int32 SlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool IsFromHotbar) {
    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    // �ж�Ҫ���ĵ���Ʒ�Ƿ����λ�е���Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
    if (targetSlot.ItemData == InventoryItemToConsume.ItemData) {
        // ������Ʒ�����������ʾ�����ɸ���ʵ����Ʒ���Ĺ�����������簴�������ġ����������ĵȣ�
        targetSlot.Amount--;
        if (targetSlot.Amount == 0) {
            // ���������Ϊ0���Ƴ�����Ʒ��λ
            TargetArray.RemoveAt(SlotIndex);
        }

        // ����Ǵӿ�������ģ���ִ�ж���Ŀ��������߼���������¿������ʾ�ȣ������ʾ�����ɰ�����չ��
        if (IsFromHotbar) {
            HandleHotbarConsumeLogic(SlotIndex);
        }

        // ���ﻹ�������������Ʒ�������������߼��������ɫ���Ա仯�ȣ����������Ӧ����������
        ApplyConsumeEffect(InventoryItemToConsume);
    }
}

void UAC_Inventory::CanInteractTrace(TArray<TEnumAsByte<EObjectTypeQuery> > TraceObject) {
    // �������ʹ��UE5����ײ��ѯ�����߼��Ȼ������ж��Ƿ���׷�ٵ����������Ķ���
    // ʾ��������һ����ײ��ѯ�����ṹ��ʵ��
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(GetOwner());  // ��������

    TArray<FHitResult> hitResults;
    bool bHitAnything = false;

    // ����׷�ٶ����������飬���ÿ�����ͽ��м�⣨�����ʾ����ʵ�ʿ��ܸ����ӣ�
    for (TEnumAsByte<EObjectTypeQuery> objectType : TraceObject) {
        // �������߼�⣨�������ӽ�ɫλ����ǰ�������ߣ��ɸ���ʵ�ʵ�����㡢����Ȳ�����
        bHitAnything = GetWorld()->LineTraceMultiByObjectType(hitResults, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * InteractionTraceDistance, objectType, queryParams);
        if (bHitAnything) {
            break;
        }
    }

    // �����Ƿ��⵽���󣬿ɽ��к���������û򷵻ؽ���������ʾ�����ɸ���ʵ�����������߼���
    if (bHitAnything) {
        SetCanInteract(true);
    }
    else {
        SetCanInteract(false);
    }
}

void UAC_Inventory::IsCharacterMeetingAllLevelRequirements(FStruct_Inventory_Slot_Item ItemToUse,
    bool& IsMeetingLevelRequirement,
    bool LocalDoesNotMeetLevelRequirement) {
    IsMeetingLevelRequirement = false;
    LocalDoesNotMeetLevelRequirement = true;

    // ������ڻ�ȡ��ɫ��ǰ�ȼ��ĺ���GetCharacterLevel
    int32 characterLevel = GetCharacterLevel();

    // �����Ʒ�Ƿ��еȼ�Ҫ���Լ���ɫ�ȼ��Ƿ����㣨���������Ʒ�ṹ������RequiredLevel��Ա������ʾ����ȼ���
    if (ItemToUse.ItemData && ItemToUse.ItemData->RequiredLevel <= characterLevel) {
        IsMeetingLevelRequirement = true;
        LocalDoesNotMeetLevelRequirement = false;
    }
}

void UAC_Inventory::CanItemBeAddedDueToWeight(FStruct_Inventory_Slot_Item item,
    bool& AllItemsCanBeAdded,
    int32& AmountThatCanBeAdded,
    int32& AmountThatCanNotBeAdded,
    double LocalWeightToBeAdded,
    int32 LocalAmountOfItemThatCanBeAddedDueToWeight) {
    AllItemsCanBeAdded = false;
    AmountThatCanBeAdded = 0;
    AmountThatCanNotBeAdded = item.Amount;
    LocalWeightToBeAdded = item.ItemData->Weight * item.Amount;
    LocalAmountOfItemThatCanBeAddedDueToWeight = 0;

    // ������ڻ�ȡ��ǰ����������ĺ���GetCurrentInventoryWeight�Լ����������������MaxInventoryWeight
    double currentWeight = GetCurrentInventoryWeight();
    double maxWeight = MaxInventoryWeight;

    // ����ʣ��ɳ�������
    double remainingWeight = maxWeight - currentWeight;

    // �ж���Ʒ�ܷ�����Լ�����ӵ������������������ƺ���Ʒ�����������㣩
    if (LocalWeightToBeAdded <= remainingWeight) {
        AllItemsCanBeAdded = true;
        AmountThatCanBeAdded = item.Amount;
        AmountThatCanNotBeAdded = 0;
        LocalAmountOfItemThatCanBeAddedDueToWeight = item.Amount;
    }
    else {
        // �������ȫ����ӣ��������ӵĲ�������������򵥰������������㣬�ɸ���ʵ���Ż���
        LocalAmountOfItemThatCanBeAddedDueToWeight = FMath::FloorToInt(remainingWeight / item.ItemData->Weight);
        AmountThatCanBeAdded = LocalAmountOfItemThatCanBeAddedDueToWeight;
        AmountThatCanNotBeAdded = item.Amount - LocalAmountOfItemThatCanBeAddedDueToWeight;
    }
}

void UAC_Inventory::RestockCompleteCategoryLists(UPARAM(ref) TArray<FStruct_Vendor_Sell_Categories_Details>& AutoIncludeList,
    const TArray<TEnumAsByte<Enum_Item_Categories> >& IgnoreCategories,
    bool LocalItemIsIgnored) {
    LocalItemIsIgnored = false;

    // �����Զ������б����ÿ������Ƿ��ں������������
    for (int32 i = 0; i < AutoIncludeList.Num(); ++i) {
        FStruct_Vendor_Sell_Categories_Details& categoryDetails = AutoIncludeList[i];
        LocalItemIsIgnored = false;
        for (TEnumAsByte<Enum_Item_Categories> ignoreCategory : IgnoreCategories) {
            if (categoryDetails.Category == ignoreCategory) {
                LocalItemIsIgnored = true;
                break;
            }
        }

        // ��������Ҫ�����ԣ����Զ������б����Ƴ����������ʾ�����ɸ���ʵ�ʵ����Ƴ��߼���
        if (LocalItemIsIgnored) {
            AutoIncludeList.RemoveAt(i);
            i--;  // �������ˣ���Ϊ�Ƴ���һ��Ԫ��
        }
    }
}

void UAC_Inventory::SetDefaultKnownCraftingRecipes() {
    // �������һ��Ĭ���䷽����DefaultCraftingRecipes�����������FStruct_Inventory_Slot_Item���͵��䷽��Ʒ��Ϣ�ȣ�
    for (const FStruct_Inventory_Slot_Item& recipeItem : DefaultCraftingRecipes) {
        // ���������֪�䷽�ĺ������������AddKnownRecipe���������䷽��ӵ������֪�䷽�б�
        AddKnownRecipe(recipeItem);
    }
}

void UAC_Inventory::Initialize_Starting_Equipment() {
    // �������һ����ʼװ������StartingEquipment��������FStruct_Inventory_Slot_Item���͵ĳ�ʼװ����Ʒ��Ϣ��
    for (const FStruct_Inventory_Slot_Item& equipmentItem : StartingEquipment) {
        // ����װ����Ʒ�����͵���Ϣ��ȷ�����Ӧ��װ����λ��������������Ӧ������ȷ����λ��
        TEnumAsByte<Enum_Equipment_Slots> equipmentSlot = DetermineEquipmentSlot(equipmentItem.ItemData);

        // ��װ����ӵ���Ӧ��װ����λ���������ӣ�ʵ�ʿ����漰�������������װ���Ĵ���ȣ�
        EquipmentSlots.Add(equipmentItem);

        // ���ﻹ�����������װ����ʼ���ԡ����Ӿ�Ч������صĳ�ʼ���߼������������Ӧ������
        SetupEquipmentInitialAttributes(equipmentItem);
        BindEquipmentVisuals(equipmentItem);
    }
}

void UAC_Inventory::OnRep_InventorySlots() {
    // ���������Ӹ���UI��ʾ����λ�仯���߼������������Ӧ����������UI��
    UpdateInventoryUI();

    // Ҳ���Խ����µĿ���λ����ͬ�������ػ����������ز��������������Ӧ�����ݲ���������
    SyncInventoryDataToLocalCache();
}

void UAC_Inventory::OnRep_Equipment() {
    // ���½�ɫ���������װ���仯�����������Ӧ���������½�ɫ��۵��Ӿ�Ч����
    UpdateCharacterEquipmentVisuals();

    // ͬ��װ�����ݵ����ػ������ز��������������Ӧ�����ݲ���������
    SyncEquipmentDataToLocalCache();
}