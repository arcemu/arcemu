/*

Purpuse : make modifications in dbc file for ascent project to set values that are missing for spells in able to work
Warning : Output result is changed in order to obtain ascent specific data. Do not try to use or interpret data for anything else
Warning : SpellEntry + sql_translation_table + SPELL_ENTRY must have the same structure !!!

project status : not finished yet..not tested

*/

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>
#include <windows.h>

#include "./resources/dbcfile.h"

#include "defines.h"

DBCFile SpellDBC;

//make this after the main
void do_fixes();

void main(int argc, TCHAR* argv[])
{
	int dotask=0;

	/////////////////////////////////////////////////////////////////////////////////////
	//open the dbc
	SpellDBC.open( "Spell_orig.dbc" );

	//check if we managed to open it
	if(!SpellDBC.getFieldCount())
	{
		printf("Error: could not open Spell.dbc file\n");
		exit(1);
	}
	else printf("Opened Spell.DBC with %u fields and %u rows\n",(int)SpellDBC.getFieldCount(),(int)SpellDBC.getRecordCount());

	// make sure format is ok
	if ( SpellDBC.GetRecordSize() != sizeof( SpellEntry ) )
	{
		printf("Spell Spell.dbc Error: Record size does not match in dbc with what we have ( them %u / us %u )\n", SpellDBC.GetRecordSize(), sizeof( SpellEntry ) );
		exit(1);
	}
	/////////////////////////////////////////////////////////////////////////////////////
	do_fixes();
}

unsigned int get_spell_row(uint32 id)
{
	//replace this later with at least a binary search
	for(unsigned int j=0;j<SpellDBC.getRecordCount();j++)
		if(SpellDBC.getRecord(j).getUInt(SPELL_ENTRY_Id)==id)
			return j;
	return 0xFFFFFFFF;
}

void assign_row_to_SpellEntry(SpellEntry **spe,uint32 row)
{
	*spe = (SpellEntry *)SpellDBC.getRecord(row).getRowStart();
}

void assign_Spell_to_SpellEntry(SpellEntry **spe,uint32 spell_id)
{
	uint32 row=get_spell_row(spell_id);
	if(row==0xFFFFFFFF)
		*spe = NULL;
	else assign_row_to_SpellEntry(spe,row);
}

void reset_spell_info( SpellEntry *spe )
{
	if ( !spe )
		return;
	memset( spe, 0, sizeof( SpellEntry ) );
	spe->School = SCHOOL_FLAG_ARCANE;	//whatever
	spe->Category = 5000;				//make sure this matches our cooldown line
	spe->CategoryRecoveryTime = 0;		
	spe->maxLevel = 5;					//use only if using dbc for client side
	spe->CastingTimeIndex = SPELL_CATING_TIME_INSTANT; //instant 
	spe->DurationIndex = DURATIONINDEX_INFINIT;
}

void do_fixes()
{
	if(!SpellDBC.getFieldCount())
	{
		printf("error, could not open dbc file\n");
		exit(1);
	}
	uint32 cnt = (uint32)SpellDBC.getRecordCount();

	SpellEntry	*sp;
	uint32		rank1_str_offset;
	uint32		rank2_str_offset;
	uint32		rank3_str_offset;
	uint32		rank4_str_offset;
	uint32		nullstr_str_offset;
	uint32		name_offset;
	uint32		desc_offset;
	uint32		bufdesc_offset;
	uint32		new_row;
	uint32		new_icon_id;

	rank1_str_offset = SpellDBC.AddString("Rank 1");
	rank2_str_offset = SpellDBC.AddString("Rank 2");
	rank3_str_offset = SpellDBC.AddString("Rank 3");
	rank4_str_offset = SpellDBC.AddString("Rank 4");
	nullstr_str_offset = SpellDBC.AddString("");

	///////////////////////////////////////////////////////////////////////
	// start : example how to make a custom spell
	sp = (SpellEntry *)SpellDBC.getRecord( get_spell_row( 36496 ) ).getRowStart();
	if( ! sp )
	{
		new_row = SpellDBC.AddRecord();
		if ( new_row )
			sp = (SpellEntry *)SpellDBC.getRecord( new_row ).getRowStart();
		else sp = 0;
	}
	new_icon_id = 0; //use only if using dbc for client side
	if ( sp )
	{
		//reset_spell_info( sp );
		sp->Id = 36496;
		name_offset = SpellDBC.AddString("forcexample");	//a spelline must have same name when using auras since stackability is based on hashes
		desc_offset = nullstr_str_offset; //use only if using dbc for client side
		bufdesc_offset = nullstr_str_offset; //use only if using dbc for client side
		sp->Name = name_offset;
		sp->Description = desc_offset;
		sp->BuffDescription = bufdesc_offset;
		sp->Rank = rank1_str_offset;
		sp->spellLevel = sp->baseLevel = 1;
		sp->rangeIndex = RANGEINDEX_SELF;
		//we wish to make a dummy effect to be able to script it server or script side
		sp->Effect[0] = SPELL_EFFECT_DUMMY;
		sp->EffectImplicitTargetA[0] = EFF_TARGET_SELF;
		//make sure to disable the rest of the effects
		sp->Effect[1] = 0;
		sp->Effect[2] = 0;
		sp->icon = new_icon_id;
	}
	// end : example how to make a custom spell
	///////////////////////////////////////////////////////////////////////
	

	//save the modified dbc file
	SpellDBC.DumpBufferToFile( "Spell.dbc" );
}
