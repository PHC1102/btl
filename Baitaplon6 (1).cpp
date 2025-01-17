#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_DEVICES 10000
#define TABLE_SIZE 100
typedef struct Device {
    int id;
    char name[50];
    char category[50];
    int usageLevel;  
    float value;     
    struct Device* next; 
} Device;


typedef struct BorrowRecord {// h�m ghi nhan muon thiet bi
    int deviceId;// id thiet bi
    char borrowerName[50];//ten nguoi muon
    int isReturned; // 0 = dang muon, 1 = da tra
    struct BorrowRecord* next;
} BorrowRecord;


Device* hashTable[TABLE_SIZE]; 
Device* deviceArray[MAX_DEVICES]; // Mang luu thiet bi de duyet va sap xep
int deviceCount = 0;// so luong thiet bi hien tai
BorrowRecord* borrowList = NULL; // danh sach ghi nhan cac thiet bi muon

// Ham tinh gia tri bam
int hashFunction(int id) {
    return id % TABLE_SIZE;
}

// Them thiet bi
void addDevice(int id, const char* name, const char* category, int usageLevel, float value) {

    int index = hashFunction(id);
    Device* newDevice = (Device*)malloc(sizeof(Device));
    newDevice->id = id;
    strcpy(newDevice->name, name);
    strcpy(newDevice->category, category);
    newDevice->usageLevel = usageLevel;
    newDevice->value = value;
    newDevice->next = hashTable[index];
    hashTable[index] = newDevice;

    deviceArray[deviceCount++] = newDevice;
}

// Tim thiet bi theo ID
Device* findDevice(int id) {
    int index = hashFunction(id);
    Device* current = hashTable[index];
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Xoa thiet bi theo ID
void removeDevice(int id) {
    int index = hashFunction(id);
    Device* current = hashTable[index];
    Device* prev = NULL;

    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } else {
                prev->next = current->next;
            }

            // Xoa khoi mang
            for (int i = 0; i < deviceCount; i++) {
                if (deviceArray[i]->id == id) {
                    deviceArray[i] = deviceArray[--deviceCount];
                    break;
                }
            }

            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Khong tim thay thiet bi: %d\n", id);
}

// Cap nhat thong tin thiet bi
void updateDevice(int id, const char* name, const char* category, int usageLevel, float value) {
    Device* device = findDevice(id);
    if (device) {
        if (name) strcpy(device->name, name);
        if (category) strcpy(device->category, category);
        device->usageLevel = usageLevel;
        device->value = value;
        printf("Da cap nhat thiet bi: %d\n", id);
    } else {
        printf("Khong tim thay thiet bi: %d\n", id);
    }
}

// Hien thi danh sach thiet bi
void displayDevices() {
    printf("\nDanh sach thiet bi:\n");
    for (int i = 0; i < deviceCount; i++) {
        Device* current = deviceArray[i];
        printf("ID: %d, Ten: %s, Loai: %s, Muc do su dung: %d, Gia tri: %.2f\n",
               current->id, current->name, current->category, current->usageLevel, current->value);
    }
}

// Muon thiet bi
void borrowDevice(int deviceId, const char* borrowerName) {
    BorrowRecord* record = (BorrowRecord*)malloc(sizeof(BorrowRecord));
    record->deviceId = deviceId;
    strcpy(record->borrowerName, borrowerName);
    record->isReturned = 0;
    record->next = borrowList;
    borrowList = record;
    printf("Thiet bi %d da duoc muon boi: %s\n", deviceId, borrowerName);
}

// Tra thiet bi
void returnDevice(int deviceId) {
    BorrowRecord* current = borrowList;
    while (current) {
        if (current->deviceId == deviceId && current->isReturned == 0) {
            current->isReturned = 1;
            printf("Thiet bi %d da duoc tra\n", deviceId);
            return;
        }
        current = current->next;
    }
    printf("Thiet bi %d khong co trong danh sach muon\n", deviceId);
}

// Hien thi danh sach muon/tra
void displayBorrowRecords() {
    BorrowRecord* current = borrowList;
    printf("\nDanh sach muon/tra:\n");
    while (current) {
        printf("ID Thiet bi: %d, Nguoi muon: %s, Trang thai: %s\n",
               current->deviceId, current->borrowerName, current->isReturned ? "Da tra" : "Dang muon");
        current = current->next;
    }
}


void heapify(int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && deviceArray[left]->value > deviceArray[largest]->value) {
        largest = left;
    }

    if (right < n && deviceArray[right]->value > deviceArray[largest]->value) {
        largest = right;
    }

    if (largest != i) {
        Device* temp = deviceArray[i];
        deviceArray[i] = deviceArray[largest];
        deviceArray[largest] = temp;

        heapify(n, largest);
    }
}

void heapSortDevices() {
    for (int i = deviceCount / 2 - 1; i >= 0; i--) {
        heapify(deviceCount, i);
    }

    for (int i = deviceCount - 1; i >= 0; i--) {
        Device* temp = deviceArray[0];
        deviceArray[0] = deviceArray[i];
        deviceArray[i] = temp;

        heapify(i, 0);
    }
}

void sortDevicesByValue() {
    heapSortDevices();
    displayDevices();
}

int main() {
    addDevice(1, "May hien song", "Dien tu", 8, 15000000);
    addDevice(2, "Dong ho van nang", "Dien tu", 5, 300000);
    addDevice(3, "Nguon cap dien", "Dien tu", 7, 500000);
    addDevice(4, "Ampe Ke", "Dien tu", 9, 200000);
    displayDevices();

    borrowDevice(1, "Truong Dinh Thanh");
    borrowDevice(2, "Vo Khac Huan");
    borrowDevice(3, "Nguyen Van A");

    returnDevice(1);
    returnDevice(3);
    displayBorrowRecords();

    updateDevice(3, "Nguon DC", "Dien tu", 9, 550000);
    sortDevicesByValue();
    removeDevice(2);


    return 0;
}


