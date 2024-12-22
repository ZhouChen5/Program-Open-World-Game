#include "UAC_Inventory.h"

// 函数实现开始，此函数尝试自动将指定已装备物品卸下，按卸装规则处理并反馈卸装结果
void UAC_Inventory::AutomaticlyUnequipItem(FStruct_Inventory_Slot_Item ItemToUnequip,
    int32 SourceIndexFromEquipmentArray,
    bool& ItemIsUnequiped) {
    ItemIsUnequiped = false;

    // 先判断源索引是否合法（针对装备数组）
    if (SourceIndexFromEquipmentArray < 0 || SourceIndexFromEquipmentArray >= EquipmentSlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = EquipmentSlots[SourceIndexFromEquipmentArray];
    // 判断要卸下的物品是否与该槽位的物品匹配（这里简单以物品数据指针相等为例，可按需完善）
    if (targetSlot.ItemData == ItemToUnequip.ItemData) {
        // 将装备槽位的物品移除，即设置相关数据为空（这里简单示例，可根据实际调整具体操作）
        targetSlot.ItemData = nullptr;
        targetSlot.Amount = 0;
        ItemIsUnequiped = true;
    }
}

void UAC_Inventory::Equip_Visual(TEnumAsByte<Enum_Equipment_Slots> EquipmentSlotToChange,
    FStruct_Inventory_Slot_Item EquipmentToChange,
    bool IsRemoving,
    FName LocalSocketName,
    FStruct_Inventory_Slot_Item localEquipmentToChange,
    UStaticMesh* QuiverMesh,
    TEnumAsByte<Enum_Equipment_Slots> localEquipmentSlotToChange) {
    localEquipmentToChange = EquipmentToChange;
    localEquipmentSlotToChange = EquipmentSlotToChange;

    // 获取角色对应的3D模型组件（这里假设类中有成员函数GetCharacterModel获取模型组件，可根据实际调整）
    UMeshComponent* characterModel = GetCharacterModel();
    if (!characterModel) {
        return;
    }

    if (IsRemoving) {
        // 如果是移除操作，从对应插槽解除装备的视觉绑定（这里假设存在解除绑定的函数DetachFromSocket，根据实际接口调整）
        characterModel->DetachFromSocket(LocalSocketName);
    }
    else {
        // 根据装备类型等情况，将对应的静态网格（如箭袋网格等）绑定到指定插槽，实现装备显示效果
        if (EquipmentSlotToChange == Enum_Equipment_Slots::Quiver && QuiverMesh) {
            characterModel->AttachToSocket(LocalSocketName, FAttachmentTransformRules::KeepRelativeTransform);
        }
        // 这里还可以添加更多针对不同装备类型的视觉绑定逻辑，比如武器、头盔等装备的显示处理
    }
}

void UAC_Inventory::LearnRecipe(FStruct_Inventory_Slot_Item RecipeItem,
    int32 SourceSlotIndex,
    UAC_Inventory_C* PlayerInventoryComponent,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    UPARAM(ref) bool& IsFromHotbar,
    bool& Success,
    FStruct_Inventory_Slot_Item LocalItemToLearn,
    int32 localSlotIndex) {
    Success = false;
    LocalItemToLearn = RecipeItem;
    localSlotIndex = SourceSlotIndex;

    // 先判断源槽位索引是否合法（针对玩家库存组件的库存数组）
    if (SourceSlotIndex < 0 || SourceSlotIndex >= PlayerInventoryComponent->InventorySlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& sourceSlotItem = PlayerInventoryComponent->InventorySlots[SourceSlotIndex];
    LocalItemToLearn = sourceSlotItem;

    // 判断要学习的配方物品是否与槽位物品匹配（这里简单以物品数据指针相等为例，可按需完善）
    if (sourceSlotItem.ItemData == RecipeItem.ItemData) {
        // 执行学习配方的逻辑，比如将配方添加到已学习配方列表中（这里假设存在相应的函数AddLearnedRecipe）
        if (AddLearnedRecipe(RecipeItem)) {
            Success = true;
            // 可选择从库存中移除该配方物品（这里简单示例，根据实际需求调整）
            PlayerInventoryComponent->InventorySlots.RemoveAt(SourceSlotIndex);
        }
    }
}

void UAC_Inventory::ArrayContainsItem(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    FStruct_Inventory_Slot_Item ItemToFind,
    bool& ItemFoundInArray,
    bool LocalItemFoundInArray) {
    ItemFoundInArray = false;
    LocalItemFoundInArray = false;

    // 遍历物品数组进行查找对比
    for (const FStruct_Inventory_Slot_Item& slotItem : Array) {
        // 判断当前槽位物品是否与要查找的物品匹配（这里简单以物品数据指针相等为例，可按需完善对比逻辑）
        if (slotItem.ItemData == ItemToFind.ItemData && slotItem.Amount == ItemToFind.Amount) {
            LocalItemFoundInArray = true;
            ItemFoundInArray = true;
            break;
        }
    }
}