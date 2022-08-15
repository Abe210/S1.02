#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__
#include "person.h"
#include <errno.h>
#include <ctype.h>

//Convert string to int
//Return 0 if error, the converted number otherwise
int ConvertToInt(char *s);

//Returns the index (int) for the hashtable of a specific person based on its name (name is the key) 
unsigned int hash(char *name);

//initialize HashTable 
//unnecessary if calloc is used
void init_hash_table(Liste *hashtable);

//Free all hashtable entries(every Person element)
void free_table(Liste *hash_table);

//print hashtable with all the entries
void print_table(Liste *hash_table);

//find an Element in the table by their name (key)
//Null if not found
Liste hash_search(Liste *hash_table, char *s);

//insert Person element in the Hashtable
//If person exists, entry is updated and param List is free
//otherwise it is added
bool hash_table_insert(Liste *hash_table, Liste L);

//Count number of distinct person
//return array [0] number of distinct women
//       array [1] number of distinct men
int *countNumberOfPerson(Liste *hash_table);

//Read the CSV and store into appropriate structure
//second argument HashTable *ht
int *readCSV(char *filename,Liste *hash_table,int *nombre);

//User menu: choice 0 to 4
void menu(Liste *hashtable, int *nb,int *numberOfNames);

#endif