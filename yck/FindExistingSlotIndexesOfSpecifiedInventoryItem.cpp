#include "UAC_Inventory.h"

// 函数实现开始，此函数用于查找指定库存物品在目标库存数组中所在的槽位索引，并设置相关返回参数
void UAC_Inventory::FindExistingSlotIndexesOfSpecifiedInventoryItem(FStruct_Inventory_Slot_Item InventoryItem,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    TArray<int32>& SlotsFoundAtIndexes,
    bool& Success,
    TArray<int32> FoundIndexes) {
    Success = false;  // 初始化查找成功标识为false
    SlotsFoundAtIndexes.Empty();  // 清空用于存储找到的槽位索引的数组，确保初始状态正确

    // 遍历目标库存数组
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& currentSlotItem = TargetArray[i];
        // 判断当前槽位的物品是否与指定的库存物品相等（这里假设相等的判断依据是物品数据指针相等以及其他关键属性相等，可根据实际结构体定义调整）
        if (currentSlotItem.ItemData == InventoryItem.ItemData &&
            currentSlotItem.Amount == InventoryItem.Amount&&
            // 可以添加更多属性的比较判断，比如物品的唯一ID等，如果结构体中有相关定义的话
            // currentSlotItem.UniqueID == InventoryItem.UniqueID
            ) {
            SlotsFoundAtIndexes.Add(i);  // 将找到的槽位索引添加到结果数组中
            Success = true;  // 如果找到了至少一个匹配的槽位，设置查找成功标识为true
        }
    }
}