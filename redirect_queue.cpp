#include "redirect_queue.h"

#include "redirect.h"
#include "config.h"
#include "util.h"

#include <enginecallback.h>

extern globalvars_t *gpGlobals;

#include <vector>

struct PendingRedirect
{
    edict_t *player;
    float executeTime;
};

static std::vector<PendingRedirect> g_Pending;

// Esta função será implementada no próximo passo.
extern void Redirect_Send(edict_t *player);

void RedirectQueue_Add(edict_t *player)
{
    if (!player)
        return;

    PendingRedirect entry;

    entry.player = player;
    entry.executeTime = gpGlobals->time + g_Config.delay;

    g_Pending.push_back(entry);

    Log("Player added to redirect queue.");
}

void RedirectQueue_Frame()
{
    for (auto it = g_Pending.begin(); it != g_Pending.end();)
    {
        if (gpGlobals->time >= it->executeTime)
        {
            Redirect_Send(it->player);
            it = g_Pending.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
