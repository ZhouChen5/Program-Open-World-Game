#include "UAC_Inventory.h"

// 函数实现开始，此函数用于在库存槽位物品数组中查找空槽位，并设置相应的返回参数
void UAC_Inventory::FindEmptySlot(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    int32& SlotIndex,
    bool& Success,
    bool localFoundEmptySlot,
    int32 localSlotIndexOfEmptySlot) {
    Success = false;  // 初始化查找成功标识为false
    SlotIndex = -1;    // 初始化槽位索引为无效值
    localFoundEmptySlot = false;  // 初始化局部的空槽位发现标识为false
    localSlotIndexOfEmptySlot = -1;  // 初始化局部的空槽位索引为无效值

    // 遍历库存槽位物品数组
    for (int32 i = 0; i < Array.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& currentSlotItem = Array[i];
        // 判断当前槽位的物品数据是否为空，以此作为空槽位的判断依据（可根据实际结构体定义调整判断条件）
        if (currentSlotItem.ItemData == nullptr) {
            SlotIndex = i;  // 记录找到的空槽位索引
            Success = true;  // 设置查找成功标识为true
            localFoundEmptySlot = true;  // 设置局部的空槽位发现标识为true
            localSlotIndexOfEmptySlot = i;  // 记录局部的空槽位索引
            break;  // 找到空槽位后可直接跳出循环，无需继续遍历
        }
    }
}