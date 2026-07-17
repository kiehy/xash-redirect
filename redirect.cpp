#include "redirect.h"
#include "config.h"
#include "util.h"

#include <enginecallback.h>

static void SendRedirect(edict_t *player)
{
    if (!g_Config.enabled)
    {
        Log("Plugin disabled, skipping redirect.");
        return;
    }

    Log(
        "Redirecting player to %s:%u",
        g_Config.destination.ip.c_str(),
        g_Config.destination.port
    );

    g_engfuncs.pfnClientCommand(
        player,
        "connect %s:%u gs\n",
        g_Config.destination.ip.c_str(),
        g_Config.destination.port
    );
}

void Redirect_OnClientPutInServer(edict_t *player)
{
    if (!player)
        return;

    const char *name = g_engfuncs.pfnSzFromIndex(player->v.netname);

    if (!name)
        name = "<unknown>";

    Log("Player connected: %s", name);

    SendRedirect(player);
}
