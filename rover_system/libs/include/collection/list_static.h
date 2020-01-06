#ifndef __LIST_STATIC_H
#define __LIST_STATIC_H


typedef struct list List;

List *list_create(void);

int list_destroy(List *list);

#endif
