#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺��������Զ�����Ʒ��ӵ����еĿ��ջ�У�������ع������������
void UAC_Inventory::TryAddItemsToExistingStacksAutomatically(FStruct_Inventory_Slot_Item InventoryItem,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool OverrideStackSize,
    int32 OverrideMaxStackSize,
    bool UseThisFunctionAsCheckOnly,
    bool& NoExistingStacksFound,
    bool& AllItemsAdded,
    int32& AmountThatCouldNotBeAddedIntoExistingStacks,
    FStruct_Inventory_Slot_Item localInventoryItem,
    int32 LocalAmountPreviouslyAdded) {
    NoExistingStacksFound = true;
    AllItemsAdded = false;
    AmountThatCouldNotBeAddedIntoExistingStacks = 0;

    // ����Ŀ�������飬������Ҫ�����Ʒƥ�������ջ
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = TargetArray[i];
        // �ж���Ʒ�Ƿ�ƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ�ʵ�ʽṹ�嶨�����ƣ�
        if (currentSlot.ItemData == InventoryItem.ItemData) {
            NoExistingStacksFound = false;
            int32 availableSpace = 0;
            if (OverrideStackSize) {
                availableSpace = OverrideMaxStackSize - currentSlot.Amount;
            }
            else {
                availableSpace = currentSlot.MaxStackSize - currentSlot.Amount;
            }

            if (InventoryItem.Amount <= availableSpace) {
                currentSlot.Amount += InventoryItem.Amount;
                AllItemsAdded = true;
                break;
            }
            else {
                currentSlot.Amount = availableSpace > 0 ? availableSpace : 0;
                AmountThatCouldNotBeAddedIntoExistingStacks = InventoryItem.Amount - availableSpace;
            }
        }
    }
}

// ����ʵ�ֿ�ʼ���˺��������Զ�����Ʒ��ӵ��ղ�λ���´����Ĳ�λ�У����չ������������
void UAC_Inventory::TryAddItemsToEmptyAndNewSlotsAutomatically(const FStruct_Inventory_Slot_Item& inventoryItemToBeAdded,
    int32 AmountToAdd,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool OverrideStackSize,
    int32 NewStackSize,
    bool OverrideDynamicSlotCreation,
    bool UseThisFunctionAsCheckOnly,
    bool& AllItemsAdded,
    int32& AmountNotAdded,
    int32& EmptySlotsFilled,
    int32 localAmountToAdd,
    FStruct_Inventory_Slot_Item LocalInventoryItemToBeAdded,
    int32 localAmountAddedIntoEmptyStacks) {
    AllItemsAdded = false;
    AmountNotAdded = 0;
    EmptySlotsFilled = 0;

    if (OverrideDynamicSlotCreation) {
        // �ȳ��Դ����²�λ�������Ʒ
        while (AmountToAdd > 0) {
            FStruct_Inventory_Slot_Item newSlotItem;
            newSlotItem.ItemData = inventoryItemToBeAdded.ItemData;
            int32 addAmount = AmountToAdd > NewStackSize ? NewStackSize : AmountToAdd;
            newSlotItem.Amount = addAmount;
            TargetArray.Add(newSlotItem);
            EmptySlotsFilled++;
            localAmountAddedIntoEmptyStacks += addAmount;
            AmountToAdd -= addAmount;
        }
    }

    // �ٲ��ҿղ�λ�����Ʒ
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = TargetArray[i];
        if (currentSlot.ItemData == nullptr) {
            int32 addAmount = AmountToAdd > NewStackSize ? NewStackSize : AmountToAdd;
            currentSlot.ItemData = inventoryItemToBeAdded.ItemData;
            currentSlot.Amount = addAmount;
            EmptySlotsFilled++;
            localAmountAddedIntoEmptyStacks += addAmount;
            AmountToAdd -= addAmount;
            if (AmountToAdd == 0) {
                AllItemsAdded = true;
                break;
            }
        }
    }

    AmountNotAdded = AmountToAdd;
}

// ����ʵ�ֿ�ʼ���˺������ڴ���һ���µĿտ���λ��������������
void UAC_Inventory::CreateEmptySlot(const TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    int32& NewSlotIndex) {
    FStruct_Inventory_Slot_Item emptySlotItem;
    emptySlotItem.ItemData = nullptr;
    emptySlotItem.Amount = 0;

    // ��ʾ�����������ｫ�¿ղ�λ��ӵ�ĳ���������ĩβ��ʵ�ʿ��ܸ��ݾ����߼��������λ�ã�
    TArray<FStruct_Inventory_Slot_Item>& inventoryArray = const_cast<TArray<FStruct_Inventory_Slot_Item>&>(TargetArray);
    NewSlotIndex = inventoryArray.Add(emptySlotItem);
}

void UAC_Inventory::GetInventoryItemInfo(const FStruct_Inventory_Slot_Item& inventoryItem,
    bool OverrideStackSize,
    int32 NewStackSize,
    FStruct_Item& Item,
    bool& Stackable,
    int32& MaxStackSize,
    int32& AmountInSlot,
    int32& RoomInSlot) {
    Stackable = false;
    MaxStackSize = 0;
    AmountInSlot = 0;
    RoomInSlot = 0;

    Item = inventoryItem.ItemData;
    if (inventoryItem.ItemData) {
        // ����FStruct_Item�ṹ�����б�ʶ�Ƿ�ɶѵ��ĳ�Ա�������ж�
        Stackable = inventoryItem.ItemData->IsStackable;
        MaxStackSize = inventoryItem.MaxStackSize;
    }
    AmountInSlot = inventoryItem.Amount;
    if (OverrideStackSize) {
        RoomInSlot = NewStackSize - AmountInSlot;
    }
    else {
        RoomInSlot = MaxStackSize - AmountInSlot;
    }
}

void UAC_Inventory::TryAddItemsToStack(const FStruct_Inventory_Slot_Item& StructInventoryItem,
    int32 SlotIndex,
    int32 AmountToBeAdded,
    int32 AmountInSlot,
    int32 RoomInSlot,
    const TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool UseThisFunctionAsCheckOnly,
    int32& AmountAdded,
    bool& AllItemsAdded,
    int32& AmountLeftToBeAdded) {
    AllItemsAdded = false;
    AmountAdded = 0;
    AmountLeftToBeAdded = AmountToBeAdded;

    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    const FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    AmountInSlot = targetSlot.Amount;
    RoomInSlot = targetSlot.MaxStackSize - targetSlot.Amount;

    if (AmountToBeAdded <= RoomInSlot) {
        if (!UseThisFunctionAsCheckOnly) {
            FStruct_Inventory_Slot_Item& mutableTargetSlot = const_cast<FStruct_Inventory_Slot_Item&>(targetSlot);
            mutableTargetSlot.Amount += AmountToBeAdded;
        }
        AmountAdded = AmountToBeAdded;
        AllItemsAdded = true;
        AmountLeftToBeAdded = 0;
    }
    else {
        if (!UseThisFunctionAsCheckOnly) {
            FStruct_Inventory_Slot_Item& mutableTargetSlot = const_cast<FStruct_Inventory_Slot_Item&>(targetSlot);
            mutableTargetSlot.Amount = targetSlot.MaxStackSize;
        }
        AmountAdded = RoomInSlot;
        AmountLeftToBeAdded = AmountToBeAdded - RoomInSlot;
    }
}

void UAC_Inventory::HowManyStacksShouldBeCreated(const FStruct_Inventory_Slot_Item& inventoryItem,
    int32 AmountToAdd,
    bool OverrideStackSize,
    int32 NewStackSize,
    int32& amountOfNewStacksRequired) {
    amountOfNewStacksRequired = 0;

    if (inventoryItem.ItemData == nullptr) {
        return;
    }

    int32 stackSize = OverrideStackSize ? NewStackSize : inventoryItem.MaxStackSize;
    amountOfNewStacksRequired = AmountToAdd / stackSize;
    if (AmountToAdd % stackSize != 0) {
        amountOfNewStacksRequired++;
    }
}