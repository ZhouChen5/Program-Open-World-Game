#include "UAC_Inventory.h"

// 函数实现开始，此函数用于根据指定槽位索引从制作队列中移除物品或整个槽位，按传入规则处理
void UAC_Inventory::RemoveItemOrSlotFromQueueOnSlotIndex(const FStruct_Inventory_Slot_Item& StructInventoryS_Item,
    bool RemoveAllFromStack,
    int32 SlotIndex) {
    // 先判断槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= CraftingQueue.Num()) {
        return;
    }

    if (RemoveAllFromStack) {
        // 如果要求移除整个栈，直接移除对应槽位的物品
        CraftingQueue.RemoveAt(SlotIndex);
    }
    else {
        // 否则只减少对应槽位物品的数量（这里假设物品数量减1，可根据实际需求调整）
        FStruct_Inventory_Slot_Item& targetSlot = CraftingQueue[SlotIndex];
        if (targetSlot.Amount > 0) {
            targetSlot.Amount--;
            if (targetSlot.Amount == 0) {
                // 如果数量减为0，可选择移除该槽位（这里示例移除，也可保留空槽位等其他处理）
                CraftingQueue.RemoveAt(SlotIndex);
            }
        }
    }
}

void UAC_Inventory::RemoveAllItemsFromQueue() {
    // 直接调用数组的Empty函数清空制作队列
    CraftingQueue.Empty();
}


void UAC_Inventory::ResetCraftingQueue() {
    // 清空制作队列
    RemoveAllItemsFromQueue();

    // 重置当前制作时间为0（假设类中有CurrentCraftingTime成员变量表示当前制作时间）
    CurrentCraftingTime = 0.0;

    // 重置制作队列定时器句柄（假设类中有CraftingQueueTimerHandle成员变量）
    CraftingQueueTimerHandle.Reset();
}


void UAC_Inventory::ResetCraftingQueue() {
    // 清空制作队列
    RemoveAllItemsFromQueue();

    // 重置当前制作时间为0（假设类中有CurrentCraftingTime成员变量表示当前制作时间）
    CurrentCraftingTime = 0.0;

    // 重置制作队列定时器句柄（假设类中有CraftingQueueTimerHandle成员变量）
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

    // 遍历要添加的物品数组
    for (const FStruct_Inventory_Slot_Item& item : ItemsToAdd) {
        bool itemCanBeAdded = false;
        // 这里可以添加具体的判断逻辑，比如查看库存中是否有空槽位、是否满足堆叠规则等
        // 示例简单判断库存中是否有空槽位（假设库存数组为InventorySlots）
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
        // 如果使用乘数，可根据乘数进一步判断（这里简单示例，可根据实际需求完善逻辑）
        if (localTotalEmptySlotsFilled < ItemsToAdd.Num() * AmountMultiplier) {
            AllItemsCanBeAdded = false;
            LocalAllItemsCanBeAdded = false;
        }
    }
}


void UAC_Inventory::TryTransferAllToDestinationInventory(UAC_Inventory_C* DestinationInventory) {
    if (!DestinationInventory) {
        return;  // 如果目标库存指针为空，无法进行转移操作，直接返回
    }

    // 遍历当前库存数组，将每个物品添加到目标库存（这里简单添加，实际可能需考虑堆叠等规则）
    for (const FStruct_Inventory_Slot_Item& item : InventorySlots) {
        DestinationInventory->InventorySlots.Add(item);
    }

    // 清空当前库存数组
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
        return;  // 如果源库存或目标库存指针为空，无法进行转移操作，直接返回
    }

    // 判断源槽位索引是否合法
    if (SourceSlotIndex < 0 || SourceSlotIndex >= Source->InventorySlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& sourceSlotItem = Source->InventorySlots[SourceSlotIndex];
    // 判断源槽位物品是否与要转移的物品匹配（这里简单以物品数据指针相等为例，可按需完善）
    if (sourceSlotItem.ItemData == InventoryItem.ItemData) {
        // 根据自定义数量进行转移
        if (sourceSlotItem.Amount >= CustomAmount) {
            sourceSlotItem.Amount -= CustomAmount;
            if (sourceSlotItem.Amount == 0) {
                sourceSlotItem.ItemData = nullptr;
            }

            // 添加物品到目标库存（这里简单添加，实际可能需考虑堆叠等规则）
            DestinationInventory->InventorySlots.Add(InventoryItem);
            Success = true;
        }
    }
}

void UAC_Inventory::SplitInventoryStackBySplitType(TEnumAsByte<Enum_split_types> SplitType,
    const FStruct_Inventory_Slot_Item& inventoryItemToBeSplitted,
    int32 CustomAmount,
    int32 SourceSlotIndex) {
    // 先判断槽位索引是否合法
    if (SourceSlotIndex < 0 || SourceSlotIndex >= InventorySlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = InventorySlots[SourceSlotIndex];
    // 判断槽位物品是否与要拆分的物品匹配（这里简单以物品数据指针相等为例，可按需完善）
    if (targetSlot.ItemData == inventoryItemToBeSplitted.ItemData) {
        // 根据拆分类型和自定义数量进行拆分逻辑处理，这里简单示例，将指定数量拆分到新的槽位
        if (targetSlot.Amount > CustomAmount) {
            FStruct_Inventory_Slot_Item newSlotItem = targetSlot;
            newSlotItem.Amount = CustomAmount;
            targetSlot.Amount -= CustomAmount;

            // 将新拆分出来的物品添加到库存数组（这里可根据实际规则确定添加位置等）
            InventorySlots.Add(newSlotItem);
        }
    }
}

