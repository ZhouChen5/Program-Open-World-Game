#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺��������Զ������ҿ�湺��ָ����Ʒ�������������߼������������Ϣ
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

    // ���ж����ҡ���ҿ������Ƿ���Ч
    if (!SellerInventoryComponent || !BuyerInventoryComponent) {
        return;
    }

    // �������ҿ����Ҫ�������Ʒ���ڲ�λ������򵥱������ң��ɸ���ʵ���Ż������߼���
    for (int32 i = 0; i < SellerInventoryComponent->InventorySlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& sellerSlotItem = SellerInventoryComponent->InventorySlots[i];
        if (sellerSlotItem.ItemData == ItemToBuy.ItemData) {
            LocalSlotIndexOfItem = i;
            break;
        }
    }

    // ���û�ҵ���Ӧ��Ʒ��λ��ֱ�ӷ���
    if (LocalSlotIndexOfItem == -1) {
        return;
    }

    // �������Ƿ����㹻��Դ����ָ����������Ʒ��������������ؼ�麯����
    if (CheckBuyerCanAfford(ItemToBuy, BuyerInventoryComponent, AmountToBuy)) {
        // ����ҿ��۳���Ӧ��Դ�����������ڿ۳���Դ�ĺ�����
        DeductBuyerResources(ItemToBuy, BuyerInventoryComponent, AmountToBuy);

        // �����ҿ����ٶ�Ӧ��������Ʒ
        FStruct_Inventory_Slot_Item& sellerSlotItem = SellerInventoryComponent->InventorySlots[LocalSlotIndexOfItem];
        sellerSlotItem.Amount -= AmountToBuy;
        if (sellerSlotItem.Amount == 0) {
            sellerSlotItem.ItemData = nullptr;
        }

        // ���������Ʒ��ӵ���ҿ�棨�������ӣ�ʵ�ʿ����迼�Ƕѵ��ȹ���
        BuyerInventoryComponent->InventorySlots.Add(ItemToBuy);
    }
}

void UAC_Inventory::RestockInventory(FStruct_Inventory_Slot_Item LocalItemToBeAdded) {
    // ���������Ӿ���Ĳ����߼���������ҿղ�λ�����Ʒ�����߰��նѵ�������ӵ����в�λ��
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
        // ���û�ҵ��ղ�λ���ɰ��նѵ�������ӵ����к��ʵĲ�λ�������ʾ�����ɸ���ʵ�����ƣ�
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
    // �Ȳ��ҿ�����Ƿ����и���Ʒ�Ĳ�λ
    bool itemFound = false;
    for (int32 i = 0; i < InventorySlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = InventorySlots[i];
        if (currentSlot.ItemData == ItemToRestock.ItemData) {
            itemFound = true;
            // �ж��Ƿ��пռ�������Ʒ�����նѵ�����
            int32 availableSpace = currentSlot.MaxStackSize - currentSlot.Amount;
            if (availableSpace > 0) {
                int32 addAmount = availableSpace > LocalItemToBeAdded.Amount ? LocalItemToBeAdded.Amount : availableSpace;
                currentSlot.Amount += addAmount;
            }
            break;
        }
    }

    if (!itemFound) {
        // ��������û�и���Ʒ��������ӵ��ղ�λ���в���������򵥲��ҿղ�λ��ӣ��ɸ���ʵ���Ż���
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
    // ��ʾ����ֱ����տ�����飬��������Ʒ�Ƴ�
    InventorySlots.Empty();

    // ���ﻹ�����������������ز������������¼��س�ʼ����������ݣ����������غ�����
    LoadInitialInventoryConfig();
}

void UAC_Inventory::CheckIfEquipmentSlotIsEmpty, IfNotSetToEmpty(TEnumAsByte<Enum_Equipment_Slots> EquipmentSlot,
    bool& SlotIsEmpty,
    int32& EquipmentSlotIndex) {
    SlotIsEmpty = false;
    EquipmentSlotIndex = -1;

    // ����װ�����飨�������������EquipmentSlots��Ա������ʾװ�����飩
    for (int32 i = 0; i < EquipmentSlots.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = EquipmentSlots[i];
        if (currentSlot.ItemData != nullptr && currentSlot.ItemData->EquipmentSlot == EquipmentSlot) {
            EquipmentSlotIndex = i;
            break;
        }
    }

    if (EquipmentSlotIndex != -1) {
        // ����ҵ��˶�Ӧװ����λ��������գ������������Ʒ����ָ��Ϊnullptr������Ϊ0�ȣ�
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

    // ����װ�����������ͬ���͵�װ����λ
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

    // �ж���Ʒ�Ƿ�����װ�������������Ʒ�ṹ�����б�ʶ�Ƿ�Ϊװ���ĳ�Ա����IsEquipment��
    if (!InventoryItem.ItemData || !InventoryItem.ItemData->IsEquipment) {
        IsNotEquipment = true;
        return;
    }

    // ����װ����λ����ö��ֵ����������װ����λ���Ͷ���Enum_Equipment_Slots�ж��壩�������ʺϵĿղ�λ
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

    // ����װ��������Ҷ�Ӧװ����λ
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

    // ���ж�Դ��λ�����Ƿ�Ϸ�
    if (SourceSlotIndex < 0 || SourceSlotIndex >= TargetArray.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& sourceSlotItem = TargetArray[SourceSlotIndex];
    LocalItemToEquip = sourceSlotItem;

    // �ж���Ʒ�Ƿ��װ�������������Ʒ�ṹ�����б�ʶ�Ƿ��װ���ĳ�Ա����IsEquippable�Լ������ɫ�ȼ�RequiredLevel�ȣ�
    if (!sourceSlotItem.ItemData || !sourceSlotItem.ItemData->IsEquippable) {
        return;
    }

    // ����ɫ�ȼ��Ƿ�����װ��Ҫ�󣨼�����ڻ�ȡ��ɫ�ȼ��ĺ���GetCharacterLevel��
    if (sourceSlotItem.ItemData->RequiredLevel > GetCharacterLevel()) {
        ItemIsNotEquipedSinceCharacterDoesNotMeetLevelRequirements = true;
        return;
    }

    // �����ʺϸ���Ʒ��װ����λ��������ü�����ڵĺ�����ȷ��װ����λ��������Ʒ���͵����أ�
    TEnumAsByte<Enum_Equipment_Slots> targetEquipmentSlot = DetermineEquipmentSlot(sourceSlotItem.ItemData);
    LocalToWhichEquipmentSlotToAdd = targetEquipmentSlot;

    // ����Ӧװ����λ�Ƿ�������Ʒ
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
        // ���װ����λ������Ʒ���ж��Ƿ�Ϊͬ������Ʒ�����������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
        if (LocalItemFoundInEquipmentSlot.ItemData == sourceSlotItem.ItemData) {
            // �����ͬ���ͣ���û�ﵽ���ѵ����������Ӷѵ�����
            if (LocalItemFoundInEquipmentSlot.Amount < LocalItemFoundInEquipmentSlot.MaxStackSize) {
                int32 availableSpace = LocalItemFoundInEquipmentSlot.MaxStackSize - LocalItemFoundInEquipmentSlot.Amount;
                int32 addAmount = availableSpace > sourceSlotItem.Amount ? sourceSlotItem.Amount : availableSpace;
                LocalItemFoundInEquipmentSlot.Amount += addAmount;
                sourceSlotItem.Amount -= addAmount;
                ItemWasAlreadyEquiped_ButAddedMoreToTheStack = true;
            }
        }
        else {
            // �������ͬ���ͣ�������Ʒ�����߼�
            RemovedItem = LocalItemFoundInEquipmentSlot;
            EquipmentSlots[LocalFoundIndexSlotInEquipmentArray] = sourceSlotItem;
            sourceSlotItem = RemovedItem;
            ItemIsSwapped = true;
        }
    }
    else {
        // ���װ����λΪ�գ�ֱ�ӽ���Ʒװ�����ò�λ
        EquipmentSlots.Add(sourceSlotItem);
        sourceSlotItem.ItemData = nullptr;
        sourceSlotItem.Amount = 0;
    }

    ItemIsEquiped = true;
    EquipedItem = LocalItemFoundInEquipmentSlot;
    EquipedToThisEquipementSlot = targetEquipmentSlot;
}

