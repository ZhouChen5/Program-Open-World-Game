#include "UAC_Inventory.h"
// ����ʵ�ֿ�ʼ���˺������ڳ����Զ�����Ʒ��ӵ�����У����ݸ��ֹ���������߼�
void UAC_Inventory::TryAddItemToInventoryAutomaticly(FStruct_Inventory_Slot_Item InventoryItemToBeAdded,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& TargetArray,
    bool OverrideStackSize,
    int32 OverrideMaxStackSize,
    bool OverrideDynamicSlotCreationToTrue,
    bool UseThisFunctionAsCheckOnly,
    bool& AllItemsAdded,
    int32& AmountNotAdded,
    int32& EmptySlotsFilled,
    int32 localAmountThatCouldNotBeAddedIntoExistingStacks,
    FStruct_Inventory_Slot_Item LocalInventoryItemToBeAdded,
    int32 localAlreadyAddedIntoNewStackInOtherLoop,
    int32 NumberOfLoopsRequired,
    int32 localAmountThatCouldNotBeAddedDueToWeight) {
    AllItemsAdded = false;  // ��ʼ���Ƿ�������Ʒ������ӵı�ʶΪfalse
    AmountNotAdded = 0;    // ��ʼ��δ��ӵ���Ʒ����Ϊ0
    EmptySlotsFilled = 0;  // ��ʼ�����Ŀղ�λ����Ϊ0

    // �����Ҫ��д��̬��λ����Ϊtrue
    if (OverrideDynamicSlotCreationToTrue) {
        // ����Ŀ�������飬�����Ƿ��пղ�λ
        for (int32 i = 0; i < TargetArray.Num(); ++i) {
            if (TargetArray[i].ItemData == nullptr) {
                // ����ǿղ�λ���ɸ�����������Ƿ��ڴ˴������µ���Ʒ���ݵȲ���
                FStruct_Inventory_Slot_Item newSlotItem;
                // ��ʾ������ʼ���²�λ��Ʒ���������
                newSlotItem.ItemData = nullptr;
                newSlotItem.Amount = 0;
                TargetArray[i] = newSlotItem;
                EmptySlotsFilled++;
            }
        }
        // ���Ŀ������Ϊ�գ�ֱ�Ӵ���һ���µĲ�λ��Ϊʾ��
        if (TargetArray.Num() == 0) {
            FStruct_Inventory_Slot_Item newSlotItem;
            newSlotItem.ItemData = nullptr;
            newSlotItem.Amount = 0;
            TargetArray.Add(newSlotItem);
            EmptySlotsFilled++;
        }
    }

    // ����Ŀ�������飬���ҿ������Ʒ�Ĳ�λ
    for (int32 i = 0; i < TargetArray.Num(); ++i) {
        FStruct_Inventory_Slot_Item& currentSlot = TargetArray[i];
        // �жϵ�ǰ��λ�Ƿ�Ϊ�ջ�����Ʒ�Ƿ�ƥ�䣨�ɶѵ������
        if (currentSlot.ItemData == nullptr || currentSlot.ItemData == InventoryItemToBeAdded.ItemData) {
            // �����Ҫ��д�ѵ���С
            if (OverrideStackSize) {
                int32 availableSpace = OverrideMaxStackSize - currentSlot.Amount;
                if (availableSpace >= InventoryItemToBeAdded.Amount) {
                    currentSlot.Amount += InventoryItemToBeAdded.Amount;
                    AllItemsAdded = true;
                }
                else {
                    currentSlot.Amount += availableSpace;
                    AmountNotAdded = InventoryItemToBeAdded.Amount - availableSpace;
                }
            }
            else {
                // �����ѵ��߼������ǵ�ǰ��λʣ��ռ������Ʒ
                int32 maxStackSize = currentSlot.MaxStackSize;  // ����ṹ���������ѵ�������Ա
                int32 availableSpace = maxStackSize - currentSlot.Amount;
                if (availableSpace >= InventoryItemToBeAdded.Amount) {
                    currentSlot.Amount += InventoryItemToBeAdded.Amount;
                    AllItemsAdded = true;
                }
                else {
                    currentSlot.Amount += availableSpace;
                    AmountNotAdded = InventoryItemToBeAdded.Amount - availableSpace;
                }
            }
            if (AllItemsAdded) {
                break;  // ���������Ʒ������ӣ�����ѭ��
            }
        }
    }
    // ��������������������Ʒδ��ӣ�����δ������������
    if (!AllItemsAdded) {
        AmountNotAdded = InventoryItemToBeAdded.Amount;
    }
}