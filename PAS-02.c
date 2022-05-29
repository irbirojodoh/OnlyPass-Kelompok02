#include <stdio.h>
#include <omp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <ctype.h>

typedef struct listPassword{
    char dataPwd[2][100];
    char namaPwd[30];
    char idPwd[30];
    int keyPwd;
    bool changed;
}List;
struct Node {
    char dataPwd[100];
    char namaAkun[30];
    char idAkun[30];
    int key;
    int countPwd;
    List password[50]; //Maksimal 50 password dalam satu akun
    struct Node* next;
};

//FUNCTION UTAMA
void loadData(struct Node** headRef, struct Node temp);
int addAccount (struct Node** headRef);
int enterAccount(struct Node* headRef, int countAccount);
void mainMenu(struct Node** headRef, int numAccount);
void helpDepan();
void helpMain();
char* Strcasestr(const char* haystack, const char* needle);
void randomPasswordGeneration(int N, char pass[100]);
char grade(char pass[100], unsigned long len, char tempStrength[]);
//SORTING DAN SEARCHING===============================================
void swapChar(char a[100], char b[100]);
void swapInt(int *a, int *b);
void swapBool(bool *a, bool *b);
void quicksortArr(struct Node** node,int start, int end);
void searchPass(struct Node* node);
//PASSWORD===============================================
int savePwd(struct Node** node, int passNum);
int keygen();
void changePass(struct Node** node, int passNum);
void searchPass(struct Node* node);
void decrypt(char pass[100], unsigned long len, int key);
void encrypt(char pass[100], unsigned long len, int key);
void lookPwd(struct Node* node);
int viewSetting(struct Node* node);


//FUNCTION UNTUK BACA TULIS COUNT AKUN=================================================================================================================
void writeCount(int count){
    const char* fileCount     = "{\n\t\"COUNTACCOUNT\": %d}\n";
    int i;
    FILE* file;
    fopen_s(&file, "Count.dat", "w");
    fprintf_s(file, fileCount, count);
    fclose(file);
}
void readCount(int *count){
    const char* fileCount     = "{\n\t\"COUNTACCOUNT\": %d}\n";
    int i, temp;
    FILE* file;
    fopen_s(&file, "Count.dat", "r");
    fscanf_s(file, fileCount, count);
    fclose(file);
}

//FUNCTION UNTUK BACA TULIS DATA AKUN===================================================================================================================
void writeFile(struct Node* node, int count){
    const char* akunOutput = "{\n\t\"namaAkun\": \"%s\",\n\t\"idAkun\": \"%s\", \n\t\"dataPwd\": \"%s\", \n\t\"key\": %d, \n\t\"countPwd\": %d\n";
    const char* passwordOutput = "\n\t\"namaPwd\": \"%s\",\n\t\"idPwd\": \"%s\", \n\t\"keyPwd\": %d,\n\t\"dataPwd\": \"%s\"";
    int i, j;
    FILE* file;
    fopen_s(&file, "akun.dat", "w");
    
    for (i = 0; i < count; i++) {
        fprintf_s(file, akunOutput, node->namaAkun,node->idAkun,node->dataPwd,node->key,node->countPwd);
        for (j = 0; j < node->countPwd; j++){
            fprintf_s(file, passwordOutput, node->password[j].namaPwd,node->password[j].idPwd,node->password[j].keyPwd,node->password[j].dataPwd[1]);
        }
        fprintf_s(file,"\n}\n\n");
        node = node->next;
    }
    fclose(file);
}
void readFile(struct Node temp[], int count) {
    const char* akunOutput = "{\n\t\"namaAkun\": \"%[^\"]\",\n\t\"idAkun\": \"%[^\"]\", \n\t\"dataPwd\": \"%[^\"]\", \n\t\"key\": %d, \n\t\"countPwd\": %d\n";
    const char* passwordOutput = "\n\t\"namaPwd\": \"%[^\"]\",\n\t\"idPwd\": \"%[^\"]\", \n\t\"keyPwd\": %d,\n\t\"dataPwd\": \"%[^\"]\"";
    int i, j;
    FILE* file;
    fopen_s(&file, "akun.dat", "r");
    //DATA DILETAKAN DI STRUCT SEMENTARA
    for (i = 0; i < count; i++) {
        fscanf_s(file, akunOutput, temp[i].namaAkun, 30, temp[i].idAkun, 30, temp[i].dataPwd, 100, &temp[i].key, &temp[i].countPwd);
        printf("{\n\t\"namaAkun\": \"%s\",\n\t\"idAkun\": \"%s\", \n\t\"dataPwd\": \"%s\", \n\t\"key\": %d, \n\t\"countPwd\": %d\n",
        temp[i].namaAkun, temp[i].idAkun, temp[i].dataPwd,temp[i].key,temp[i].countPwd);
        for (j = 0; j < temp[i].countPwd; j++){
            fscanf_s(file, passwordOutput, temp[i].password[j].namaPwd, 30, temp[i].password[j].idPwd, 30, &temp[i].password[j].keyPwd,temp[i].password[j].dataPwd[1], 100);
            printf("\n\t\"namaPwd\": \"%s\",\n\t\"idPwd\": \"%s\", \n\t\"keyPwd\": %d,\n\t\"dataPwd\": \"%s\"",
            temp[i].password[j].namaPwd, temp[i].password[j].idPwd, temp[i].password[j].keyPwd, temp[i].password[j].dataPwd[1]);
        }
        fscanf_s(file,"\n}\n\n");
        printf("\n}\n\n");
    }
    fclose(file);
}

//FUNCTION UNTUK MELOAD DATA DARI FILE
void loadData(struct Node** headRef, struct Node temp){
    int i;
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = *headRef;
    //DATA DARI STRUCT DIPINDAHKAN KE LINKED LIST
    strcpy(new_node->namaAkun, temp.namaAkun);
    strcpy(new_node->idAkun, temp.idAkun);
    strcpy(new_node->dataPwd, temp.dataPwd);
    new_node->key =temp.key;
    new_node->countPwd =temp.countPwd;

    for(i =0; i<new_node->countPwd; i++){
        strcpy(new_node->password[i].namaPwd, temp.password[i].namaPwd);
        strcpy(new_node->password[i].idPwd, temp.password[i].idPwd);
        strcpy(new_node->password[i].dataPwd[0], temp.password[i].dataPwd[0]);
        strcpy(new_node->password[i].dataPwd[1], temp.password[i].dataPwd[1]);
        new_node->password[i].keyPwd = temp.password[i].keyPwd;
    }
    new_node->next = NULL;
    if (*headRef == NULL){
          *headRef = new_node;
          return;
    }
    //menaruh akun baru ke node paling akhir
    while (last->next != NULL)
        last = last->next;
            
    last->next = new_node;
    return;
}

void startAnimation(){                  //Function untuk menampilkan animasi opening
    system("color E");
    system("CLS");
    printf("\t==================");
    Sleep(200);
    printf("==================");
    Sleep(200);
    printf("==================\n");
    Sleep(200);
    printf("\t||              Welcome to OnlyPass!                ||\n");
    Sleep(500);
    printf("\t||                  Presented By:                   ||\n");
    Sleep(500);
    printf("\t||              Kelompok 2 Proglan 2                ||\n");
    Sleep(500);
    printf("\t==================");
    Sleep(200);
    printf("==================");
    Sleep(200);
    printf("==================\n\n\t");
    Sleep(200);
    system("Pause");
}

//DRIVER FUNCTION==================================================================================================================================
int main(){
    int input, i, loop = 0, countAccount = 0;
    readCount(&countAccount);
    struct Node* head = NULL;
    struct Node temp[countAccount];
    readFile(temp, countAccount);
    for(i = 0; i < countAccount; i++){
        loadData(&head, temp[i]);
    }
    printf("\t\t============ Check status multithreading ==============");
    #pragma omp parallel
    {
        printf("\n\t\tThread no. %d", omp_get_thread_num());
        #pragma omp barrier
        #pragma omp single
        printf("\n\t\t%d thread aktif", omp_get_num_threads());
    
    }
    Sleep(800);
    startAnimation();
    //readFile(&head, countAccount);
       do{
        do{
            system("CLS");
            printf("\n\t\t              ==== OnlyPass ====   \n");
            printf("\t\t======================================================\n"
                   "\t\t|              1. Masuk Akun                         |\n"
                   "\t\t|              2. Buat Akun                          |\n"
                   "\t\t|              3. Bantuan                            |\n"
                   "\t\t|              4. Hapus Akun                         |\n"
                   "\t\t|              5. Keluar Bantuan                     |\n"
                   "\t\t======================================================\n");
            printf("\t\t> ");
            loop = 0;
            scanf("%d", &input);
            if (input == 1){
                if (countAccount == 0){
                    printf("\n\n\t\tTerdapat Kesalahan Input!!\n");
                    printf("\t\tData Akun Masih Kosong!!\n\n\t\t");
                    loop = 1;
                    system("Pause");
                    do{
                        input = addAccount(&head);
                    }while(input == 2);
                    if(input == 1)
                        countAccount++;
                }
            }
        }while(loop == 1);
        switch(input){
            case 1:
                do{
                    input = enterAccount(head, countAccount);
                }while(input == -2);
                if(input != -1){
                    mainMenu(&head, input);
                }
                break;
            case 2:
                do{
                    input = addAccount(&head);
                }while(input == 2);
                if(input == 1)
                        countAccount++;
                break;
            case 3:
                helpDepan();
                break;
            case 4: deleteAcc(&head, countAccount);
                
            case 5:
                writeFile(head, countAccount);
                writeCount(countAccount);
                return 0;
            default:
                break;
        }
      }while(loop == 0);
    return 0;
}

//FUNCTION MENAMBAHKAN NODE AKUN====================================================================================
int addAccount (struct Node** headRef){ //Function untuk menambahkan akun berupa Node
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = *headRef;
    int tempKey = keygen(), input, i;
    char tempPass[100] = {0};
    char strength[15];
    char tempC;
    
    system("CLS");
    printf("\n\t\t================ Sign Upd ===================\n");
    printf("\t\tNama\t\t: ");
    scanf(" %[^\n]s", new_node -> namaAkun );
    printf("\t\tID Akun\t\t: ");
    scanf(" %[^\n]s", new_node -> idAkun );
    printf("\t\tPassword\t: ");
    scanf(" %[^\n]s", tempPass);
    /*    //implementasi masking password tidak jadi digunakan
    do{
        if((tempC = getch()) != 13){
            printf("*");
            tempPass[i] = tempC;
            i++;
        }
    }while(tempC != 13);
    */
    printf("\t\t============================================\n");
    printf("\t\tMemeriksa Password...\n\t\t");
    system("Pause");
    system("CLS");
        
    printf("\n\t\t================ Account ===================\n");
    printf("\t\tNama\t\t: %s\n", new_node -> namaAkun );
    printf("\t\tID Akun\t\t: %s\n", new_node -> idAkun);
    grade(tempPass, strlen(tempPass), strength);
    printf("\t\tKekuatan Password adalah -%s-", strength);
    printf("\n\t\t============================================");
    printf("\n\t\tLanjut? (1=Y, 0=N, 2=Ulangi)\n\t\t> ");
    scanf("%d", &input);
        
    if(input == 0){
        return 0;
    }else if(input == 2){
        return 2;
    }
    
    encrypt(tempPass, strlen(tempPass), tempKey);
    //Enkripsi password sebelum disimpan
    new_node -> key = tempKey;
    strcpy(new_node->dataPwd, tempPass);
    new_node->next = NULL;
    new_node -> countPwd = 0;
       
    if (*headRef == NULL){
        *headRef = new_node;
        return 1;
    }
    
    //menaruh akun baru ke node paling akhir
    while (last->next != NULL)
        last = last->next;
            
    last->next = new_node;
    return 1;
}

//FUNCTION UNTUK MENGHAPUS AKUN============================================================================
void deleteAcc(struct Node** headRef, int countAccount) {
    int i, indicator = 0;
    bool loop = false;
    char tempID[30], tempPass[100], tempPass2[100];
    struct Node *ptr = *headRef;
    struct Node *current = *headRef;
    struct Node *previous = *headRef;
    if (*headRef == NULL){
        printf("TIDAK ADA AKUN");
    }
    else{
        do{
            i = 0;
            ptr = *headRef;
            indicator = 0;
            system("CLS");
            printf("\n\t\t              ==== OnlyPass ====   \n");
            printf("\t\t======================================================\n");
            printf("\t\tID Akun\t\t: ");
            scanf(" %[^\n]s", tempID);
            printf("\t\tPassword\t: ");
            scanf(" %[^\n]s", tempPass);
            printf("\t\t======================================================\n");
            
            while(ptr != NULL && i != countAccount && indicator == 0){
                strcpy(tempPass2, ptr->dataPwd);
                decrypt(tempPass2, strlen(tempPass2), ptr->key);
                if(strcmp(tempID, ptr->idAkun)==0 && strcmp(tempPass2, tempPass)==0){
                    indicator = 1;
                    i++;
                }
                else{
                    i++;
                    ptr = ptr->next;
                }
            }
            if(ptr == NULL){
                loop = true;
            }
            if(loop == true){
                printf("\t\tAwas: ID atau Password Salah!!\n");
                printf("\n\t\tCoba Lagi? (1=Y, 0=N)\n\t\t> ");
                scanf("%d", &i);
                if(i == 1){
                    loop = true;
                }
            }
        }while(loop == true);
        if(i == 1){
            *headRef = current -> next;
            free(current);
            current = NULL;
        }
        else{
            while( i != 1){
                previous = current;
                current = current->next;
                i--;
            }
            previous -> next = current -> next;
            free(current);
            current = NULL;
            
        }
    }
    
   
    
}

//FUNCTION UNTUK AUTENTIKASI AKUN==============================================================================================================
int enterAccount(struct Node* headRef, int countAccount) {
    int i, indicator = 0;
    bool loop = false;
    char tempID[30], tempPass[100], tempPass2[100];
    struct Node *ptr;
    do{
        ptr = headRef;
        i = 0;
        indicator = 0;
        struct Node *ptr = headRef;
        system("CLS");
        printf("\n\t\t              ==== OnlyPass ====   \n");
        printf("\t\t======================================================\n");
        printf("\t\tID Akun\t\t: ");
        scanf(" %[^\n]s", tempID);
        printf("\t\tPassword\t: ");
        scanf(" %[^\n]s", tempPass);
        printf("\t\t======================================================\n");
        
        while(ptr != NULL && i != countAccount && indicator == 0){
            strcpy(tempPass2, ptr->dataPwd);
            decrypt(tempPass2, strlen(tempPass2), ptr->key);
            if(strcmp(tempID, ptr->idAkun)==0 && strcmp(tempPass2, tempPass)==0){
                indicator = 1;
            }
            else{
                i++;
                ptr = ptr->next;
            }
        }
        if(i == countAccount){
            loop = true;
        }
        if(loop == true){
            printf("\t\tAwas: ID atau Password Salah!!\n");
            printf("\n\t\tCoba Lagi? (1=Y, 0=N)\n\t\t> ");
            scanf("%d", &i);
            if(i == 1){
                system("Pause");
                return -2; //-2 adalah tanda untuk loop masuk akun di main
            }
            else{
                system("Pause");
                ptr = headRef;
                return -1; //-1 adalah tanda keluar masukakun dan balik ke main
            }
        }
        else{
            ptr = headRef;
            return i;
        }
    }while(loop == 1);
    return 0;
}

//FUNCTION MENU UTAMA============================================================================================================================
void mainMenu(struct Node** headRef, int numAccount){    //Function untuk menampilkan main menu
    int i, input, loop = false;
    struct Node *ptr = *headRef;
    for(i = 0; i < numAccount; i++){
        ptr = ptr->next;
    }
    
    do{
        #pragma omp parallel
        {
            #pragma omp single //Implementasi QuickSort dengan multithreading
            quicksortArr(&ptr, 0, ptr->countPwd -1);
        }
        system("CLS");
        printf("\n\t\t                ==== Welcome ====   \n");
        printf("\t\t======================================================\n");
        printf("\t\t\t%15s %s\n", "Nama : ", ptr->namaAkun);
        printf("\t\t\t%15s %s\n", "ID Akun : ", ptr->idAkun);
        printf("\t\t%15s %d\n", "     Jumlah Password : ", ptr->countPwd);
        printf("\t\t======================================================\n"
               "\t\t|              1. Simpan Password                    |\n"
               "\t\t|              2. Lihat Password                     |\n"
               "\t\t|              3. Cari Password                      |\n"
               "\t\t|              4. Ubah Password                      |\n"
               "\t\t|              5. Bantuan                            |\n"
               "\t\t|              6. Keluar                             |\n"
               "\t\t======================================================\n");
            printf("\t\t> ");
            loop = 0;
            scanf("%d", &input);
        //Fungsi utama program dijalankan di dalam main menu
            switch(input){
                case 1:
                    do{
                        input = savePwd(&ptr, ptr->countPwd);
                    }while(input == 2);
                    break;
                case 2:
                    do{
                        input = 0;
                        lookPwd(ptr);
                        input = viewSetting(ptr);
                    }while(input == 0);
                    printf("\t");
                    system("Pause");
                    break;
                case 3:
                    searchPass(ptr);
                    break;
                case 4:
                    changePass(&ptr, ptr->countPwd);
                    break;
                case 5:
                    helpMain();
                    break;
                case 6:
                    loop = true;
                    break;
            }
    }while(loop == false);
}

//FUNCTION UNTUK MENAMPILKAN LIST PASSWORD====================================================================================
void lookPwd(struct Node* node){
    int i, j, input;
    char tempPass[100], strength[15];
   // bool loop;
    //do{
        //loop = true;
        system("CLS");
        printf("\t==============================================================="
               "===========================================\n");
        printf("\t|                                            Password Lists"
               "                                              |\n");
        printf("\t==============================================================="
               "===========================================\n");
        printf("\t|%20s|%20s|%20s|%20s|%20s|\n",
               "No", "Nama", "ID", "Password", "Strength");
        printf("\t|--------------------------------------------------------------"
               "------------------------------------------|\n");
        for(i = 0; i < node->countPwd; i++){
            strcpy(tempPass, node->password[i].dataPwd[true]);
            decrypt(tempPass, strlen(tempPass), node->password[i].keyPwd);
            grade(tempPass, strlen(tempPass), strength);
            for(j = 0; j < strlen(tempPass); j++){
                tempPass[j] = '*';
            }
            printf("\t|%20d|%20s|%20s|%20s|%20s|\n",
                i+1, node->password[i].namaPwd,
                node->password[i].idPwd, tempPass, strength);
            strcpy(tempPass, "-empty-");
        }
        printf("\t=============================================================="
               "============================================\n\n");
//    }while(loop == true);
}
//FUNCTION UNTUK MENAMPILKAN DETAIL PASSWORD====================================================================================
int viewSetting(struct Node* node){
    int input;
    char tempID[30], tempPass[100], tempPass2[100];
    printf("\t===========================================\n"
               "\t|         Opsi: 1. Lihat Password         |\n"
               "\t|               2. Keluar                 |\n"
               "\t===========================================\n");
        printf("\t> ");
        scanf("%d", &input);
        switch(input){
            case 1:
                printf("\t===========================================\n");
                printf("\tID Akun\t\t: ");
                scanf(" %[^\n]s", tempID);
                printf("\tPassword\t: ");
                scanf(" %[^\n]s", tempPass);
                printf("\t===========================================\n");
                strcpy(tempPass2, node->dataPwd);
                decrypt(tempPass2, strlen(tempPass2), node->key);
                if(strcmp(tempID, node->idAkun) == 0 && strcmp(tempPass2, tempPass) == 0){
                    printf("\tNomor Password\t: ");
                    scanf("%d", &input);
                    if(input > 0 && input <= node->countPwd){
                        strcpy(tempPass, node->password[input-1].dataPwd[true]);
                        decrypt(tempPass, strlen(tempPass), node->password[input-1].keyPwd);
                        printf("\tPassword\t: %s\n", tempPass);
                    }
                    else
                        printf("\tPassword Tidak Ditemukan!!\n", tempPass);
                    
                    printf("\t===========================================\n\t");
                    system("Pause");
                }
                else{
                    printf("\tAwas: ID atau Password Salah!!\n\t");
                    system("Pause");
                }
                break;
            case 2:
                return 3;
                break;
        }
    return 0;
}
//FUNCTION UNTUK MENAMBAHKAN PASSWORD====================================================================================
int savePwd(struct Node** node, int passNum){

    char tempPass[100], tempID[30], tempName[30], strength[15];
    char tempC;
    int tempKey = keygen(), input, i;
    system("CLS");
    printf("\n\t\t================= Simpan Password ====================\n");
    printf("\t\tNama Password\t: ");
    scanf(" %[^\n]s", tempName);
    printf("\t\tID Password\t: ");
    scanf(" %[^\n]s", tempID);
    printf("\t\tBuat Password Otomatis? (1=Y, 0=N)\n\t\t> ");
    scanf("%d", &input);
    if(input == 1){
        randomPasswordGeneration(20, tempPass);
        printf("\t\tPassword\t: %s", tempPass);
    }
    else{
        printf("\t\tPassword\t: ");
        /*do{
        if((tempC = getch()) != 13){
            printf("*");
            tempPass[i] = tempC;
            i++;
        }
        }while(tempC != 13);*/
        scanf(" %[^\n]s", tempPass);
    }
        
    
        
    printf("\n\t\t======================================================\n");
    printf("\t\tMemeriksa Password...\n\t\t");
    system("Pause");
    system("CLS");
    
    printf("\n\t\t================= Simpan Password ====================\n");
    printf("\t\tNama Password\t: %s\n", tempName);
    printf("\t\tID Password\t: %s\n", tempID);
    grade(tempPass, strlen(tempPass), strength);
    printf("\t\tKekuatan Password adalah -%s-", strength);
    printf("\n\t\t======================================================");
    printf("\n\t\tLanjut? (1=Y, 0=N, 2=Ulangi)\n\t\t> ");
    scanf("%d", &input);
        
    if(input == 0){
        return 0;
    }else if(input == 2){
        return 2;
    }

      (*node)->countPwd = passNum+1;
    (*node)->password[passNum].changed=false;
    (*node)->password[passNum].keyPwd = tempKey;
    strcpy((*node)->password[passNum].namaPwd, tempName);
    strcpy((*node)->password[passNum].idPwd, tempID);
    encrypt(tempPass, strlen(tempPass), tempKey);
    (*node)->password[passNum].keyPwd = tempKey;
    strcpy((*node)->password[passNum].dataPwd[true], tempPass);
    strcpy((*node)->password[passNum].dataPwd[false], "-empty-");
    return 1;
}

//FUNCTION UNTUK MENGGANTI PASSWORD=============================================================================================
void changePass(struct Node** node, int countPwd){
    int input, input2;
    char tempID[30], tempPass[100], tempPass2[100];
    bool loop = true;
    do{
        lookPwd(*node);
        printf("\t===========================================\n");
        printf("\tID Akun\t\t: ");
        scanf(" %[^\n]s", tempID);
        printf("\tPassword\t: ");
        scanf(" %[^\n]s", tempPass);
        printf("\t===========================================\n");
        strcpy(tempPass2, (*node)->dataPwd);
                decrypt(tempPass2, strlen(tempPass2), (*node)->key);
                if(strcmp(tempID, (*node)->idAkun) == 0 && strcmp(tempPass2, tempPass) == 0){
                    printf("\tNomor Password\t: ");
                    scanf("%d", &input);
                    if(input >= 1 && input <= (*node)->countPwd){
                        strcpy(tempPass, (*node)->password[input-1].dataPwd[true]);
                        decrypt(tempPass, strlen(tempPass), (*node)->password[input-1].keyPwd);
                        printf("\tBuat Password Otomatis? (1=Y, 0=N)\n\t> ");
                        scanf("%d", &input2);
                        if(input2 == 1){
                            randomPasswordGeneration(20, tempPass2);
                            printf("\tPassword\t: %s\n", tempPass2);
                        }
                        else{
                            printf("\tMasukan password baru: ");
                            scanf(" %[^\n]s", tempPass2);
                        }
                            encrypt(tempPass2, strlen(tempPass2), (*node)->password[input-1].keyPwd);
                            strcpy((*node)->password[input-1].dataPwd[false], (*node)->password[input-1].dataPwd[true]);
                            strcpy((*node)->password[input-1].dataPwd[true], tempPass2);
                        //dataPwd[false] merupakan arsip password yang tidak jadi digunakan
                            printf("\t===========================================\n\t");
                            (*node)->password[input-1].changed = true;
                            printf("\n\tPassword Lama\t: %s\n", tempPass);
                            decrypt(tempPass2, strlen(tempPass2), (*node)->password[input-1].keyPwd);
                            printf("\tPassword Baru\t: %s\n", tempPass2);
                            loop = false;
                        
                    }
                    else{
                        printf("\tPassword Tidak Ditemukan!!\n", tempPass);
                    }
                    printf("\t===========================================\n\t");
                    system("Pause");
                    loop = false;
                }
                else{
                    printf("\t\tAwas: ID atau Password Salah!!\n");
                    system("Pause");
                    loop = false;
                }
    }while (loop == true);
}
void helpDepan(){ //panduan penggunaan
    int input, loop;
    do{
        loop = 1;
        system("CLS");
        printf("\n\t\t            ==== Bantuan OnlyPass ====   \n");
        printf("\t\t======================================================\n"
               "\t\t|              1. Masuk Akun                         |\n"
               "\t\t|              2. Buat Akun                          |\n"
               "\t\t|              3. Bantuan                            |\n"
               "\t\t|              4. Hapus Akun                         |\n"
               "\t\t|              4. Keluar Bantuan                     |\n"
               "\t\t======================================================\n");
        printf("\t\t> ");
        scanf("%d", &input);
        if (input < 1 || input > 4){
            printf("\n\n\tTerdapat Kesalahan Input!!\n\n\t");
            system("Pause");
            return;
        }
        switch(input){
            case 2:
                printf("\t\t========= Panduan Fitur 'Buat Akun' ==========\n");
                printf("\n\t\t================ Buat Akun ===================\n");
                printf("\t\t\tNama\t\t: Andi\n");
                printf("\t\t\tID Akun\t\t: Andi69\n");
                printf("\t\t\tPassword\t: ********\n");
                printf("\t\t======================================================\n\n");
                printf("\t\tDalam menu ini, anda akan diminta untuk memasukkan\n");
                printf("\t\tinput data akun. Terdapat 3 data yakni Nama,\n");
                printf("\t\tID akun, dan password \n\n\t\t");
                system("Pause");
                break;
            case 1:
    
                printf("\t\t========= Panduan Fitur 'Masuk Akun' ==========\n");
                printf("\n\t\t================ Masuk Akun ===================\n");
                printf("\t\t\tID Akun\t\t: Andi69\n");
                printf("\t\t\tPassword\t: ********\n");
                printf("\t\t======================================================\n\n");
                printf("\t\tDalam menu ini, anda akan diminta untuk login dengan\n");
                printf("\t\tinput data akun yang telah  dibuat berupa ID dan Password\n\n\t\t");
                system("Pause");
                break;
                
            case 3:
                printf("\t\tDalam menu ini, anda akan dapat melihat bantuan  \n");
                printf("\t\tdalam halaman depan sebelum masuk ke akun \n\n\t\t");
          
                system("Pause");
                break;
            case 4:
                printf("\t\tDalam menu ini, anda akan dapat keluar dari program\n\n\t\t");
                system("Pause");
                loop = 0;
                return;
        }
    } while (loop == 1);
    
}
void helpMain(){ //panduan penggunaan
    int input, loop, j;
    do{
        loop = 1;
        system("CLS");
        printf("\n\t\t                  =MENU BANTUAN=   \n");
        printf("\t\t======================================================\n");
        printf("\n\t\t                ==== Welcome ====   \n");
        printf("\t\t======================================================\n");
        printf("\t\t\t%15s %s\n", "Nama : ", "Ibrahim Rijal");
        printf("\t\t\t%15s %s\n", "ID Akun : ", "R1j4lG4nt3ng");
        printf("\t\t%15s %d\n", "Jumlah Password : "), 5;
        printf("\t\t======================================================\n"
               "\t\t|              1. Simpan Password                    |\n"
               "\t\t|              2. Lihat Password                     |\n"
               "\t\t|              3. Cari Password                      |\n"
               "\t\t|              4. Ubah Password                      |\n"
               "\t\t|              5. Bantuan                            |\n"
               "\t\t|              6. Keluar                             |\n"
               "\t\t======================================================\n");
            printf("\t\t> ");
        scanf("%d", &input);
        if (input < 1 || input > 6){
            printf("\n\n\t\tTerdapat Kesalahan Input!!\n\n\t");
            system("Pause");
            return;
        }
        switch(input){
            case 1:
                system("CLS");
                printf("\t\t========== Panduan Fitur 'Simpan Password' ===========\n");
                printf("\n\t\t================= Simpan Password ====================\n");
                printf("\t\tNama Password\t: Instagram\n");
                printf("\t\tID Password\t: irphotoarts\n");
                printf("\t\tPassword\t: *****\n");
                printf("\t\tKekuatan Password adalah -SANGAT KUAT-");
                printf("\n\t\t================= -----------------====================\n");
                printf("\t\tPada bagian ini user akan diminta untuk memasukan detail dari\n");
                printf("\t\takun yang ingin disimpan.\n");
                printf("\t\tPada bagian ini program akan melakukan penilaian terhadap\n");
                printf("\t\tpassword yang dimasukan user.\n\n\t\t");
                system("Pause");
                break;
            case 2:
                system("CLS");
                printf("\t========= Panduan Fitur 'Lihat Password' ==========\n");
                printf("\n\t================ Lihat Password ===================\n");
                
                printf("\t=============================================================="
                       "=======================\n");
                printf("\t|                                  Password Lists"
                       "                                   |\n");
                printf("\t=============================================================="
                       "=======================\n");
                printf("\t|%20s|%20s|   %17s|%20s|\n",
                       "No", "Nama", "ID", "Password");
                printf("\t|-------------------------------------------------------------"
                       "----------------------|\n");
                for (j = 0; j <5; j++){
                    printf("\t|%20d|%20s|%20s|%20s|\n", j+1, "Nama", "User ID", "*********");
    
                }
                printf("\t=============================================================="
                       "=======================\n");
                printf("\tPada bagian ini user dapat melihat semua password yang telah \n");
                printf("\tdiinput.\n\n]t]t");
                system("Pause");
                break;
                
            case 3:
                system("CLS");
                printf("\t========= Panduan Fitur 'Cari' ==========\n");
                printf("\n\t================ Cari ===================\n");
                printf("\tMasukan search: insta \n");
                
                printf("\t=============================================================="
                       "=======================\n");
                printf("\t|                                  Password Lists"
                       "                                   |\n");
                printf("\t=============================================================="
                       "=======================\n");
                printf("\t|%20s|%20s|   %17s|%20s|\n",
                       "No", "Nama", "ID", "Password");
                printf("\t|-------------------------------------------------------------"
                       "----------------------|\n");
                
                printf("\t|%20d|%20s| %17s|%20s|\n", 1, "Instagram", "irphotoarts", "*********");
    
                
                printf("\t=============================================================="
                       "=======================\n");
                printf("\tPada bagian ini user dapat mencari nama password yang \n");
                printf("\tdiinginkan.\n\n\t");
                system("Pause");
                break;
            case 4:
                system("CLS");
                printf("\t========= Panduan Fitur 'Ubah Password' ==========\n");
                printf("\n\t================ Ubah Password ===================\n");
                
                printf("\t=============================================================="
                       "=======================\n");
                printf("\t|                                  Password Lists"
                       "                                   |\n");
                printf("\t=============================================================="
                       "=======================\n");
                printf("\t|%20s|%20s|   %17s|%20s|\n",
                       "No", "Nama", "ID", "Password");
                printf("\t|-------------------------------------------------------------"
                       "----------------------|\n");
                for (j = 0; j <5; j++){
                    printf("\t|%20d|%20s| %17s|%20s|\n", j+1, "Nama", "User ID", "*********");
    
                }
                printf("\t=============================================================="
                       "=======================\n");
                
                printf("\tPilih password yang ingin diganti: ");
                printf("\n\n\t\tPada bagian ini user dapat mengubah password yang \n");
                printf("\t\tdiinginkan.\n\n\t");
                system("Pause");
                break;
                
            case 5:
                system("CLS");
                printf("\t\tMenu ini akan menampilkan bantuan untuk main menu\n\n\t\t");
                system("Pause");
                break;
            case 6:
                system("CLS");
                printf("\t\tMenu ini akan keluar dari akun\n\n\t\t");
                system("Pause");
                loop = 0;
        }
    } while (loop == 1);
    
}
//====================================================================================
//SORTING DAN SEARCHING
//==========================================================
void searchPass(struct Node* node){
    char input [100];
    char tempPass[100], strength[15];
    int i, j;
    system("CLS");
    printf("\n\t================ Cari ===================\n");
    printf("\tMasukan search: ");
    scanf(" %[^\n]s", input);
    printf("\n\t==============================================================="
               "===========================================\n");
        printf("\t|                                            Password Lists"
               "                                              |\n");
        printf("\t==============================================================="
               "===========================================\n");
        printf("\t|%20s|%20s|%20s|%20s|%20s|\n",
               "No", "Nama", "ID", "Password", "Strength");
        printf("\t|--------------------------------------------------------------"
               "------------------------------------------|\n");
    
    for(i = 0; i < node->countPwd; i++){
        if(Strcasestr(node->password[i].namaPwd, input)){
            strcpy(tempPass, node->password[i].dataPwd[true]);
            decrypt(tempPass, strlen(tempPass), node->password[i].keyPwd);
            grade(tempPass, strlen(tempPass), strength);
            for(j = 0; j < strlen(tempPass); j++){
                tempPass[j] = '*';
            }
            printf("\t|%20d|%20s|%20s|%20s|%20s|\n",
                i+1, node->password[i].namaPwd,
                node->password[i].idPwd, tempPass, strength);
            strcpy(tempPass, "-empty-");
        }
    }
    printf("\t=============================================================="
           "============================================\n\n");
    system("Pause");
}
char* Strcasestr(const char* haystack, const char* needle) {    //fungsi strcasestr
    if (!needle[0]) return (char*) haystack;
    int i, j;

    for (i = 0; haystack[i]; i++) {
        int matches = 1;
        for (j = 0; needle[j]; j++) {
            if (!haystack[i + j]) return NULL;

            if (tolower((unsigned char)needle[j]) !=
                tolower((unsigned char)haystack[i + j])) {
                matches = 0;
                break;
            }
        }
        if (matches) return (char *)(haystack + i);
    }
    return NULL;
}
//==================================QUICKSORT ARRAY===================================
void quicksortArr(struct Node** node,int start, int end){
    int i, j, pivot;
   if(start < end){
       pivot = start;
       i = start;
       j = end;
      while(i < j){
          while(strcmp((*node)->password[i].namaPwd, (*node)->password[pivot].namaPwd) <= 0 && i<end)
            i++;
          while(strcmp((*node)->password[j].namaPwd, (*node)->password[pivot].namaPwd) > 0 && j > start)
            j--;
         if(i<j){
             //Swapping untuk tiap data dalam struct
             swapChar((*node)->password[i].namaPwd, (*node)->password[j].namaPwd);
             swapChar((*node)->password[i].idPwd, (*node)->password[j].idPwd);
             swapChar((*node)->password[i].dataPwd[false], (*node)->password[j].dataPwd[false]);
             swapChar((*node)->password[i].dataPwd[true], (*node)->password[j].dataPwd[true]);
             swapInt(&(*node)->password[i].keyPwd, &(*node)->password[j].keyPwd);
             swapBool(&(*node)->password[i].changed, &(*node)->password[j].changed);
         }
      }
           swapChar((*node)->password[pivot].namaPwd, (*node)->password[j].namaPwd);
           swapChar((*node)->password[pivot].idPwd, (*node)->password[j].idPwd);
           swapChar((*node)->password[pivot].dataPwd[false], (*node)->password[j].dataPwd[false]);
           swapChar((*node)->password[pivot].dataPwd[true], (*node)->password[j].dataPwd[true]);
           swapInt(&(*node)->password[pivot].keyPwd, &(*node)->password[j].keyPwd);
           swapBool(&(*node)->password[pivot].changed, &(*node)->password[j].changed);
 
    #pragma omp task
      quicksortArr(node,start,j-1);
    #pragma omp task
       quicksortArr(node,j+1,end);
    #pragma omp taskwait

   }
}
void swapChar (char a[100], char b[100]){
    char temp[100];
    char aTemp[100];
    char bTemp[100];
    strcpy(aTemp,a);
    strcpy(bTemp,b);
    //string dipindahkan ke variabel lokal untuk menghindari tabrakan dalam mengakses memori
    strcpy(temp,aTemp);
    strcpy(aTemp,bTemp);
    strcpy(bTemp,temp);
    
    strcpy(a, aTemp);
    strcpy(b, bTemp);
}
void swapInt(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void swapBool(bool *a, bool *b){
    bool temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
//====================================================================================
//MANAJEMEN PASSWORD
//====================================================================================

//ENKRIPSI DAN DEKRIPSI MENGGUNAKAN ALGOTITMA CAESAR
//DIMANA KARAKTER AKAN DIGESER SEJUMLAH KEY KE SEBELAH KANAN
void encrypt(char pass[100], unsigned long len, int key){
    int i;
    #pragma omp parallel for
    for(i = 0; i < len; i++){
        pass[i] = pass[i] + key;
    }
}
void decrypt(char pass[100], unsigned long len, int key){
    int i;
    #pragma omp parallel for
    for(i = 0; i < len; i++){
        pass[i] = pass[i] - key;
    }
}

//FUNCTION GRADE PASSWORD BERDASARKAN KOMBINASI KARAKTER==============================================================
char grade(char pass[100], unsigned long len, char tempStrength[]){
    int grade = 0, i;
    char status[5][15] = {"Sangat Lemah", "Lemah", "Sedang", "Kuat", "Sangat Kuat"};
    bool strength[5] = {false};
    #pragma omp parallel for
    for (i = 0; i < len; i++){
        if(pass[i] >= 48 && pass[i] <= 57){
            strength[0] = true;
        }
        if(pass[i] >= 97 && pass[i] <= 122){
            strength[1] = true;
        }
        if(pass[i] >= 65 && pass[i] <= 90){
            strength[2] = true;
        }
        if(pass[i] >= 33 && pass[i] <= 47){
            strength[3] = true;
        }
        if(pass[i] >= 58 && pass[i] <= 64){
            strength[3] = true;
        }
        if(pass[i] >= 91 && pass[i] <= 96){
            strength[3] = true;
        }
        if( len >= 8){
            strength[4] = true;
        }
    }
    #pragma omp parallel for reduction (+ : grade)
    for(i = 0; i<5; i++){
        //#pragma omp critical
        grade += strength[i];
    }
    if (strcasecmp(pass, "PakDodiGanteng") == 0){
        grade = 5;
    }
    strcpy(tempStrength, status[grade-1]);
    return *tempStrength;
}

//FUNCTION UNTUK MEMBUAT PASSWORD RANDOM============================================================================
void randomPasswordGeneration(int N, char pass[100]){
    int i = 0;
    int randomizer = 0;
    srand((unsigned int)(time(NULL)));
  
    char numbers[] = "0123456789";
    char letter[] = "abcdefghijklmnoqprstuvwyzx";
    char LETTER[] = "ABCDEFGHIJKLMNOQPRSTUYWVZX";
    char symbols[] = "!@#$^&*?";
  
    for (i = 0; i < N; i++) {
        if (i == 5 || i == 11){
            pass[i] = '-';
        }
        else if (randomizer == 1) {
            pass[i] = numbers[rand() % 10];
            randomizer = rand() % 4;
        }
        else if (randomizer == 2) {
            pass[i] = symbols[rand() % 8];
            randomizer = rand() % 4;
        }
        else if (randomizer == 3) {
            pass[i] = LETTER[rand() % 26];
            randomizer = rand() % 4;
        }
        else {
            pass[i] = letter[rand() % 26];
            randomizer = rand() % 4;
        }
        
    }
}

//KEYGEN UNTUK ENKRIPSI PASSWORD
int keygen(){
    int key;
    srand((unsigned int)(time(NULL)));
       key = rand() %20 ;
    if(key == 0){
        key = 18;
    }
    return key;
}
//====================================================================================




