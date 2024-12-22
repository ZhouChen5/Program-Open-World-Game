#include "UAC_Inventory.h"

// 函数实现开始，此函数尝试自动将物品添加到已有的库存栈中，按照相关规则处理并反馈结果
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

    // 遍历目标库存数组，查找与要添加物品匹配的已有栈
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = TargetArray[i];
        // 判断物品是否匹配（这里简单以物品数据指针相等为例，可按实际结构体定义完善）
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

// 函数实现开始，此函数尝试自动将物品添加到空槽位和新创建的槽位中，按照规则处理并反馈结果
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
        // 先尝试创建新槽位来添加物品
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

    // 再查找空槽位添加物品
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

// 函数实现开始，此函数用于创建一个新的空库存槽位，并返回其索引
void UAC_Inventory::CreateEmptySlot(const TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    int32& NewSlotIndex) {
    FStruct_Inventory_Slot_Item emptySlotItem;
    emptySlotItem.ItemData = nullptr;
    emptySlotItem.Amount = 0;

    // 简单示例，假设这里将新空槽位添加到某个库存数组末尾（实际可能根据具体逻辑调整添加位置）
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
        // 假设FStruct_Item结构体中有标识是否可堆叠的成员变量来判断
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