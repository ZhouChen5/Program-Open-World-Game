#include "UAC_Inventory.h"

// 函数实现开始，此函数尝试将物品添加到已有的库存栈中，按照相关规则处理并反馈结果
void UAC_Inventory::TryProcessItemToExistingStack(int32 SlotIndex,
    FStruct_Inventory_Slot_Item ItemToAdd,
    int32 correctionOfAmountPreviouslyAdded,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    bool OverrideStackSize,
    int32 OverrideMaxStackSize,
    bool UseThisFunctionAsCheckOnly,
    int32& amountAdded,
    bool& Success,
    int32 localAmountAdded,
    int32 localSlotIndex,
    FStruct_Inventory_Slot_Item localItemToAdd,
    int32 localCurrentAmountInSlot,
    int32 localMaxStackSize,
    int32 localAmountToBeAdded,
    int32 LocalRoomInStack,
    bool localOverrideStackSize,
    int32 localOverrideMaxStackSize) {
    Success = false;  // 初始化添加成功标识为false
    amountAdded = 0;    // 初始化实际添加的数量为0

    // 先判断传入的槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= Array.Num()) {
        return;  // 槽位索引不合法，直接返回，无法进行添加操作
    }

    FStruct_Inventory_Slot_Item& targetSlot = Array[SlotIndex];
    // 获取目标槽位当前的物品数量（假设结构体中有对应数量成员变量）
    localCurrentAmountInSlot = targetSlot.Amount;
    localMaxStackSize = targetSlot.MaxStackSize;  // 获取目标槽位的最大堆叠数量（假设结构体中有该成员变量）

    if (OverrideStackSize) {
        localOverrideStackSize = true;
        localOverrideMaxStackSize = OverrideMaxStackSize;
        localMaxStackSize = OverrideMaxStackSize;  // 使用重写的最大堆叠数量
    }

    // 计算目标槽位剩余可添加的空间
    LocalRoomInStack = localMaxStackSize - localCurrentAmountInSlot;
    localAmountToBeAdded = ItemToAdd.Amount;

    // 判断要添加的物品数量是否超过剩余空间
    if (localAmountToBeAdded <= LocalRoomInStack) {
        // 如果不超过，将物品添加到栈中
        targetSlot.Amount += localAmountToBeAdded;
        amountAdded = localAmountToBeAdded;
        Success = true;
    }
    else {
        // 如果超过，只能添加剩余空间允许的数量
        targetSlot.Amount = localMaxStackSize;
        amountAdded = LocalRoomInStack;
    }

    localAmountAdded = amountAdded;  // 更新局部添加数量变量
    localSlotIndex = SlotIndex;      // 更新局部槽位索引变量
    localItemToAdd = ItemToAdd;      // 更新局部要添加的物品变量
}