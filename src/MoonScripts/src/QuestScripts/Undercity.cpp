#include "StdAfx.h"
#include "Setup.h"
#include "EAS/EasyFunctions.h"

class SCRIPT_DECL Quest_JourneytoUndercity : public QuestScript 
{ 
public:
  void CanSelect(Creature *creat)
  {
    creat->SetUInt32Value(UNIT_NPC_FLAGS, 2);
  }

  void OnQuestComplete(Player* mTarget, QuestLogEntry *qLogEntry)
  {
    Creature *creat = mTarget->GetMapMgr()->GetSqlIdCreature(19175); //Lady Sylvanas Windrunner - NCDB guid
    if(creat == NULL)
      return;

    sEAS.SpawnCreatureExtended(mTarget, 21628, 1295.222656, 314.253998, -57.320854, 2.365611, 35, 180000, true, true, true);
    sEAS.SpawnCreatureExtended(mTarget, 21628, 1293.403931, 311.264465, -57.320854, 1.939140, 35, 180000, true, true, true);
    sEAS.SpawnCreatureExtended(mTarget, 21628, 1286.532104, 311.452423, -57.320854, 0.592182, 35, 180000, true, true, true);
    sEAS.SpawnCreatureExtended(mTarget, 21628, 1284.536011, 314.496338, -57.320845, 0.580401, 35, 180000, true, true, true);

    creat->PlaySoundToSet(10896);
    creat->CastSpell(creat->GetGUID(), dbcSpell.LookupEntry(36568), false);

    creat->SetUInt32Value(UNIT_NPC_FLAGS, 0);

    // Players can't interact with Sylvanas for 180000 ms.
    TimedEvent *te = TimedEvent::Allocate(this, new CallbackP1<Quest_JourneytoUndercity, Creature*>(this, &Quest_JourneytoUndercity::CanSelect, creat), 0, 180000, 1);  
    creat->event_AddEvent(te);
  }
};

void SetupUndercity(ScriptMgr *mgr)
{
  mgr->register_quest_script(9180, CREATE_QUESTSCRIPT(Quest_JourneytoUndercity));
}
