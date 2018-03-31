#ifndef _UI_H
#define _UI_H 1

#include "struct.h"
extern List indent;
extern List printed;

void set_ui_env();
void print_cmd(const char *cmd);
void print_help();
void print_mac(List *ml);
void print_tree(TNode *tn);
void print_err();
void print_war();
void print_new(const char *name);

#endif
