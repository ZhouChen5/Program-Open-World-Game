#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺�������ָ����������Ϊ�����Լ��Զ�������������������ָ����Ʒ��ִ����Ӧ�����߼�
void UAC_Inventory::CraftItemByActionType(FStruct_Inventory_Slot_Item WhichItemToBeMade,
    int32 CustomAmountToBeMade,
    TEnumAsByte<Enum_CraftingAction_Types> CraftingActionType) {
    // ����ɸ��ݲ�ͬ��������Ϊ���ͣ�CraftingActionType�����в�ͬ�������߼���֧����
    switch (CraftingActionType) {
        case Enum_CraftingAction_Types::SimpleCraft:
            // �������߼�ʾ�����������һ����������CraftSimpleItem�����ݲ��Ͽ۳���沢���ɳ�Ʒ
            for (int32 i = 0; i < CustomAmountToBeMade; ++i) {
                if (CraftSimpleItem(WhichItemToBeMade)) {
                    // ��������ɹ����ɽ�����Ӧ����ʾ��������������ʾ�����ɰ�����չ��
                    UE_LOG(LogTemp, Log, TEXT("Successfully crafted one %s."), *WhichItemToBeMade.ItemData->Name);
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to craft %s due to insufficient materials."), *WhichItemToBeMade.ItemData->Name);
                }
            }
            break;
        case Enum_CraftingAction_Types::AdvancedCraft:
            // �߼������߼��������漰�����ӵĲ����䷽������������ж�
            if (CraftAdvancedItem(WhichItemToBeMade, CustomAmountToBeMade)) {
                UE_LOG(LogTemp, Log, TEXT("Successfully crafted %d %s using advanced crafting."), CustomAmountToBeMade, *WhichItemToBeMade.ItemData->Name);
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Advanced crafting of %d %s failed due to various reasons."), CustomAmountToBeMade, *WhichItemToBeMade.ItemData->Name);
            }
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unrecognized crafting action type."));
            break;
    }
}

void UAC_Inventory::DropItemBySlotIndex(int32 SlotIndex,
    int32 Amount,
    FStruct_Inventory_Slot_Item InventoryItem,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool IsFromHotbar) {
    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    // �жϲ�λ�е���Ʒ�Ƿ���Ҫ��������Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
    if (targetSlot.ItemData == InventoryItem.ItemData) {
        // ���Ҫ�������������ڵ��ڲ�λ����Ʒ��������ֱ���Ƴ�������λ��Ʒ
        if (Amount >= targetSlot.Amount) {
            TargetArray.RemoveAt(SlotIndex);
        }
        else {
            // ������ٶ�Ӧ��������Ʒ
            targetSlot.Amount -= Amount;
        }

        // ����Ǵӿ������������ִ�ж���Ŀ������ش����߼��������ʾ�����ɰ�����չ��
        if (IsFromHotbar) {
            HandleHotbarItemDrop(SlotIndex);
        }
    }
}

void UAC_Inventory::RemoveAllEmptySlotsFromStartIndexToEndOfArray(int32 IndexToStartRemoving,
    const TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool& Completed) {
    Completed = false;

    // ����ʼ������ʼ����Ŀ������
    for (int32 i = IndexToStartRemoving; i < TargetArray.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& slotItem = TargetArray[i];
        // �жϲ�λ�Ƿ�Ϊ�գ���������Ʒ����ָ���Ƿ�Ϊ��Ϊ�����ɰ�������ж�������
        if (slotItem.ItemData == nullptr) {
            // ���Ϊ�գ��Ƴ��ò�λ������������ֱ�Ӳ���Ŀ�����飬ʵ�ʿ����迼�Ǹ��������
            TArray<FStruct_Inventory_Slot_Item>& mutableTargetArray = const_cast<TArray<FStruct_Inventory_Slot_Item>&>(TargetArray);
            mutableTargetArray.RemoveAt(i);
            i--;  // ��Ϊ�Ƴ���һ��Ԫ�أ�������Ҫ����һλ���������һ��λ��
        }
    }

    Completed = true;
}

void UAC_Inventory::ExecuteOnOffLogic(bool TurnOn,
    bool SwitchState,
    bool& IsOn) {
    if (SwitchState) {
        IsOn = TurnOn;
    }
    else {
        // ������л�״̬�����ֵ�ǰ�Ŀ���״̬����
        // ���������Ӹ����뵱ǰ״̬��ص��߼���������ִ����Ӧ�Ĺ��ܿ���/�رղ�����
    }
}

void UAC_Inventory::ProcessCraftingQueueSlotIndex0AndInitializeTimer() {
    if (CraftingQueue.Num() > 0) {
        FStruct_Inventory_Slot_Item& firstSlotItem = CraftingQueue[0];
        // ������Ե���������غ����������һ����λ����Ʒ�������������ProcessCrafting����
        if (ProcessCrafting(firstSlotItem)) {
            // ��������ɹ����Ӷ������Ƴ��ò�λ��Ʒ
            CraftingQueue.RemoveAt(0);
            // ��ʼ��������ʱ�������������г�Ա����CraftingQueueTimerHandle���ڶ�ʱ��������
            InitializeCraftingTimer();
        }
    }
}

void UAC_Inventory::CheckToStartFuelTimer(bool& FoundAndRemovedCombustibleInCurrentInventory,
    bool LocalFoundCombustibleInCurrentInventory) {
    FoundAndRemovedCombustibleInCurrentInventory = false;
    LocalFoundCombustibleInCurrentInventory = false;

    // ������ǰ������飬���ҿ�ȼ��Ʒ�����������Ʒ�ṹ�����б�ʶ�Ƿ��ȼ�ĳ�Ա����IsCombustible��
    for (const FStruct_Inventory_Slot_Item& slotItem : InventorySlots) {
        if (slotItem.ItemData && slotItem.ItemData->IsCombustible) {
            LocalFoundCombustibleInCurrentInventory = true;
            // �ҵ���ȼ��Ʒ�󣬿��Ƴ�һ���������ʾ���������������ɸ���ʵ�����������
            if (slotItem.Amount > 0) {
                FStruct_Inventory_Slot_Item& mutableSlotItem = const_cast<FStruct_Inventory_Slot_Item&>(slotItem);
                mutableSlotItem.Amount--;
                if (mutableSlotItem.Amount == 0) {
                    mutableSlotItem.ItemData = nullptr;
                }
                FoundAndRemovedCombustibleInCurrentInventory = true;
                break;
            }
        }
    }
}

void UAC_Inventory::FindItemIndexesToDecay(TArray<int32>& FoundItemIndexesToDecay,
    TArray<int32> localFoundItemIndexesToDecay,
    TArray<FStruct_Inventory_Slot_Item> ItemsToDecay) {
    FoundItemIndexesToDecay.Empty();
    localFoundItemIndexesToDecay.Empty();

    // ������Ҫ�������Ʒ���飨ItemsToDecay�������ҷ���˥����������Ʒ���������������Ʒ���;ö��������Durability��
    for (int32 i = 0; i < ItemsToDecay.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& item = ItemsToDecay[i];
        if (item.ItemData && item.ItemData->Durability > 0 && item.ItemData->Durability < item.ItemData->MaxDurability) {
            FoundItemIndexesToDecay.Add(i);
            localFoundItemIndexesToDecay.Add(i);
        }
    }
}

void UAC_Inventory::ProcessDurabilityAndDecayLogicBySlotIndex(int32 SlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool& zeroDurability,
    FStruct_Inventory_Slot_Item LocalInventorySlotItem,
    int32 LocalSlotIndex) {
    zeroDurability = false;
    LocalInventorySlotItem = FStruct_Inventory_Slot_Item();
    LocalSlotIndex = SlotIndex;

    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventorySlotItem = targetSlot;

    // ִ���;ö�˥���߼���������Ʒ�ṹ������Durability��Ա������ʾ��ǰ�;öȣ�DecayRate��ʾ˥������
    targetSlot.ItemData->Durability -= targetSlot.ItemData->DecayRate;
    if (targetSlot.ItemData->Durability <= 0) {
        zeroDurability = true;
        // �;öȽ�Ϊ0�󣬿ɽ�����Ӧ����Ʒ�𻵴��������Ƴ���Ʒ�ȣ������ʾ�����ɰ�����չ��
        TargetArray.RemoveAt(SlotIndex);
    }
}

void UAC_Inventory::DecayItemBySlotIndex(int32 SlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool& DurabilityIsZero,
    bool& NoItemsLeftToDecay,
    FStruct_Inventory_Slot_Item LocalInventorySlotItem,
    int32 LocalSlotIndex) {
    DurabilityIsZero = false;
    NoItemsLeftToDecay = false;
    LocalInventorySlotItem = FStruct_Inventory_Slot_Item();
    LocalSlotIndex = SlotIndex;

    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventorySlotItem = targetSlot;

    // ִ���;ö�˥�����������������غ�������������˥���߼�������ApplyDecay����
    if (ApplyDecay(targetSlot)) {
        DurabilityIsZero = targetSlot.ItemData->Durability == 0;
        if (DurabilityIsZero) {
            // ����;öȽ�Ϊ0���Ƴ�����Ʒ
            TargetArray.RemoveAt(SlotIndex);
            // �ж��Ƿ���������Ʒ��Ҫ˥��
            NoItemsLeftToDecay = true;
            for (const FStruct_Inventory_Slot_Item& item : TargetArray) {
                if (item.ItemData && item.ItemData->Durability > 0 && item.ItemData->Durability < item.ItemData->MaxDurability) {
                    NoItemsLeftToDecay = false;
                    break;
                }
            }
        }
    }
}