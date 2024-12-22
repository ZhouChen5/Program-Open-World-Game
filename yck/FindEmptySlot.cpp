#include "UAC_Inventory.h"

// ����ʵ�ֿ�ʼ���˺��������ڿ���λ��Ʒ�����в��ҿղ�λ����������Ӧ�ķ��ز���
void UAC_Inventory::FindEmptySlot(UPARAM(ref) TArray<FStruct_Inventory_Slot_Item>& Array,
    int32& SlotIndex,
    bool& Success,
    bool localFoundEmptySlot,
    int32 localSlotIndexOfEmptySlot) {
    Success = false;  // ��ʼ�����ҳɹ���ʶΪfalse
    SlotIndex = -1;    // ��ʼ����λ����Ϊ��Чֵ
    localFoundEmptySlot = false;  // ��ʼ���ֲ��Ŀղ�λ���ֱ�ʶΪfalse
    localSlotIndexOfEmptySlot = -1;  // ��ʼ���ֲ��Ŀղ�λ����Ϊ��Чֵ

    // ��������λ��Ʒ����
    for (int32 i = 0; i < Array.Num(); ++i) {
        const FStruct_Inventory_Slot_Item& currentSlotItem = Array[i];
        // �жϵ�ǰ��λ����Ʒ�����Ƿ�Ϊ�գ��Դ���Ϊ�ղ�λ���ж����ݣ��ɸ���ʵ�ʽṹ�嶨������ж�������
        if (currentSlotItem.ItemData == nullptr) {
            SlotIndex = i;  // ��¼�ҵ��Ŀղ�λ����
            Success = true;  // ���ò��ҳɹ���ʶΪtrue
            localFoundEmptySlot = true;  // ���þֲ��Ŀղ�λ���ֱ�ʶΪtrue
            localSlotIndexOfEmptySlot = i;  // ��¼�ֲ��Ŀղ�λ����
            break;  // �ҵ��ղ�λ���ֱ������ѭ���������������
        }
    }
}