#include "person.h"

bool EstVide(Liste L) { return L == NULL ? true : false; }

Liste GetSucc(Liste L) {
  if (EstVide(L))
    return NULL;
  return L->next;
}

//dynamically create a Person element
Liste create_person(char* name, int sexe, int birthdate,int number) {
    // Creates a pointer to a new hash table person item
    Liste ptrperson = calloc(1,sizeof(person)); // malloc(sizeof(person));
    if (ptrperson == NULL) {
        printf("Error! memory not allocated.");
        exit(-1);
    }
    if(ptrperson){
        //strcpy(ptrperson->name, name);
        ptrperson->name = strdup(name); 
        switch (sexe)
        {
            case 2: //it is a woman
                ptrperson->sex[0] = number;
                ptrperson->birthdate[0] = birthdate;
                ptrperson->birthdate[1] = birthdate;
                break;
            case 1: //it is a man
                ptrperson->sex[1] = number;
                ptrperson->birthdate[2] = birthdate;
                ptrperson->birthdate[3] = birthdate;
                break;
            default:
                //printf("Error format - Column 1");
                break;
        }
        ptrperson->next = NULL;
    }
    return ptrperson;
}