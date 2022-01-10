#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include<ctype.h>

#define TABLE_SIZE 3500000
#define MAX_NAME 256 //Longueur maximale d'un Nom
// DEFINITION D'UN TYPE PERSON : a person
// Element: un prénom unique (Ex: ALain=Alain mais Alain!=Marc)
//min_w = "Minimum year of appearance" 
struct person{
    char name[MAX_NAME]; //Hash key
    int sex[2]; //sex[0]=nb_women & sex[1]=nb_men => nb_total = sex[0] + sex[1]
    int birthdate[4]; // birthdate[0]=min_w, birthdate[1]=max_w, birthdate[2]=min_m & birthdate[3]=max_m
    struct person *next;
};
typedef struct person person;

//List
//Array of List 
//A person can be easily found by index = hash("name")
// -During collision, if hash("Name") = hash("Name1") then the person is 
//  chained on the List otherwise the List is in front
typedef struct person *Liste;
Liste liste_vide = (Liste)NULL;


Liste create_person(char* name, int sexe, int birthdate,int number) {
    // Creates a pointer to a new hash table person item
    Liste ptrperson = malloc(sizeof(person));
    if (ptrperson == NULL) {
        printf("Error! memory not allocated.");
        exit(-1);
    }
    strcpy(ptrperson->name, name);
    switch (sexe)
    {
        case 1: //it is a woman
            ptrperson->sex[0] = number;
            ptrperson->birthdate[0] = birthdate;
            ptrperson->birthdate[1] = birthdate;
            break;
        case 2: //it is a man
            ptrperson->sex[1] = number;
            ptrperson->birthdate[2] = birthdate;
            ptrperson->birthdate[3] = birthdate;
        default:
            //printf("Error format - Column 1");
            break;
    }
    //ptrperson->next = NULL;
    return ptrperson;
}
bool EstVide(Liste L) { return L == liste_vide ? true : false; }
Liste GetSucc(Liste L) {
  if (EstVide(L))
    return liste_vide;
  return L->next;
}

//Return 0 if error, the converted number otherwise
int ConvertToInt(char *s){
    char * pEnd;
    long sl = strtol(s, &pEnd, 10);
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

Liste init_hash_table(Liste *hashtable){
    for(int i=0; i < TABLE_SIZE; i++){
        hashtable[i] = NULL;
    }
    return hashtable[0];
}

void print_table(Liste *hash_table){
    Liste L;
    for(int i=0; i < TABLE_SIZE; i++){
        L = hash_table[i];
        if(L == NULL){
            printf("%i\t-------\n",i);
        }else{
            while(!EstVide(L)){
                printf("%i) %i %i %s %i %i %i %i --> ",i, L->sex[0], L->sex[1], L->name, L->birthdate[0],L->birthdate[1],L->birthdate[2],L->birthdate[3]);
                L = GetSucc(L);
            }
            printf("\n");
        }
    }
}

void free_table(Liste *hash_table){
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
Liste hash_search(Liste *hash_table, char *s){
    int i = hash(s);
    Liste tmph = hash_table[i];
    while(!EstVide(tmph) && strcmp(tmph->name,s)!=0){        
        tmph = GetSucc(tmph);
    }
    return tmph;
}

bool hash_table_insert(Liste *hash_table, Liste L){
    if (EstVide(L)) return false;
    int i = hash(L->name);
    Liste tmpl = NULL;
    if (hash_table[i] != NULL) //collision
    {
        tmpl = hash_search(hash_table,L->name); //Search if Person already exist
        if (tmpl != NULL){ //Found
            if (L->sex[0] > 0)
            {
                tmpl->sex[0] = tmpl->sex[0] + L->sex[0];
                if (L->birthdate[0] < tmpl->birthdate[0] && L->birthdate[0] != 0) //datebirth check if less than min_date_of_birth for women
                {
                    hash_table[i]->birthdate[0] = L->birthdate[0]; 
                }
                if (L->birthdate[1] > tmpl->birthdate[1]) //datebirth check if greater than max_date_of_birth for women
                {
                    hash_table[i]->birthdate[1] = L->birthdate[1];
                }
            }else if (L->sex[1] > 0)
            {
                hash_table[i]->sex[1] = hash_table[i]->sex[0] + L->sex[1];
                if (L->birthdate[2] < tmpl->birthdate[2] && L->birthdate[0] != 0) //datebirth check if less than min_date_of_birth for men
                {
                    hash_table[i]->birthdate[2] = tmpl->birthdate[2]; 
                }
                if (L->birthdate[3] > tmpl->birthdate[3]) //datebirth check if greater than max_date_of_birth for men
                {
                    hash_table[i]->birthdate[3] = tmpl->birthdate[3];
                }
            }
            free(L); // data saved so we don't need this allocated space
            return true; 
        }
    }
    L->next = hash_table[i];
    hash_table[i] = L; //put in front List if not exist
    return true;
}

//Read the CSV and store into appropriate structure
//second argument HashTable *ht
void readCSV(char *filename,Liste *hash_table,int *nombre){
    FILE *fp = fopen(filename,"r");
    if(fp == NULL) {
        perror("Error in opening file - Empty file");
        exit(1);
    }
    printf("--Lecture de fichier--\n");
    // Process Data here
    nombre[0]=0; //number of women
    nombre[1]=0; //number of men
    char *token;
    char delim = ';';
    char *line; //pour stocker une ligne de strsep

    int s=0;
    char *prenom = NULL;
    int annee=0;
    int nb=0;

    fgets(line, 50, fp); //1st line: column names - "sexe;preusuel;annais;dpt;nombre"
    while (fgets(line, 50, fp))
    {
        for (int i = 0; i < 5; i++) { // 5 columns
            token = strsep(&line, &delim);
            switch (i){
                /* for every element we store it in appropriate type */
                case 0: //Convert sexe and year to int
                    s = ConvertToInt(token); // gère le cas ou token est invalide return 0 if error
                    break;
                case 1: //store name as string
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
            if(s == 1) nombre[1] = nombre[1] + nb;
            if(s = 2) nombre[0] = nombre[0] + nb;
        }
        Liste ret = create_person(prenom,s,annee,nb);
        hash_table_insert(hash_table,ret);
    }
    //close file
    fclose(fp);
    system("clear");
}

//User menu: choice 0 to 4
void menu(Liste *hashtable, int *nb){ 
    Liste L;
    int total=0;
    int choix = -1;
    char choix1 = 'x';
    char prenom[MAX_NAME];
    printf("0: Ce menu\n");
    printf("1: Le nombre de naissances\n");
    printf("2: Le nombre de prénoms\n");
    printf("3: Statistiques sur un prénom\n");
    printf("4: Quitter\n");
    while (choix != 4)
    {
        printf("Que voulez-vous afficher ? (0 pour le menu) > ");
        scanf("%d", &choix);
        switch (choix){
            case 0:
                printf("0: Ce menu\n");
                printf("1: Le nombre de naissances\n");
                printf("2: Le nombre de prénoms\n");
                printf("3: Statistiques sur un prénom\n");
                printf("4: Quitter\n");
                break;
            case 1:
                printf("Le fichier recouvre %d naissances.\n",nb[0]+nb[1]);
                break;
            case 2:
                //Le nombre de prénoms.
                printf("Souhaitez-vous distinger le genre ? (O/N)");
                scanf("%c", &choix1);
                if(choix1 == 'O' || choix1 == 'o'){
                    printf("Le fichier recouvre %d prénoms masculins et %d prénoms féminins.\n",nb[1],nb[0]);
                }else if(choix1 == 'N' || choix1 == 'n'){
                    printf("Le fichier recouvre %d prénoms\n",nb[1]+nb[0]);
                }
                break;
            case 3:
                //
                printf("Quel prénom ? ");
                scanf("%s", prenom);
                L = hash_search(hashtable,prenom);
                if(L == NULL){
                    printf("Ce prénom n'existe pas\n");
                    break;
                } 
                if(L->sex[0] != 0 && L->sex[1] != 0){
                    printf("Souhaitez-vous distinger le genre ? (O/N)");
                    scanf("%c", &choix1);
                    if(choix1 == 'O' || choix1 == 'o'){
                        printf("Le prénom %s a été donné à %d garçons et %d filles.\n",prenom,L->sex[1],L->sex[0]);
                    }else if(choix1 == 'N' || choix1 == 'n'){
                        printf("Le prénom %s a été donné à %d enfants.\n",prenom,L->sex[1]+L->sex[0]);
                    }
                    if(L->birthdate[0] < L->birthdate[2])
                        printf("Année de première apparition %d.\n",L->birthdate[0]);
                    printf("Année de première apparition %d.\n",L->birthdate[1]);
                    if(L->birthdate[1] < L->birthdate[3])
                        printf("Année de dernière apparition %d.\n",L->birthdate[3]);
                    printf("Année de dernière apparition %d.\n",L->birthdate[2]);
                    break;
                }
                printf("Le prénom %s a été donné à %d enfants.\n",prenom,L->sex[1]+L->sex[0]);
                break;
            
            default:
                break;
        }
    }
    printf("\nBye\n\n");
}

int main(int argc, char *argv[]){
    int nb_total[2]; //nb_total[0] = nb_femme & nb_total[1]=nb_homme
    //Liste hashtable[TABLE_SIZE];  /* create CONST number of pointers to type */
    Liste *hashtable = calloc(TABLE_SIZE, sizeof(Liste));
    if (hashtable == NULL) {
        printf("Error! memory not allocated.");
        exit(-1);
    }
    printf("TABLE INITIALIZED! \n\n");

    char filename[100] = "Test.csv";
    // char filename[100] = "dpt2020.csv";
    //make table "heads" NULL
    
    readCSV(filename,hashtable,nb_total);
    menu(hashtable,nb_total);

    print_table(hashtable);

    free_table(hashtable);


    return 0;
}

