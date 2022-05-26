PA#include <stdio.h>
//#include <omp.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
//#include <conio.h>
//#include <curses.h>
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
    List password[50];
    struct Node* next;
};

void writeCount(int count){
    const char* fileCount     = "{\n\t\"COUNT\": %d}\n";
    int i;
    FILE* file;
    fopen_s(&file, "Count.dat", "w");
    fprintf_s(file, fileCount, count);
    fclose(file);
}
void readCount(int *count){
    const char* fileCount     = "{\n\t\"COUNT\": %d}\n";
    int i, temp;
    FILE* file;
    fopen_s(&file, "Count.dat", "r");
    fscanf_s(file, fileCount, count);
    fclose(file);
}
void writeFile(struct Node* node, int count){
	int i, j, k, temp;
    FILE* file;
    file = fopen("akun.txt", "w");
    for (i = 0; i < count; i++) {
        fprintf(file,"%s %s %s %d %d;",node->namaAkun,node->idAkun,node->dataPwd,node->key,node->countPwd);
        for (j = 0; j < node->countPwd; j++){
        	if(node->password[j].changed == true){
        		temp = 1;
			}
			else if(node->password[j].changed == false){
				temp = 0;
			}
        	fprintf(file,"%s %s %d %d;",node->password[j].namaPwd,node->password[j].idPwd,node->password[j].keyPwd,temp);
        	if(temp == 1){
        		for(k=0;k<2;k++){
        			fprintf(file,"%s ",node->password[j].dataPwd[k]);
				}
			}else if(temp == 0){
				fprintf(file,"%s ",node->password[j].dataPwd[1]);
			}
		}
		fprintf(file,"\n");
		node = node->next;
    }
    fclose(file);
}
void readFile(struct Node** node, int count) {
    int i, j, k, temp;
    FILE* file;
    file = fopen("akun.txt", "r");
    for (i = 0; i < count; i++) {
        fscanf(file,"%s %s %s %d %d ",(*node)->namaAkun,(*node)->idAkun,(*node)->dataPwd,&(*node)->key,&(*node)->countPwd);
        for (j = 0; j < (*node)->countPwd; j++){
        	fscanf(file,"%s %s %d %d ",(*node)->password[j].namaPwd,(*node)->password[j].idPwd,&(*node)->password[j].keyPwd,&temp);
        	if(temp == 1){
        		for(k=0;k<2;k++){
        			fscanf(file,"%s ",(*node)->password[j].dataPwd[k]);
				}
				(*node)->password[j].changed = true;
			}else if(temp == 0){
				fscanf(file,"%s ",(*node)->password[j].dataPwd[1]);
				(*node)->password[j].changed = false;
			}
		}
		fprintf(file,"\n");
		(*node) = (*node)->next;
    }
    fclose(file);
}
int addAccount (struct Node** headRef);
int enterAccount(struct Node* headRef, int countAccount);
void mainMenu(struct Node** headRef, int numAccount);
void printList(struct Node* node);
void helpDepan();
void helpMain();
char* Strcasestr(const char* haystack, const char* needle);
void push(struct Node** head_ref, char new_data[100], char new_ID[100], char new_name[100], int new_key);
struct Node* partition(struct Node* head, struct Node* end, struct Node** newHead, struct Node** newEnd);
struct Node* getTail(struct Node* cur);
struct Node* quickSortRecur(struct Node* head, struct Node* end);
void quickSort(struct Node** headRef);
void randomPasswordGeneration(int N, char pass[100]);
char grade(char pass[100], unsigned long len, char tempStrength[]);
//SORTING===============================================
void swapChar(char a[100], char b[100]);
void swapInt(int *a, int *b);
void swapBool(bool *a, bool *b);
void quicksortArr(struct Node** node,int start, int end);
//PASSWORD===============================================
int savePwd(struct Node** node, int passNum);
void searchPass(struct Node* node);
int keygen();
void printNode(struct Node* node);
void changePass(struct Node** node, int passNum);
void searchPass(struct Node* node);
void decrypt(char pass[100], unsigned long len, int key);
void encrypt(char pass[100], unsigned long len, int key);
void lookPwd(struct Node* node);
int viewSetting(struct Node* node);

int main(){
    int input, loop = 0, countAccount = 0;
    readCount(&countAccount);
    struct Node* head = NULL;
    //readFile(&head, countAccount);
       do{
        do{
            system("CLS");
            printf("\n\t\t              ==== OnlyPass ====   \n");
            printf("\t\t======================================================\n"
                   "\t\t|              1. Masuk Akun                         |\n"
                   "\t\t|              2. Buat Akun                          |\n"
                   "\t\t|              3. Bantuan                            |\n"
                   "\t\t|              4. Keluar                             |\n"
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
                    input = addAccount(&head);
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
                }while(input == 0);
                countAccount++;
                break;
            case 3:
            	helpDepan();
                break;
            case 4:
            	writeFile(head, countAccount);
            	writeCount(countAccount);
                return 0;
            default:
                break;
        }
      }while(loop == 0);
    return 0;
}
//====================================================================================
//USER
//====================================================================================
int addAccount (struct Node** headRef){ //Function untuk menambahkan akun berupa Node
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    struct Node *last = *headRef;
    int tempKey = keygen(), input;
    char tempPass[100] = {0};
    char strength[15];
    
    system("CLS");
    printf("\n\t\t================ Sign Up ===================\n");
    printf("\t\tNama\t\t: ");
    scanf(" %[^\n]s", new_node -> namaAkun );
    printf("\t\tID Akun\t\t: ");
    scanf(" %[^\n]s", new_node -> idAkun );
    printf("\t\tPassword\t: ");
    scanf(" %[^\n]s", tempPass);
        
//    do{
//        if((tempC = getch()) != 13){
//            printf("*");
//            tempPass[i] = tempC;
//            i++;
//        }
//    }while(tempC != 13);
        
    printf("\n\t\t============================================\n");
    printf("\t\tMemeriksa Password...\n\t\t");
    system("Pause");
    system("CLS");
        
    printf("\n\t\t================ Account ===================\n");
    printf("\t\tNama\t\t: %s\n", new_node -> namaAkun );
    printf("\t\tID Akun\t\t: %s\n", new_node -> idAkun);
    grade(tempPass, strlen(tempPass), strength);
    printf("\t\tKekuatan Password adalah -%s-", strength);
    printf("\n\t\t============================================");
    printf("\n\t\tLanjut? (1=Y, 0=N)\n\t\t> ");
    scanf("%d", &input);
        
    if(input == 0){
        return 0;
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
int enterAccount(struct Node* headRef, int countAccount) {   //BLM JALAN ERROR DI NODE NGECEK ID/PASSWORDNYA
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
void mainMenu(struct Node** headRef, int numAccount){    //Function untuk menampilkan main menu
    int i, input, loop = false;
    struct Node *ptr = *headRef;
    for(i = 0; i < numAccount; i++){
        ptr = ptr->next;
    }
    
    do{
    	quicksortArr(&ptr, 0, ptr->countPwd -1);
        system("CLS");
        printf("\n\t\t              ==== Welcome, s ====   \n");
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
                    }while(input == 0);
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
//	}while(loop == true);
}
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
	            	printf("\t\tAwas: ID atau Password Salah!!\n");
				}
				break;
			case 2:
				return 3;
				break;
		}
}
int savePwd(struct Node** node, int passNum){

    char tempPass[100], tempID[30], tempName[30], strength[15];
    int tempKey = keygen(), input;
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
	    scanf(" %[^\n]s", tempPass);
	}
        
//    do{
//        if((tempC = getch()) != 13){
//            printf("*");
//            tempPass[i] = tempC;
//            i++;
//        }
//    }while(tempC != 13);
        
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
    printf("\n\t\tLanjut? (1=Y, 0=N)\n\t\t> ");
    scanf("%d", &input);
        
    if(input == 0){
        return 0;
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
}
void changePass(struct Node** node, int countPwd){
    int input;
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
	                if(input < 1 && input >= (*node)->countPwd){
	                	strcpy(tempPass, (*node)->password[input-1].dataPwd[true]);
					    decrypt(tempPass, strlen(tempPass), (*node)->password[input-1].keyPwd);
					    
					    printf("\tMasukan Password Lama: ");
					    scanf(" %[^\n]s", tempPass2);
					    if(strcmp(tempPass, tempPass2) != 0){
					        printf("\tPassword Salah!!");
	            			loop = false;
					    }
					    else{
					        printf("\tMasukan password baru: ");
						    scanf(" %[^\n]s", tempPass2);
						    encrypt(tempPass2, strlen(tempPass2), (*node)->password[input-1].keyPwd);
						    strcpy((*node)->password[input-1].dataPwd[false], (*node)->password[input-1].dataPwd[true]);
						    strcpy((*node)->password[input-1].dataPwd[true], tempPass2);
						    printf("\t===========================================\n\t");
						    (*node)->password[input-1].changed = true;
						    printf("\n\tPassword Lama\t: %s\n", tempPass);
						    decrypt(tempPass2, strlen(tempPass), (*node)->password[input-1].keyPwd);
						    printf("\tPassword Baru\t: %s\n", tempPass2);
						    loop = false;
						}
					}
					else{
						printf("\tPassword Tidak Ditemukan!!\n", tempPass);
	            	}
	                printf("\t===========================================\n\t");
	                system("Pause");
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
        printf("\n\t\t              ==== OnlyPass ====   \n");
        printf("\t\t======================================================\n"
               "\t\t|              1. Masuk Akun                         |\n"
               "\t\t|              2. Buat Akun                          |\n"
               "\t\t|              3. Bantuan                            |\n"
               "\t\t|              4. Keluar                             |\n"
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
                printf("\t========= Panduan Fitur 'Buat Akun' ==========\n");
                printf("\n\t================ Buat Akun ===================\n");
                printf("\t\tNama\t\t\t: Andi\n");
                printf("\t\tID Akun\t\t: Andi69\n");
                printf("\t\tPassword\t\t: ********\n");
                printf("\t======================================================\n\n");
                printf("\tDalam menu ini, anda akan diminta untuk memasukkan\n");
                printf("\tinput data akun. Terdapat 3 data yakni Nama,\n");
                printf("\tID akun, dan password \n");
                system("Pause");
                break;
            case 1:
    
                printf("\t========= Panduan Fitur 'Masuk Akun' ==========\n");
                printf("\n\t================ Masuk Akun ===================\n");
                printf("\t\tID Akun\t\t: Andi69\n");
                printf("\t\tPassword\t\t: ********\n");
                printf("\t======================================================\n\n");
                printf("\tDalam menu ini, anda akan diminta untuk login dengan\n");
                printf("\tinput data akun yang telah  dibuat berupa ID dan Password\n");
                system("Pause");
                break;
                
            case 3:
                printf("\tDalam menu ini, anda akan dapat melihat bantuan  \n");
                printf("\tdalam halaman depan sebelum masuk ke akun \n");
          
                system("Pause");
                break;
            case 4:
                printf("\tDalam menu ini, anda akan dapat keluar dari program\n");
                system("Pause");
                return;
        }
    } while (loop == 1);
    
}
void helpMain(){ //panduan penggunaan
    int input, loop, j;
    do{
        loop = 1;
        system("CLS");
        printf("\n\t\t              ==== Welcome, s ====   \n");
        printf("\t\t======================================================\n");
        printf("\t\t\t%15s %s\n", "Nama : Ibrahim Rijal");
        printf("\t\t\t%15s %s\n", "ID Akun : R1j4lG4nt3ng");
        printf("\t\t%15s %d\n", "     Jumlah Password : ");
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
            printf("\n\n\tTerdapat Kesalahan Input!!\n\n\t");
            system("Pause");
            return;
        }
        switch(input){
            case 1:
                system("CLS");
                printf("\t========= Panduan Fitur 'Simpan Password' ==========\n");
                printf("\n\t\t================= Simpan Password ====================\n");
                printf("\t\tNama Password\t: Instagram\n");
                printf("\t\tID Password\t: irphotoarts\n");
                printf("\t\tPassword\t: *****\n");
                printf("\t\tKekuatan Password adalah -SANGAT KUAT-");
                printf("\n\t\t================= -----------------====================\n");
                printf("\t\tPada bagian ini user akan diminta untuk memasukan detail dari\n");
                printf("\t\takun yang ingin disimpan.\n");
                printf("\t\tPada bagian ini program akan melakukan penilaian terhadap\n");
                printf("\t\tpassword yang dimasukan user.\n");
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
                printf("\t\tPada bagian ini user dapat melihat semua password yang telah \n");
                printf("\t\tdiinput.\n");
                system("Pause");
                break;
                
            case 3:
                system("CLS");
                printf("\t========= Panduan Fitur 'Cari' ==========\n");
                printf("\n\t================ Cari ===================\n");
                printf("Masukan search: insta \n");
                
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
                printf("\t\tPada bagian ini user dapat mencari nama password yang \n");
                printf("\t\tdiinginkan.\n");
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
                
                printf("Pilih password yang ingin diganti: ");
                printf("\t\tPada bagian ini user dapat mengubah password yang \n");
                printf("\t\tdiinginkan.\n");
                system("Pause");
                break;
                
            case 5:
                system("CLS");
                printf("\t\tMenu ini akan menampilkan bantuan untuk main menu\n");
                system("Pause");
            case 6:
                system("CLS");
                printf("\t\tMenu ini akan keluar dari akun\n");
                system("Pause");
        }
    } while (loop == 1);
    
}
//====================================================================================
//SORTING DAN SEARCHING
//==========================================================
void searchPass(struct Node* node){ //gw ganti jd strcasestr
    char input [100];
    char tempPass[100], strength[15];
    int i, j;
    system("CLS");
    printf("Masukan search: ");
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
       
      quicksortArr(node,start,j-1);
      quicksortArr(node,j+1,end);

   }
}
void swapChar (char a[100], char b[100]){
    char temp[100];
    strcpy(temp,a);
    strcpy(a, b);
    strcpy(b, temp);
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
void encrypt(char pass[100], unsigned long len, int key){ 
    int i;
    for(i = 0; i < len; i++){
        pass[i] = pass[i] + key;
    }
}
void decrypt(char pass[100], unsigned long len, int key){
    int i;
    for(i = 0; i < len; i++){
        pass[i] = pass[i] - key;
    }
}
char grade(char pass[100], unsigned long len, char tempStrength[]){
    int grade, i;
    char status[5][15] = {"Sangat Lemah", "Lemah", "Sedang", "Kuat", "Sangat Kuat"};
    bool enoughChar, hasLower, hasUpper, hasNumber, hasSymbol;
    for (i = 0; i < len; i++){
        if(pass[i] >= 48 && pass[i] <= 57){
            hasNumber = true;
        }
        if(pass[i] >= 97 && pass[i] <= 122){
            hasLower = true;
        }
        if(pass[i] >= 65 && pass[i] <= 90){
            hasUpper = true;
        }
        if(pass[i] >= 33 && pass[i] <= 47){
            hasSymbol = true;
        }
        if(pass[i] >= 58 && pass[i] <= 64){
            hasSymbol = true;
        }
        if(pass[i] >= 91 && pass[i] <= 96){
            hasSymbol = true;
        }
        if( len >= 8){
            enoughChar = true;
        }
    }
    grade = enoughChar + hasLower + hasSymbol + hasNumber + hasUpper;
    if (strcasecmp(pass, "PakDodiGanteng") == 0){
        grade = 5;
    }
    strcpy(tempStrength, status[grade-1]);
    return *tempStrength;
}
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
int keygen(){
    int key;
    srand((unsigned int)(time(NULL)));
    //for(i = 0; i < 100; i++){
   key = rand() %20 ;
    if(key == 0){
        key = 18;
    }
    return key;
}
//====================================================================================

//UNUSED========================================================
void printNode(struct Node* node){
    char temp[100];
    strcpy(temp, node->dataPwd);
    decrypt(temp, strlen(temp), node->key);
    printf("Akun\t: %s\n", node->namaAkun);
    printf("ID\t: %s\n", node->idAkun);
    printf("Password: %s\n", temp);
}
void printList(struct Node* node){
    char temp[100];
    
    
    //printf("\n%sstrlen: %lu", temp, strlen(temp));
    
    while (node != NULL) {
        strcpy(temp, node->dataPwd);
        decrypt(temp, strlen(temp), node->key);
        printf("Akun\t: %s\n", node->namaAkun);
        printf("ID\t: %s\n", node->idAkun);
        printf("Password: %s\n", temp);
        node = node->next;
    }
    printf("\n");
}
//==================================QUICKSORT LINKED LIST=============================
void push(struct Node** head_ref, char new_data[100],
          char new_ID[100], char new_name[100], int new_key){
    /* 1. allocate node */
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
 
    /* 2. put in the data  */
    strcpy(new_node->dataPwd, new_data );
    strcpy(new_node->idAkun, new_ID );
    strcpy(new_node->namaAkun, new_name );
    new_node -> key = new_key;
    
    
    //new_node->data  = new_data;
 
    /* 3. Make next of new node as head */
    new_node->next = (*head_ref);
 
    /* 4. move the head to point to the new node */
    (*head_ref)    = new_node;
}
struct Node* getTail(struct Node* cur){
    if(cur != NULL){
        while (cur != NULL && cur->next != NULL)
            cur = cur->next;
    }
    return cur;
}
struct Node* partition(struct Node* head, struct Node* end,
                       struct Node** newHead,
                       struct Node** newEnd){
    struct Node* pivot = end;
    struct Node *prev = NULL, *cur = head, *tail = pivot;
    while (cur != pivot) {
        if (strcmp(cur->namaAkun, pivot->namaAkun)<0) {
            // First node that has a value less than the
            // pivot - becomes the new head
            if ((*newHead) == NULL)
                (*newHead) = cur;
 
            prev = cur;
            cur = cur->next;
        }
        else // If cur node is greater than pivot
        {
            // Move cur node to next of tail, and change
            // tail
            if (prev)
                prev->next = cur->next;
            struct Node* tmp = cur->next;
            cur->next = NULL;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }
    if ((*newHead) == NULL)
        (*newHead) = pivot;
    (*newEnd) = tail;
    return pivot;
}
struct Node* quickSortRecur(struct Node* head, struct Node* end){
    // base condition
    if (!head || head == end)
        return head;
    struct Node *newHead = NULL, *newEnd = NULL;
    struct Node* pivot
        = partition(head, end, &newHead, &newEnd);
    if (newHead != pivot) {
        struct Node* tmp = newHead;
        while (tmp->next != pivot)
            tmp = tmp->next;
        tmp->next = NULL;
        newHead = quickSortRecur(newHead, tmp);
        tmp = getTail(newHead);
        tmp->next = pivot;
    }
    pivot->next = quickSortRecur(pivot->next, newEnd);
    return newHead;
}
void quickSort(struct Node** headRef){
    (*headRef) = quickSortRecur(*headRef, getTail(*headRef));
    return;
}



