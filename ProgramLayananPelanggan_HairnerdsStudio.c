#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CABANG 10

// Struktur data untuk simpul BST
struct Node {
    int orderNumber;
    char layanan[50];
    int harga;
    struct Node* left;
    struct Node* right;
};

// Struktur data untuk merepresentasikan cabang restoran dalam graph
struct Cabang {
    int id;
    char nama[50];
    double latitude;
    double longitude;
};

// Struktur data untuk merepresentasikan graph
struct Graph {
    struct Cabang cabang[MAX_CABANG];
    int adjacencyMatrix[MAX_CABANG][MAX_CABANG];
    int cabangCount;
};

// Graph global
struct Graph barberGraph;

// Membuat simpul BST baru
struct Node* createNode(int orderNumber, char layanan[], int harga) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->orderNumber = orderNumber;
    strcpy(node->layanan, layanan);
    node->harga = harga;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Menyisipkan simpul baru ke dalam BST
struct Node* insert(struct Node* root, int orderNumber, char layanan[], int harga) {
    if (root == NULL) {
        return createNode(orderNumber, layanan, harga);
    }
    if (orderNumber < root->orderNumber) {
        root->left = insert(root->left, orderNumber, layanan, harga);
    }
    else if (orderNumber > root->orderNumber) {
        root->right = insert(root->right, orderNumber, layanan, harga);
    }
    return root;
}

// Mencari simpul dengan nomor antrian tertentu dalam BST
struct Node* search(struct Node* root, int orderNumber) {
    if (root == NULL || root->orderNumber == orderNumber) {
        return root;
    }
    if (orderNumber < root->orderNumber) {
        return search(root->left, orderNumber);
    }
    return search(root->right, orderNumber);
}

// Implementasi antrian menggunakan linked list
struct QueueNode {
    int orderIndex;
    char nama[50];
    struct Node* data;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

// Inisialisasi antrian baru
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

// Menambahkan elemen ke dalam antrian
void enqueue(struct Queue* queue, char nama[], struct Node* data) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newNode->orderIndex = queue->rear != NULL ? queue->rear->orderIndex + 1 : 1;
    strcpy(newNode->nama, nama);
    newNode->data = data;
    newNode->next = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    }
    else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Menghapus elemen dari antrian
struct Node* dequeue(struct Queue* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    struct Node* data = queue->front->data;
    struct QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return data;
}

// Menampilkan daftar pesanan
void displayOrderList(struct Queue* queue) {
    if (queue->front == NULL) {
        printf("Tidak ada pesanan yang tersedia.\n");
        return;
    }
    struct QueueNode* currNode = queue->front;
    printf("=====Daftar Pesanan=====\n");
    while (currNode != NULL) {
        printf("Nomor Antrian: %d\n", currNode->orderIndex);
        printf("Nama: %s\n", currNode->nama);
        printf("layanan: %s\n", currNode->data->layanan);
        printf("Harga: Rp%d\n", currNode->data->harga);
        printf("-----------------\n");
        currNode = currNode->next;
    }
}

// Mencari cabang terdekat berdasarkan lokasi pengguna
int findNearestBranch(double latitude, double longitude) {
    int nearestBranch = -1;
    double minDistance = INFINITY;

    for (int i = 0; i < barberGraph.cabangCount; i++) {
        double latDiff = latitude - barberGraph.cabang[i].latitude;
        double lonDiff = longitude - barberGraph.cabang[i].longitude;
        double distance = sqrt(latDiff * latDiff + lonDiff * lonDiff);

        if (distance < minDistance) {
            minDistance = distance;
            nearestBranch = i;
        }
    }

    return nearestBranch;
}

// Fungsi utama
int main() {
    struct Node* bstRoot = NULL;
    struct Queue* orderQueue = createQueue();
    int orderIndex = 1;
    int nearestBranch;
    double latitude, longitude;

    // Menambahkan layanan dan harga ke dalam BST
    bstRoot = insert(bstRoot, 1, "Mens Haircut & Styling", 170000);
    bstRoot = insert(bstRoot, 2, "Womens Haircut & Styling", 300000);
    bstRoot = insert(bstRoot, 3, "Mens Darken Hair Color", 250000);
    bstRoot = insert(bstRoot, 4, "Mens Basic Hair Color", 400000);
    bstRoot = insert(bstRoot, 5, "Mens Highlight", 800000);
    bstRoot = insert(bstRoot, 6, "Mens Fashion Hair Color", 1000000);
    bstRoot = insert(bstRoot, 7, "Mens Platinum Hair Color", 1100000);
    bstRoot = insert(bstRoot, 8, "women Basic Hair Color", 700000);
    bstRoot = insert(bstRoot, 9, "Women Highlight Fashion Color", 1400000);
    bstRoot = insert(bstRoot, 10, "Women Fashion Hair Color", 1400000);
    bstRoot = insert(bstRoot, 11, "Women Platinum Hair Color", 1500000);
    bstRoot = insert(bstRoot, 12, "Mens Dreadlock", 1500000);
    bstRoot = insert(bstRoot, 13, "Mens Dreadlock & Coloring", 2500000);
    bstRoot = insert(bstRoot, 14, "womens Dreadlock", 1500000);
    bstRoot = insert(bstRoot, 15, "Womens Dreadlock * Coloring", 2500000);
    bstRoot = insert(bstRoot, 16, "Mens Conrow", 300000);
    bstRoot = insert(bstRoot, 17, "Mens Braids", 400000);
    bstRoot = insert(bstRoot, 18, "Womens Conrow", 400000);
    bstRoot = insert(bstRoot, 19, "Womens Braids", 700000);
    bstRoot = insert(bstRoot, 20, "Hair Wrap/pcs", 100000);
    bstRoot = insert(bstRoot, 21, "Mens Perming", 500000);
    bstRoot = insert(bstRoot, 22, "Mens Downperm", 300000);
    bstRoot = insert(bstRoot, 23, "Womens Perming", 800000);
    bstRoot = insert(bstRoot, 24, "Mens Hair Smoothing", 800000);
    bstRoot = insert(bstRoot, 25, "Womens Hair Smoothing", 1000000);
    bstRoot = insert(bstRoot, 26, "Mens Keratin Hair Treatment", 1300000);
    bstRoot = insert(bstRoot, 27, "Womens Keratin Hair Treatment", 1800000);
    bstRoot = insert(bstRoot, 28, "Mens Hair Tatto", 85000);
    bstRoot = insert(bstRoot, 29, "Womens Hair Tatto", 85000);

    // Inisialisasi graph dan cabang-cabang restoran
    barberGraph.cabangCount = 4;

    // Cabang 0
    barberGraph.cabang[0].id = 0;
    strcpy(barberGraph.cabang[0].nama, "Hairnerds Studio Gading Serpong");
    barberGraph.cabang[0].latitude = -6.24333391571246;
    barberGraph.cabang[0].longitude = 106.61988672376597;

    // Cabang 1
    barberGraph.cabang[1].id = 1;
    strcpy(barberGraph.cabang[1].nama, "Hairnerds Studio Kebayoran");
    barberGraph.cabang[1].latitude = -6.242953085181709;
    barberGraph.cabang[1].longitude = 106.7915758256141;

    // Cabang 2
    barberGraph.cabang[2].id = 2;
    strcpy(barberGraph.cabang[2].nama, "Hairnerds Studio Bandung");
    barberGraph.cabang[2].latitude = -6.901310028793618;
    barberGraph.cabang[2].longitude = 107.60866569309167;

    // Cabang 3
    barberGraph.cabang[3].id = 3;
    strcpy(barberGraph.cabang[3].nama, "Hairnerds Studio PIK");
    barberGraph.cabang[3].latitude = -6.104615165987099;
    barberGraph.cabang[3].longitude = 106.74108732191601;

    // Menginisialisasi adjacency matrix dengan nilai 0 pada semua elemen
    for (int i = 0; i < barberGraph.cabangCount; i++) {
        for (int j = 0; j < barberGraph.cabangCount; j++) {
            barberGraph.adjacencyMatrix[i][j] = 0;
        }
    }

    // Menambahkan keterhubungan antara cabang-cabang restoran
    barberGraph.adjacencyMatrix[0][1] = 1;
    barberGraph.adjacencyMatrix[1][0] = 1;
    barberGraph.adjacencyMatrix[1][2] = 1;
    barberGraph.adjacencyMatrix[2][1] = 1;
    barberGraph.adjacencyMatrix[2][3] = 1;
    barberGraph.adjacencyMatrix[3][2] = 1;

    int choice;
    int orderNumber;
    char nama[50];
    struct Node* foundOrder;

    printf("Selamat datang di Hairnerds Studio\n");
    printf("Pertama-tama, silakan masukkan koordinat latitude dan longitude anda untuk mencari cabang terdekat\n");
    /*
    int pilihan = 1;
    do {
        // Mengambil lokasi sekarang
        printf("Masukkan lokasi latitude: ");
        scanf("%lf", &latitude);
        printf("Masukkan lokasi longitude: ");
        scanf("%lf", &longitude);
        // Mencari cabang terdekat
        nearestBranch = findNearestBranch(latitude, longitude);
        if (nearestBranch != -1) {
            printf("Cabang terdekat: %s\n", barberGraph.cabang[nearestBranch].nama);
            pilihan = 0;
        }
        else {
            printf("Tidak ada cabang Hairnerds Studio yang tersedia.\n");
        }
    } while ( pilihan != 0);
    */
    do {
        printf("\n========= MENU =========\n");
        printf("1. Pesan Layanan\n");
        printf("2. Cari Pesanan\n");
        printf("3. Ambil Pesanan\n");
        printf("4. Daftar Pesanan\n");
        printf("0. Keluar\n");
        printf("========================\n");
        printf("Masukkan pilihan Anda: ");
        scanf("%d", &choice);

        if (choice==1) {
            // Pesan layanan
            printf("Masukkan nama Anda: ");
            scanf("%s", nama);
            printf("\n========= Layanan =========\n");
            printf("1. Mens Haircut & Styling - Rp170.000\n");
            printf("2. Womens Haircut & Styling - Rp300.000\n");
            printf("3. Mens Darken Hair Color - Rp250.000\n");
            printf("4. Mens Basic Hair Color - Rp400.000\n");
            printf("5. Mens Highlight - Rp800.000\n");
            printf("6. Mens Fashion Hair Color - Rp1.000.000\n");
            printf("7. Mens Platinum Hair Color - Rp1.100.000\n");
            printf("8. Women Basic Hair Color - Rp700.000\n");
            printf("9. Women Highlight Fashion Color - Rp1.400.000\n");
            printf("10. Women Fashion Hair Color - Rp1.400.000\n");
            printf("11. Women Platinum Hair Color - Rp1.500.000\n");
            printf("12. Mens Dreadlock - Rp1.500.000\n");
            printf("13. Mens Dreadlock & Coloring - Rp2.500.000\n");
            printf("14. Womens Dreadlock - Rp1.500.000\n");
            printf("15. Womens Dreadlock & Coloring - Rp2.500.000\n");
            printf("16. Mens Conrow - Rp300.000\n");
            printf("17. Mens Braids - Rp400.000\n");
            printf("18. Womens Conrow - Rp400.000\n");
            printf("19. Womens Braids - Rp700.000\n");
            printf("20. Hair Wrap/pcs - Rp100.000\n");
            printf("21. Mens Perming - Rp500.000\n");
            printf("22. Mens Downperm - Rp300.000\n");
            printf("23. Womens Perming - Rp800.000\n");
            printf("24. Mens Hair Smoothing - Rp800.000\n");
            printf("25. Womens Hair Smoothing - Rp1.000.000\n");
            printf("26. Mens Keratin Hair Treatment - Rp1.300.000\n");
            printf("27. Womens Keratin Hair Treatment - Rp1.800.000\n");
            printf("28. Mens Hair Tatto - Rp85.000\n");
            printf("29. Womens Hair Tatto - Rp85.000\n");
            printf("========================\n");
            printf("Masukkan nomor layanan yang ingin dipesan: ");
            scanf("%d", &orderNumber);

            foundOrder = search(bstRoot, orderNumber);
            if (foundOrder != NULL) {
                enqueue(orderQueue, nama, foundOrder);
                printf("Pesanan berhasil ditambahkan ke dalam antrian!\n");
                printf("Nomor Antrian: %d\n", orderQueue->rear->orderIndex);
                printf("Layanan: %s\n", foundOrder->layanan);
                printf("Harga: Rp%d\n", foundOrder->harga);
            }
            else {
                printf("Layanan dengan nomor tersebut tidak tersedia!\n");
            }
        }

        else if (choice==2) {
            // Cari Pesanan
            printf("Masukkan nomor antrian pesanan yang ingin dicari: ");
            scanf("%d", &orderNumber);

            struct QueueNode* currNode = orderQueue->front;
            while (currNode != NULL) {
                if (currNode->orderIndex == orderNumber) {
                    foundOrder = currNode->data;
                    break;
                }
                currNode = currNode->next;
            }

            if (foundOrder != NULL) {
                printf("Nomor Antrian: %d\n", currNode->orderIndex);
                printf("Nama: %s\n", currNode->nama);
                printf("Layanan: %s\n", foundOrder->layanan);
                printf("Harga: Rp%d\n", foundOrder->harga);
            }
            else {
                printf("Pesanan dengan nomor antrian tersebut tidak ditemukan!\n");
            }
        }

        else if (choice==3) {
            struct Node* takenOrder = dequeue(orderQueue);
            // Ambil Pesanan
            if (takenOrder == NULL) {
                printf("Tidak ada pesanan yang tersedia.\n");
                orderIndex = 1;
            }
            else {
                printf("Pesanan dengan nomor antrian %d telah diambil.\n", orderIndex);
                printf("Layanan: %s\n", takenOrder->layanan);
                printf("Harga: Rp%d\n", takenOrder->harga);
                orderIndex++;
            }
        }

        else if (choice==4) {
            // Daftar Antrian
            displayOrderList(orderQueue);
        }

        else if (choice==0) {
            printf("Terima kasih telah menggunakan layanan kami!\n");
        }

        else {
            printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    } while (choice != 0);

    return 0;
}
