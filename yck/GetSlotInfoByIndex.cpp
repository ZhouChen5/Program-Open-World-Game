#include "UAC_Inventory.h"

// 函数实现开始，此函数通过槽位索引获取对应库存槽位的各项信息，并设置相关返回参数
void UAC_Inventory::GetSlotInfoByIndex(int32 slotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    bool OverrideStackSize,
    int32 NewStackSize,
    FStruct_Inventory_Slot_Item& InventoryItemDataSlot,
    FStruct_Item& ItemData,
    bool& Stackable,
    int32& MaxStackSize,
    int32& CurrentAmount,
    int32& RoomInStack,
    bool& StackIsFull) {
    Stackable = false;  // 初始化是否可堆叠标识为false
    MaxStackSize = 0;   // 初始化最大堆叠数量为0
    CurrentAmount = 0;  // 初始化当前数量为0
    RoomInStack = 0;    // 初始化剩余空间为0
    StackIsFull = false; // 初始化栈是否已满标识为false

    // 先判断传入的槽位索引是否合法
    if (slotIndex < 0 || slotIndex >= Array.Num()) {
        return;  // 槽位索引不合法，直接返回，无法获取信息
    }

    const FStruct_Inventory_Slot_Item& targetSlot = Array[slotIndex];
    InventoryItemDataSlot = targetSlot;  // 获取对应槽位的库存物品数据

    // 获取物品数据（假设FStruct_Inventory_Slot_Item结构体中有对应成员能获取到FStruct_Item类型的数据）
    ItemData = targetSlot.ItemData;

    // 判断物品是否可堆叠（这里假设FStruct_Item结构体中有对应标识成员变量，可根据实际情况调整判断逻辑）
    if (targetSlot.ItemData && targetSlot.ItemData->IsStackable) {
        Stackable = true;
    }

    MaxStackSize = targetSlot.MaxStackSize;  // 获取最大堆叠数量（假设结构体中有该成员变量）
    CurrentAmount = targetSlot.Amount;      // 获取当前槽位的物品数量

    // 计算剩余空间
    if (OverrideStackSize) {
        RoomInStack = NewStackSize - CurrentAmount;
    }
    else {
        RoomInStack = MaxStackSize - CurrentAmount;
    }

    // 判断栈是否已满
    if (CurrentAmount >= MaxStackSize) {
        StackIsFull = true;
    }
}