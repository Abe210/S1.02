#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#define MAX_NAME 256
#define TABLE_SIZE 4000000
#define MAXCHAR 100

#define MAX_NAME 256 //Longueur maximale d'un Nom
// DEFINITION D'UN TYPE PERSON : a person
// Element: un prénom unique (Ex: ALain=Alain mais Alain!=Marc)
//min_w = "Minimum year of appearance" 
struct person{
    char *name; //Hash key
    int sex[2]; //sex[0]=nb_women & sex[1]=nb_men => nb_total = sex[0] + sex[1]
    int birthdate[4]; // birthdate[0]=min_w, birthdate[1]=max_w, birthdate[2]=min_m & birthdate[3]=max_m
    struct person *next;
};
typedef struct person person;

//List
typedef struct person *Liste;
Liste liste_vide = (Liste)NULL;

//Array of List 
//A person can be easily found by index = hash("name")
// -During collision, if hash("Name") = hash("Name1") then the person is 
//  chained on the List otherwise the List is in front
typedef Liste *HashTable;
HashTable hash_vide = (HashTable)NULL;

Liste create_person(char* name, int sexe, int birthdate,int number) {
    // Creates a pointer to a new hash table person item
    Liste ptrperson = malloc (sizeof(person));
    ptrperson->name = malloc (strlen(name)+1);
    strcpy(ptrperson->name, name);
    switch (sexe)
    {
        case 1: //it is a woman
            ptrperson->sex[0] = number;
            ptrperson->sex[1] = 0;
            ptrperson->birthdate[0] = birthdate;
            ptrperson->birthdate[1] = birthdate;
            break;
        case 2: //it is a man
            ptrperson->sex[1] = number;
            ptrperson->sex[0] = 0;
            ptrperson->birthdate[2] = birthdate;
            ptrperson->birthdate[3] = birthdate;
        default:
            //printf("Error format - Column 1");
            break;
    }
    ptrperson->next = NULL;
    return ptrperson;
}
bool EstVide(Liste L) { return L == liste_vide ? true : false; }
Liste GetSucc(Liste L) {
  if (EstVide(L))
    return liste_vide;
  return L->next;
}
//Display a person
void displayPerson(Liste L){
    Liste tmpL = L;
    while (EstVide(tmpL) == false) {
        printf("%i\t%s\t%i -- ",tmpL->sex[0], tmpL->name, tmpL->birthdate[0]);
        tmpL = GetSucc(tmpL);
    }
    printf("\n");
}

//Return 0 if error, the converted number otherwise
int ConvertToInt(char *s){
    char * pEnd;
    const long sl = strtol(s, &pEnd, 10);
    errno = 0;
    if (pEnd == s) {
        //fprintf(stderr, "%s: not a decimal number\n", s);
        return 0;
    } else if ('\0' != *pEnd) {
        //fprintf(stderr, "%s: extra characters at end of input: %s\n", s, pEnd);
        return 0;
    } else {
        return (int)sl;
    }
    
}

//Returns the index for the table of a specific person based on its name (name is the key) 
unsigned int hash(char *name){
    const int prime_number = 31; 
    int length = strlen(name);
    unsigned int hash = 0;
    for(int i=0; i < length; i++){
        hash = ((prime_number^i) * hash + name[i])% TABLE_SIZE; //modulo TABLE_SIZE to have 0 <= hash <= Table of Person, otherwise the index will be out of range
    }
    return hash;
}

HashTable init_hash_table(HashTable hashtable){
    for(int i=0; i < TABLE_SIZE; i++){
        hashtable[i] = NULL;
    }
    return hashtable;
}

void print_table(HashTable hash_table){
    Liste L = NULL;
    for(int i=0; i < TABLE_SIZE; i++){
        L = hash_table[i];
        if(L == NULL){
            printf("%i\t-------\n",i);
        }else{
            while(!EstVide(L)){
                printf("%i) %i %i %s %i %i %i %i --> ",i, L->sex[0], L->sex[1], L->name, L->birthdate[0],L->birthdate[1],L->birthdate[2],L->birthdate[3]);
                L = L->next;
            }
            printf("\n");
        }
    }
}

void free_table(HashTable hash_table){
    Liste Lcurrent = NULL;
    Liste Lnext = NULL;
    for(int i=0; i < TABLE_SIZE; i++){
        Lcurrent = hash_table[i];
        while(!EstVide(Lcurrent)){
            Lnext = Lcurrent->next;
            free(Lcurrent);
            Lcurrent = Lnext;
        }
    }
    free(hash_table);
}

//find an Element in the table by their name (key)
Liste hash_search(HashTable hash_table, Liste L){
    int i = hash(L->name);
    Liste tmpl = hash_table[i];
    while(!EstVide(tmpl) && strcmp(tmpl->name,L->name)!=0){
        tmpl = GetSucc(tmpl);
    }
    return tmpl;
}

bool hash_table_insert(HashTable hash_table, Liste L){
    if (EstVide(L)) return false;
    int i = hash(L->name);
    Liste tmpl=NULL;
    if (hash_table[i] != NULL) //collision
    {
        tmpl = hash_search(hash_table,L);
        if (tmpl != NULL){ //Found
            if (L->sex[0] > 0)
            {
                tmpl->sex[0] = tmpl->sex[0] + L->sex[0];
                if (L->birthdate[0] < tmpl->birthdate[0] && L->birthdate[0] != 0) //datebirth check if less than min_date_of_birth for women
                {
                    tmpl->birthdate[0] = L->birthdate[0]; 
                }
                if (L->birthdate[1] > tmpl->birthdate[1]) //datebirth check if greater than max_date_of_birth for women
                {
                    tmpl->birthdate[1] = L->birthdate[1];
                }
            }else if (L->sex[1] > 0)
            {
                tmpl->sex[1] = tmpl->sex[0] + + L->sex[1];
                if (L->birthdate[2] < tmpl->birthdate[2] && L->birthdate[0] != 0) //datebirth check if less than min_date_of_birth for men
                {
                    L->birthdate[2] = tmpl->birthdate[2]; 
                }
                if (L->birthdate[3] > tmpl->birthdate[3]) //datebirth check if greater than max_date_of_birth for men
                {
                    L->birthdate[3] = tmpl->birthdate[3];
                }
            }
            return true; 
        }
    }
    L->next = hash_table[i];
    hash_table[i] = L; //put in front List if not exist
    return true;
}

//Read the CSV and store into appropriate structure
//second argument HashTable *ht
void readCSV(char *filename,HashTable hash_table){
    //arguments (File *fp, Liste Hashtable[SIZE], char *fileName)

    FILE *fp;
    fp = fopen(filename,"r");
    if(fp == NULL) {
        perror("Error in opening file - Empty file");
        exit(1);
    }

    // Process Data here
    Liste tmp = NULL;
    char *b = NULL;
    size_t bufsize = 50;
    b = malloc(bufsize * sizeof(char));
    char *token;
    char delim = ';';
    char *line; //pour stocker une ligne de strsep

    int s=0;
    char *prenom = NULL;
    int annee=0;
    int nb=0;

    getline(&b,&bufsize,fp); //1st line: column names - "sexe;preusuel;annais;dpt;nombre"
    while (!feof(fp))
    {
        getline(&b,&bufsize,fp);
        for (int i = 0; i < 5; i++) { // 5 columns
            token = strsep(&b, &delim);
            switch (i){
                /* for every element we store it in appropriate type */
                case 0: //Convert sexe and year to int
                    s = ConvertToInt(token); // gère le cas ou token est invalide
                    break;
                case 1: //store name as string
                    //bool x = hash_table_insert(hashtable,);
                    //printf("hash_table[6]->data->name = %s\n",hash_table->sex hash_table[6]->data->name);
                    prenom = token;
                    break;
                
                case 2:
                    annee = ConvertToInt(token);
                    break;
                
                case 4:
                    s = strcspn(token, "\r\n"); //return the index of the first occurence of "\r\n"
                    token[s] = '\0'; // "\r\n" =>"\0" (or "\0\n")
                    nb = ConvertToInt(token);
                    break; 

                default:
                    break;
            }
        }
        tmp = create_person(prenom,s,annee,nb);
        hash_table_insert(hash_table,tmp);
    }
        fclose(fp);
}

//User menu: choice 0 to 4
void menu(HashTable hashtable){ //param: HashTable *ht,
  int total=0;
  int choix = -1;
  char choix1 = 'x';
  char prenom[MAXCHAR];
  while (choix != 4)
  {
    printf("Que voulez-vous afficher ? (0 pour le menu) > ");
    scanf("%d", &choix);
    switch (choix)
    {
      case 0:
        printf("0: Ce menu\n");
        printf("1: Le nombre de naissances\n");
        printf("2: Le nombre de prénoms\n");
        printf("3: Statistiques sur un prénom\n");
        printf("4: Quitter\n");
        break;
      case 1:
        //printf("Le fichier recouvre %d naissances.\n");
        break;
      case 2:

        break;
      case 3:
        printf("Quel prénom ? ");
        scanf("%s", prenom);
        break;
      
      default:
        break;
    }
  }
  printf("\nBye\n\n");
}

int main(int argc, char *argv[]){

    char filename[100] = "Test.csv";
    //char filename[100] = "dpt2020.csv";
    HashTable hashtable = calloc(TABLE_SIZE, sizeof(Liste));
    //make table "heads" NULL
    for(int i = 0; i < TABLE_SIZE; i++){
    	hashtable[i] = NULL;
    }
    printf("TABLE INITIALIZED! \n\n");

    readCSV(filename,hashtable);
    //print_table(hashtable);
    person person = {.name="Justine"};
    Liste ptr = &person;   
    Liste someone = hash_search(hashtable,ptr);
    printf("Nom: %s %i %i %i %i",someone->name, someone->birthdate[0], someone->birthdate[1], someone->birthdate[2], someone->birthdate[3]);
    // print_table();
    free_table(hashtable);


    return 0;
}

