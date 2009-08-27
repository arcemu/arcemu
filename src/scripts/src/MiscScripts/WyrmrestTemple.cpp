#include "StdAfx.h"
#include "Setup.h"

class Torastrasza : public GossipScript
{
public:
   void GossipHello(Object* pObject, Player * plr, bool AutoSend)
   {
      GossipMenu *Menu;
      objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 12714, plr);
      Menu->AddItem( 0, "Yes, please, I would like to return to the ground level of the temple.", 1);
      Menu->AddItem( 0, "I would like to go Lord Afrasastrasz in the middle of the temple.", 2);
      if( AutoSend )
         Menu->SendTo( plr );
   }

   void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
   {
      Creature * pCreature = ( pObject->GetTypeId() == TYPEID_UNIT ) ? ( (Creature*)pObject ) : NULL;
      if( pCreature == NULL )
         return;

      if( plr->IsMounted() )
      {
         plr->GetSession()->SendNotification("You are already mounted! Dismount first.");
         return;
      }

      switch(IntId)
      {
      case 1:
         {
            TaxiPath * path = sTaxiMgr.GetTaxiPath( 879 );
            plr->TaxiStart( path, 6371, 0 );
         }break;
         
      case 2:
         {
            TaxiPath * path = sTaxiMgr.GetTaxiPath( 880 );
            plr->TaxiStart( path, 6371, 0 ); 
         }break;
      }
   }

   void Destroy()
   {
      delete this;
   }
 };

class Afrasastrasz : public GossipScript
{
public:
   void GossipHello(Object* pObject, Player * plr, bool AutoSend)
   {
      GossipMenu *Menu;
      objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 27575, plr);
      Menu->AddItem( 0, "My lord, I need to get to the top of the temple.", 3);
      Menu->AddItem( 0,"Can I get a ride back to ground level, Lord Afrasastrasz?", 4);
      if( AutoSend )
         Menu->SendTo( plr );
   }
   void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
   {
      Creature * pCreature = ( pObject->GetTypeId() == TYPEID_UNIT ) ? ( (Creature*)pObject ) : NULL;
      if( pCreature == NULL )
         return;

      if( plr->IsMounted() )
      {
         plr->GetSession()->SendNotification("You are already mounted! Dismount first.");
         return;
      }

      switch( IntId )
      {
      case 3:
         {
            TaxiPath * path = sTaxiMgr.GetTaxiPath( 881 );
            plr->TaxiStart( path, 6371, 0 ); 
         }break;
      case 4:
         {
            TaxiPath * path = sTaxiMgr.GetTaxiPath( 882 );
            plr->TaxiStart( path, 6371, 0 ); 
         }break;
      }
   }

   void Destroy()
   {
      delete this;
   }
 };

class Tariolstrasz : public GossipScript
{
public:
   void GossipHello(Object* pObject, Player * plr, bool AutoSend)
   {
      GossipMenu *Menu;
      objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 26443, plr);
      Menu->AddItem( 0, "Steward, please allow me to ride one of the drakes to the queen's chamber at the top of the temple.", 5);
      Menu->AddItem( 0, "Can you spare a drake to take me to Lord Afrasastrasz in the middle of the temple?", 6);
      if( AutoSend )
         Menu->SendTo( plr );
   }

   void GossipSelectOption(Object* pObject, Player * plr, uint32 Id, uint32 IntId, const char * Code)
   {
      Creature * pCreature = ( pObject->GetTypeId() == TYPEID_UNIT ) ? ( (Creature*)pObject ) : NULL;
      if( pCreature == NULL )
         return;

      if( plr->IsMounted() )
      {
         plr->GetSession()->SendNotification("You are already mounted! Dismount first.");
         return;
      }

      switch( IntId )
      {
      case 5:
         {
            TaxiPath * path = sTaxiMgr.GetTaxiPath( 878 );
            plr->TaxiStart( path, 6371, 0 );
         }break;
      case 6:
         {
            TaxiPath * path = sTaxiMgr.GetTaxiPath( 883 );
            plr->TaxiStart( path, 6371, 0 );
         }break;
      }
   }

   void Destroy()
   {
      delete this;
   }
};

void SetupWyrmrestTemple(ScriptMgr * mgr)
{
   GossipScript * WyrmrestTemple1 = (GossipScript*) new Torastrasza();
   GossipScript * WyrmrestTemple2 = (GossipScript*) new Afrasastrasz();
   GossipScript * WyrmrestTemple3 = (GossipScript*) new Tariolstrasz();

   mgr->register_gossip_script(26949, WyrmrestTemple1);   // Torastrasza
   mgr->register_gossip_script(27575, WyrmrestTemple2);   // Afrasastrasz
   mgr->register_gossip_script(26443, WyrmrestTemple3);   // Tariolstrasz
}
