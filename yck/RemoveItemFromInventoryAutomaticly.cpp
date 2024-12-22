#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺��������Զ��ӿ�����Ƴ�ָ����Ʒ����Ӧ���������չ����������Ƴ����
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

    // ����������飬����Ҫ�Ƴ�����Ʒ���ڲ�λ
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = TargetArray[i];
        if (currentSlot.ItemData == InventoryItemToRemove.ItemData) {
            localFoundSlotIndex = i;
            localCurrentAmountInSlot = currentSlot.Amount;
            // �жϵ�ǰ��λ��Ʒ�����Ƿ��㹻�Ƴ�ָ������
            if (currentSlot.Amount >= AmountToRemove) {
                currentSlot.Amount -= AmountToRemove;
                localAmountRemoved = AmountToRemove;
                amountNotRemoved = 0;
                Success = true;
                break;
            }
            else {
                // �����ǰ��λ��Ʒ�������㣬ȫ���Ƴ���ǰ��λ��Ʒ��������ʣ��Ҫ�Ƴ�������
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

    // ���жϲ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = TargetArray[SlotIndex];
    LocalInventorySlotItem = targetSlot;

    // �ж�Ҫ�Ƴ��������Ƿ񳬹���λ����Ʒ����������
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

    // ���������Ӿ���ĳ�ʼ���߼����������Ԥ��ĳ�ʼ��Ʒ�б�����������
    // �������һ��Ĭ�ϵĳ�ʼ��Ʒ����StartingItems�������ж���ĳ�Ա������
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

    // ���ж�Դ��λ��Ŀ���λ�����Ƿ�Ϸ�
    if (SourceSlotIndex < 0 || SourceSlotIndex >= SourceArray.Num() ||
        DestinationSlotIndex < 0 || DestinationSlotIndex >= DestinationArray.Num()) {
        return;
    }

    localSourceItem = SourceArray[SourceSlotIndex];
    localDestinationItem = DestinationArray[DestinationSlotIndex];

    // ��Դ����λ�Ƴ���Ʒ
    SourceArray[SourceSlotIndex].Amount -= InventoryItem.Amount;
    if (SourceArray[SourceSlotIndex].Amount == 0) {
        SourceArray[SourceSlotIndex].ItemData = nullptr;
    }

    // �����Ʒ��Ŀ�����λ�������ʾ�����ɸ��ݸ���������ƣ�����ѵ������
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

    // ������Ը���������Ʒ����Ĳ����䷽���������������ͳ�ƿ�����������
    // ����LocalRecipeToBeMade�����а�������������ĸ��ֲ��ϼ���Ӧ����Ҫ�󣨼�ʾ����ʵ�ʸ����ӣ�
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
        // ������ڵ�ǰ���Ͽ������ĸ���Ʒ������ȡ�����Ͽ�������������Сֵ��
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

