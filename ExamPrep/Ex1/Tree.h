//
// Created by edoar on 25/08/2024.
//

#ifndef EX1_TREE_H
#define EX1_TREE_H

// ADT I classe => nell'header non si definisce in maniera completa la struttura per limitare la visibilitò
typedef struct TREE_s *TREE;

int TREEMaxLength(TREE T);
TREE TREEInit(int childNum);
void TREEFree(TREE T);

#endif //EX1_TREE_H
