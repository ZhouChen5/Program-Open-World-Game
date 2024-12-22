#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺�������ͳ�ƿ��������ָ����Ʒ����������ͨ����ز�������������Ϣ����Ӧ��Ʒ
void UAC_Inventory::HowManyOfItemInInventory(FStruct_Inventory_Slot_Item InventoryItem,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    int32& amountOfItemsFound,
    FStruct_Inventory_Slot_Item& ofItem,
    int32 localItemsFound) {
    amountOfItemsFound = 0;
    localItemsFound = 0;
    ofItem = FStruct_Inventory_Slot_Item();

    // �����������
    for (const FStruct_Inventory_Slot_Item& slotItem : Array) {
        // �жϵ�ǰ��λ��Ʒ�Ƿ���ָ����Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
        if (slotItem.ItemData == InventoryItem.ItemData) {
            localItemsFound++;
            amountOfItemsFound += slotItem.Amount;
        }
    }
    ofItem = InventoryItem;
}

void UAC_Inventory::InitializeCraftingQueue() {
    // ��������������飨����������CraftingQueue��Ա������ʾ�������У�
    CraftingQueue.Empty();

    // ���ﻹ���Խ����������������г�ʼ����صĲ���������������صļ���������ʱ����
    // ���磬���õ�ǰ����ʱ��Ϊ0������������CurrentCraftingTime��Ա������
    CurrentCraftingTime = 0.0;
    // �����������ж�ʱ�����������������CraftingQueueTimerHandle��Ա������
    CraftingQueueTimerHandle.Reset();
}

void UAC_Inventory::AddItemToCraftingQueue(const FStruct_Inventory_Slot_Item& Item) {
    // ���ж����������Ƿ�����������������AmountOfCraftingQueueSlots��Ա������ʾ�����ܲ�λ����
    if (CraftingQueue.Num() < AmountOfCraftingQueueSlots) {
        // ����Ʒ��ӵ���������������
        CraftingQueue.Add(Item);
    }
    else {
        // ���������Ӷ�����������ʾ�����������߼������紥��UI��ʾ��
        UE_LOG(LogTemp, Warning, TEXT("Crafting queue is full, cannot add item."));
    }
}

void UAC_Inventory::CheckAmountOfFilledSlotsInQueue(int32& AmountInCraftingQueue,
    bool& AllQueueSlotsUsed,
    int32 LocalAmountInCraftingQueue) {
    AmountInCraftingQueue = CraftingQueue.Num();
    LocalAmountInCraftingQueue = AmountInCraftingQueue;
    AllQueueSlotsUsed = false;

    // �ж϶��в�λ�Ƿ�ȫ����ʹ��
    if (AmountInCraftingQueue == AmountOfCraftingQueueSlots) {
        AllQueueSlotsUsed = true;
    }
}

void UAC_Inventory::TryMoveItemAutomatically(UAC_Inventory_C* DestinationInventory,
    FStruct_Inventory_Slot_Item InventoryItemToBeMoved,
    UAC_Inventory_C* SourceInventory,
    bool& Success) {
    Success = false;

    if (!SourceInventory || !DestinationInventory) {
        return;  // ���Դ����Ŀ����ָ��Ϊ�գ��޷������ƶ�������ֱ�ӷ���
    }

    // ����Դ������飬����Ҫ�ƶ�����Ʒ
    for (int32 i = 0; i < SourceInventory->InventorySlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& sourceSlotItem = SourceInventory->InventorySlots[i];
        if (sourceSlotItem.ItemData == InventoryItemToBeMoved.ItemData) {
            // ��Դ����λ�Ƴ���Ʒ�������ʾ�����ɸ��ݸ���������ƣ�������������ȣ�
            sourceSlotItem.Amount--;
            if (sourceSlotItem.Amount == 0) {
                sourceSlotItem.ItemData = nullptr;
            }

            // �����Ʒ��Ŀ���棨�������ӣ�ʵ�ʿ����迼�Ƕѵ��ȹ���
            DestinationInventory->InventorySlots.Add(InventoryItemToBeMoved);
            Success = true;
            break;
        }
    }
}

