#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺��������Զ���ָ����װ����Ʒж�£���жװ����������жװ���
void UAC_Inventory::AutomaticlyUnequipItem(FStruct_Inventory_Slot_Item ItemToUnequip,
    int32 SourceIndexFromEquipmentArray,
    bool& ItemIsUnequiped) {
    ItemIsUnequiped = false;

    // ���ж�Դ�����Ƿ�Ϸ������װ�����飩
    if (SourceIndexFromEquipmentArray < 0 || SourceIndexFromEquipmentArray >= EquipmentSlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& targetSlot = EquipmentSlots[SourceIndexFromEquipmentArray];
    // �ж�Ҫж�µ���Ʒ�Ƿ���ò�λ����Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
    if (targetSlot.ItemData == ItemToUnequip.ItemData) {
        // ��װ����λ����Ʒ�Ƴ����������������Ϊ�գ������ʾ�����ɸ���ʵ�ʵ������������
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

    // ��ȡ��ɫ��Ӧ��3Dģ�������������������г�Ա����GetCharacterModel��ȡģ��������ɸ���ʵ�ʵ�����
    UMeshComponent* characterModel = GetCharacterModel();
    if (!characterModel) {
        return;
    }

    if (IsRemoving) {
        // ������Ƴ��������Ӷ�Ӧ��۽��װ�����Ӿ��󶨣����������ڽ���󶨵ĺ���DetachFromSocket������ʵ�ʽӿڵ�����
        characterModel->DetachFromSocket(LocalSocketName);
    }
    else {
        // ����װ�����͵����������Ӧ�ľ�̬�������������ȣ��󶨵�ָ����ۣ�ʵ��װ����ʾЧ��
        if (EquipmentSlotToChange == Enum_Equipment_Slots::Quiver && QuiverMesh) {
            characterModel->AttachToSocket(LocalSocketName, FAttachmentTransformRules::KeepRelativeTransform);
        }
        // ���ﻹ������Ӹ�����Բ�ͬװ�����͵��Ӿ����߼�������������ͷ����װ������ʾ����
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

    // ���ж�Դ��λ�����Ƿ�Ϸ��������ҿ������Ŀ�����飩
    if (SourceSlotIndex < 0 || SourceSlotIndex >= PlayerInventoryComponent->InventorySlots.Num()) {
        return;
    }

    FStruct_Inventory_Slot_Item& sourceSlotItem = PlayerInventoryComponent->InventorySlots[SourceSlotIndex];
    LocalItemToLearn = sourceSlotItem;

    // �ж�Ҫѧϰ���䷽��Ʒ�Ƿ����λ��Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƣ�
    if (sourceSlotItem.ItemData == RecipeItem.ItemData) {
        // ִ��ѧϰ�䷽���߼������罫�䷽��ӵ���ѧϰ�䷽�б��У�������������Ӧ�ĺ���AddLearnedRecipe��
        if (AddLearnedRecipe(RecipeItem)) {
            Success = true;
            // ��ѡ��ӿ�����Ƴ����䷽��Ʒ�������ʾ��������ʵ�����������
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

    // ������Ʒ������в��ҶԱ�
    for (const FStruct_Inventory_Slot_Item& slotItem : Array) {
        // �жϵ�ǰ��λ��Ʒ�Ƿ���Ҫ���ҵ���Ʒƥ�䣨���������Ʒ����ָ�����Ϊ�����ɰ������ƶԱ��߼���
        if (slotItem.ItemData == ItemToFind.ItemData && slotItem.Amount == ItemToFind.Amount) {
            LocalItemFoundInArray = true;
            ItemFoundInArray = true;
            break;
        }
    }
}