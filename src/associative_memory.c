#include "common.h"
#include "hdc.h"
#include "lib.h"
#include "list.h"
#include <string.h>

static struct list_head *query_cosine(struct list_head *am, hv_t *hv)
{
    am_t *am_iter;
    struct list_head *query;
    MALLOC(query, sizeof(qry_t));
    INIT_LIST_HEAD(query);
    list_for_each_entry (am_iter, am, list) {
        qry_t *new;
        MALLOC(new, sizeof(qry_t));
        new->class_name = am_iter->class_name;
        new->cosine = cosine_similarity(am_iter->hv, hv);
        list_add_tail(&new->list, query);
    }
    return query;
}

static struct list_head *query_hamming(struct list_head *am, hv_t *hv)
{
    am_t *am_iter;
    struct list_head *query;
    MALLOC(query, sizeof(qry_t));
    INIT_LIST_HEAD(query);
    list_for_each_entry (am_iter, am, list) {
        qry_t *new;
        MALLOC(new, sizeof(qry_t));
        new->class_name = am_iter->class_name;
        new->hamming = hamming_distance(am_iter->hv, hv);
        list_add_tail(&new->list, query);
    }
    return query;
}

struct list_head *query_memory(struct list_head *am, hv_t *hv,
                               enum similarity_method m)
{
    return m == COSINE ? query_cosine(am, hv) : query_hamming(am, hv);
}

void add_memory_item(struct list_head *am, hv_t *hv, char *class_name)

{
    am_t *new;
    MALLOC(new, sizeof(am_t));
    new->class_name = strdup(class_name);
    new->hv = clone_hypervector(hv);
    list_add_tail(&new->list, am);
}

/* TODO: API to store trained sample HVs */
