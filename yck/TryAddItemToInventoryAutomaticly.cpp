#include "UAC_Inventory.h"
// 函数实现开始，此函数用于尝试自动将物品添加到库存中，根据各种规则处理添加逻辑
void UAC_Inventory::TryAddItemToInventoryAutomaticly(FStruct_Inventory_Slot_Item InventoryItemToBeAdded,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool OverrideStackSize,
    int32 OverrideMaxStackSize,
    bool OverrideDynamicSlotCreationToTrue,
    bool UseThisFunctionAsCheckOnly,
    bool& AllItemsAdded,
    int32& AmountNotAdded,
    int32& EmptySlotsFilled,
    int32 localAmountThatCouldNotBeAddedIntoExistingStacks,
    FStruct_Inventory_Slot_Item LocalInventoryItemToBeAdded,
    int32 localAlreadyAddedIntoNewStackInOtherLoop,
    int32 NumberOfLoopsRequired,
    int32 localAmountThatCouldNotBeAddedDueToWeight) {
    AllItemsAdded = false;  // 初始化是否所有物品都已添加的标识为false
    AmountNotAdded = 0;    // 初始化未添加的物品数量为0
    EmptySlotsFilled = 0;  // 初始化填充的空槽位数量为0

    // 如果需要重写动态槽位创建为true
    if (OverrideDynamicSlotCreationToTrue) {
        // 遍历目标库存数组，查找是否有空槽位
        for (int32 i = 0; i < TargetArray.Num(); ++i) {
            if (TargetArray[i].ItemData == nullptr) {
                // 如果是空槽位，可根据情况决定是否在此处创建新的物品数据等操作
                FStruct_Inventory_Slot_Item newSlotItem;
                // 简单示例，初始化新槽位物品的相关属性
                newSlotItem.ItemData = nullptr;
                newSlotItem.Amount = 0;
                TargetArray[i] = newSlotItem;
                EmptySlotsFilled++;
            }
        }
        // 如果目标数组为空，直接创建一个新的槽位作为示例
        if (TargetArray.Num() == 0) {
            FStruct_Inventory_Slot_Item newSlotItem;
            newSlotItem.ItemData = nullptr;
            newSlotItem.Amount = 0;
            TargetArray.Add(newSlotItem);
            EmptySlotsFilled++;
        }
    }

    // 遍历目标库存数组，查找可添加物品的槽位
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = TargetArray[i];
        // 判断当前槽位是否为空或者物品是否匹配（可堆叠情况）
        if (currentSlot.ItemData == nullptr || currentSlot.ItemData == InventoryItemToBeAdded.ItemData) {
            // 如果需要重写堆叠大小
            if (OverrideStackSize) {
                int32 availableSpace = OverrideMaxStackSize - currentSlot.Amount;
                if (availableSpace >= InventoryItemToBeAdded.Amount) {
                    currentSlot.Amount += InventoryItemToBeAdded.Amount;
                    AllItemsAdded = true;
                }
                else {
                    currentSlot.Amount += availableSpace;
                    AmountNotAdded = InventoryItemToBeAdded.Amount - availableSpace;
                }
            }
            else {
                // 正常堆叠逻辑，考虑当前槽位剩余空间添加物品
                int32 maxStackSize = currentSlot.MaxStackSize;  // 假设结构体中有最大堆叠数量成员
                int32 availableSpace = maxStackSize - currentSlot.Amount;
                if (availableSpace >= InventoryItemToBeAdded.Amount) {
                    currentSlot.Amount += InventoryItemToBeAdded.Amount;
                    AllItemsAdded = true;
                }
                else {
                    currentSlot.Amount += availableSpace;
                    AmountNotAdded = InventoryItemToBeAdded.Amount - availableSpace;
                }
            }
            if (AllItemsAdded) {
                break;  // 如果所有物品都已添加，跳出循环
            }
        }
    }
    // 如果遍历完数组后仍有物品未添加，处理未添加数量等情况
    if (!AllItemsAdded) {
        AmountNotAdded = InventoryItemToBeAdded.Amount;
    }
}