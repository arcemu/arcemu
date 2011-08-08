#include "Setup.h"

class ShatteredSunSpawner : public CreatureAIScript
{
	public:
		ADD_CREATURE_FACTORY_FUNCTION(ShatteredSunSpawner);
		ShatteredSunSpawner(Creature* pCreature) : CreatureAIScript(pCreature) {}

		void OnCombatStart(Unit* mTarget)
		{
			if(mTarget != NULL && mTarget->IsPlayer())
			{
				for(uint32 i = 0; i < 3; ++i)
				{
					float x = mTarget->GetPositionX() + RandomUInt(20) - 10;
					float y = mTarget->GetPositionY() + RandomUInt(20) - 10;
					float z = mTarget->GetPositionZ();
					Creature* guard = _unit->GetMapMgr()->GetInterface()->SpawnCreature(26253, x, y, z, 0, true, false, _unit->GetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE), 50);

					if(guard != NULL)
					{
						guard->SetGuardWaypoints();
						guard->GetAIInterface()->AttackReaction(mTarget, 1, 0);
						_unit->Despawn(60000, 0);
					}
				}
			}
		}
};

void SetupNeutralGuards(ScriptMgr* mgr)
{
	/*
	mgr->register_creature_script(4624, &NeutralGuard::Create); //BB bruiser
	mgr->register_creature_script(4624, &NeutralGuard::Create);   //BB bruiser
	mgr->register_creature_script(15088, &NeutralGuard::Create); //BB elite
	mgr->register_creature_script(11102, &NeutralGuard::Create); //Argent Rider
	mgr->register_creature_script(16378, &NeutralGuard::Create); //Argent Sentry
	mgr->register_creature_script(15184, &NeutralGuard::Create); //Cen Hold Infantry
	mgr->register_creature_script(26253, &NeutralGuard::Create); //Shat Sun Peacekeeper
	mgr->register_creature_script(9460, &NeutralGuard::Create); //Gadgetzan bruiser
	mgr->register_creature_script(21448, &NeutralGuard::Create); //Gadgetzan sniper
	mgr->register_creature_script(3502, &NeutralGuard::Create); //Ratschet bruiser
	mgr->register_creature_script(19687, &NeutralGuard::Create); //Shattrath City Peacekeeper
	mgr->register_creature_script(23115, &NeutralGuard::Create); //Ogri'la Peacekeeper
	mgr->register_creature_script(23257, &NeutralGuard::Create); //Skyguard Windcharger
	*/
	mgr->register_creature_script(24994, &ShatteredSunSpawner::Create); //Shat Sun Sentry
}