#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺������ݸ�������������Ŀ���������ж�Ӧ��λ����Ϊ�գ���Ʒ���������գ�
void UAC_Inventory::SetInventorySlotToEmptyByIndex(int32 Index,
    const TArray<FStruct_Inventory_Slot_Item>& TargetArray) {
    // ���ж������Ƿ�Ϸ�
    if (Index < 0 || Index >= TargetArray.Num()) {
        return;
    }

    // ��ȡĿ��������Ŀ��޸����ã����������Խ��������Ĳ�����ʵ�ʿ����迼�Ǹ������ƣ�
    TArray<FStruct_Inventory_Slot_Item>& mutableTargetArray = const_cast<TArray<FStruct_Inventory_Slot_Item>&>(TargetArray);
    FStruct_Inventory_Slot_Item& targetSlot = mutableTargetArray[Index];
    // ����λ��Ʒ���������գ������ʾ��������Ʒ����ָ����Ϊnullptr��������Ϊ0
    targetSlot.ItemData = nullptr;
    targetSlot.Amount = 0;
}

void UAC_Inventory::SetInventorySlotToEmptyByIndex(int32 Index,
    const TArray<FStruct_Inventory_Slot_Item>& TargetArray) {
    // ���ж������Ƿ�Ϸ�
    if (Index < 0 || Index >= TargetArray.Num()) {
        return;
    }

    // ��ȡĿ��������Ŀ��޸����ã����������Խ��������Ĳ�����ʵ�ʿ����迼�Ǹ������ƣ�
    TArray<FStruct_Inventory_Slot_Item>& mutableTargetArray = const_cast<TArray<FStruct_Inventory_Slot_Item>&>(TargetArray);
    FStruct_Inventory_Slot_Item& targetSlot = mutableTargetArray[Index];
    // ����λ��Ʒ���������գ������ʾ��������Ʒ����ָ����Ϊnullptr��������Ϊ0
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

    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventoryItemReference = targetSlot;

    // ����Ƿ����㹻���޸���Դ������������һ������CheckRepairResources�������Դ�Ƿ��㹻��
    if (CheckRepairResources(sourceInventory, targetSlot)) {
        // �����Դ�㹻��ִ���޸��߼���������Ʒ�ṹ������Durability��Ա������ʾ�;öȣ�MaxDurability��ʾ����;ö�
        targetSlot.ItemData->Durability = targetSlot.ItemData->MaxDurability;
        // ����Ǵӿ�����޸�����ִ�ж���Ŀ������ش����߼��������ʾ�����ɰ�����չ��
        if (IsFromHotbar) {
            HandleHotbarRepairLogic(SlotIndex);
        }
    }
}

void UAC_Inventory::Initialize_Stocking() {
    // ʾ�������ò���������������������StockingCounter��Ա�������ڼ�¼�������������
    StockingCounter = 0;

    // ���س�ʼ�������ݣ��������һ������LoadInitialStockingData���������ļ�����������Դ��������
    LoadInitialStockingData();

    // ���ﻹ���Խ��������벹����ʼ����صĲ����������ʼ����صĶ�ʱ�������ò�����صı�ǵ�
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

    // ��������Դ�Ƿ��㹻����ָ����������Ʒ������������һ������CheckBuyerResources��������Դ���
    BuyerHasResourcesForThisManyItems = CheckBuyerResources(BuyerInventoryComponent, ItemToBeBought, AmountOfItemsToBuy);
    BuyerHasSufficientResourcesToBuyCompleteAmount = BuyerHasResourcesForThisManyItems >= AmountOfItemsToBuy;

    // ������ҿ���Ƿ����㹻��������Ʒ�ɹ����ۣ��������ҿ������ΪSellerInventoryComponent->InventorySlots
    int32 sellerItemCount = 0;
    for (const FStruct_Inventory_Slot_Item& slotItem : SellerInventoryComponent->InventorySlots) {
        if (slotItem.ItemData == ItemToBeBought.ItemData) {
            sellerItemCount += slotItem.Amount;
        }
    }
    SellerHasSufficientAmountInInventory = sellerItemCount >= AmountOfItemsToBuy;

    // ���������Դ�����ҿ�����ȷ�������Ƿ�������Ʒ���ܱ������Լ�ʵ�ʿɹ��������
    AmountThatCanBeBought = FMath::Min(BuyerHasResourcesForThisManyItems, sellerItemCount);
    AllItemsCanBeBought = AmountThatCanBeBought == AmountOfItemsToBuy;
}

void UAC_Inventory::WholeArrayOfItemsCanBeFoundInInventory(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    bool& SufficientItemsFound,
    bool SufficientItems) {
    SufficientItemsFound = true;
    SufficientItems = true;

    // ����Ҫ������Ʒ����
    for (const FStruct_Inventory_Slot_Item& item : Array) {
        bool itemFound = false;
        // �ڿ�����飨�������ΪInventorySlots���в��ҵ�ǰ��Ʒ�Ƿ����
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

    // ������Ը�����Ʒ�Ļ����۸񣨼�����Ʒ�ṹ������BasePrice��Ա�������Լ����ҡ�������������������ۼ۸�
    Cost = ItemToBeBought.ItemData->BasePrice * SellerInventoryComponent->GetSellFactor();
    // �Լ۸����ȡ�������������ʾ�����ɸ���ʵ������ʹ�ø���ȷ��ȡ����ʽ��
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

    // ���жϲ�λ�����Ƿ�Ϸ��Լ����ҡ���ҿ������Ƿ���Ч
    if (SlotIndexOfItem < 0 || SlotIndexOfItem >= SellerInventoryComponent->InventorySlots.Num() ||
        !SellerInventoryComponent || !BuyerInventoryComponent) {
        return;
    }

    FStruct_Inventory_Slot_Item& sellerSlotItem = SellerInventoryComponent->InventorySlots[SlotIndexOfItem];
    // �ж����Ҳ�λ��Ʒ�Ƿ���Ҫ�������Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
    if (sellerSlotItem.ItemData == ItemToBuy.ItemData) {
        // �������Ƿ����㹻��Դ����ָ����������Ʒ��������������ؼ�麯����
        if (CheckBuyerCanAfford(ItemToBuy, BuyerInventoryComponent, AmountToBuy)) {
            // ����ҿ��۳���Ӧ��Դ�����������ڿ۳���Դ�ĺ�����
            DeductBuyerResources(ItemToBuy, BuyerInventoryComponent, AmountToBuy);

            // �����ҿ����ٶ�Ӧ��������Ʒ
            sellerSlotItem.Amount -= AmountToBuy;
            if (sellerSlotItem.Amount == 0) {
                sellerSlotItem.ItemData = nullptr;
            }

            // ���������Ʒ��ӵ���ҿ�棨�������ӣ�ʵ�ʿ����迼�Ƕѵ��ȹ���
            BuyerInventoryComponent->InventorySlots.Add(ItemToBuy);
        }
    }
}

void UAC_Inventory::ForHowManyItemsDoesTheBuyerHasResources(UAC_Inventory_C* BuyerInventoryComponent,
    const FStruct_Inventory_Slot_Item& ItemToBeBought,
    UAC_Inventory_C* SellerInventoryComponent,
    int32& BuyerHasResourcesForThisManyItems) {
    BuyerHasResourcesForThisManyItems = 0;

    // ������Ը�����ҿ����Դ����Լ���Ʒ�۸񣨼�����Ʒ�м۸�������ԣ���������ɹ��������
    // �������һ������CalculateAffordableAmount��������������Դ�ɹ������Ʒ����
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

    // ���ݲ�ͬ�Ľ�����Ϊ���ͽ��в�ͬ�Ĺ����߼�����
    switch (TradeAction) {
        case Enum_Trading_Action_Types::NormalBuy:
            TryBuyItemFromSellerBySlotIndex(ItemToBuy, SellerInventoryComponent, BuyerInventoryComponent, SlotIndexOfItem, CustomAmountToBuy);
            break;
        case Enum_Trading_Action_Types::BulkBuy:
            // ���������߼��������漰�ۿ۵ȶ��⴦�������ʾ����������ͨ�����߼����
            for (int32 i = 0; i < CustomAmountToBuy; ++i) {
                TryBuyItemFromSellerBySlotIndex(ItemToBuy, SellerInventoryComponent, BuyerInventoryComponent, SlotIndexOfItem, 1);
            }
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unrecognized trading action type."));
            break;
    }
}