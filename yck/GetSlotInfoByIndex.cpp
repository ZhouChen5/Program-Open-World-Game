#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺���ͨ����λ������ȡ��Ӧ����λ�ĸ�����Ϣ����������ط��ز���
void UAC_Inventory::GetSlotInfoByIndex(int32 slotIndex,
    UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    bool OverrideStackSize,
    int32 NewStackSize,
    FStruct_Inventory_Slot_Item& InventoryItemDataSlot,
    FStruct_Item& ItemData,
    bool& Stackable,
    int32& MaxStackSize,
    int32& CurrentAmount,
    int32& RoomInStack,
    bool& StackIsFull) {
    Stackable = false;  // ��ʼ���Ƿ�ɶѵ���ʶΪfalse
    MaxStackSize = 0;   // ��ʼ�����ѵ�����Ϊ0
    CurrentAmount = 0;  // ��ʼ����ǰ����Ϊ0
    RoomInStack = 0;    // ��ʼ��ʣ��ռ�Ϊ0
    StackIsFull = false; // ��ʼ��ջ�Ƿ�������ʶΪfalse

    // ���жϴ���Ĳ�λ�����Ƿ�Ϸ�
    if (slotIndex < 0 || slotIndex >= Array.Num()) {
        return;  // ��λ�������Ϸ���ֱ�ӷ��أ��޷���ȡ��Ϣ
    }

    const FStruct_Inventory_Slot_Item& targetSlot = Array[slotIndex];
    InventoryItemDataSlot = targetSlot;  // ��ȡ��Ӧ��λ�Ŀ����Ʒ����

    // ��ȡ��Ʒ���ݣ�����FStruct_Inventory_Slot_Item�ṹ�����ж�Ӧ��Ա�ܻ�ȡ��FStruct_Item���͵����ݣ�
    ItemData = targetSlot.ItemData;

    // �ж���Ʒ�Ƿ�ɶѵ����������FStruct_Item�ṹ�����ж�Ӧ��ʶ��Ա�������ɸ���ʵ����������ж��߼���
    if (targetSlot.ItemData && targetSlot.ItemData->IsStackable) {
        Stackable = true;
    }

    MaxStackSize = targetSlot.MaxStackSize;  // ��ȡ���ѵ�����������ṹ�����иó�Ա������
    CurrentAmount = targetSlot.Amount;      // ��ȡ��ǰ��λ����Ʒ����

    // ����ʣ��ռ�
    if (OverrideStackSize) {
        RoomInStack = NewStackSize - CurrentAmount;
    }
    else {
        RoomInStack = MaxStackSize - CurrentAmount;
    }

    // �ж�ջ�Ƿ�����
    if (CurrentAmount >= MaxStackSize) {
        StackIsFull = true;
    }
}