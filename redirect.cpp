#include "redirect.h"
#include "util.h"

#include <enginecallback.h>

void Redirect_OnClientPutInServer(edict_t *player)
{
    if (!player)
        return;

    const char *name = g_engfuncs.pfnSzFromIndex(player->v.netname);

    if (!name)
        name = "<unknown>";

    Log("Player connected: %s", name);

    g_engfuncs.pfnClientCommand(player, "echo xash_redirect_test\n");
}
