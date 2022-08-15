#include "hashtable.h"

#define TABLE_SIZE 45523 //prime number larger than 1.3 times the number of keys in CSV file

int main(int argc, char *argv[]){
    int nb_total[2]; //nb_total[0] = nb_femme & nb_total[1]=nb_homme
    int *numberofNames; //distinct number of Name, numberofNames[0]=women & numberofNames[1]=men 
    //Liste hashtable[TABLE_SIZE];  /* create CONST number of pointers to type */
    Liste *hashtable = calloc(TABLE_SIZE, sizeof(Liste));
    if (hashtable == NULL) {
        printf("Error! memory not allocated.");
        exit(-1);
    }
    //char filename[100] = "Test.csv";
    char filename[100] = "dpt2020.csv";
    if(argc == 2)
    {
        strcpy(filename,argv[1]);
    }
    printf("Reading Data... \n\n");
    numberofNames = readCSV(filename,hashtable,nb_total);
    menu(hashtable,nb_total,numberofNames);

    //print_table(hashtable);
    free_table(hashtable);
    return 0;
}