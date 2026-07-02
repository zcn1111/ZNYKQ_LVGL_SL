#include "query_context.h"
#include <string.h>

static query_context_t g_ctx;
static bool g_valid = false;

void query_context_clear(void)
{
    memset(&g_ctx, 0, sizeof(g_ctx));
    g_valid = false;
}

void query_context_set_boarding_key(const char *key)
{
    if(!key || key[0] == '\0') {
        query_context_clear();
        return;
    }

    memset(&g_ctx, 0, sizeof(g_ctx));
    strncpy(g_ctx.boarding_key, key, sizeof(g_ctx.boarding_key) - 1);
    g_ctx.data_type = QUERY_DATA_TYPE_NONE;
    g_valid = true;
}

bool query_context_get(query_context_t *out)
{
    if(!out || !g_valid) return false;
    memcpy(out, &g_ctx, sizeof(query_context_t));
    return true;
}

void query_context_set_data_type(query_data_type_t type)
{
    if(!g_valid) return;
    g_ctx.data_type = type;
}