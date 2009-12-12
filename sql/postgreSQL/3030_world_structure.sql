--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- Name: plpgsql; Type: PROCEDURAL LANGUAGE; Schema: -; Owner: postgres
--

CREATE PROCEDURAL LANGUAGE plpgsql;


ALTER PROCEDURAL LANGUAGE plpgsql OWNER TO postgres;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: ai_agents; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE ai_agents (
    entry integer DEFAULT 0 NOT NULL,
    instance_mode integer DEFAULT 4 NOT NULL,
    type smallint DEFAULT 0 NOT NULL,
    event integer DEFAULT 0 NOT NULL,
    chance integer DEFAULT 0 NOT NULL,
    maxcount integer DEFAULT 0 NOT NULL,
    spell integer DEFAULT 0 NOT NULL,
    spelltype integer DEFAULT 0 NOT NULL,
    targettype integer DEFAULT 0 NOT NULL,
    cooldown integer DEFAULT 0 NOT NULL,
    floatmisc1 real DEFAULT 0 NOT NULL,
    misc2 integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.ai_agents OWNER TO arcemu;

--
-- Name: ai_threattospellid; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE ai_threattospellid (
    spell integer DEFAULT 0 NOT NULL,
    mod integer DEFAULT 0 NOT NULL,
    modcoef real DEFAULT 1 NOT NULL
);


ALTER TABLE public.ai_threattospellid OWNER TO arcemu;

--
-- Name: areatriggers; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE areatriggers (
    entry integer DEFAULT 0 NOT NULL,
    type numeric(3,0) DEFAULT 0,
    map integer,
    screen integer,
    name text,
    position_x real DEFAULT 0 NOT NULL,
    position_y real DEFAULT 0 NOT NULL,
    position_z real DEFAULT 0 NOT NULL,
    orientation real DEFAULT 0 NOT NULL,
    required_honor_rank integer DEFAULT 0 NOT NULL,
    required_level numeric(3,0) DEFAULT 0 NOT NULL
);


ALTER TABLE public.areatriggers OWNER TO arcemu;

--
-- Name: auctionhouse; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE auctionhouse (
    id integer NOT NULL,
    creature_entry bigint DEFAULT 0 NOT NULL,
    "group" integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.auctionhouse OWNER TO arcemu;

--
-- Name: auctionhouse_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE auctionhouse_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.auctionhouse_id_seq OWNER TO arcemu;

--
-- Name: auctionhouse_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE auctionhouse_id_seq OWNED BY auctionhouse.id;


--
-- Name: banned_phrases; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE banned_phrases (
    phrase text NOT NULL
);


ALTER TABLE public.banned_phrases OWNER TO arcemu;

--
-- Name: clientaddons; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE clientaddons (
    id integer NOT NULL,
    name text,
    crc bigint,
    banned integer DEFAULT 0 NOT NULL,
    showinlist integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.clientaddons OWNER TO arcemu;

--
-- Name: clientaddons_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE clientaddons_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.clientaddons_id_seq OWNER TO arcemu;

--
-- Name: clientaddons_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE clientaddons_id_seq OWNED BY clientaddons.id;


--
-- Name: command_overrides; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE command_overrides (
    command_name text NOT NULL,
    access_level text NOT NULL
);


ALTER TABLE public.command_overrides OWNER TO arcemu;

--
-- Name: creature_formations; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_formations (
    spawn_id integer DEFAULT 0 NOT NULL,
    target_spawn_id integer DEFAULT 0 NOT NULL,
    follow_angle real DEFAULT 0 NOT NULL,
    follow_dist real DEFAULT 0 NOT NULL
);


ALTER TABLE public.creature_formations OWNER TO arcemu;

--
-- Name: creature_names; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_names (
    entry integer DEFAULT 0 NOT NULL,
    name text NOT NULL,
    subname text NOT NULL,
    info_str text NOT NULL,
    flags1 integer DEFAULT 0 NOT NULL,
    type integer DEFAULT 0 NOT NULL,
    family integer DEFAULT 0 NOT NULL,
    rank integer DEFAULT 0 NOT NULL,
    killcredit1 integer DEFAULT 0 NOT NULL,
    killcredit2 integer DEFAULT 0 NOT NULL,
    male_displayid integer DEFAULT 0 NOT NULL,
    female_displayid integer DEFAULT 0 NOT NULL,
    male_displayid2 integer DEFAULT 0 NOT NULL,
    female_displayid2 integer DEFAULT 0 NOT NULL,
    unknown_float1 real DEFAULT 1 NOT NULL,
    unknown_float2 real DEFAULT 1 NOT NULL,
    leader numeric(3,0) DEFAULT 0 NOT NULL,
    questitem1 integer DEFAULT 0 NOT NULL,
    questitem2 integer DEFAULT 0 NOT NULL,
    questitem3 integer DEFAULT 0 NOT NULL,
    questitem4 integer DEFAULT 0 NOT NULL,
    questitem5 integer DEFAULT 0 NOT NULL,
    questitem6 integer DEFAULT 0 NOT NULL,
    waypointid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.creature_names OWNER TO arcemu;

--
-- Name: creature_names_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_names_localized (
    id integer NOT NULL,
    language_code text NOT NULL,
    name text NOT NULL,
    subname text NOT NULL
);


ALTER TABLE public.creature_names_localized OWNER TO arcemu;

--
-- Name: creature_proto; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_proto (
    entry integer DEFAULT 0 NOT NULL,
    minlevel integer DEFAULT 1 NOT NULL,
    maxlevel integer DEFAULT 1 NOT NULL,
    faction integer DEFAULT 35 NOT NULL,
    minhealth integer DEFAULT 1 NOT NULL,
    maxhealth integer DEFAULT 1 NOT NULL,
    mana integer DEFAULT 0 NOT NULL,
    scale real DEFAULT 1 NOT NULL,
    npcflags integer DEFAULT 1 NOT NULL,
    attacktime integer DEFAULT 1000 NOT NULL,
    attacktype integer DEFAULT 0 NOT NULL,
    mindamage real DEFAULT 1 NOT NULL,
    maxdamage real DEFAULT 1 NOT NULL,
    can_ranged integer DEFAULT 0 NOT NULL,
    rangedattacktime integer DEFAULT 0 NOT NULL,
    rangedmindamage bytea NOT NULL,
    rangedmaxdamage bytea NOT NULL,
    respawntime integer DEFAULT 30000 NOT NULL,
    armor integer DEFAULT 0 NOT NULL,
    resistance1 integer DEFAULT 0 NOT NULL,
    resistance2 integer DEFAULT 0 NOT NULL,
    resistance3 integer DEFAULT 0 NOT NULL,
    resistance4 integer DEFAULT 0 NOT NULL,
    resistance5 integer DEFAULT 0 NOT NULL,
    resistance6 integer DEFAULT 0 NOT NULL,
    combat_reach real DEFAULT 1 NOT NULL,
    bounding_radius real DEFAULT 1 NOT NULL,
    auras text NOT NULL,
    boss integer DEFAULT 0 NOT NULL,
    money integer DEFAULT 0 NOT NULL,
    invisibility_type integer DEFAULT 0 NOT NULL,
    death_state integer DEFAULT 0 NOT NULL,
    walk_speed real DEFAULT 2.5 NOT NULL,
    run_speed real DEFAULT 8 NOT NULL,
    fly_speed real DEFAULT 14 NOT NULL,
    extra_a9_flags integer DEFAULT 0 NOT NULL,
    spell1 integer DEFAULT 0 NOT NULL,
    spell2 integer DEFAULT 0 NOT NULL,
    spell3 integer DEFAULT 0 NOT NULL,
    spell4 integer DEFAULT 0 NOT NULL,
    spell_flags integer DEFAULT 0 NOT NULL,
    modimmunities integer DEFAULT 0 NOT NULL,
    istrainingdummy integer DEFAULT 0 NOT NULL,
    guardtype integer DEFAULT 0 NOT NULL,
    summonguard integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.creature_proto OWNER TO arcemu;

--
-- Name: creature_quest_finisher; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_quest_finisher (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.creature_quest_finisher OWNER TO arcemu;

--
-- Name: creature_quest_starter; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_quest_starter (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.creature_quest_starter OWNER TO arcemu;

--
-- Name: creature_spawns; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_spawns (
    id integer NOT NULL,
    entry integer NOT NULL,
    map integer NOT NULL,
    position_x real NOT NULL,
    position_y real NOT NULL,
    position_z real NOT NULL,
    orientation real NOT NULL,
    movetype integer DEFAULT 0 NOT NULL,
    displayid integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 14 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    bytes0 integer DEFAULT 0 NOT NULL,
    bytes1 integer DEFAULT 0 NOT NULL,
    bytes2 integer DEFAULT 0 NOT NULL,
    emote_state integer DEFAULT 0 NOT NULL,
    npc_respawn_link integer DEFAULT 0 NOT NULL,
    channel_spell integer DEFAULT 0 NOT NULL,
    channel_target_sqlid integer DEFAULT 0 NOT NULL,
    channel_target_sqlid_creature integer DEFAULT 0 NOT NULL,
    standstate integer DEFAULT 0 NOT NULL,
    mountdisplayid integer DEFAULT 0 NOT NULL,
    slot1item integer DEFAULT 0 NOT NULL,
    slot2item integer DEFAULT 0 NOT NULL,
    slot3item integer DEFAULT 0 NOT NULL,
    canfly smallint DEFAULT 0 NOT NULL,
    phase integer DEFAULT 1 NOT NULL
);


ALTER TABLE public.creature_spawns OWNER TO arcemu;

--
-- Name: creature_spawns_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE creature_spawns_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.creature_spawns_id_seq OWNER TO arcemu;

--
-- Name: creature_spawns_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE creature_spawns_id_seq OWNED BY creature_spawns.id;


--
-- Name: creature_staticspawns; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_staticspawns (
    id integer NOT NULL,
    entry integer NOT NULL,
    map integer NOT NULL,
    x real NOT NULL,
    y real NOT NULL,
    z real NOT NULL,
    o real NOT NULL,
    movetype integer DEFAULT 0 NOT NULL,
    displayid integer DEFAULT 0 NOT NULL,
    factionid integer DEFAULT 35 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    bytes0 integer DEFAULT 0 NOT NULL,
    bytes1 integer DEFAULT 0 NOT NULL,
    bytes2 integer DEFAULT 0 NOT NULL,
    emote_state integer DEFAULT 0 NOT NULL,
    npc_respawn_link integer DEFAULT 0 NOT NULL,
    channel_spell integer DEFAULT 0 NOT NULL,
    channel_target_sqlid integer DEFAULT 0 NOT NULL,
    channel_target_sqlid_creature integer DEFAULT 0 NOT NULL,
    standstate integer DEFAULT 0 NOT NULL,
    mountdisplayid integer DEFAULT 0 NOT NULL,
    slot1item integer DEFAULT 0 NOT NULL,
    slot2item integer DEFAULT 0 NOT NULL,
    slot3item integer DEFAULT 0 NOT NULL,
    canfly smallint DEFAULT 0 NOT NULL,
    phase integer DEFAULT 1 NOT NULL
);


ALTER TABLE public.creature_staticspawns OWNER TO arcemu;

--
-- Name: creature_staticspawns_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE creature_staticspawns_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.creature_staticspawns_id_seq OWNER TO arcemu;

--
-- Name: creature_staticspawns_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE creature_staticspawns_id_seq OWNED BY creature_staticspawns.id;


--
-- Name: creature_timed_emotes; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_timed_emotes (
    spawnid integer DEFAULT 0 NOT NULL,
    rowid integer DEFAULT 0 NOT NULL,
    type numeric(3,0) DEFAULT 1 NOT NULL,
    value integer DEFAULT 0 NOT NULL,
    msg text,
    msg_type integer DEFAULT 0 NOT NULL,
    msg_lang integer DEFAULT 0 NOT NULL,
    expire_after integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.creature_timed_emotes OWNER TO arcemu;

--
-- Name: creature_waypoints; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE creature_waypoints (
    spawnid integer DEFAULT 0 NOT NULL,
    waypointid integer DEFAULT 0 NOT NULL,
    position_x real DEFAULT 0 NOT NULL,
    position_y real DEFAULT 0 NOT NULL,
    position_z real DEFAULT 0 NOT NULL,
    waittime integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    forwardemoteoneshot numeric(3,0) DEFAULT 0 NOT NULL,
    forwardemoteid integer DEFAULT 0 NOT NULL,
    backwardemoteoneshot numeric(3,0) DEFAULT 0 NOT NULL,
    backwardemoteid integer DEFAULT 0 NOT NULL,
    forwardskinid integer DEFAULT 0 NOT NULL,
    backwardskinid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.creature_waypoints OWNER TO arcemu;

--
-- Name: fishing; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE fishing (
    zone integer DEFAULT 0 NOT NULL,
    minskill integer,
    maxskill integer
);


ALTER TABLE public.fishing OWNER TO arcemu;

--
-- Name: gameobject_names; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_names (
    entry integer DEFAULT 0 NOT NULL,
    type integer DEFAULT 0 NOT NULL,
    displayid integer DEFAULT 0 NOT NULL,
    name text NOT NULL,
    category text NOT NULL,
    castbartext text NOT NULL,
    unkstr text NOT NULL,
    spellfocus integer DEFAULT 0 NOT NULL,
    sound1 integer DEFAULT 0 NOT NULL,
    sound2 integer DEFAULT 0 NOT NULL,
    sound3 integer DEFAULT 0 NOT NULL,
    sound4 integer DEFAULT 0 NOT NULL,
    sound5 integer DEFAULT 0 NOT NULL,
    sound6 integer DEFAULT 0 NOT NULL,
    sound7 integer DEFAULT 0 NOT NULL,
    sound8 integer DEFAULT 0 NOT NULL,
    sound9 integer DEFAULT 0 NOT NULL,
    unknown1 integer DEFAULT 0 NOT NULL,
    unknown2 integer DEFAULT 0 NOT NULL,
    unknown3 integer DEFAULT 0 NOT NULL,
    unknown4 integer DEFAULT 0 NOT NULL,
    unknown5 integer DEFAULT 0 NOT NULL,
    unknown6 integer DEFAULT 0 NOT NULL,
    unknown7 integer DEFAULT 0 NOT NULL,
    unknown8 integer DEFAULT 0 NOT NULL,
    unknown9 integer DEFAULT 0 NOT NULL,
    unknown10 integer DEFAULT 0 NOT NULL,
    unknown11 integer DEFAULT 0 NOT NULL,
    unknown12 integer DEFAULT 0 NOT NULL,
    unknown13 integer DEFAULT 0 NOT NULL,
    unknown14 integer DEFAULT 0 NOT NULL,
    size real DEFAULT 1 NOT NULL,
    questitem1 integer DEFAULT 0 NOT NULL,
    questitem2 integer DEFAULT 0 NOT NULL,
    questitem3 integer DEFAULT 0 NOT NULL,
    questitem4 integer DEFAULT 0 NOT NULL,
    questitem5 integer DEFAULT 0 NOT NULL,
    questitem6 integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.gameobject_names OWNER TO arcemu;

--
-- Name: gameobject_names_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_names_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    name text NOT NULL
);


ALTER TABLE public.gameobject_names_localized OWNER TO arcemu;

--
-- Name: gameobject_quest_finisher; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_quest_finisher (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.gameobject_quest_finisher OWNER TO arcemu;

--
-- Name: gameobject_quest_item_binding; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_quest_item_binding (
    entry integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    item integer DEFAULT 0 NOT NULL,
    item_count integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.gameobject_quest_item_binding OWNER TO arcemu;

--
-- Name: gameobject_quest_pickup_binding; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_quest_pickup_binding (
    entry integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    required_count integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.gameobject_quest_pickup_binding OWNER TO arcemu;

--
-- Name: gameobject_quest_starter; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_quest_starter (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.gameobject_quest_starter OWNER TO arcemu;

--
-- Name: gameobject_spawns; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_spawns (
    id integer NOT NULL,
    entry integer DEFAULT 0 NOT NULL,
    map integer DEFAULT 0 NOT NULL,
    position_x real DEFAULT 0 NOT NULL,
    position_y real DEFAULT 0 NOT NULL,
    position_z real DEFAULT 0 NOT NULL,
    facing real DEFAULT 0 NOT NULL,
    orientation1 real DEFAULT 0 NOT NULL,
    orientation2 real DEFAULT 0 NOT NULL,
    orientation3 real DEFAULT 0 NOT NULL,
    orientation4 real DEFAULT 0 NOT NULL,
    state integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 0 NOT NULL,
    scale real DEFAULT 0 NOT NULL,
    statenpclink integer DEFAULT 0 NOT NULL,
    phase integer DEFAULT 1 NOT NULL,
    overrides integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.gameobject_spawns OWNER TO arcemu;

--
-- Name: gameobject_spawns_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE gameobject_spawns_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.gameobject_spawns_id_seq OWNER TO arcemu;

--
-- Name: gameobject_spawns_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE gameobject_spawns_id_seq OWNED BY gameobject_spawns.id;


--
-- Name: gameobject_staticspawns; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_staticspawns (
    id integer NOT NULL,
    entry integer NOT NULL,
    map integer DEFAULT 0 NOT NULL,
    x real NOT NULL,
    y real NOT NULL,
    z real NOT NULL,
    facing real NOT NULL,
    o real NOT NULL,
    o1 real NOT NULL,
    o2 real NOT NULL,
    o3 real NOT NULL,
    state integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 0 NOT NULL,
    scale real NOT NULL,
    respawnnpclink integer DEFAULT 0 NOT NULL,
    phase integer DEFAULT 1 NOT NULL,
    overrides integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.gameobject_staticspawns OWNER TO arcemu;

--
-- Name: gameobject_staticspawns_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE gameobject_staticspawns_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.gameobject_staticspawns_id_seq OWNER TO arcemu;

--
-- Name: gameobject_staticspawns_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE gameobject_staticspawns_id_seq OWNED BY gameobject_staticspawns.id;


--
-- Name: gameobject_teleports; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gameobject_teleports (
    entry integer NOT NULL,
    mapid integer NOT NULL,
    x_pos real NOT NULL,
    y_pos real NOT NULL,
    z_pos real NOT NULL,
    orientation real NOT NULL,
    required_level integer NOT NULL
);


ALTER TABLE public.gameobject_teleports OWNER TO arcemu;

--
-- Name: gameobject_teleports_entry_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE gameobject_teleports_entry_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.gameobject_teleports_entry_seq OWNER TO arcemu;

--
-- Name: gameobject_teleports_entry_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE gameobject_teleports_entry_seq OWNED BY gameobject_teleports.entry;


--
-- Name: graveyards; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE graveyards (
    id integer NOT NULL,
    position_x real DEFAULT 0 NOT NULL,
    position_y real DEFAULT 0 NOT NULL,
    position_z real DEFAULT 0 NOT NULL,
    orientation real DEFAULT 0 NOT NULL,
    zoneid integer DEFAULT 0 NOT NULL,
    adjacentzoneid integer DEFAULT 0 NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 0 NOT NULL,
    name text NOT NULL
);


ALTER TABLE public.graveyards OWNER TO arcemu;

--
-- Name: graveyards_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE graveyards_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.graveyards_id_seq OWNER TO arcemu;

--
-- Name: graveyards_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE graveyards_id_seq OWNED BY graveyards.id;


--
-- Name: instance_bosses; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE instance_bosses (
    mapid integer DEFAULT 0 NOT NULL,
    creatureid integer DEFAULT 0 NOT NULL,
    trash text NOT NULL,
    trash_respawn_override integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.instance_bosses OWNER TO arcemu;

--
-- Name: item_quest_association; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE item_quest_association (
    item integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    item_count integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.item_quest_association OWNER TO arcemu;

--
-- Name: item_randomprop_groups; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE item_randomprop_groups (
    entry_id integer NOT NULL,
    randomprops_entryid integer NOT NULL,
    chance real NOT NULL
);


ALTER TABLE public.item_randomprop_groups OWNER TO arcemu;

--
-- Name: item_randomsuffix_groups; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE item_randomsuffix_groups (
    entry_id integer NOT NULL,
    randomsuffix_entryid integer NOT NULL,
    chance real NOT NULL
);


ALTER TABLE public.item_randomsuffix_groups OWNER TO arcemu;

--
-- Name: itemnames; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE itemnames (
    entry integer DEFAULT 0 NOT NULL,
    name text NOT NULL,
    slotid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.itemnames OWNER TO arcemu;

--
-- Name: itempages; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE itempages (
    entry integer DEFAULT 0 NOT NULL,
    text text NOT NULL,
    next_page integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.itempages OWNER TO arcemu;

--
-- Name: itempages_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE itempages_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    text text NOT NULL
);


ALTER TABLE public.itempages_localized OWNER TO arcemu;

--
-- Name: itempetfood; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE itempetfood (
    entry integer NOT NULL,
    food_type integer NOT NULL
);


ALTER TABLE public.itempetfood OWNER TO arcemu;

--
-- Name: items; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE items (
    entry integer DEFAULT 0 NOT NULL,
    class integer DEFAULT 0 NOT NULL,
    subclass integer DEFAULT 0 NOT NULL,
    field4 integer DEFAULT (-1) NOT NULL,
    name1 text NOT NULL,
    displayid integer DEFAULT 0 NOT NULL,
    quality integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 0 NOT NULL,
    buyprice integer DEFAULT 0 NOT NULL,
    sellprice integer DEFAULT 0 NOT NULL,
    inventorytype integer DEFAULT 0 NOT NULL,
    allowableclass integer DEFAULT 0 NOT NULL,
    allowablerace integer DEFAULT 0 NOT NULL,
    itemlevel integer DEFAULT 0 NOT NULL,
    requiredlevel integer DEFAULT 0 NOT NULL,
    requiredskill integer DEFAULT 0 NOT NULL,
    requiredskillrank integer DEFAULT 0 NOT NULL,
    requiredspell integer DEFAULT 0 NOT NULL,
    requiredplayerrank1 integer DEFAULT 0 NOT NULL,
    requiredplayerrank2 integer DEFAULT 0 NOT NULL,
    requiredfaction integer DEFAULT 0 NOT NULL,
    requiredfactionstanding integer DEFAULT 0 NOT NULL,
    "unique" integer DEFAULT 0 NOT NULL,
    maxcount integer DEFAULT 0 NOT NULL,
    containerslots integer DEFAULT 0 NOT NULL,
    itemstatscount integer DEFAULT 10 NOT NULL,
    stat_type1 integer DEFAULT 0 NOT NULL,
    stat_value1 integer DEFAULT 0 NOT NULL,
    stat_type2 integer DEFAULT 0 NOT NULL,
    stat_value2 integer DEFAULT 0 NOT NULL,
    stat_type3 integer DEFAULT 0 NOT NULL,
    stat_value3 integer DEFAULT 0 NOT NULL,
    stat_type4 integer DEFAULT 0 NOT NULL,
    stat_value4 integer DEFAULT 0 NOT NULL,
    stat_type5 integer DEFAULT 0 NOT NULL,
    stat_value5 integer DEFAULT 0 NOT NULL,
    stat_type6 integer DEFAULT 0 NOT NULL,
    stat_value6 integer DEFAULT 0 NOT NULL,
    stat_type7 integer DEFAULT 0 NOT NULL,
    stat_value7 integer DEFAULT 0 NOT NULL,
    stat_type8 integer DEFAULT 0 NOT NULL,
    stat_value8 integer DEFAULT 0 NOT NULL,
    stat_type9 integer DEFAULT 0 NOT NULL,
    stat_value9 integer DEFAULT 0 NOT NULL,
    stat_type10 integer DEFAULT 0 NOT NULL,
    stat_value10 integer DEFAULT 0 NOT NULL,
    scaledstatsdistributionid integer DEFAULT 0 NOT NULL,
    scaledstatsdistributionflags integer DEFAULT 0 NOT NULL,
    dmg_min1 real DEFAULT 0 NOT NULL,
    dmg_max1 real DEFAULT 0 NOT NULL,
    dmg_type1 integer DEFAULT 0 NOT NULL,
    dmg_min2 real DEFAULT 0 NOT NULL,
    dmg_max2 real DEFAULT 0 NOT NULL,
    dmg_type2 integer DEFAULT 0 NOT NULL,
    armor integer DEFAULT 0 NOT NULL,
    holy_res integer DEFAULT 0 NOT NULL,
    fire_res integer DEFAULT 0 NOT NULL,
    nature_res integer DEFAULT 0 NOT NULL,
    frost_res integer DEFAULT 0 NOT NULL,
    shadow_res integer DEFAULT 0 NOT NULL,
    arcane_res integer DEFAULT 0 NOT NULL,
    delay integer DEFAULT 0 NOT NULL,
    ammo_type integer DEFAULT 0 NOT NULL,
    range real DEFAULT 0 NOT NULL,
    spellid_1 integer DEFAULT 0 NOT NULL,
    spelltrigger_1 integer DEFAULT 0 NOT NULL,
    spellcharges_1 integer DEFAULT 0 NOT NULL,
    spellcooldown_1 integer DEFAULT 0 NOT NULL,
    spellcategory_1 integer DEFAULT 0 NOT NULL,
    spellcategorycooldown_1 integer DEFAULT 0 NOT NULL,
    spellid_2 integer DEFAULT 0 NOT NULL,
    spelltrigger_2 integer DEFAULT 0 NOT NULL,
    spellcharges_2 integer DEFAULT 0 NOT NULL,
    spellcooldown_2 integer DEFAULT 0 NOT NULL,
    spellcategory_2 integer DEFAULT 0 NOT NULL,
    spellcategorycooldown_2 integer DEFAULT 0 NOT NULL,
    spellid_3 integer DEFAULT 0 NOT NULL,
    spelltrigger_3 integer DEFAULT 0 NOT NULL,
    spellcharges_3 integer DEFAULT 0 NOT NULL,
    spellcooldown_3 integer DEFAULT 0 NOT NULL,
    spellcategory_3 integer DEFAULT 0 NOT NULL,
    spellcategorycooldown_3 integer DEFAULT 0 NOT NULL,
    spellid_4 integer DEFAULT 0 NOT NULL,
    spelltrigger_4 integer DEFAULT 0 NOT NULL,
    spellcharges_4 integer DEFAULT 0 NOT NULL,
    spellcooldown_4 integer DEFAULT 0 NOT NULL,
    spellcategory_4 integer DEFAULT 0 NOT NULL,
    spellcategorycooldown_4 integer DEFAULT 0 NOT NULL,
    spellid_5 integer DEFAULT 0 NOT NULL,
    spelltrigger_5 integer DEFAULT 0 NOT NULL,
    spellcharges_5 integer DEFAULT 0 NOT NULL,
    spellcooldown_5 integer DEFAULT 0 NOT NULL,
    spellcategory_5 integer DEFAULT 0 NOT NULL,
    spellcategorycooldown_5 integer DEFAULT 0 NOT NULL,
    bonding integer DEFAULT 0 NOT NULL,
    description text NOT NULL,
    page_id integer DEFAULT 0 NOT NULL,
    page_language integer DEFAULT 0 NOT NULL,
    page_material integer DEFAULT 0 NOT NULL,
    quest_id integer DEFAULT 0 NOT NULL,
    lock_id integer DEFAULT 0 NOT NULL,
    lock_material integer DEFAULT 0 NOT NULL,
    sheathid integer DEFAULT 0 NOT NULL,
    randomprop integer DEFAULT 0 NOT NULL,
    randomsuffix integer DEFAULT 0 NOT NULL,
    block integer DEFAULT 0 NOT NULL,
    itemset integer DEFAULT 0 NOT NULL,
    maxdurability integer DEFAULT 0 NOT NULL,
    zonenameid integer DEFAULT 0 NOT NULL,
    mapid integer,
    bagfamily integer DEFAULT 0 NOT NULL,
    totemcategory integer,
    socket_color_1 integer,
    unk201_3 integer DEFAULT 0 NOT NULL,
    socket_color_2 integer,
    unk201_5 integer DEFAULT 0 NOT NULL,
    socket_color_3 integer,
    unk201_7 integer DEFAULT 0 NOT NULL,
    socket_bonus integer,
    gemproperties integer,
    reqdisenchantskill integer DEFAULT (-1) NOT NULL,
    armordamagemodifier integer DEFAULT 0 NOT NULL,
    existingduration integer DEFAULT 0 NOT NULL,
    itemlimitcategoryid integer DEFAULT 0 NOT NULL,
    holidayid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.items OWNER TO arcemu;

--
-- Name: items_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE items_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    name text NOT NULL,
    description text NOT NULL
);


ALTER TABLE public.items_localized OWNER TO arcemu;

--
-- Name: loot_creatures; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE loot_creatures (
    index integer NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance real DEFAULT 0 NOT NULL,
    normal25percentchance real DEFAULT 0 NOT NULL,
    heroic10percentchance real DEFAULT 0 NOT NULL,
    heroic25percentchance real DEFAULT 0 NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.loot_creatures OWNER TO arcemu;

--
-- Name: loot_creatures_index_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE loot_creatures_index_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.loot_creatures_index_seq OWNER TO arcemu;

--
-- Name: loot_creatures_index_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE loot_creatures_index_seq OWNED BY loot_creatures.index;


--
-- Name: loot_fishing; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE loot_fishing (
    index integer NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance real DEFAULT 0 NOT NULL,
    normal25percentchance real DEFAULT 0 NOT NULL,
    heroic10percentchance real DEFAULT 0 NOT NULL,
    heroic25percentchance real DEFAULT 0 NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.loot_fishing OWNER TO arcemu;

--
-- Name: loot_fishing_index_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE loot_fishing_index_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.loot_fishing_index_seq OWNER TO arcemu;

--
-- Name: loot_fishing_index_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE loot_fishing_index_seq OWNED BY loot_fishing.index;


--
-- Name: loot_gameobjects; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE loot_gameobjects (
    index integer NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance real DEFAULT 0 NOT NULL,
    normal25percentchance real DEFAULT 0 NOT NULL,
    heroic10percentchance real DEFAULT 0 NOT NULL,
    heroic25percentchance real DEFAULT 0 NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.loot_gameobjects OWNER TO arcemu;

--
-- Name: loot_gameobjects_index_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE loot_gameobjects_index_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.loot_gameobjects_index_seq OWNER TO arcemu;

--
-- Name: loot_gameobjects_index_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE loot_gameobjects_index_seq OWNED BY loot_gameobjects.index;


--
-- Name: loot_items; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE loot_items (
    index integer NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 25 NOT NULL,
    normal10percentchance real DEFAULT 0 NOT NULL,
    normal25percentchance real DEFAULT 0 NOT NULL,
    heroic10percentchance real DEFAULT 0 NOT NULL,
    heroic25percentchance real DEFAULT 0 NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.loot_items OWNER TO arcemu;

--
-- Name: loot_items_index_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE loot_items_index_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.loot_items_index_seq OWNER TO arcemu;

--
-- Name: loot_items_index_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE loot_items_index_seq OWNED BY loot_items.index;


--
-- Name: loot_pickpocketing; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE loot_pickpocketing (
    index integer NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 25 NOT NULL,
    normal10percentchance real DEFAULT 0 NOT NULL,
    normal25percentchance real DEFAULT 0 NOT NULL,
    heroic10percentchance real DEFAULT 0 NOT NULL,
    heroic25percentchance real DEFAULT 0 NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.loot_pickpocketing OWNER TO arcemu;

--
-- Name: loot_pickpocketing_index_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE loot_pickpocketing_index_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.loot_pickpocketing_index_seq OWNER TO arcemu;

--
-- Name: loot_pickpocketing_index_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE loot_pickpocketing_index_seq OWNED BY loot_pickpocketing.index;


--
-- Name: loot_skinning; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE loot_skinning (
    index integer NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance real DEFAULT 0 NOT NULL,
    normal25percentchance real DEFAULT 0 NOT NULL,
    heroic10percentchance real DEFAULT 0 NOT NULL,
    heroic25percentchance real DEFAULT 0 NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.loot_skinning OWNER TO arcemu;

--
-- Name: loot_skinning_index_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE loot_skinning_index_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.loot_skinning_index_seq OWNER TO arcemu;

--
-- Name: loot_skinning_index_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE loot_skinning_index_seq OWNED BY loot_skinning.index;


--
-- Name: map_checkpoint; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE map_checkpoint (
    entry integer NOT NULL,
    prereq_checkpoint_id integer NOT NULL,
    creature_id integer NOT NULL,
    name text NOT NULL
);


ALTER TABLE public.map_checkpoint OWNER TO arcemu;

--
-- Name: npc_gossip_textid; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE npc_gossip_textid (
    creatureid integer DEFAULT 0 NOT NULL,
    textid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.npc_gossip_textid OWNER TO arcemu;

--
-- Name: npc_monstersay; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE npc_monstersay (
    entry integer DEFAULT 0 NOT NULL,
    event integer DEFAULT 0 NOT NULL,
    chance real DEFAULT 0 NOT NULL,
    language integer DEFAULT 0 NOT NULL,
    type integer DEFAULT 0 NOT NULL,
    monstername text,
    text0 text,
    text1 text,
    text2 text,
    text3 text,
    text4 text
);


ALTER TABLE public.npc_monstersay OWNER TO arcemu;

--
-- Name: npc_text; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE npc_text (
    entry integer DEFAULT 0 NOT NULL,
    prob0 real DEFAULT 0 NOT NULL,
    text0_0 text NOT NULL,
    text0_1 text NOT NULL,
    lang0 integer DEFAULT 0 NOT NULL,
    em0_0 integer DEFAULT 0 NOT NULL,
    em0_1 integer DEFAULT 0 NOT NULL,
    em0_2 integer DEFAULT 0 NOT NULL,
    em0_3 integer DEFAULT 0 NOT NULL,
    em0_4 integer DEFAULT 0 NOT NULL,
    em0_5 integer DEFAULT 0 NOT NULL,
    prob1 real DEFAULT 0 NOT NULL,
    text1_0 text NOT NULL,
    text1_1 text NOT NULL,
    lang1 integer DEFAULT 0 NOT NULL,
    em1_0 integer DEFAULT 0 NOT NULL,
    em1_1 integer DEFAULT 0 NOT NULL,
    em1_2 integer DEFAULT 0 NOT NULL,
    em1_3 integer DEFAULT 0 NOT NULL,
    em1_4 integer DEFAULT 0 NOT NULL,
    em1_5 integer DEFAULT 0 NOT NULL,
    prob2 real DEFAULT 0 NOT NULL,
    text2_0 text NOT NULL,
    text2_1 text NOT NULL,
    lang2 integer DEFAULT 0 NOT NULL,
    em2_0 integer DEFAULT 0 NOT NULL,
    em2_1 integer DEFAULT 0 NOT NULL,
    em2_2 integer DEFAULT 0 NOT NULL,
    em2_3 integer DEFAULT 0 NOT NULL,
    em2_4 integer DEFAULT 0 NOT NULL,
    em2_5 integer DEFAULT 0 NOT NULL,
    prob3 real DEFAULT 0 NOT NULL,
    text3_0 text NOT NULL,
    text3_1 text NOT NULL,
    lang3 integer DEFAULT 0 NOT NULL,
    em3_0 integer DEFAULT 0 NOT NULL,
    em3_1 integer DEFAULT 0 NOT NULL,
    em3_2 integer DEFAULT 0 NOT NULL,
    em3_3 integer DEFAULT 0 NOT NULL,
    em3_4 integer DEFAULT 0 NOT NULL,
    em3_5 integer DEFAULT 0 NOT NULL,
    prob4 real DEFAULT 0 NOT NULL,
    text4_0 text NOT NULL,
    text4_1 text NOT NULL,
    lang4 integer DEFAULT 0 NOT NULL,
    em4_0 integer DEFAULT 0 NOT NULL,
    em4_1 integer DEFAULT 0 NOT NULL,
    em4_2 integer DEFAULT 0 NOT NULL,
    em4_3 integer DEFAULT 0 NOT NULL,
    em4_4 integer DEFAULT 0 NOT NULL,
    em4_5 integer DEFAULT 0 NOT NULL,
    prob5 real DEFAULT 0 NOT NULL,
    text5_0 text NOT NULL,
    text5_1 text NOT NULL,
    lang5 integer DEFAULT 0 NOT NULL,
    em5_0 integer DEFAULT 0 NOT NULL,
    em5_1 integer DEFAULT 0 NOT NULL,
    em5_2 integer DEFAULT 0 NOT NULL,
    em5_3 integer DEFAULT 0 NOT NULL,
    em5_4 integer DEFAULT 0 NOT NULL,
    em5_5 integer DEFAULT 0 NOT NULL,
    prob6 real DEFAULT 0 NOT NULL,
    text6_0 text NOT NULL,
    text6_1 text NOT NULL,
    lang6 integer DEFAULT 0 NOT NULL,
    em6_0 integer DEFAULT 0 NOT NULL,
    em6_1 integer DEFAULT 0 NOT NULL,
    em6_2 integer DEFAULT 0 NOT NULL,
    em6_3 integer DEFAULT 0 NOT NULL,
    em6_4 integer DEFAULT 0 NOT NULL,
    em6_5 integer DEFAULT 0 NOT NULL,
    prob7 real DEFAULT 0 NOT NULL,
    text7_0 text NOT NULL,
    text7_1 text NOT NULL,
    lang7 integer DEFAULT 0 NOT NULL,
    em7_0 integer DEFAULT 0 NOT NULL,
    em7_1 integer DEFAULT 0 NOT NULL,
    em7_2 integer DEFAULT 0 NOT NULL,
    em7_3 integer DEFAULT 0 NOT NULL,
    em7_4 integer DEFAULT 0 NOT NULL,
    em7_5 integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.npc_text OWNER TO arcemu;

--
-- Name: npc_text_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE npc_text_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    text0 text NOT NULL,
    text0_1 text NOT NULL,
    text1 text NOT NULL,
    text1_1 text NOT NULL,
    text2 text NOT NULL,
    text2_1 text NOT NULL,
    text3 text NOT NULL,
    text3_1 text NOT NULL,
    text4 text NOT NULL,
    text4_1 text NOT NULL,
    text5 text NOT NULL,
    text5_1 text NOT NULL,
    text6 text NOT NULL,
    text6_1 text NOT NULL,
    text7 text NOT NULL,
    text7_1 text NOT NULL
);


ALTER TABLE public.npc_text_localized OWNER TO arcemu;

--
-- Name: petdefaultspells; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE petdefaultspells (
    entry integer DEFAULT 0 NOT NULL,
    spell integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.petdefaultspells OWNER TO arcemu;

--
-- Name: playercreateinfo; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playercreateinfo (
    index integer NOT NULL,
    race numeric(3,0) DEFAULT 0 NOT NULL,
    factiontemplate integer DEFAULT 0 NOT NULL,
    class numeric(3,0) DEFAULT 0 NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    zoneid integer DEFAULT 0 NOT NULL,
    positionx real DEFAULT 0 NOT NULL,
    positiony real DEFAULT 0 NOT NULL,
    positionz real DEFAULT 0 NOT NULL,
    displayid smallint DEFAULT 0 NOT NULL,
    basestrength numeric(3,0) DEFAULT 0 NOT NULL,
    baseagility numeric(3,0) DEFAULT 0 NOT NULL,
    basestamina numeric(3,0) DEFAULT 0 NOT NULL,
    baseintellect numeric(3,0) DEFAULT 0 NOT NULL,
    basespirit numeric(3,0) DEFAULT 0 NOT NULL,
    basehealth integer DEFAULT 0 NOT NULL,
    basemana integer DEFAULT 0 NOT NULL,
    baserage integer DEFAULT 0 NOT NULL,
    basefocus integer DEFAULT 0 NOT NULL,
    baseenergy integer DEFAULT 0 NOT NULL,
    attackpower integer DEFAULT 0 NOT NULL,
    mindmg real DEFAULT 0 NOT NULL,
    maxdmg real DEFAULT 0 NOT NULL,
    introid integer DEFAULT 0 NOT NULL,
    taximask text
);


ALTER TABLE public.playercreateinfo OWNER TO arcemu;

--
-- Name: playercreateinfo_bars; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playercreateinfo_bars (
    race numeric(3,0),
    class numeric(3,0),
    button integer,
    action integer,
    type integer,
    misc integer
);


ALTER TABLE public.playercreateinfo_bars OWNER TO arcemu;

--
-- Name: playercreateinfo_index_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE playercreateinfo_index_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.playercreateinfo_index_seq OWNER TO arcemu;

--
-- Name: playercreateinfo_index_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE playercreateinfo_index_seq OWNED BY playercreateinfo.index;


--
-- Name: playercreateinfo_items; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playercreateinfo_items (
    indexid numeric(3,0) DEFAULT 0 NOT NULL,
    protoid integer DEFAULT 0 NOT NULL,
    slotid numeric(3,0) DEFAULT 0 NOT NULL,
    amount integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.playercreateinfo_items OWNER TO arcemu;

--
-- Name: playercreateinfo_skills; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playercreateinfo_skills (
    indexid numeric(3,0) DEFAULT 0 NOT NULL,
    skillid integer DEFAULT 0 NOT NULL,
    level integer DEFAULT 0 NOT NULL,
    maxlevel integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.playercreateinfo_skills OWNER TO arcemu;

--
-- Name: playercreateinfo_spells; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playercreateinfo_spells (
    indexid numeric(3,0) DEFAULT 0 NOT NULL,
    spellid smallint DEFAULT 0 NOT NULL
);


ALTER TABLE public.playercreateinfo_spells OWNER TO arcemu;

--
-- Name: professiondiscoveries; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE professiondiscoveries (
    spellid integer DEFAULT 0 NOT NULL,
    spelltodiscover integer DEFAULT 0 NOT NULL,
    skillvalue integer DEFAULT 0 NOT NULL,
    chance real DEFAULT 0 NOT NULL
);


ALTER TABLE public.professiondiscoveries OWNER TO arcemu;

--
-- Name: quests; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE quests (
    entry integer DEFAULT 0 NOT NULL,
    zoneid integer DEFAULT 0 NOT NULL,
    sort integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    minlevel integer DEFAULT 0 NOT NULL,
    questlevel integer DEFAULT 0 NOT NULL,
    type integer DEFAULT 0 NOT NULL,
    requiredraces integer DEFAULT 0 NOT NULL,
    requiredclass integer DEFAULT 0 NOT NULL,
    requiredtradeskill integer DEFAULT 0 NOT NULL,
    requiredtradeskillvalue integer DEFAULT 0 NOT NULL,
    requiredrepfaction integer DEFAULT 0 NOT NULL,
    requiredrepvalue integer DEFAULT 0 NOT NULL,
    limittime integer DEFAULT 0 NOT NULL,
    specialflags integer DEFAULT 0 NOT NULL,
    prevquestid integer DEFAULT 0 NOT NULL,
    nextquestid integer DEFAULT 0 NOT NULL,
    srcitem integer DEFAULT 0 NOT NULL,
    srcitemcount integer DEFAULT 0 NOT NULL,
    title character(255) NOT NULL,
    details text NOT NULL,
    objectives text NOT NULL,
    completiontext text NOT NULL,
    incompletetext text NOT NULL,
    endtext text NOT NULL,
    objectivetext1 text NOT NULL,
    objectivetext2 text NOT NULL,
    objectivetext3 text NOT NULL,
    objectivetext4 text NOT NULL,
    reqitemid1 integer DEFAULT 0 NOT NULL,
    reqitemid2 integer DEFAULT 0 NOT NULL,
    reqitemid3 integer DEFAULT 0 NOT NULL,
    reqitemid4 integer DEFAULT 0 NOT NULL,
    reqitemid5 integer DEFAULT 0 NOT NULL,
    reqitemid6 integer DEFAULT 0 NOT NULL,
    reqitemcount1 integer DEFAULT 0 NOT NULL,
    reqitemcount2 integer DEFAULT 0 NOT NULL,
    reqitemcount3 integer DEFAULT 0 NOT NULL,
    reqitemcount4 integer DEFAULT 0 NOT NULL,
    reqitemcount5 integer DEFAULT 0 NOT NULL,
    reqitemcount6 integer DEFAULT 0 NOT NULL,
    reqkillmoborgoid1 integer DEFAULT 0 NOT NULL,
    reqkillmoborgoid2 integer DEFAULT 0 NOT NULL,
    reqkillmoborgoid3 integer DEFAULT 0 NOT NULL,
    reqkillmoborgoid4 integer DEFAULT 0 NOT NULL,
    reqkillmoborgocount1 integer DEFAULT 0 NOT NULL,
    reqkillmoborgocount2 integer DEFAULT 0 NOT NULL,
    reqkillmoborgocount3 integer DEFAULT 0 NOT NULL,
    reqkillmoborgocount4 integer DEFAULT 0 NOT NULL,
    reqcastspellid1 integer DEFAULT 0 NOT NULL,
    reqcastspellid2 integer DEFAULT 0 NOT NULL,
    reqcastspellid3 integer DEFAULT 0 NOT NULL,
    reqcastspellid4 integer DEFAULT 0 NOT NULL,
    reqemoteid1 integer DEFAULT 0,
    reqemoteid2 integer DEFAULT 0,
    reqemoteid3 integer DEFAULT 0,
    reqemoteid4 integer DEFAULT 0,
    rewchoiceitemid1 integer DEFAULT 0 NOT NULL,
    rewchoiceitemid2 integer DEFAULT 0 NOT NULL,
    rewchoiceitemid3 integer DEFAULT 0 NOT NULL,
    rewchoiceitemid4 integer DEFAULT 0 NOT NULL,
    rewchoiceitemid5 integer DEFAULT 0 NOT NULL,
    rewchoiceitemid6 integer DEFAULT 0 NOT NULL,
    rewchoiceitemcount1 integer DEFAULT 0 NOT NULL,
    rewchoiceitemcount2 integer DEFAULT 0 NOT NULL,
    rewchoiceitemcount3 integer DEFAULT 0 NOT NULL,
    rewchoiceitemcount4 integer DEFAULT 0 NOT NULL,
    rewchoiceitemcount5 integer DEFAULT 0 NOT NULL,
    rewchoiceitemcount6 integer DEFAULT 0 NOT NULL,
    rewitemid1 integer DEFAULT 0 NOT NULL,
    rewitemid2 integer DEFAULT 0 NOT NULL,
    rewitemid3 integer DEFAULT 0 NOT NULL,
    rewitemid4 integer DEFAULT 0 NOT NULL,
    rewitemcount1 integer DEFAULT 0 NOT NULL,
    rewitemcount2 integer DEFAULT 0 NOT NULL,
    rewitemcount3 integer DEFAULT 0 NOT NULL,
    rewitemcount4 integer DEFAULT 0 NOT NULL,
    rewrepfaction1 integer DEFAULT 0 NOT NULL,
    rewrepfaction2 integer DEFAULT 0 NOT NULL,
    rewrepfaction3 integer DEFAULT 0 NOT NULL,
    rewrepfaction4 integer DEFAULT 0 NOT NULL,
    rewrepfaction5 integer DEFAULT 0 NOT NULL,
    rewrepfaction6 integer DEFAULT 0 NOT NULL,
    rewrepvalue1 integer DEFAULT 0 NOT NULL,
    rewrepvalue2 integer DEFAULT 0 NOT NULL,
    rewrepvalue3 integer DEFAULT 0 NOT NULL,
    rewrepvalue4 integer DEFAULT 0 NOT NULL,
    rewrepvalue5 integer DEFAULT 0 NOT NULL,
    rewrepvalue6 integer DEFAULT 0 NOT NULL,
    rewreplimit integer DEFAULT 0 NOT NULL,
    rewmoney integer DEFAULT 0 NOT NULL,
    rewxp integer DEFAULT 0 NOT NULL,
    rewspell integer DEFAULT 0 NOT NULL,
    castspell integer DEFAULT 0 NOT NULL,
    pointmapid integer DEFAULT 0 NOT NULL,
    pointx real DEFAULT 0 NOT NULL,
    pointy real DEFAULT 0 NOT NULL,
    pointopt integer DEFAULT 0 NOT NULL,
    rewardmoneyatmaxlevel integer DEFAULT 0 NOT NULL,
    exploretrigger1 integer DEFAULT 0 NOT NULL,
    exploretrigger2 integer DEFAULT 0 NOT NULL,
    exploretrigger3 integer DEFAULT 0 NOT NULL,
    exploretrigger4 integer DEFAULT 0 NOT NULL,
    requiredoneofquest text NOT NULL,
    requiredquest1 integer DEFAULT 0 NOT NULL,
    requiredquest2 integer DEFAULT 0 NOT NULL,
    requiredquest3 integer DEFAULT 0 NOT NULL,
    requiredquest4 integer DEFAULT 0 NOT NULL,
    removequests text NOT NULL,
    receiveitemid1 integer DEFAULT 0 NOT NULL,
    receiveitemid2 integer DEFAULT 0 NOT NULL,
    receiveitemid3 integer DEFAULT 0 NOT NULL,
    receiveitemid4 integer DEFAULT 0 NOT NULL,
    receiveitemcount1 integer DEFAULT 0 NOT NULL,
    receiveitemcount2 integer DEFAULT 0 NOT NULL,
    receiveitemcount3 integer DEFAULT 0 NOT NULL,
    receiveitemcount4 integer DEFAULT 0 NOT NULL,
    isrepeatable integer DEFAULT 0 NOT NULL,
    bonushonor integer DEFAULT 0 NOT NULL,
    rewardtitleid integer DEFAULT 0 NOT NULL,
    rewardtalents integer DEFAULT 0 NOT NULL,
    suggestedplayers integer DEFAULT 0 NOT NULL,
    detailemotecount integer DEFAULT 0 NOT NULL,
    detailemote1 integer DEFAULT 0 NOT NULL,
    detailemote2 integer DEFAULT 0 NOT NULL,
    detailemote3 integer DEFAULT 0 NOT NULL,
    detailemote4 integer DEFAULT 0 NOT NULL,
    detailemotedelay1 integer DEFAULT 0 NOT NULL,
    detailemotedelay2 integer DEFAULT 0 NOT NULL,
    detailemotedelay3 integer DEFAULT 0 NOT NULL,
    detailemotedelay4 integer DEFAULT 0 NOT NULL,
    completionemotecnt integer DEFAULT 0 NOT NULL,
    completionemote1 integer DEFAULT 0 NOT NULL,
    completionemote2 integer DEFAULT 0 NOT NULL,
    completionemote3 integer DEFAULT 0 NOT NULL,
    completionemote4 integer DEFAULT 0 NOT NULL,
    completionemotedelay1 integer DEFAULT 0 NOT NULL,
    completionemotedelay2 integer DEFAULT 0 NOT NULL,
    completionemotedelay3 integer DEFAULT 0 NOT NULL,
    completionemotedelay4 integer DEFAULT 0 NOT NULL,
    completeemote integer DEFAULT 0 NOT NULL,
    incompleteemote integer DEFAULT 0 NOT NULL,
    iscompletedbyspelleffect integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.quests OWNER TO arcemu;

--
-- Name: quests_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE quests_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    title text NOT NULL,
    details text NOT NULL,
    objectives text NOT NULL,
    completiontext text NOT NULL,
    incompletetext text NOT NULL,
    endtext text NOT NULL,
    objectivetext1 text NOT NULL,
    objectivetext2 text NOT NULL,
    objectivetext3 text NOT NULL,
    objectivetext4 text NOT NULL
);


ALTER TABLE public.quests_localized OWNER TO arcemu;

--
-- Name: recall; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE recall (
    id bigint NOT NULL,
    name text NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    positionx real DEFAULT 0 NOT NULL,
    positiony real DEFAULT 0 NOT NULL,
    positionz real DEFAULT 0 NOT NULL,
    orientation bytea NOT NULL
);


ALTER TABLE public.recall OWNER TO arcemu;

--
-- Name: recall_id_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE recall_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.recall_id_seq OWNER TO arcemu;

--
-- Name: recall_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE recall_id_seq OWNED BY recall.id;


--
-- Name: reputation_creature_onkill; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE reputation_creature_onkill (
    creature_id integer NOT NULL,
    faction_change_alliance integer NOT NULL,
    faction_change_horde integer NOT NULL,
    change_value integer NOT NULL,
    rep_limit integer NOT NULL
);


ALTER TABLE public.reputation_creature_onkill OWNER TO arcemu;

--
-- Name: reputation_faction_onkill; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE reputation_faction_onkill (
    faction_id integer NOT NULL,
    change_factionid_alliance integer NOT NULL,
    change_deltamin_alliance integer NOT NULL,
    change_deltamax_alliance integer NOT NULL,
    change_factionid_horde integer NOT NULL,
    change_deltamin_horde integer NOT NULL,
    change_deltamax_horde integer NOT NULL
);


ALTER TABLE public.reputation_faction_onkill OWNER TO arcemu;

--
-- Name: reputation_instance_onkill; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE reputation_instance_onkill (
    mapid integer NOT NULL,
    mob_rep_reward integer NOT NULL,
    mob_rep_limit integer NOT NULL,
    boss_rep_reward integer NOT NULL,
    boss_rep_limit integer NOT NULL,
    faction_change_alliance integer NOT NULL,
    faction_change_horde integer NOT NULL
);


ALTER TABLE public.reputation_instance_onkill OWNER TO arcemu;

--
-- Name: spell_coef_override; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE spell_coef_override (
    id double precision,
    name text,
    dspell_coef_override real,
    otspell_coef_override real
);


ALTER TABLE public.spell_coef_override OWNER TO arcemu;

--
-- Name: spell_disable; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE spell_disable (
    spellid integer NOT NULL,
    replacement_spellid integer NOT NULL
);


ALTER TABLE public.spell_disable OWNER TO arcemu;

--
-- Name: spell_disable_trainers; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE spell_disable_trainers (
    spellid integer NOT NULL,
    replacement_spellid integer NOT NULL
);


ALTER TABLE public.spell_disable_trainers OWNER TO arcemu;

--
-- Name: spell_effects_override; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE spell_effects_override (
    spellid integer DEFAULT 0 NOT NULL,
    effectid integer NOT NULL,
    disable integer NOT NULL,
    effect integer DEFAULT 0 NOT NULL,
    basepoints integer DEFAULT 0 NOT NULL,
    applyauraname integer DEFAULT 0 NOT NULL,
    spellgrouprelation integer DEFAULT 0 NOT NULL,
    miscvalue integer DEFAULT 0 NOT NULL,
    triggerspell integer DEFAULT 0 NOT NULL,
    implicittargeta integer DEFAULT 0 NOT NULL,
    implicittargetb integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.spell_effects_override OWNER TO arcemu;

--
-- Name: spell_proc; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE spell_proc (
    spellid integer DEFAULT 0 NOT NULL,
    proconnamehash integer DEFAULT 0 NOT NULL,
    procflag integer DEFAULT 0 NOT NULL,
    targetself numeric(3,0) DEFAULT 0 NOT NULL,
    procchance integer DEFAULT (-1) NOT NULL,
    proccharges smallint DEFAULT (-1) NOT NULL,
    procinterval integer DEFAULT 0 NOT NULL,
    effecttriggerspell_0_ integer DEFAULT (-1) NOT NULL,
    effecttriggerspell_1_ integer DEFAULT (-1) NOT NULL,
    effecttriggerspell_2_ integer DEFAULT (-1) NOT NULL
);


ALTER TABLE public.spell_proc OWNER TO arcemu;

--
-- Name: spelloverride; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE spelloverride (
    overrideid integer DEFAULT 0 NOT NULL,
    spellid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.spelloverride OWNER TO arcemu;

--
-- Name: teleport_coords; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE teleport_coords (
    id integer DEFAULT 0 NOT NULL,
    name character(255) NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    position_x real DEFAULT 0 NOT NULL,
    position_y real DEFAULT 0 NOT NULL,
    position_z real DEFAULT 0 NOT NULL,
    totrigger integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.teleport_coords OWNER TO arcemu;

--
-- Name: totemspells; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE totemspells (
    spell integer DEFAULT 0 NOT NULL,
    castspell1 integer DEFAULT 0 NOT NULL,
    castspell2 integer DEFAULT 0 NOT NULL,
    castspell3 integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.totemspells OWNER TO arcemu;

--
-- Name: trainer_defs; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE trainer_defs (
    entry integer DEFAULT 0 NOT NULL,
    required_skill integer DEFAULT 0 NOT NULL,
    required_skillvalue integer DEFAULT 0,
    req_class integer DEFAULT 0 NOT NULL,
    requiredrace integer DEFAULT 0 NOT NULL,
    requiredreputation integer DEFAULT 0 NOT NULL,
    requiredreputationvalue integer DEFAULT 0 NOT NULL,
    trainer_type integer DEFAULT 0 NOT NULL,
    trainer_ui_window_message text,
    can_train_gossip_textid integer NOT NULL,
    cannot_train_gossip_textid integer NOT NULL
);


ALTER TABLE public.trainer_defs OWNER TO arcemu;

--
-- Name: trainer_spells; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE trainer_spells (
    entry integer DEFAULT 0 NOT NULL,
    cast_spell integer DEFAULT 0 NOT NULL,
    learn_spell integer NOT NULL,
    spellcost integer DEFAULT 0 NOT NULL,
    reqspell integer DEFAULT 0 NOT NULL,
    reqskill integer DEFAULT 0 NOT NULL,
    reqskillvalue integer DEFAULT 0 NOT NULL,
    reqlevel integer DEFAULT 0 NOT NULL,
    deletespell integer DEFAULT 0 NOT NULL,
    is_prof numeric(3,0) DEFAULT 0 NOT NULL
);


ALTER TABLE public.trainer_spells OWNER TO arcemu;

--
-- Name: trainerspelloverride; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE trainerspelloverride (
    spellid integer DEFAULT 0 NOT NULL,
    cost integer DEFAULT 0 NOT NULL,
    requiredspell integer DEFAULT 0 NOT NULL,
    deletespell integer DEFAULT 0 NOT NULL,
    requiredskill integer DEFAULT 0 NOT NULL,
    requiredskillvalue integer DEFAULT 0 NOT NULL,
    reqlevel integer DEFAULT 0 NOT NULL,
    requiredclass integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.trainerspelloverride OWNER TO arcemu;

--
-- Name: transport_creatures; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE transport_creatures (
    transport_entry integer NOT NULL,
    creature_entry integer NOT NULL,
    position_x real NOT NULL,
    position_y real NOT NULL,
    position_z real NOT NULL,
    orientation real NOT NULL
);


ALTER TABLE public.transport_creatures OWNER TO arcemu;

--
-- Name: transport_data; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE transport_data (
    entry numeric(7,0) DEFAULT 0 NOT NULL,
    name text,
    period numeric(7,0) DEFAULT 0 NOT NULL
);


ALTER TABLE public.transport_data OWNER TO arcemu;

--
-- Name: unit_display_sizes; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE unit_display_sizes (
    displayid integer NOT NULL,
    halfsize real DEFAULT 1 NOT NULL,
    modelid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.unit_display_sizes OWNER TO arcemu;

--
-- Name: vendor_restrictions; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE vendor_restrictions (
    entry integer NOT NULL,
    racemask integer DEFAULT (-1) NOT NULL,
    reqrepfaction integer DEFAULT 0 NOT NULL,
    reqrepfactionvalue integer DEFAULT 0 NOT NULL,
    canbuyattextid integer DEFAULT 0 NOT NULL,
    cannotbuyattextid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.vendor_restrictions OWNER TO arcemu;

--
-- Name: vendors; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE vendors (
    entry integer DEFAULT 0 NOT NULL,
    item integer DEFAULT 0 NOT NULL,
    amount integer DEFAULT 0 NOT NULL,
    max_amount integer DEFAULT 0 NOT NULL,
    inctime bigint DEFAULT 0 NOT NULL,
    extended_cost integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.vendors OWNER TO arcemu;

--
-- Name: weather; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE weather (
    zoneid integer DEFAULT 0 NOT NULL,
    high_chance integer DEFAULT 0 NOT NULL,
    high_type integer DEFAULT 0 NOT NULL,
    med_chance integer DEFAULT 0 NOT NULL,
    med_type integer DEFAULT 0 NOT NULL,
    low_chance integer DEFAULT 0 NOT NULL,
    low_type integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.weather OWNER TO arcemu;

--
-- Name: wordfilter_character_names; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE wordfilter_character_names (
    regex_match text NOT NULL,
    regex_ignore_if_matched text NOT NULL
);


ALTER TABLE public.wordfilter_character_names OWNER TO arcemu;

--
-- Name: wordfilter_chat; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE wordfilter_chat (
    regex_match text NOT NULL,
    regex_ignore_if_matched text NOT NULL
);


ALTER TABLE public.wordfilter_chat OWNER TO arcemu;

--
-- Name: worldbroadcast; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE worldbroadcast (
    entry integer NOT NULL,
    text text NOT NULL,
    percent integer DEFAULT 100 NOT NULL
);


ALTER TABLE public.worldbroadcast OWNER TO arcemu;

--
-- Name: worldbroadcast_entry_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE worldbroadcast_entry_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.worldbroadcast_entry_seq OWNER TO arcemu;

--
-- Name: worldbroadcast_entry_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE worldbroadcast_entry_seq OWNED BY worldbroadcast.entry;


--
-- Name: worldbroadcast_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE worldbroadcast_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    text text NOT NULL
);


ALTER TABLE public.worldbroadcast_localized OWNER TO arcemu;

--
-- Name: worldmap_info; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE worldmap_info (
    entry integer DEFAULT 0 NOT NULL,
    screenid integer DEFAULT 0,
    type integer DEFAULT 0,
    maxplayers integer DEFAULT 0,
    minlevel integer DEFAULT 1,
    minlevel_heroic integer DEFAULT 0 NOT NULL,
    repopx real DEFAULT 0,
    repopy real DEFAULT 0,
    repopz real DEFAULT 0,
    repopentry integer DEFAULT 0,
    area_name text,
    flags integer DEFAULT 0 NOT NULL,
    cooldown integer DEFAULT 0 NOT NULL,
    lvl_mod_a integer DEFAULT 0 NOT NULL,
    required_quest integer DEFAULT 0 NOT NULL,
    required_item integer DEFAULT 0 NOT NULL,
    heroic_keyid_1 integer DEFAULT 0 NOT NULL,
    heroic_keyid_2 integer DEFAULT 0 NOT NULL,
    viewingdistance real DEFAULT 80 NOT NULL,
    required_checkpoint integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.worldmap_info OWNER TO arcemu;

--
-- Name: worldmap_info_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE worldmap_info_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    text text NOT NULL
);


ALTER TABLE public.worldmap_info_localized OWNER TO arcemu;

--
-- Name: worldstring_tables; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE worldstring_tables (
    entry integer NOT NULL,
    text text NOT NULL
);


ALTER TABLE public.worldstring_tables OWNER TO arcemu;

--
-- Name: worldstring_tables_entry_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE worldstring_tables_entry_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.worldstring_tables_entry_seq OWNER TO arcemu;

--
-- Name: worldstring_tables_entry_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE worldstring_tables_entry_seq OWNED BY worldstring_tables.entry;


--
-- Name: worldstring_tables_localized; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE worldstring_tables_localized (
    entry integer NOT NULL,
    language_code text NOT NULL,
    text text NOT NULL
);


ALTER TABLE public.worldstring_tables_localized OWNER TO arcemu;

--
-- Name: zoneguards; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE zoneguards (
    zone integer NOT NULL,
    horde_entry integer,
    alliance_entry integer
);


ALTER TABLE public.zoneguards OWNER TO arcemu;

--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE auctionhouse ALTER COLUMN id SET DEFAULT nextval('auctionhouse_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE clientaddons ALTER COLUMN id SET DEFAULT nextval('clientaddons_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE creature_spawns ALTER COLUMN id SET DEFAULT nextval('creature_spawns_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE creature_staticspawns ALTER COLUMN id SET DEFAULT nextval('creature_staticspawns_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE gameobject_spawns ALTER COLUMN id SET DEFAULT nextval('gameobject_spawns_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE gameobject_staticspawns ALTER COLUMN id SET DEFAULT nextval('gameobject_staticspawns_id_seq'::regclass);


--
-- Name: entry; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE gameobject_teleports ALTER COLUMN entry SET DEFAULT nextval('gameobject_teleports_entry_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE graveyards ALTER COLUMN id SET DEFAULT nextval('graveyards_id_seq'::regclass);


--
-- Name: index; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE loot_creatures ALTER COLUMN index SET DEFAULT nextval('loot_creatures_index_seq'::regclass);


--
-- Name: index; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE loot_fishing ALTER COLUMN index SET DEFAULT nextval('loot_fishing_index_seq'::regclass);


--
-- Name: index; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE loot_gameobjects ALTER COLUMN index SET DEFAULT nextval('loot_gameobjects_index_seq'::regclass);


--
-- Name: index; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE loot_items ALTER COLUMN index SET DEFAULT nextval('loot_items_index_seq'::regclass);


--
-- Name: index; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE loot_pickpocketing ALTER COLUMN index SET DEFAULT nextval('loot_pickpocketing_index_seq'::regclass);


--
-- Name: index; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE loot_skinning ALTER COLUMN index SET DEFAULT nextval('loot_skinning_index_seq'::regclass);


--
-- Name: index; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE playercreateinfo ALTER COLUMN index SET DEFAULT nextval('playercreateinfo_index_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE recall ALTER COLUMN id SET DEFAULT nextval('recall_id_seq'::regclass);


--
-- Name: entry; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE worldbroadcast ALTER COLUMN entry SET DEFAULT nextval('worldbroadcast_entry_seq'::regclass);


--
-- Name: entry; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE worldstring_tables ALTER COLUMN entry SET DEFAULT nextval('worldstring_tables_entry_seq'::regclass);


--
-- Name: pk_ai_agents; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY ai_agents
    ADD CONSTRAINT pk_ai_agents PRIMARY KEY (entry, spell);


--
-- Name: pk_ai_threattospellid; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY ai_threattospellid
    ADD CONSTRAINT pk_ai_threattospellid PRIMARY KEY (spell);


--
-- Name: pk_areatriggers; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY areatriggers
    ADD CONSTRAINT pk_areatriggers PRIMARY KEY (entry);


--
-- Name: pk_auctionhouse; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY auctionhouse
    ADD CONSTRAINT pk_auctionhouse PRIMARY KEY (id);


--
-- Name: pk_banned_phrases; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY banned_phrases
    ADD CONSTRAINT pk_banned_phrases PRIMARY KEY (phrase);


--
-- Name: pk_clientaddons; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY clientaddons
    ADD CONSTRAINT pk_clientaddons PRIMARY KEY (id);


--
-- Name: pk_command_overrides; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY command_overrides
    ADD CONSTRAINT pk_command_overrides PRIMARY KEY (command_name);


--
-- Name: pk_creature_formations; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_formations
    ADD CONSTRAINT pk_creature_formations PRIMARY KEY (spawn_id);


--
-- Name: pk_creature_names; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_names
    ADD CONSTRAINT pk_creature_names PRIMARY KEY (entry);


--
-- Name: pk_creature_names_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_names_localized
    ADD CONSTRAINT pk_creature_names_localized PRIMARY KEY (id, language_code);


--
-- Name: pk_creature_proto; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_proto
    ADD CONSTRAINT pk_creature_proto PRIMARY KEY (entry);


--
-- Name: pk_creature_quest_finisher; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_quest_finisher
    ADD CONSTRAINT pk_creature_quest_finisher PRIMARY KEY (id, quest);


--
-- Name: pk_creature_quest_starter; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_quest_starter
    ADD CONSTRAINT pk_creature_quest_starter PRIMARY KEY (id, quest);


--
-- Name: pk_creature_spawns; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_spawns
    ADD CONSTRAINT pk_creature_spawns PRIMARY KEY (id);


--
-- Name: pk_creature_staticspawns; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_staticspawns
    ADD CONSTRAINT pk_creature_staticspawns PRIMARY KEY (id);


--
-- Name: pk_creature_timed_emotes; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_timed_emotes
    ADD CONSTRAINT pk_creature_timed_emotes PRIMARY KEY (spawnid, rowid);


--
-- Name: pk_creature_waypoints; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY creature_waypoints
    ADD CONSTRAINT pk_creature_waypoints PRIMARY KEY (spawnid, waypointid);


--
-- Name: pk_fishing; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY fishing
    ADD CONSTRAINT pk_fishing PRIMARY KEY (zone);


--
-- Name: pk_gameobject_names; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_names
    ADD CONSTRAINT pk_gameobject_names PRIMARY KEY (entry);


--
-- Name: pk_gameobject_names_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_names_localized
    ADD CONSTRAINT pk_gameobject_names_localized PRIMARY KEY (entry, language_code);


--
-- Name: pk_gameobject_quest_finisher; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_finisher
    ADD CONSTRAINT pk_gameobject_quest_finisher PRIMARY KEY (id, quest);


--
-- Name: pk_gameobject_quest_item_binding; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_item_binding
    ADD CONSTRAINT pk_gameobject_quest_item_binding PRIMARY KEY (entry, item, quest);


--
-- Name: pk_gameobject_quest_pickup_binding; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_pickup_binding
    ADD CONSTRAINT pk_gameobject_quest_pickup_binding PRIMARY KEY (entry, quest);


--
-- Name: pk_gameobject_quest_starter; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_starter
    ADD CONSTRAINT pk_gameobject_quest_starter PRIMARY KEY (id, quest);


--
-- Name: pk_gameobject_spawns; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_spawns
    ADD CONSTRAINT pk_gameobject_spawns PRIMARY KEY (id);


--
-- Name: pk_gameobject_staticspawns; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_staticspawns
    ADD CONSTRAINT pk_gameobject_staticspawns PRIMARY KEY (id);


--
-- Name: pk_gameobject_teleports; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gameobject_teleports
    ADD CONSTRAINT pk_gameobject_teleports PRIMARY KEY (entry);


--
-- Name: pk_graveyards; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY graveyards
    ADD CONSTRAINT pk_graveyards PRIMARY KEY (id);


--
-- Name: pk_instance_bosses; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY instance_bosses
    ADD CONSTRAINT pk_instance_bosses PRIMARY KEY (mapid, creatureid);


--
-- Name: pk_item_quest_association; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY item_quest_association
    ADD CONSTRAINT pk_item_quest_association PRIMARY KEY (item, quest);


--
-- Name: pk_item_randomprop_groups; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY item_randomprop_groups
    ADD CONSTRAINT pk_item_randomprop_groups PRIMARY KEY (entry_id, randomprops_entryid);


--
-- Name: pk_item_randomsuffix_groups; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY item_randomsuffix_groups
    ADD CONSTRAINT pk_item_randomsuffix_groups PRIMARY KEY (entry_id, randomsuffix_entryid);


--
-- Name: pk_itemnames; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY itemnames
    ADD CONSTRAINT pk_itemnames PRIMARY KEY (entry);


--
-- Name: pk_itempages; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY itempages
    ADD CONSTRAINT pk_itempages PRIMARY KEY (entry);


--
-- Name: pk_itempages_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY itempages_localized
    ADD CONSTRAINT pk_itempages_localized PRIMARY KEY (entry, language_code);


--
-- Name: pk_itempetfood; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY itempetfood
    ADD CONSTRAINT pk_itempetfood PRIMARY KEY (entry);


--
-- Name: pk_items; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY items
    ADD CONSTRAINT pk_items PRIMARY KEY (entry);


--
-- Name: pk_items_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY items_localized
    ADD CONSTRAINT pk_items_localized PRIMARY KEY (entry, language_code);


--
-- Name: pk_loot_creatures; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY loot_creatures
    ADD CONSTRAINT pk_loot_creatures PRIMARY KEY (entryid, itemid);


--
-- Name: pk_loot_fishing; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY loot_fishing
    ADD CONSTRAINT pk_loot_fishing PRIMARY KEY (itemid, entryid);


--
-- Name: pk_loot_gameobjects; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY loot_gameobjects
    ADD CONSTRAINT pk_loot_gameobjects PRIMARY KEY (entryid, itemid);


--
-- Name: pk_loot_items; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY loot_items
    ADD CONSTRAINT pk_loot_items PRIMARY KEY (entryid, itemid);


--
-- Name: pk_loot_pickpocketing; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY loot_pickpocketing
    ADD CONSTRAINT pk_loot_pickpocketing PRIMARY KEY (entryid, itemid);


--
-- Name: pk_loot_skinning; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY loot_skinning
    ADD CONSTRAINT pk_loot_skinning PRIMARY KEY (itemid, entryid);


--
-- Name: pk_map_checkpoint; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY map_checkpoint
    ADD CONSTRAINT pk_map_checkpoint PRIMARY KEY (entry);


--
-- Name: pk_npc_gossip_textid; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY npc_gossip_textid
    ADD CONSTRAINT pk_npc_gossip_textid PRIMARY KEY (creatureid);


--
-- Name: pk_npc_monstersay; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY npc_monstersay
    ADD CONSTRAINT pk_npc_monstersay PRIMARY KEY (entry, event);


--
-- Name: pk_npc_text; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY npc_text
    ADD CONSTRAINT pk_npc_text PRIMARY KEY (entry);


--
-- Name: pk_npc_text_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY npc_text_localized
    ADD CONSTRAINT pk_npc_text_localized PRIMARY KEY (entry, language_code);


--
-- Name: pk_playercreateinfo; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY playercreateinfo
    ADD CONSTRAINT pk_playercreateinfo PRIMARY KEY (index);


--
-- Name: pk_professiondiscoveries; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY professiondiscoveries
    ADD CONSTRAINT pk_professiondiscoveries PRIMARY KEY (spellid, spelltodiscover);


--
-- Name: pk_quests; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY quests
    ADD CONSTRAINT pk_quests PRIMARY KEY (entry);


--
-- Name: pk_quests_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY quests_localized
    ADD CONSTRAINT pk_quests_localized PRIMARY KEY (entry, language_code);


--
-- Name: pk_recall; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY recall
    ADD CONSTRAINT pk_recall PRIMARY KEY (id);


--
-- Name: pk_reputation_creature_onkill; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY reputation_creature_onkill
    ADD CONSTRAINT pk_reputation_creature_onkill PRIMARY KEY (creature_id, faction_change_horde, faction_change_alliance);


--
-- Name: pk_reputation_faction_onkill; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY reputation_faction_onkill
    ADD CONSTRAINT pk_reputation_faction_onkill PRIMARY KEY (faction_id, change_factionid_horde, change_factionid_alliance);


--
-- Name: pk_reputation_instance_onkill; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY reputation_instance_onkill
    ADD CONSTRAINT pk_reputation_instance_onkill PRIMARY KEY (mapid, faction_change_horde, faction_change_alliance);


--
-- Name: pk_spell_effects_override; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY spell_effects_override
    ADD CONSTRAINT pk_spell_effects_override PRIMARY KEY (spellid);


--
-- Name: pk_spell_proc; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY spell_proc
    ADD CONSTRAINT pk_spell_proc PRIMARY KEY (spellid);


--
-- Name: pk_teleport_coords; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY teleport_coords
    ADD CONSTRAINT pk_teleport_coords PRIMARY KEY (id);


--
-- Name: pk_totemspells; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY totemspells
    ADD CONSTRAINT pk_totemspells PRIMARY KEY (spell);


--
-- Name: pk_trainer_defs; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY trainer_defs
    ADD CONSTRAINT pk_trainer_defs PRIMARY KEY (entry);


--
-- Name: pk_trainer_spells; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY trainer_spells
    ADD CONSTRAINT pk_trainer_spells PRIMARY KEY (entry, cast_spell, learn_spell);


--
-- Name: pk_transport_data; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY transport_data
    ADD CONSTRAINT pk_transport_data PRIMARY KEY (entry);


--
-- Name: pk_unit_display_sizes; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY unit_display_sizes
    ADD CONSTRAINT pk_unit_display_sizes PRIMARY KEY (displayid);


--
-- Name: pk_vendor_restrictions; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY vendor_restrictions
    ADD CONSTRAINT pk_vendor_restrictions PRIMARY KEY (entry);


--
-- Name: pk_vendors; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY vendors
    ADD CONSTRAINT pk_vendors PRIMARY KEY (entry, item);


--
-- Name: pk_weather; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY weather
    ADD CONSTRAINT pk_weather PRIMARY KEY (zoneid);


--
-- Name: pk_worldbroadcast; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY worldbroadcast
    ADD CONSTRAINT pk_worldbroadcast PRIMARY KEY (entry);


--
-- Name: pk_worldbroadcast_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY worldbroadcast_localized
    ADD CONSTRAINT pk_worldbroadcast_localized PRIMARY KEY (entry);


--
-- Name: pk_worldmap_info; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY worldmap_info
    ADD CONSTRAINT pk_worldmap_info PRIMARY KEY (entry);


--
-- Name: pk_worldmap_info_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY worldmap_info_localized
    ADD CONSTRAINT pk_worldmap_info_localized PRIMARY KEY (entry);


--
-- Name: pk_worldstring_tables; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY worldstring_tables
    ADD CONSTRAINT pk_worldstring_tables PRIMARY KEY (entry);


--
-- Name: pk_worldstring_tables_localized; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY worldstring_tables_localized
    ADD CONSTRAINT pk_worldstring_tables_localized PRIMARY KEY (entry);


--
-- Name: ai_threattospellid_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX ai_threattospellid_a ON ai_threattospellid USING btree (spell);


--
-- Name: clientaddons_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX clientaddons_a ON clientaddons USING btree (id);


--
-- Name: loot_creatures_index; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX loot_creatures_index ON loot_creatures USING btree (index);


--
-- Name: loot_fishing_index; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX loot_fishing_index ON loot_fishing USING btree (index);


--
-- Name: loot_gameobjects_index; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX loot_gameobjects_index ON loot_gameobjects USING btree (index);


--
-- Name: loot_items_index; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX loot_items_index ON loot_items USING btree (index);


--
-- Name: loot_pickpocketing_index; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX loot_pickpocketing_index ON loot_pickpocketing USING btree (index);


--
-- Name: loot_skinning_index; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX loot_skinning_index ON loot_skinning USING btree (index);


--
-- Name: spelloverride_overrideid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX spelloverride_overrideid ON spelloverride USING btree (overrideid, spellid);


--
-- Name: trainerspelloverride_spellid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX trainerspelloverride_spellid ON trainerspelloverride USING btree (spellid);


--
-- Name: unit_display_sizes_displayid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX unit_display_sizes_displayid ON unit_display_sizes USING btree (displayid);


--
-- Name: unit_display_sizes_displayid_2; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX unit_display_sizes_displayid_2 ON unit_display_sizes USING btree (displayid);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

