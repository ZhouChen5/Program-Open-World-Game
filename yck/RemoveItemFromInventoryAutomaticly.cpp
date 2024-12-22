#include "UAC_Inventory.h"

// 函数实现开始，此函数用于自动从库存中移除指定物品及对应数量，按照规则处理并反馈移除结果
void UAC_Inventory::RemoveItemFromInventoryAutomaticly(FStruct_Inventory_Slot_Item InventoryItemToRemove,
    int32 AmountToRemove,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool& Success,
    int32& amountNotRemoved,
    int32 localAmountToRemove,
    int32 localCurrentAmountInSlot,
    int32 localRemainderToBeRemoved,
    int32 localFoundSlotIndex,
    FDataTableRowHandle localExistingSlotFound,
    int32 localAmountRemoved) {
    Success = false;
    amountNotRemoved = AmountToRemove;
    localAmountToRemove = AmountToRemove;
    localAmountRemoved = 0;

    // 遍历库存数组，查找要移除的物品所在槽位
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = TargetArray[i];
        if (currentSlot.ItemData == InventoryItemToRemove.ItemData) {
            localFoundSlotIndex = i;
            localCurrentAmountInSlot = currentSlot.Amount;
            // 判断当前槽位物品数量是否足够移除指定数量
            if (currentSlot.Amount >= AmountToRemove) {
                currentSlot.Amount -= AmountToRemove;
                localAmountRemoved = AmountToRemove;
                amountNotRemoved = 0;
                Success = true;
                break;
            }
            else {
                // 如果当前槽位物品数量不足，全部移除当前槽位物品，并更新剩余要移除的数量
                localAmountRemoved = currentSlot.Amount;
                amountNotRemoved = AmountToRemove - currentSlot.Amount;
                currentSlot.Amount = 0;
            }
        }
    }
}


void UAC_Inventory::RemoveItemAtSlotIndex(int32 SlotIndex,
    int32 Amount,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool ToBeRemovedItemIsEquiped,
    bool& Success,
    bool& EmptySlot,
    FStruct_Inventory_Slot_Item LocalInventorySlotItem,
    int32 LocalAmountToRemove,
    int32 localSlotIndex) {
    Success = false;
    EmptySlot = false;
    LocalInventorySlotItem = FStruct_Inventory_Slot_Item();
    LocalAmountToRemove = Amount;
    localSlotIndex = SlotIndex;

    // 先判断槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventorySlotItem = targetSlot;

    // 判断要移除的数量是否超过槽位中物品的现有数量
    if (targetSlot.Amount >= Amount) {
        targetSlot.Amount -= Amount;
        if (targetSlot.Amount == 0) {
            EmptySlot = true;
        }
        Success = true;
    }
}

void UAC_Inventory::InitializeStartingInventory(bool& InitialInventoryLoaded) {
    InitialInventoryLoaded = false;

    // 这里可以添加具体的初始化逻辑，比如根据预设的初始物品列表填充库存数组等
    // 假设存在一个默认的初始物品数组StartingItems（在类中定义的成员变量）
    for (const FStruct_Inventory_Slot_Item& item : StartingItems) {
        InventorySlots.Add(item);
    }
    InitialInventoryLoaded = true;
}


void UAC_Inventory::MoveItemToSlotIndex(FStruct_Inventory_Slot_Item InventoryItem,
    UAC_Inventory_C* SourceInventoryComponent,
    UAC_Inventory_C* DestinationInventory,
    int32 SourceSlotIndex,
    int32 DestinationSlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& SourceArray,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& DestinationArray,
    bool AutomaticPlacement,
    bool IsFromEquipment,
    bool& AllItemsAdded,
    FStruct_Inventory_Slot_Item localInventoryItemToBeMoved,
    UAC_Inventory_C* localDestinationInventoryComp,
    int32 localSourceSlotIndex,
    int32 localDestinationSlotIndex,
    FStruct_Inventory_Slot_Item localSourceItem,
    FStruct_Inventory_Slot_Item localDestinationItem,
    bool localIsAnEquipmentMove,
    UAC_Inventory_C* localSourceInventoryComponent,
    TArray<FStruct_Inventory_Slot_Item> LocalSourceArray,
    TArray<FStruct_Inventory_Slot_Item> LocalDestinationArray,
    bool LocalIsAutomaticPlacement,
    TEnumAsByte<Enum_Equipment_Slots> LocalToWhichEquipmentSlotShouldThisItemToBeAdded,
    FStruct_Inventory_Slot_Item LocalDestinationSlotInfo) {
    AllItemsAdded = false;
    localInventoryItemToBeMoved = InventoryItem;
    localSourceSlotIndex = SourceSlotIndex;
    localDestinationSlotIndex = DestinationSlotIndex;
    localSourceItem = FStruct_Inventory_Slot_Item();
    localDestinationItem = FStruct_Inventory_Slot_Item();
    localIsAnEquipmentMove = IsFromEquipment;
    localSourceInventoryComponent = SourceInventoryComponent;
    localDestinationInventoryComp = DestinationInventory;

    // 先判断源槽位和目标槽位索引是否合法
    if (SourceSlotIndex < 0 || SourceSlotIndex >= SourceArray.Num() ||
        DestinationSlotIndex < 0 || DestinationSlotIndex >= DestinationArray.Num()) {
        return;
    }

    localSourceItem = SourceArray[SourceSlotIndex];
    localDestinationItem = DestinationArray[DestinationSlotIndex];

    // 从源库存槽位移除物品
    SourceArray[SourceSlotIndex].Amount -= InventoryItem.Amount;
    if (SourceArray[SourceSlotIndex].Amount == 0) {
        SourceArray[SourceSlotIndex].ItemData = nullptr;
    }

    // 添加物品到目标库存槽位，这里简单示例，可根据更多规则完善，比如堆叠规则等
    if (DestinationArray[DestinationSlotIndex].ItemData == nullptr) {
        DestinationArray[DestinationSlotIndex] = InventoryItem;
    }
    else {
        DestinationArray[DestinationSlotIndex].Amount += InventoryItem.Amount;
    }

    AllItemsAdded = true;
}

void UAC_Inventory::HowManyMaxItemsAreCraftableFromInventory(FStruct_Inventory_Slot_Item WhichItemToBeMade,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    int32& NumberOfItemsCraftable,
    bool& NoItemsCanBeMade,
    int32& NumberOfItemsFitInCraftingQueue,
    int32 HowManyItemsFound,
    TArray<FStruct_Inventory_Slot_Item> LocalRecipeToBeMade,
    int32 LowestNumberOfPossibleItemsToBeCrafted) {
    NumberOfItemsCraftable = 0;
    NoItemsCanBeMade = true;
    NumberOfItemsFitInCraftingQueue = 0;
    HowManyItemsFound = 0;
    LowestNumberOfPossibleItemsToBeCrafted = 0;

    // 这里可以根据制作物品所需的材料配方，遍历库存数组来统计可制作的数量
    // 假设LocalRecipeToBeMade数组中包含了制作所需的各种材料及对应数量要求（简单示例，实际更复杂）
    for (int32 i = 0; i < LocalRecipeToBeMade.Num(); ++i) {
        FStruct_Inventory_Slot_Item requiredItem = LocalRecipeToBeMade[i];
        int32 requiredAmount = requiredItem.Amount;
        int32 foundAmount = 0;
        for (const FStruct_Inventory_Slot_Item& slotItem : TargetArray) {
            if (slotItem.ItemData == requiredItem.ItemData) {
                foundAmount += slotItem.Amount;
            }
        }
        HowManyItemsFound += foundAmount;
        // 计算基于当前材料可制作的该物品数量（取各材料可制作数量的最小值）
        int32 itemsCanBeMadeWithThisItem = foundAmount / requiredAmount;
        if (i == 0) {
            LowestNumberOfPossibleItemsToBeCrafted = itemsCanBeMadeWithThisItem;
        }
        else {
            LowestNumberOfPossibleItemsToBeCrafted = FMath::Min(LowestNumberOfPossibleItemsToBeCrafted, itemsCanBeMadeWithThisItem);
        }
    }

    if (LowestNumberOfPossibleItemsToBeCrafted > 0) {
        NoItemsCanBeMade = false;
        NumberOfItemsCraftable = LowestNumberOfPossibleItemsToBeCrafted;
    }
}

