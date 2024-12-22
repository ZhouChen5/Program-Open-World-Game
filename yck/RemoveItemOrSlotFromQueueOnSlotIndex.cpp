#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺������ڸ���ָ����λ�����������������Ƴ���Ʒ��������λ�������������
void UAC_Inventory::RemoveItemOrSlotFromQueueOnSlotIndex(const FStruct_Inventory_Slot_Item& StructInventoryS_Item,
    bool RemoveAllFromStack,
    int32 SlotIndex) {
    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= CraftingQueue.Num()) {
        return;
    }

    if (RemoveAllFromStack) {
        // ���Ҫ���Ƴ�����ջ��ֱ���Ƴ���Ӧ��λ����Ʒ
        CraftingQueue.RemoveAt(SlotIndex);
    }
    else {
        // ����ֻ���ٶ�Ӧ��λ��Ʒ�����������������Ʒ������1���ɸ���ʵ�����������
        FStruct_Inventory_Slot_Item& targetSlot = CraftingQueue[SlotIndex];
        if (targetSlot.Amount > 0) {
            targetSlot.Amount--;
            if (targetSlot.Amount == 0) {
                // ���������Ϊ0����ѡ���Ƴ��ò�λ������ʾ���Ƴ���Ҳ�ɱ����ղ�λ����������
                CraftingQueue.RemoveAt(SlotIndex);
            }
        }
    }
}

void UAC_Inventory::RemoveAllItemsFromQueue() {
    // ֱ�ӵ��������Empty���������������
    CraftingQueue.Empty();
}


void UAC_Inventory::ResetCraftingQueue() {
    // �����������
    RemoveAllItemsFromQueue();

    // ���õ�ǰ����ʱ��Ϊ0������������CurrentCraftingTime��Ա������ʾ��ǰ����ʱ�䣩
    CurrentCraftingTime = 0.0;

    // �����������ж�ʱ�����������������CraftingQueueTimerHandle��Ա������
    CraftingQueueTimerHandle.Reset();
}


void UAC_Inventory::ResetCraftingQueue() {
    // �����������
    RemoveAllItemsFromQueue();

    // ���õ�ǰ����ʱ��Ϊ0������������CurrentCraftingTime��Ա������ʾ��ǰ����ʱ�䣩
    CurrentCraftingTime = 0.0;

    // �����������ж�ʱ�����������������CraftingQueueTimerHandle��Ա������
    CraftingQueueTimerHandle.Reset();
}

void UAC_Inventory::CanAllItemsBeAdded(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& ItemsToAdd,
    bool UseMultiplier,
    int32 AmountMultiplier,
    bool& AllItemsCanBeAdded,
    bool LocalAllItemsCanBeAdded,
    int32 localTotalEmptySlotsFilled) {
    AllItemsCanBeAdded = true;
    localTotalEmptySlotsFilled = 0;
    LocalAllItemsCanBeAdded = true;

    // ����Ҫ��ӵ���Ʒ����
    for (const FStruct_Inventory_Slot_Item& item : ItemsToAdd) {
        bool itemCanBeAdded = false;
        // ���������Ӿ�����ж��߼�������鿴������Ƿ��пղ�λ���Ƿ�����ѵ������
        // ʾ�����жϿ�����Ƿ��пղ�λ������������ΪInventorySlots��
        for (const FStruct_Inventory_Slot_Item& slotItem : InventorySlots) {
            if (slotItem.ItemData == nullptr) {
                localTotalEmptySlotsFilled++;
                itemCanBeAdded = true;
                break;
            }
        }

        if (!itemCanBeAdded) {
            AllItemsCanBeAdded = false;
            LocalAllItemsCanBeAdded = false;
            break;
        }
    }

    if (UseMultiplier) {
        // ���ʹ�ó������ɸ��ݳ�����һ���жϣ������ʾ�����ɸ���ʵ�����������߼���
        if (localTotalEmptySlotsFilled < ItemsToAdd.Num() * AmountMultiplier) {
            AllItemsCanBeAdded = false;
            LocalAllItemsCanBeAdded = false;
        }
    }
}


void UAC_Inventory::TryTransferAllToDestinationInventory(UAC_Inventory_C* DestinationInventory) {
    if (!DestinationInventory) {
        return;  // ���Ŀ����ָ��Ϊ�գ��޷�����ת�Ʋ�����ֱ�ӷ���
    }

    // ������ǰ������飬��ÿ����Ʒ��ӵ�Ŀ���棨�������ӣ�ʵ�ʿ����迼�Ƕѵ��ȹ���
    for (const FStruct_Inventory_Slot_Item& item : InventorySlots) {
        DestinationInventory->InventorySlots.Add(item);
    }

    // ��յ�ǰ�������
    InventorySlots.Empty();
}

void UAC_Inventory::TransferItemByTransferType(TEnumAsByte<Enum_Transfer_Types> Selection,
    FStruct_Inventory_Slot_Item InventoryItem,
    int32 CustomAmount,
    UAC_Inventory_C* DestinationInventory,
    int32 SourceSlotIndex,
    UAC_Inventory_C* Source,
    bool& Success) {
    Success = false;

    if (!Source || !DestinationInventory) {
        return;  // ���Դ����Ŀ����ָ��Ϊ�գ��޷�����ת�Ʋ�����ֱ�ӷ���
    }

    // �ж�Դ��λ�����Ƿ�Ϸ�
    if (SourceSlotIndex < 0 || SourceSlotIndex >= Source->InventorySlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& sourceSlotItem = Source->InventorySlots[SourceSlotIndex];
    // �ж�Դ��λ��Ʒ�Ƿ���Ҫת�Ƶ���Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
    if (sourceSlotItem.ItemData == InventoryItem.ItemData) {
        // �����Զ�����������ת��
        if (sourceSlotItem.Amount >= CustomAmount) {
            sourceSlotItem.Amount -= CustomAmount;
            if (sourceSlotItem.Amount == 0) {
                sourceSlotItem.ItemData = nullptr;
            }

            // �����Ʒ��Ŀ���棨�������ӣ�ʵ�ʿ����迼�Ƕѵ��ȹ���
            DestinationInventory->InventorySlots.Add(InventoryItem);
            Success = true;
        }
    }
}

void UAC_Inventory::SplitInventoryStackBySplitType(TEnumAsByte<Enum_split_types> SplitType,
    const FStruct_Inventory_Slot_Item& inventoryItemToBeSplitted,
    int32 CustomAmount,
    int32 SourceSlotIndex) {
    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SourceSlotIndex < 0 || SourceSlotIndex >= InventorySlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = InventorySlots[SourceSlotIndex];
    // �жϲ�λ��Ʒ�Ƿ���Ҫ��ֵ���Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
    if (targetSlot.ItemData == inventoryItemToBeSplitted.ItemData) {
        // ���ݲ�����ͺ��Զ����������в���߼����������ʾ������ָ��������ֵ��µĲ�λ
        if (targetSlot.Amount > CustomAmount) {
            FStruct_Inventory_Slot_Item newSlotItem = targetSlot;
            newSlotItem.Amount = CustomAmount;
            targetSlot.Amount -= CustomAmount;

            // ���²�ֳ�������Ʒ��ӵ�������飨����ɸ���ʵ�ʹ���ȷ�����λ�õȣ�
            InventorySlots.Add(newSlotItem);
        }
    }
}

