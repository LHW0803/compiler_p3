/*
 * File Name    : hash.c
 * Description  : An implementation file for the hash table.
 *
 * Course       : Introduction to Compilers
 * Dept. of Electrical and Computer Engineering, Seoul National University
 */

#include "subc.h"
#include <stdlib.h>
#include <string.h>

#define  HASH_TABLE_SIZE   101

typedef struct nlist {
  struct nlist *next;
  id *data;
} nlist;

static nlist *hashTable[HASH_TABLE_SIZE];

// Using project1 code
// Hash function
unsigned hash(char *name) {
  unsigned hashval = 0;
  for (; *name != '\0'; name++) {
    hashval = *name + 31 * hashval;
  }
  return hashval % HASH_TABLE_SIZE;
}

// Create enter function
id *enter(int tokenType, char *name, int length) {
  unsigned hashval = hash(name);
  nlist *current;

  // Search existing slot
  for (current = hashTable[hashval]; current != NULL; current = current->next) {
    if (strcmp(name, current->data->name) == 0) {
      // Found existing slot
      return current->data;
    }
  }

  // Create a new entry
  current = (nlist *)malloc(sizeof(nlist));
  current->data = (id *)malloc(sizeof(id));
  current->data->tokenType = tokenType;
  current->data->name = (char *)malloc(length + 1);
  strncpy(current->data->name, name, length);
  current->data->name[length] = '\0';
  // not using count in this project
  current->data->count = 0;

  // Insert at the beginning
  current->next = hashTable[hashval];
  hashTable[hashval] = current;

  return current->data;
}
