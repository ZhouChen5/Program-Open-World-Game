#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺������Խ���Ʒ��ӵ����еĿ��ջ�У�������ع������������
void UAC_Inventory::TryProcessItemToExistingStack(int32 SlotIndex,
    FStruct_Inventory_Slot_Item ItemToAdd,
    int32 correctionOfAmountPreviouslyAdded,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    bool OverrideStackSize,
    int32 OverrideMaxStackSize,
    bool UseThisFunctionAsCheckOnly,
    int32& amountAdded,
    bool& Success,
    int32 localAmountAdded,
    int32 localSlotIndex,
    FStruct_Inventory_Slot_Item localItemToAdd,
    int32 localCurrentAmountInSlot,
    int32 localMaxStackSize,
    int32 localAmountToBeAdded,
    int32 LocalRoomInStack,
    bool localOverrideStackSize,
    int32 localOverrideMaxStackSize) {
    Success = false;  // ��ʼ����ӳɹ���ʶΪfalse
    amountAdded = 0;    // ��ʼ��ʵ����ӵ�����Ϊ0

    // ���жϴ���Ĳ�λ�����Ƿ�Ϸ�
    if (SlotIndex < 0 || SlotIndex >= Array.Num()) {
        return;  // ��λ�������Ϸ���ֱ�ӷ��أ��޷�������Ӳ���
    }

    FStruct_Inventory_Slot_Item& targetSlot = Array[SlotIndex];
    // ��ȡĿ���λ��ǰ����Ʒ����������ṹ�����ж�Ӧ������Ա������
    localCurrentAmountInSlot = targetSlot.Amount;
    localMaxStackSize = targetSlot.MaxStackSize;  // ��ȡĿ���λ�����ѵ�����������ṹ�����иó�Ա������

    if (OverrideStackSize) {
        localOverrideStackSize = true;
        localOverrideMaxStackSize = OverrideMaxStackSize;
        localMaxStackSize = OverrideMaxStackSize;  // ʹ����д�����ѵ�����
    }

    // ����Ŀ���λʣ�����ӵĿռ�
    LocalRoomInStack = localMaxStackSize - localCurrentAmountInSlot;
    localAmountToBeAdded = ItemToAdd.Amount;

    // �ж�Ҫ��ӵ���Ʒ�����Ƿ񳬹�ʣ��ռ�
    if (localAmountToBeAdded <= LocalRoomInStack) {
        // ���������������Ʒ��ӵ�ջ��
        targetSlot.Amount += localAmountToBeAdded;
        amountAdded = localAmountToBeAdded;
        Success = true;
    }
    else {
        // ���������ֻ�����ʣ��ռ����������
        targetSlot.Amount = localMaxStackSize;
        amountAdded = LocalRoomInStack;
    }

    localAmountAdded = amountAdded;  // ���¾ֲ������������
    localSlotIndex = SlotIndex;      // ���¾ֲ���λ��������
    localItemToAdd = ItemToAdd;      // ���¾ֲ�Ҫ��ӵ���Ʒ����
}