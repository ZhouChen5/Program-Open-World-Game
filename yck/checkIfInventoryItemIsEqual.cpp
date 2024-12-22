#include "UAC_Inventory.h"

// 函数实现开始，此函数用于比较两个库存槽位物品是否相等，并设置相应的布尔返回参数
void UAC_Inventory::checkIfInventoryItemIsEqual(const FStruct_Inventory_Slot_Item& Struct_Inventory_Item1,
    const FStruct_Inventory_Slot_Item& Struct_Inventory_Item2,
    bool& True) {
    True = false;  // 先默认设置为不相等，即False

    // 比较两个物品的关键属性来判断是否相等，这里假设关键属性包含物品数据指针、数量、唯一ID等（可根据实际结构体定义调整）
    if (Struct_Inventory_Item1.ItemData == Struct_Inventory_Item2.ItemData &&
        Struct_Inventory_Item1.Amount == Struct_Inventory_Item2.Amount &&
        Struct_Inventory_Item1.UniqueID == Struct_Inventory_Item2.UniqueID) {
        True = true;  // 如果各项关键属性都相等，则设置为相等，即True
    }
}