#include "UAC_Inventory.h"

// 函数实现开始，此函数根据指定的制作行为类型以及自定义制作数量，来制作指定物品，执行相应制作逻辑
void UAC_Inventory::CraftItemByActionType(FStruct_Inventory_Slot_Item WhichItemToBeMade,
    int32 CustomAmountToBeMade,
    TEnumAsByte<Enum_CraftingAction_Types> CraftingActionType) {
    // 这里可根据不同的制作行为类型（CraftingActionType）进行不同的制作逻辑分支处理
    switch (CraftingActionType) {
        case Enum_CraftingAction_Types::SimpleCraft:
            // 简单制作逻辑示例，假设存在一个制作函数CraftSimpleItem，根据材料扣除库存并生成成品
            for (int32 i = 0; i < CustomAmountToBeMade; ++i) {
                if (CraftSimpleItem(WhichItemToBeMade)) {
                    // 如果制作成功，可进行相应的提示或后续处理（这里简单示例，可按需扩展）
                    UE_LOG(LogTemp, Log, TEXT("Successfully crafted one %s."), *WhichItemToBeMade.ItemData->Name);
                }
                else {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to craft %s due to insufficient materials."), *WhichItemToBeMade.ItemData->Name);
                }
            }
            break;
        case Enum_CraftingAction_Types::AdvancedCraft:
            // 高级制作逻辑，可能涉及更复杂的材料配方、工具需求等判断
            if (CraftAdvancedItem(WhichItemToBeMade, CustomAmountToBeMade)) {
                UE_LOG(LogTemp, Log, TEXT("Successfully crafted %d %s using advanced crafting."), CustomAmountToBeMade, *WhichItemToBeMade.ItemData->Name);
            }
            else {
                UE_LOG(LogTemp, Warning, TEXT("Advanced crafting of %d %s failed due to various reasons."), CustomAmountToBeMade, *WhichItemToBeMade.ItemData->Name);
            }
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unrecognized crafting action type."));
            break;
    }
}

void UAC_Inventory::DropItemBySlotIndex(int32 SlotIndex,
    int32 Amount,
    FStruct_Inventory_Slot_Item InventoryItem,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool IsFromHotbar) {
    // 先判断槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    // 判断槽位中的物品是否与要丢弃的物品匹配（这里简单以物品数据指针相等为例，可按需完善）
    if (targetSlot.ItemData == InventoryItem.ItemData) {
        // 如果要丢弃的数量大于等于槽位中物品的数量，直接移除整个槽位物品
        if (Amount >= targetSlot.Amount) {
            TargetArray.RemoveAt(SlotIndex);
        }
        else {
            // 否则减少对应数量的物品
            targetSlot.Amount -= Amount;
        }

        // 如果是从快捷栏丢弃，可执行额外的快捷栏相关处理逻辑（这里简单示例，可按需扩展）
        if (IsFromHotbar) {
            HandleHotbarItemDrop(SlotIndex);
        }
    }
}

void UAC_Inventory::RemoveAllEmptySlotsFromStartIndexToEndOfArray(int32 IndexToStartRemoving,
    const TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool& Completed) {
    Completed = false;

    // 从起始索引开始遍历目标数组
    for (int32 i = IndexToStartRemoving; i < TargetArray.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& slotItem = TargetArray[i];
        // 判断槽位是否为空（这里以物品数据指针是否为空为例，可按需调整判断条件）
        if (slotItem.ItemData == nullptr) {
            // 如果为空，移除该槽位（这里假设可以直接操作目标数组，实际可能需考虑更多情况）
            TArray<FStruct_Inventory_Slot_Item>& mutableTargetArray = const_cast<TArray<FStruct_Inventory_Slot_Item>&>(TargetArray);
            mutableTargetArray.RemoveAt(i);
            i--;  // 因为移除了一个元素，索引需要回退一位继续检查下一个位置
        }
    }

    Completed = true;
}

void UAC_Inventory::ExecuteOnOffLogic(bool TurnOn,
    bool SwitchState,
    bool& IsOn) {
    if (SwitchState) {
        IsOn = TurnOn;
    }
    else {
        // 如果不切换状态，保持当前的开启状态不变
        // 这里可以添加更多与当前状态相关的逻辑处理，比如执行相应的功能开启/关闭操作等
    }
}

void UAC_Inventory::ProcessCraftingQueueSlotIndex0AndInitializeTimer() {
    if (CraftingQueue.Num() > 0) {
        FStruct_Inventory_Slot_Item& firstSlotItem = CraftingQueue[0];
        // 这里可以调用制作相关函数来处理第一个槽位的物品制作，假设存在ProcessCrafting函数
        if (ProcessCrafting(firstSlotItem)) {
            // 如果制作成功，从队列中移除该槽位物品
            CraftingQueue.RemoveAt(0);
            // 初始化制作定时器（假设类中有成员变量CraftingQueueTimerHandle用于定时器操作）
            InitializeCraftingTimer();
        }
    }
}

void UAC_Inventory::CheckToStartFuelTimer(bool& FoundAndRemovedCombustibleInCurrentInventory,
    bool LocalFoundCombustibleInCurrentInventory) {
    FoundAndRemovedCombustibleInCurrentInventory = false;
    LocalFoundCombustibleInCurrentInventory = false;

    // 遍历当前库存数组，查找可燃物品（这里假设物品结构体中有标识是否可燃的成员变量IsCombustible）
    for (const FStruct_Inventory_Slot_Item& slotItem : InventorySlots) {
        if (slotItem.ItemData && slotItem.ItemData->IsCombustible) {
            LocalFoundCombustibleInCurrentInventory = true;
            // 找到可燃物品后，可移除一个（这里简单示例，减少数量，可根据实际情况调整）
            if (slotItem.Amount > 0) {
                FStruct_Inventory_Slot_Item& mutableSlotItem = const_cast<FStruct_Inventory_Slot_Item&>(slotItem);
                mutableSlotItem.Amount--;
                if (mutableSlotItem.Amount == 0) {
                    mutableSlotItem.ItemData = nullptr;
                }
                FoundAndRemovedCombustibleInCurrentInventory = true;
                break;
            }
        }
    }
}

void UAC_Inventory::FindItemIndexesToDecay(TArray<int32>& FoundItemIndexesToDecay,
    TArray<int32> localFoundItemIndexesToDecay,
    TArray<FStruct_Inventory_Slot_Item> ItemsToDecay) {
    FoundItemIndexesToDecay.Empty();
    localFoundItemIndexesToDecay.Empty();

    // 遍历需要处理的物品数组（ItemsToDecay），查找符合衰减条件的物品索引（这里假设物品有耐久度相关属性Durability）
    for (int32 i = 0; i < ItemsToDecay.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& item = ItemsToDecay[i];
        if (item.ItemData && item.ItemData->Durability > 0 && item.ItemData->Durability < item.ItemData->MaxDurability) {
            FoundItemIndexesToDecay.Add(i);
            localFoundItemIndexesToDecay.Add(i);
        }
    }
}

void UAC_Inventory::ProcessDurabilityAndDecayLogicBySlotIndex(int32 SlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool& zeroDurability,
    FStruct_Inventory_Slot_Item LocalInventorySlotItem,
    int32 LocalSlotIndex) {
    zeroDurability = false;
    LocalInventorySlotItem = FStruct_Inventory_Slot_Item();
    LocalSlotIndex = SlotIndex;

    // 先判断槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventorySlotItem = targetSlot;

    // 执行耐久度衰减逻辑，假设物品结构体中有Durability成员变量表示当前耐久度，DecayRate表示衰减速率
    targetSlot.ItemData->Durability -= targetSlot.ItemData->DecayRate;
    if (targetSlot.ItemData->Durability <= 0) {
        zeroDurability = true;
        // 耐久度降为0后，可进行相应的物品损坏处理，比如移除物品等（这里简单示例，可按需扩展）
        TargetArray.RemoveAt(SlotIndex);
    }
}

void UAC_Inventory::DecayItemBySlotIndex(int32 SlotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool& DurabilityIsZero,
    bool& NoItemsLeftToDecay,
    FStruct_Inventory_Slot_Item LocalInventorySlotItem,
    int32 LocalSlotIndex) {
    DurabilityIsZero = false;
    NoItemsLeftToDecay = false;
    LocalInventorySlotItem = FStruct_Inventory_Slot_Item();
    LocalSlotIndex = SlotIndex;

    // 先判断槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventorySlotItem = targetSlot;

    // 执行耐久度衰减操作，假设存在相关函数来处理具体的衰减逻辑，比如ApplyDecay函数
    if (ApplyDecay(targetSlot)) {
        DurabilityIsZero = targetSlot.ItemData->Durability == 0;
        if (DurabilityIsZero) {
            // 如果耐久度降为0，移除该物品
            TargetArray.RemoveAt(SlotIndex);
            // 判断是否还有其他物品需要衰减
            NoItemsLeftToDecay = true;
            for (const FStruct_Inventory_Slot_Item& item : TargetArray) {
                if (item.ItemData && item.ItemData->Durability > 0 && item.ItemData->Durability < item.ItemData->MaxDurability) {
                    NoItemsLeftToDecay = false;
                    break;
                }
            }
        }
    }
}