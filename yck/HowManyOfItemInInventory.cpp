#include "UAC_Inventory.h"

// 函数实现开始，此函数用于统计库存数组中指定物品的数量，并通过相关参数返回数量信息及对应物品
void UAC_Inventory::HowManyOfItemInInventory(FStruct_Inventory_Slot_Item InventoryItem,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    int32& amountOfItemsFound,
    FStruct_Inventory_Slot_Item& ofItem,
    int32 localItemsFound) {
    amountOfItemsFound = 0;
    localItemsFound = 0;
    ofItem = FStruct_Inventory_Slot_Item();

    // 遍历库存数组
    for (const FStruct_Inventory_Slot_Item& slotItem : Array) {
        // 判断当前槽位物品是否与指定物品匹配（这里简单以物品数据指针相等为例，可按需完善）
        if (slotItem.ItemData == InventoryItem.ItemData) {
            localItemsFound++;
            amountOfItemsFound += slotItem.Amount;
        }
    }
    ofItem = InventoryItem;
}

void UAC_Inventory::InitializeCraftingQueue() {
    // 清空制作队列数组（假设类中有CraftingQueue成员变量表示制作队列）
    CraftingQueue.Empty();

    // 这里还可以进行其他与制作队列初始化相关的操作，比如重置相关的计数器、定时器等
    // 例如，重置当前制作时间为0（假设类中有CurrentCraftingTime成员变量）
    CurrentCraftingTime = 0.0;
    // 重置制作队列定时器句柄（假设类中有CraftingQueueTimerHandle成员变量）
    CraftingQueueTimerHandle.Reset();
}

void UAC_Inventory::AddItemToCraftingQueue(const FStruct_Inventory_Slot_Item& Item) {
    // 先判断制作队列是否已满（假设类中有AmountOfCraftingQueueSlots成员变量表示队列总槽位数）
    if (CraftingQueue.Num() < AmountOfCraftingQueueSlots) {
        // 将物品添加到制作队列数组中
        CraftingQueue.Add(Item);
    }
    else {
        // 这里可以添加队列已满的提示或其他处理逻辑，比如触发UI提示等
        UE_LOG(LogTemp, Warning, TEXT("Crafting queue is full, cannot add item."));
    }
}

void UAC_Inventory::CheckAmountOfFilledSlotsInQueue(int32& AmountInCraftingQueue,
    bool& AllQueueSlotsUsed,
    int32 LocalAmountInCraftingQueue) {
    AmountInCraftingQueue = CraftingQueue.Num();
    LocalAmountInCraftingQueue = AmountInCraftingQueue;
    AllQueueSlotsUsed = false;

    // 判断队列槽位是否全部被使用
    if (AmountInCraftingQueue == AmountOfCraftingQueueSlots) {
        AllQueueSlotsUsed = true;
    }
}

void UAC_Inventory::TryMoveItemAutomatically(UAC_Inventory_C* DestinationInventory,
    FStruct_Inventory_Slot_Item InventoryItemToBeMoved,
    UAC_Inventory_C* SourceInventory,
    bool& Success) {
    Success = false;

    if (!SourceInventory || !DestinationInventory) {
        return;  // 如果源库存或目标库存指针为空，无法进行移动操作，直接返回
    }

    // 遍历源库存数组，查找要移动的物品
    for (int32 i = 0; i < SourceInventory->InventorySlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& sourceSlotItem = SourceInventory->InventorySlots[i];
        if (sourceSlotItem.ItemData == InventoryItemToBeMoved.ItemData) {
            // 从源库存槽位移除物品（这里简单示例，可根据更多规则完善，比如数量处理等）
            sourceSlotItem.Amount--;
            if (sourceSlotItem.Amount == 0) {
                sourceSlotItem.ItemData = nullptr;
            }

            // 添加物品到目标库存（这里简单添加，实际可能需考虑堆叠等规则）
            DestinationInventory->InventorySlots.Add(InventoryItemToBeMoved);
            Success = true;
            break;
        }
    }
}

