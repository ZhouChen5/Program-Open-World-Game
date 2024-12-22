#include "UAC_Inventory.h"

// 函数实现开始，此函数根据给定的索引，将目标库存数组中对应槽位设置为空（物品相关数据清空）
void UAC_Inventory::SetInventorySlotToEmptyByIndex(int32 Index,
    const TArray<FStruct_Inventory_Slot_Item>& TargetArray) {
    // 先判断索引是否合法
    if (Index < 0 || Index >= TargetArray.Num()) {
        return;
    }

    // 获取目标库存数组的可修改引用（这里假设可以进行这样的操作，实际可能需考虑更多限制）
    TArray<FStruct_Inventory_Slot_Item>& mutableTargetArray = const_cast<TArray<FStruct_Inventory_Slot_Item>&>(TargetArray);
    FStruct_Inventory_Slot_Item& targetSlot = mutableTargetArray[Index];
    // 将槽位物品相关数据清空，这里简单示例，将物品数据指针设为nullptr，数量设为0
    targetSlot.ItemData = nullptr;
    targetSlot.Amount = 0;
}

void UAC_Inventory::SetInventorySlotToEmptyByIndex(int32 Index,
    const TArray<FStruct_Inventory_Slot_Item>& TargetArray) {
    // 先判断索引是否合法
    if (Index < 0 || Index >= TargetArray.Num()) {
        return;
    }

    // 获取目标库存数组的可修改引用（这里假设可以进行这样的操作，实际可能需考虑更多限制）
    TArray<FStruct_Inventory_Slot_Item>& mutableTargetArray = const_cast<TArray<FStruct_Inventory_Slot_Item>&>(TargetArray);
    FStruct_Inventory_Slot_Item& targetSlot = mutableTargetArray[Index];
    // 将槽位物品相关数据清空，这里简单示例，将物品数据指针设为nullptr，数量设为0
    targetSlot.ItemData = nullptr;
    targetSlot.Amount = 0;
}

void UAC_Inventory::TryRepairItemBySlotIndex(int32 SlotIndex,
    UAC_Inventory_C* sourceInventory,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool IsFromHotbar,
    FStruct_Inventory_Slot_Item LocalInventoryItemReference,
    int32 localSlotIndex) {
    LocalInventoryItemReference = FStruct_Inventory_Slot_Item();
    localSlotIndex = SlotIndex;

    // 先判断槽位索引是否合法
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventoryItemReference = targetSlot;

    // 检查是否有足够的修复资源（这里假设存在一个函数CheckRepairResources来检查资源是否足够）
    if (CheckRepairResources(sourceInventory, targetSlot)) {
        // 如果资源足够，执行修复逻辑，假设物品结构体中有Durability成员变量表示耐久度，MaxDurability表示最大耐久度
        targetSlot.ItemData->Durability = targetSlot.ItemData->MaxDurability;
        // 如果是从快捷栏修复，可执行额外的快捷栏相关处理逻辑（这里简单示例，可按需扩展）
        if (IsFromHotbar) {
            HandleHotbarRepairLogic(SlotIndex);
        }
    }
}

void UAC_Inventory::Initialize_Stocking() {
    // 示例：重置补货计数器（假设类中有StockingCounter成员变量用于记录补货相关数量）
    StockingCounter = 0;

    // 加载初始补货数据，假设存在一个函数LoadInitialStockingData来从配置文件或其他数据源加载数据
    LoadInitialStockingData();

    // 这里还可以进行其他与补货初始化相关的操作，比如初始化相关的定时器、设置补货相关的标记等
    InitializeStockingTimer();
    IsStockingInitialized = true;
}

void UAC_Inventory::CanAllItemsBeBought(const FStruct_Inventory_Slot_Item& ItemToBeBought,
    UAC_Inventory_C* SellerInventoryComponent,
    UAC_Inventory_C* BuyerInventoryComponent,
    int32 AmountOfItemsToBuy,
    int32& BuyerHasResourcesForThisManyItems,
    bool& BuyerHasSufficientResourcesToBuyCompleteAmount,
    bool& SellerHasSufficientAmountInInventory,
    bool& AllItemsCanBeBought,
    int32& AmountThatCanBeBought) {
    BuyerHasResourcesForThisManyItems = 0;
    BuyerHasSufficientResourcesToBuyCompleteAmount = false;
    SellerHasSufficientAmountInInventory = false;
    AllItemsCanBeBought = false;
    AmountThatCanBeBought = 0;

    // 检查买家资源是否足够购买指定数量的物品，这里假设存在一个函数CheckBuyerResources来进行资源检查
    BuyerHasResourcesForThisManyItems = CheckBuyerResources(BuyerInventoryComponent, ItemToBeBought, AmountOfItemsToBuy);
    BuyerHasSufficientResourcesToBuyCompleteAmount = BuyerHasResourcesForThisManyItems >= AmountOfItemsToBuy;

    // 检查卖家库存是否有足够数量的物品可供出售，假设卖家库存数组为SellerInventoryComponent->InventorySlots
    int32 sellerItemCount = 0;
    for (const FStruct_Inventory_Slot_Item& slotItem : SellerInventoryComponent->InventorySlots) {
        if (slotItem.ItemData == ItemToBeBought.ItemData) {
            sellerItemCount += slotItem.Amount;
        }
    }
    SellerHasSufficientAmountInInventory = sellerItemCount >= AmountOfItemsToBuy;

    // 根据买家资源和卖家库存情况确定最终是否所有物品都能被购买以及实际可购买的数量
    AmountThatCanBeBought = FMath::Min(BuyerHasResourcesForThisManyItems, sellerItemCount);
    AllItemsCanBeBought = AmountThatCanBeBought == AmountOfItemsToBuy;
}

void UAC_Inventory::WholeArrayOfItemsCanBeFoundInInventory(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    bool& SufficientItemsFound,
    bool SufficientItems) {
    SufficientItemsFound = true;
    SufficientItems = true;

    // 遍历要检查的物品数组
    for (const FStruct_Inventory_Slot_Item& item : Array) {
        bool itemFound = false;
        // 在库存数组（这里假设为InventorySlots）中查找当前物品是否存在
        for (const FStruct_Inventory_Slot_Item& slotItem : InventorySlots) {
            if (slotItem.ItemData == item.ItemData && slotItem.Amount >= item.Amount) {
                itemFound = true;
                break;
            }
        }

        if (!itemFound) {
            SufficientItemsFound = false;
            SufficientItems = false;
            break;
        }
    }
}

void UAC_Inventory::CalculateSellCostsOfOneItem(const FStruct_Inventory_Slot_Item& ItemToBeBought,
    UAC_Inventory_C* SellerInventoryComponent,
    UAC_Inventory_C* BuyerInventoryComponent,
    double& Cost,
    FStruct_Inventory_Slot_Item& OfItem,
    int32& RoundedCost) {
    Cost = 0.0;
    OfItem = ItemToBeBought;
    RoundedCost = 0;

    // 这里可以根据物品的基础价格（假设物品结构体中有BasePrice成员变量）以及卖家、买家相关因素来计算出售价格
    Cost = ItemToBeBought.ItemData->BasePrice * SellerInventoryComponent->GetSellFactor();
    // 对价格进行取整操作（这里简单示例，可根据实际需求使用更精确的取整方式）
    RoundedCost = FMath::RoundToInt(Cost);
}

void UAC_Inventory::TryBuyItemFromSellerBySlotIndex(FStruct_Inventory_Slot_Item ItemToBuy,
    UAC_Inventory_C* SellerInventoryComponent,
    UAC_Inventory_C* BuyerInventoryComponent,
    int32 SlotIndexOfItem,
    int32 AmountToBuy,
    FStruct_Inventory_Slot_Item LocalItemToBuy,
    UAC_Inventory_C* LocalSellerInventoryComponent,
    UAC_Inventory_C* LocalBuyerInventoryComponent,
    int32 LocalAmountToBuy,
    int32 LocalSlotIndexOfItem) {
    LocalItemToBuy = ItemToBuy;
    LocalSlotIndexOfItem = SlotIndexOfItem;
    LocalAmountToBuy = AmountToBuy;
    LocalSellerInventoryComponent = SellerInventoryComponent;
    LocalBuyerInventoryComponent = BuyerInventoryComponent;

    // 先判断槽位索引是否合法以及卖家、买家库存组件是否有效
    if (SlotIndexOfItem < 0 || SlotIndexOfItem >= SellerInventoryComponent->InventorySlots.Num() ||
        !SellerInventoryComponent || !BuyerInventoryComponent) {
        return;
    }

    FStruct_Inventory_Slot_Item& sellerSlotItem = SellerInventoryComponent->InventorySlots[SlotIndexOfItem];
    // 判断卖家槽位物品是否与要购买的物品匹配（这里简单以物品数据指针相等为例，可按需完善）
    if (sellerSlotItem.ItemData == ItemToBuy.ItemData) {
        // 检查买家是否有足够资源购买指定数量的物品（这里假设存在相关检查函数）
        if (CheckBuyerCanAfford(ItemToBuy, BuyerInventoryComponent, AmountToBuy)) {
            // 从买家库存扣除相应资源（这里假设存在扣除资源的函数）
            DeductBuyerResources(ItemToBuy, BuyerInventoryComponent, AmountToBuy);

            // 从卖家库存减少对应数量的物品
            sellerSlotItem.Amount -= AmountToBuy;
            if (sellerSlotItem.Amount == 0) {
                sellerSlotItem.ItemData = nullptr;
            }

            // 将购买的物品添加到买家库存（这里简单添加，实际可能需考虑堆叠等规则）
            BuyerInventoryComponent->InventorySlots.Add(ItemToBuy);
        }
    }
}

void UAC_Inventory::ForHowManyItemsDoesTheBuyerHasResources(UAC_Inventory_C* BuyerInventoryComponent,
    const FStruct_Inventory_Slot_Item& ItemToBeBought,
    UAC_Inventory_C* SellerInventoryComponent,
    int32& BuyerHasResourcesForThisManyItems) {
    BuyerHasResourcesForThisManyItems = 0;

    // 这里可以根据买家库存资源情况以及物品价格（假设物品有价格相关属性）等来计算可购买的数量
    // 假设存在一个函数CalculateAffordableAmount来计算基于买家资源可购买的物品数量
    BuyerHasResourcesForThisManyItems = CalculateAffordableAmount(BuyerInventoryComponent, ItemToBeBought);
}

void UAC_Inventory::ProcessBuyItemByTradeAction(TEnumAsByte<Enum_Trading_Action_Types> TradeAction,
    FStruct_Inventory_Slot_Item ItemToBuy,
    UAC_Inventory_C* SellerInventoryComponent,
    UAC_Inventory_C* BuyerInventoryComponent,
    int32 CustomAmountToBuy,
    int32 SlotIndexOfItem,
    FStruct_Inventory_Slot_Item LocalItemToBuy,
    UAC_Inventory_C* LocalSellerInventoryComponent,
    UAC_Inventory_C* LocalBuyerInventoryComponent,
    int32 LocalCustomAmountToBuy,
    int32 LocalSlotIndex,
    int32 LocalAmountToBuy) {
    LocalItemToBuy = ItemToBuy;
    LocalSlotIndex = SlotIndexOfItem;
    LocalCustomAmountToBuy = CustomAmountToBuy;
    LocalSellerInventoryComponent = SellerInventoryComponent;
    LocalBuyerInventoryComponent = BuyerInventoryComponent;
    LocalAmountToBuy = CustomAmountToBuy;

    // 根据不同的交易行为类型进行不同的购买逻辑处理
    switch (TradeAction) {
        case Enum_Trading_Action_Types::NormalBuy:
            TryBuyItemFromSellerBySlotIndex(ItemToBuy, SellerInventoryComponent, BuyerInventoryComponent, SlotIndexOfItem, CustomAmountToBuy);
            break;
        case Enum_Trading_Action_Types::BulkBuy:
            // 批量购买逻辑，可能涉及折扣等额外处理，这里简单示例，调用普通购买逻辑多次
            for (int32 i = 0; i < CustomAmountToBuy; ++i) {
                TryBuyItemFromSellerBySlotIndex(ItemToBuy, SellerInventoryComponent, BuyerInventoryComponent, SlotIndexOfItem, 1);
            }
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unrecognized trading action type."));
            break;
    }
}