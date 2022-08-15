#include "hashtable.h"

#define TABLE_SIZE 45523 //prime number larger than 1.3 times the number of keys in CSV file
#define MAX_NAME 256 //Longueur maximale d'un Nom

//Convert string to int
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

//Returns the index (int) for the hashtable of a specific person based on its name (name is the key) 
unsigned int hash(char *name){
    const int prime_number = 31; 
    int length = strlen(name);
    unsigned int hash = 0;
    for(int i=0; i < length; i++){
        hash = ((prime_number^i) * hash + name[i])% TABLE_SIZE; //modulo TABLE_SIZE to have 0 <= hash <= Table of Person, otherwise the index will be out of range
    }
    return hash;
}

void init_hash_table(Liste *hashtable){
    for(int i=0; i < TABLE_SIZE; i++){
        hashtable[i] = NULL;
    }
}

void free_table(Liste *hash_table){
    Liste Lcurrent = NULL, Lnext = NULL;
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
                    tmpl->birthdate[0] = L->birthdate[0]; 
                }
                if (L->birthdate[1] > tmpl->birthdate[1]) //datebirth check if greater than max_date_of_birth for women
                {
                    tmpl->birthdate[1] = L->birthdate[1];
                }
            }else if (L->sex[1] > 0)
            {
                tmpl->sex[1] = tmpl->sex[0] + L->sex[1];
                if (L->birthdate[2] < tmpl->birthdate[2] && L->birthdate[2] != 0) //datebirth check if less than min_date_of_birth for men
                {
                    tmpl->birthdate[2] = L->birthdate[2]; 
                }
                if (L->birthdate[3] > tmpl->birthdate[3]) //datebirth check if greater than max_date_of_birth for men
                {
                    tmpl->birthdate[3] = L->birthdate[3];
                }
            }
            free(L); // data saved so we don't need this allocated space
            return false; 
        }
    }
    L->next = hash_table[i]; 
    hash_table[i] = L; //put in front List if not exist
    return true;
}

int *countNumberOfPerson(Liste *hash_table){
    int *numberOfPerson = calloc(2,sizeof(int));
    Liste L;
    for(int i=0;i<TABLE_SIZE;i++){
        L = hash_table[i];
        while(!EstVide(L)){
            if (L->sex[0] != 0) //women name
            {
                numberOfPerson[0]++; 
            }
            if (L->sex[1] != 0) //men name
            {
                numberOfPerson[1]++;
            }             
            L = GetSucc(L);
        }
    }
    return numberOfPerson;
}

int *readCSV(char *filename,Liste *hash_table,int *nombre){
    FILE *fp;
    fp = fopen(filename,"r");
    if(fp == NULL) {
        perror("Error in opening file - Empty file");
        exit(1);
    }

    // Process Data here
    nombre[0]=0;
    nombre[1]=0;
    char *b = NULL;
    size_t bufsize = 50;
    b = malloc(bufsize * sizeof(char));
    char *token;
    char delim = ';';
    //char *line; //pour stocker une ligne de strsep

    int s=0;
    char *prenom = NULL;
    int annee=0;
    int nb=0;
    int *numberofNames = calloc(2,sizeof(int));

    getline(&b,&bufsize,fp); //1st line: column names - "sexe;preusuel;annais;dpt;nombre"
    while (!feof(fp))
    {
        getline(&b,&bufsize,fp);
        if(b[0] != '\0')
        {
            for (int i = 0; i < 5; i++) { // 5 columns
                token = strsep(&b, &delim);
                switch (i){
                    /* for every element we store it in appropriate type */
                    case 0: //Convert sexe and year to int
                        s = ConvertToInt(token); // gère le cas ou token est invalide return 0 if error
                        break;
                    case 1: //store name as string
                        prenom = token;
                        while (*token) {
                            *token = toupper((unsigned char) *token);
                            token++;
                        }
                        break;          
                    case 2:
                        annee = ConvertToInt(token);
                        break;
                    
                    case 4:
                        nb = strcspn(token, "\r\n"); //return the index of the first occurence of "\r\n"
                        token[nb] = '\0'; // "\r\n" =>"\0" (or "\0\n")
                        nb = ConvertToInt(token);
                        break; 

                    default:
                        break;
                }
            }
            Liste ret;
            if(s == 1){
                nombre[1] = nombre[1] + nb;
                ret = create_person(prenom,s,annee,nb);
            }else{
                nombre[0] = nombre[0] + nb;
                ret = create_person(prenom,s,annee,nb);
            }
            if(hash_table_insert(hash_table,ret)){
                if(s == 1) numberofNames[1]++;
                if(s == 2) numberofNames[0]++;
            }
        }
    }
    fclose(fp);
    return numberofNames;
}

void menu(Liste *hashtable, int *nb,int *numberOfNames){ 
    Liste L;
    char prenom[MAX_NAME];
    int *distinctNumberOfNames;
    distinctNumberOfNames = countNumberOfPerson(hashtable);
    int choix = -1;
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
            case 2: ;
                //Le nombre de prénoms.
                char choix1;
                printf("Souhaitez-vous distinger le genre ? (O/N) ");
                scanf(" %c", &choix1);
                if(choix1 == 'O' || choix1 == 'o'){
                    printf("Le fichier recouvre %d prénoms masculins et %d prénoms féminins.\n",distinctNumberOfNames[1],distinctNumberOfNames[0]);
                }else if(choix1 == 'N' || choix1 == 'n'){
                    printf("Le fichier recouvre %d prénoms\n",distinctNumberOfNames[1]+distinctNumberOfNames[0]);
                }
                break;
            case 3:
                //
                printf("Quel prénom ? ");
                scanf("%s", prenom);
                char *temp=prenom;
                while (*temp) {
                    *temp = toupper((unsigned char) *temp);
                    temp++;
                }
                L = hash_search(hashtable,prenom);
                if(L == NULL){
                    printf("Ce prénom n'existe pas\n");
                    break;
                } 
                if(L->sex[0] != 0 && L->sex[1] != 0){
                    printf("Souhaitez-vous distinger le genre ? (O/N) ");
                    scanf(" %c", &choix1);
                    if(choix1 == 'O' || choix1 == 'o'){
                        printf("Le prénom %s a été donné à %d garçons et %d filles.\n",prenom,L->sex[1],L->sex[0]);
                    }else if(choix1 == 'N' || choix1 == 'n'){
                        printf("Le prénom %s a été donné à %d enfants.\n",prenom,L->sex[1]+L->sex[0]);
                    }
                }else{
                    printf("Le prénom %s a été donné à %d enfants.\n",prenom,L->sex[1]+L->sex[0]);
                }

                if (L->birthdate[0] != 0 && L->birthdate[2] != 0)
                {
                    if (L->birthdate[0] < L->birthdate[2])
                    {
                        printf("Année de première apparition %d.\n",L->birthdate[0]);
                    }
                    printf("Année de première apparition %d.\n",L->birthdate[2]);                       
                }else if (L->birthdate[0] != 0)
                {
                    printf("Année de première apparition %d.\n",L->birthdate[0]);
                }else if (L->birthdate[2] != 0)
                {
                    printf("Année de première apparition %d.\n",L->birthdate[2]);
                    
                }else{
                    printf("Année de première apparition inconnu.\n");
                }

                if (L->birthdate[1] != 0 && L->birthdate[3] != 0)
                {
                    if (L->birthdate[1] > L->birthdate[3])
                    {
                        printf("Année de dernière apparition %d.\n",L->birthdate[1]);
                    }else{
                        printf("Année de dernière apparition %d.\n",L->birthdate[3]);                       
                    }
                }else if (L->birthdate[1] != 0)
                {
                    printf("Année de dernière apparition %d.\n",L->birthdate[1]);
                }else if (L->birthdate[3] != 0)
                {
                    printf("Année de dernière apparition %d.\n",L->birthdate[3]);
                    
                }else{
                    printf("Année de dernière apparition inconnu.\n");
                }
                break;
            
            default:
                break;
        }
    }
    printf("\nBye\n\n");
}