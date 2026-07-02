#ifndef QUERY_CONTEXT_H
#define QUERY_CONTEXT_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    QUERY_DATA_TYPE_NONE = 0,
    QUERY_DATA_TYPE_OVERLIMIT,
    QUERY_DATA_TYPE_NORMAL_MARK,
    QUERY_DATA_TYPE_AUDIO_MARK
} query_data_type_t;

typedef struct {
    char boarding_key[32];
    query_data_type_t data_type;
} query_context_t;

void query_context_clear(void);
void query_context_set_boarding_key(const char *key);
bool query_context_get(query_context_t *out);
void query_context_set_data_type(query_data_type_t type);

#ifdef __cplusplus
}
#endif

#endif /* QUERY_CONTEXT_H */