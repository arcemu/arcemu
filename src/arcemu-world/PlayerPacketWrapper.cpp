/*
* arcemu MMORPG Server
* Copyright (C) 2005-2007 arcemu Team <http://www.arcemuemu.com/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "StdAfx.h"

void Player::SendWorldStateUpdate(uint32 WorldState, uint32 Value)
{
    WorldPacket data( SMSG_UPDATE_WORLD_STATE, 8 );
    
    data << uint32( WorldState );
    data << uint32( Value );

    m_session->SendPacket( &data );
}


void Player::Gossip_SendPOI(float X, float Y, uint32 Icon, uint32 Flags, uint32 Data, const char* Name)
{
	size_t namelen = 0;

	if( Name != NULL )
		namelen = strlen( Name );
   
	WorldPacket data( SMSG_GOSSIP_POI, 11 + namelen );

	data << uint32( Flags );
	data << float( X );
	data << float( Y );
	data << uint32( Icon );
	data << uint32( Data );

	if( namelen == 0 )
		data << uint8( 0 );
	else
		data.append( (const uint8*)Name, namelen + 1 );

	m_session->SendPacket( &data );
}
  
void Player::SendLevelupInfo(uint32 level, uint32 Hp, uint32 Mana, uint32 Stat0, uint32 Stat1, uint32 Stat2, uint32 Stat3, uint32 Stat4)
{
    WorldPacket data( SMSG_LEVELUP_INFO, 14 * 4 );

    data << uint32( level );
    data << uint32( Hp );
    data << uint32( Mana );
    
    for( int i = 0; i < 6; ++i )
        data << uint32( 0 );

    data << uint32( Stat0 );
    data << uint32( Stat1 );
    data << uint32( Stat2 );
    data << uint32( Stat3 );
    data << uint32( Stat4 );

    m_session->SendPacket( &data );
}

void Player::SendLogXPGain(uint64 guid, uint32 NormalXP, uint32 RestedXP, bool type)
{
    WorldPacket data( SMSG_LOG_XPGAIN, 24 );

    if (type == false)
    {
        data << uint64( guid );
        data << uint32( NormalXP );
        
        if( type )
            data << uint8( 1 );
        else
            data << uint8( 0 );

        data << uint32( RestedXP );
        data << float( 1.0f );

    }
    else if (type == true)
    {
        data << uint64( 0 );          // does not need to be set for questxp
        data << uint32( NormalXP );
        
        if( type )
            data << uint8( 1 );
        else
            data << uint8( 0 );

        data << uint8( 0 );

    }

    m_session->SendPacket( &data );
}

// this one needs to be send inrange...
void Player::SendEnvironmentalDamageLog(const uint64 & guid, uint8 type, uint32 damage){

    WorldPacket data( SMSG_ENVIRONMENTALDAMAGELOG, 12 );

    data << uint64( guid );
    data << uint8( type );
    data << uint32( damage );

    m_session->SendPacket( &data );
}


void Player::SendCastResult(uint32 SpellId, uint8 ErrorMessage, uint8 MultiCast, uint32 Extra){

    WorldPacket data( SMSG_CAST_FAILED, 80 );

    data << uint32( SpellId );
    data << uint8( ErrorMessage );
    data << uint8( MultiCast );
    
    if( Extra )
        data << uint32( Extra );

    m_session->SendPacket( &data );
}

void Player::SendSpellCooldownEvent(uint32 SpellId){
    
    WorldPacket data( SMSG_COOLDOWN_EVENT, 12 );

    data << uint32( SpellId );
    data << uint64( GetGUID() );

    m_session->SendPacket( &data );
}


void Player::SendFlatSpellModifier(uint8 spellgroup, uint8 spelltype, int32 v){
    
    WorldPacket data( SMSG_SET_FLAT_SPELL_MODIFIER, 48 );

    data << uint8( spellgroup );
    data << uint8( spelltype );
    data << uint32( v );

    m_session->SendPacket( &data );
}

void Player::SendItemPushResult( uint64 guid, bool created, bool recieved, bool sendtoset, bool newitem, uint8 destbagslot, uint32 destslot, uint32 count, uint32 entry, uint32 suffix, uint32 randomprop, uint32 stack ){

    WorldPacket data( SMSG_ITEM_PUSH_RESULT, 8 + 4 + 4 + 4 + 1 + 4 + 4 + 4 + 4 + 4 + 4 );

    data << uint64( guid );

    if( recieved )
        data << uint32( 1 );
    else
        data << uint32( 0 );

    if( created )
        data << uint32( 1 );
    else
        data << uint32( 0 );

    data << uint32( 1 );
    data << uint8( destbagslot );

    if( newitem )
        data << uint32( destslot );
    else
        data << uint32( -1 );

    data << uint32( entry );
    data << uint32( suffix );
    data << uint32( randomprop );
    data << uint32( count );
    data << uint32( stack );

    if( sendtoset && InGroup() )
        GetGroup()->SendPacketToAll( &data );
    else
        m_session->SendPacket( &data );

}

void Player::SendSetProficiency( uint8 ItemClass, uint32 Proficiency ){

    WorldPacket data( SMSG_SET_PROFICIENCY, 40 );

    data << uint8( ItemClass );
    data << uint32( Proficiency );

    m_session->SendPacket( &data );
}

void Player::SendLoginVerifyWorld(uint32 MapId, float X, float Y, float Z, float O){

    WorldPacket data( SMSG_LOGIN_VERIFY_WORLD, 20 );

    data << uint32( MapId );
    data << float( X );
    data << float( Y );
    data << float( Z );
    data << float( O );

    m_session->SendPacket( &data );
}

void Player::SendPlaySpellVisual(uint64 guid, uint32 visualid){

    WorldPacket data( SMSG_PLAY_SPELL_VISUAL, 12 );

    data << uint64( guid );
    data << uint32( visualid );

    m_session->SendPacket( &data );
}