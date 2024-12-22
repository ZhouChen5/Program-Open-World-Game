#include "UAC_Inventory.h"

// 函数实现开始，此函数尝试自动从卖家库存购买指定物品，按规则处理购买逻辑并反馈相关信息
void UAC_Inventory::TryToAutomaticlyBuyItemFromSeller(FStruct_Inventory_Slot_Item ItemToBuy,
    UAC_Inventory_C* SellerInventoryComponent,
    UAC_Inventory_C* BuyerInventoryComponent,
    int32 AmountToBuy,
    FStruct_Inventory_Slot_Item LocalItemToBuy,
    UAC_Inventory_C* LocalSellerInventoryComponent,
    UAC_Inventory_C* LocalBuyerInventoryComponent,
    int32 LocalAmountToBuy,
    int32 LocalSlotIndexOfItem) {
    LocalItemToBuy = ItemToBuy;
    LocalSellerInventoryComponent = SellerInventoryComponent;
    LocalBuyerInventoryComponent = BuyerInventoryComponent;
    LocalAmountToBuy = AmountToBuy;
    LocalSlotIndexOfItem = -1;

    // 先判断卖家、买家库存组件是否有效
    if (!SellerInventoryComponent || !BuyerInventoryComponent) {
        return;
    }

    // 查找卖家库存中要购买的物品所在槽位（这里简单遍历查找，可根据实际优化查找逻辑）
    for (int32 i = 0; i < SellerInventoryComponent->InventorySlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& sellerSlotItem = SellerInventoryComponent->InventorySlots[i];
        if (sellerSlotItem.ItemData == ItemToBuy.ItemData) {
            LocalSlotIndexOfItem = i;
            break;
        }
    }

    // 如果没找到对应物品槽位，直接返回
    if (LocalSlotIndexOfItem == -1) {
        return;
    }

    // 检查买家是否有足够资源购买指定数量的物品（这里假设存在相关检查函数）
    if (CheckBuyerCanAfford(ItemToBuy, BuyerInventoryComponent, AmountToBuy)) {
        // 从买家库存扣除相应资源（这里假设存在扣除资源的函数）
        DeductBuyerResources(ItemToBuy, BuyerInventoryComponent, AmountToBuy);

        // 从卖家库存减少对应数量的物品
        FStruct_Inventory_Slot_Item& sellerSlotItem = SellerInventoryComponent->InventorySlots[LocalSlotIndexOfItem];
        sellerSlotItem.Amount -= AmountToBuy;
        if (sellerSlotItem.Amount == 0) {
            sellerSlotItem.ItemData = nullptr;
        }

        // 将购买的物品添加到买家库存（这里简单添加，实际可能需考虑堆叠等规则）
        BuyerInventoryComponent->InventorySlots.Add(ItemToBuy);
    }
}

void UAC_Inventory::RestockInventory(FStruct_Inventory_Slot_Item LocalItemToBeAdded) {
    // 这里可以添加具体的补货逻辑，比如查找空槽位添加物品，或者按照堆叠规则添加到已有槽位等
    bool foundEmptySlot = false;
    for (int32 i = 0; i < InventorySlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = InventorySlots[i];
        if (currentSlot.ItemData == nullptr) {
            currentSlot = LocalItemToBeAdded;
            foundEmptySlot = true;
            break;
        }
    }

    if (!foundEmptySlot) {
        // 如果没找到空槽位，可按照堆叠规则添加到已有合适的槽位（这里简单示例，可根据实际完善）
        for (int32 i = 0; i < InventorySlots.Num(); ++i) {
            FStruct_Inventory_Slot_Item& currentSlot = InventorySlots[i];
            if (currentSlot.ItemData == LocalItemToBeAdded.ItemData &&
                currentSlot.Amount < currentSlot.MaxStackSize) {
                int32 availableSpace = currentSlot.MaxStackSize - currentSlot.Amount;
                int32 addAmount = availableSpace > LocalItemToBeAdded.Amount ? LocalItemToBeAdded.Amount : availableSpace;
                currentSlot.Amount += addAmount;
                break;
            }
        }
    }
}

void UAC_Inventory::TryRestockItem(const FStruct_Inventory_Slot_Item& ItemToRestock,
    FStruct_Inventory_Slot_Item LocalItemToBeAdded) {
    // 先查找库存中是否已有该物品的槽位
    bool itemFound = false;
    for (int32 i = 0; i < InventorySlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = InventorySlots[i];
        if (currentSlot.ItemData == ItemToRestock.ItemData) {
            itemFound = true;
            // 判断是否还有空间可添加物品（按照堆叠规则）
            int32 availableSpace = currentSlot.MaxStackSize - currentSlot.Amount;
            if (availableSpace > 0) {
                int32 addAmount = availableSpace > LocalItemToBeAdded.Amount ? LocalItemToBeAdded.Amount : availableSpace;
                currentSlot.Amount += addAmount;
            }
            break;
        }
    }

    if (!itemFound) {
        // 如果库存中没有该物品，尝试添加到空槽位进行补货（这里简单查找空槽位添加，可根据实际优化）
        for (int32 i = 0; i < InventorySlots.Num(); ++i) {
            FStruct_Inventory_Slot_Item& currentSlot = InventorySlots[i];
            if (currentSlot.ItemData == nullptr) {
                currentSlot = LocalItemToBeAdded;
                break;
            }
        }
    }
}

void UAC_Inventory::ResetInventoryStock() {
    // 简单示例，直接清空库存数组，将所有物品移除
    InventorySlots.Empty();

    // 这里还可以添加其他重置相关操作，比如重新加载初始库存配置数据（假设存在相关函数）
    LoadInitialInventoryConfig();
}

void UAC_Inventory::CheckIfEquipmentSlotIsEmpty, IfNotSetToEmpty(TEnumAsByte<Enum_Equipment_Slots> EquipmentSlot,
    bool& SlotIsEmpty,
    int32& EquipmentSlotIndex) {
    SlotIsEmpty = false;
    EquipmentSlotIndex = -1;

    // 遍历装备数组（这里假设类中有EquipmentSlots成员变量表示装备数组）
    for (int32 i = 0; i < EquipmentSlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = EquipmentSlots[i];
        if (currentSlot.ItemData != nullptr && currentSlot.ItemData->EquipmentSlot == EquipmentSlot) {
            EquipmentSlotIndex = i;
            break;
        }
    }

    if (EquipmentSlotIndex != -1) {
        // 如果找到了对应装备槽位，将其清空（这里简单设置物品数据指针为nullptr，数量为0等）
        FStruct_Inventory_Slot_Item& targetSlot = EquipmentSlots[EquipmentSlotIndex];
        targetSlot.ItemData = nullptr;
        targetSlot.Amount = 0;
        SlotIsEmpty = true;
    }
}

void UAC_Inventory::GetEquipmentSlotIndexByEquipmentSlotType(TEnumAsByte<Enum_Equipment_Slots> EquipmentSlot,
    int32& FoundSlotIndexInEquipmentArrayOfSameEquipmentSlotType,
    bool& FoundSameEquipmentSlotType,
    FStruct_Inventory_Slot_Item& InventoryItemInEquipmentSlot,
    int32 LocalFoundSlotIndexOfSameEquipmentSlotType,
    bool LocalFoundSameEquipmentSlotType) {
    FoundSlotIndexInEquipmentArrayOfSameEquipmentSlotType = -1;
    FoundSameEquipmentSlotType = false;
    InventoryItemInEquipmentSlot = FStruct_Inventory_Slot_Item();
    LocalFoundSlotIndexOfSameEquipmentSlotType = -1;
    LocalFoundSameEquipmentSlotType = false;

    // 遍历装备数组查找相同类型的装备槽位
    for (int32 i = 0; i < EquipmentSlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = EquipmentSlots[i];
        if (currentSlot.ItemData != nullptr && currentSlot.ItemData->EquipmentSlot == EquipmentSlot) {
            FoundSlotIndexInEquipmentArrayOfSameEquipmentSlotType = i;
            FoundSameEquipmentSlotType = true;
            InventoryItemInEquipmentSlot = currentSlot;
            break;
        }
    }

    LocalFoundSlotIndexOfSameEquipmentSlotType = FoundSlotIndexInEquipmentArrayOfSameEquipmentSlotType;
    LocalFoundSameEquipmentSlotType = FoundSameEquipmentSlotType;
}

void UAC_Inventory::ToWhichEquipmentSlotShouldThisItemBeAdded(FStruct_Inventory_Slot_Item InventoryItem,
    TEnumAsByte<Enum_Equipment_Slots>& EquipmentSlotToAdd,
    bool& EmptySlotFound,
    bool& IsNotEquipment,
    TEnumAsByte<Enum_Equipment_Slots> LocalEquipmentSlotToAdd,
    bool FoundAnEmptySlotToAddTo) {
    EquipmentSlotToAdd = Enum_Equipment_Slots::None;
    EmptySlotFound = false;
    IsNotEquipment = false;
    LocalEquipmentSlotToAdd = Enum_Equipment_Slots::None;
    FoundAnEmptySlotToAddTo = false;

    // 判断物品是否属于装备（这里假设物品结构体中有标识是否为装备的成员变量IsEquipment）
    if (!InventoryItem.ItemData || !InventoryItem.ItemData->IsEquipment) {
        IsNotEquipment = true;
        return;
    }

    // 遍历装备槽位类型枚举值（假设所有装备槽位类型都在Enum_Equipment_Slots中定义），查找适合的空槽位
    for (int32 i = 0; i < static_cast<int32>(Enum_Equipment_Slots::Max); ++i) {
        TEnumAsByte<Enum_Equipment_Slots> currentSlotType = static_cast<TEnumAsByte<Enum_Equipment_Slots>>(i);
        bool slotIsEmpty = true;
        for (const FStruct_Inventory_Slot_Item& slotItem : EquipmentSlots) {
            if (slotItem.ItemData != nullptr && slotItem.ItemData->EquipmentSlot == currentSlotType) {
                slotIsEmpty = false;
                break;
            }
        }

        if (slotIsEmpty) {
            EquipmentSlotToAdd = currentSlotType;
            EmptySlotFound = true;
            FoundAnEmptySlotToAddTo = true;
            LocalEquipmentSlotToAdd = currentSlotType;
            break;
        }
    }
}

void UAC_Inventory::CheckIfEquipmentSlotIsEmpty(TEnumAsByte<Enum_Equipment_Slots> EquipmentSlot,
    bool& Empty,
    FStruct_Inventory_Slot_Item& InventoryItemInSlot,
    int32& AtSlotIndexInEquipmentArrayOfSameEquipmentSlotType) {
    Empty = false;
    InventoryItemInSlot = FStruct_Inventory_Slot_Item();
    AtSlotIndexInEquipmentArrayOfSameEquipmentSlotType = -1;

    // 遍历装备数组查找对应装备槽位
    for (int32 i = 0; i < EquipmentSlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = EquipmentSlots[i];
        if (currentSlot.ItemData != nullptr && currentSlot.ItemData->EquipmentSlot == EquipmentSlot) {
            AtSlotIndexInEquipmentArrayOfSameEquipmentSlotType = i;
            InventoryItemInSlot = currentSlot;
            break;
        }
    }

    if (AtSlotIndexInEquipmentArrayOfSameEquipmentSlotType != -1) {
        Empty = false;
    }
    else {
        Empty = true;
    }
}

void UAC_Inventory::AutomaticlyEquipItem(FStruct_Inventory_Slot_Item ItemToEquip,
    int32 SourceSlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool IsFromHotbar,
    bool& ItemIsEquiped,
    FStruct_Inventory_Slot_Item& EquipedItem,
    TEnumAsByte<Enum_Equipment_Slots>& EquipedToThisEquipementSlot,
    bool& ItemWasAlreadyEquiped_ButAddedMoreToTheStack,
    bool& ItemIsSwapped,
    FStruct_Inventory_Slot_Item& RemovedItem,
    bool& ItemIsNotEquipedSinceCharacterDoesNotMeetLevelRequirements,
    FStruct_Inventory_Slot_Item LocalItemToEquip,
    int32 LocalFoundIndexSlotInEquipmentArray,
    TEnumAsByte<Enum_Equipment_Slots> LocalToWhichEquipmentSlotToAdd,
    int32 LocalSourceSlotIndex,
    FStruct_Inventory_Slot_Item LocalItemFoundInEquipmentSlot) {
    ItemIsEquiped = false;
    ItemWasAlreadyEquiped_ButAddedMoreToTheStack = false;
    ItemIsSwapped = false;
    ItemIsNotEquipedSinceCharacterDoesNotMeetLevelRequirements = false;
    EquipedItem = FStruct_Inventory_Slot_Item();
    RemovedItem = FStruct_Inventory_Slot_Item();
    EquipedToThisEquipementSlot = Enum_Equipment_Slots::None;
    LocalItemToEquip = ItemToEquip;
    LocalSourceSlotIndex = SourceSlotIndex;

    // 先判断源槽位索引是否合法
    if (SourceSlotIndex < 0 || SourceSlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& sourceSlotItem = TargetArray[SourceSlotIndex];
    LocalItemToEquip = sourceSlotItem;

    // 判断物品是否可装备（这里假设物品结构体中有标识是否可装备的成员变量IsEquippable以及所需角色等级RequiredLevel等）
    if (!sourceSlotItem.ItemData || !sourceSlotItem.ItemData->IsEquippable) {
        return;
    }

    // 检查角色等级是否满足装备要求（假设存在获取角色等级的函数GetCharacterLevel）
    if (sourceSlotItem.ItemData->RequiredLevel > GetCharacterLevel()) {
        ItemIsNotEquipedSinceCharacterDoesNotMeetLevelRequirements = true;
        return;
    }

    // 查找适合该物品的装备槽位（这里调用假设存在的函数来确定装备槽位，根据物品类型等因素）
    TEnumAsByte<Enum_Equipment_Slots> targetEquipmentSlot = DetermineEquipmentSlot(sourceSlotItem.ItemData);
    LocalToWhichEquipmentSlotToAdd = targetEquipmentSlot;

    // 检查对应装备槽位是否已有物品
    bool slotIsOccupied = false;
    for (const FStruct_Inventory_Slot_Item& equipmentSlotItem : EquipmentSlots) {
        if (equipmentSlotItem.ItemData != nullptr && equipmentSlotItem.ItemData->EquipmentSlot == targetEquipmentSlot) {
            slotIsOccupied = true;
            LocalFoundIndexSlotInEquipmentArray = GetEquipmentSlotIndex(targetEquipmentSlot);
            LocalItemFoundInEquipmentSlot = equipmentSlotItem;
            break;
        }
    }

    if (slotIsOccupied) {
        // 如果装备槽位已有物品，判断是否为同类型物品（这里简单以物品数据指针相等为例，可按需完善）
        if (LocalItemFoundInEquipmentSlot.ItemData == sourceSlotItem.ItemData) {
            // 如果是同类型，且没达到最大堆叠数量，增加堆叠数量
            if (LocalItemFoundInEquipmentSlot.Amount < LocalItemFoundInEquipmentSlot.MaxStackSize) {
                int32 availableSpace = LocalItemFoundInEquipmentSlot.MaxStackSize - LocalItemFoundInEquipmentSlot.Amount;
                int32 addAmount = availableSpace > sourceSlotItem.Amount ? sourceSlotItem.Amount : availableSpace;
                LocalItemFoundInEquipmentSlot.Amount += addAmount;
                sourceSlotItem.Amount -= addAmount;
                ItemWasAlreadyEquiped_ButAddedMoreToTheStack = true;
            }
        }
        else {
            // 如果不是同类型，进行物品交换逻辑
            RemovedItem = LocalItemFoundInEquipmentSlot;
            EquipmentSlots[LocalFoundIndexSlotInEquipmentArray] = sourceSlotItem;
            sourceSlotItem = RemovedItem;
            ItemIsSwapped = true;
        }
    }
    else {
        // 如果装备槽位为空，直接将物品装备到该槽位
        EquipmentSlots.Add(sourceSlotItem);
        sourceSlotItem.ItemData = nullptr;
        sourceSlotItem.Amount = 0;
    }

    ItemIsEquiped = true;
    EquipedItem = LocalItemFoundInEquipmentSlot;
    EquipedToThisEquipementSlot = targetEquipmentSlot;
}

