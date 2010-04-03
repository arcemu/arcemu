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
-- Name: account_data; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE account_data (
    acct integer NOT NULL,
    uiconfig0 bytea,
    uiconfig1 bytea,
    uiconfig2 bytea,
    uiconfig3 bytea,
    uiconfig4 bytea,
    uiconfig5 bytea,
    uiconfig6 bytea,
    uiconfig7 bytea,
    uiconfig8 bytea
);


ALTER TABLE public.account_data OWNER TO arcemu;

--
-- Name: account_forced_permissions; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE account_forced_permissions (
    login text NOT NULL,
    permissions text NOT NULL
);


ALTER TABLE public.account_forced_permissions OWNER TO arcemu;

--
-- Name: arenateams; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE arenateams (
    id integer NOT NULL,
    type integer NOT NULL,
    leader integer NOT NULL,
    name text NOT NULL,
    emblemstyle integer NOT NULL,
    emblemcolour bigint NOT NULL,
    borderstyle integer NOT NULL,
    bordercolour bigint NOT NULL,
    backgroundcolour bigint NOT NULL,
    rating integer NOT NULL,
    data text NOT NULL,
    ranking integer NOT NULL,
    player_data1 text NOT NULL,
    player_data2 text NOT NULL,
    player_data3 text NOT NULL,
    player_data4 text NOT NULL,
    player_data5 text NOT NULL,
    player_data6 text NOT NULL,
    player_data7 text NOT NULL,
    player_data8 text NOT NULL,
    player_data9 text NOT NULL,
    player_data10 text NOT NULL
);


ALTER TABLE public.arenateams OWNER TO arcemu;

--
-- Name: auctions; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE auctions (
    auctionid integer NOT NULL,
    auctionhouse integer,
    item bigint,
    owner bigint,
    buyout integer,
    "time" integer,
    bidder bigint,
    bid integer,
    deposit integer
);


ALTER TABLE public.auctions OWNER TO arcemu;

--
-- Name: auctions_auctionid_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE auctions_auctionid_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.auctions_auctionid_seq OWNER TO arcemu;

--
-- Name: auctions_auctionid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE auctions_auctionid_seq OWNED BY auctions.auctionid;


--
-- Name: banned_names; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE banned_names (
    name text NOT NULL
);


ALTER TABLE public.banned_names OWNER TO arcemu;

--
-- Name: character_achievement; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE character_achievement (
    guid integer DEFAULT 0 NOT NULL,
    achievement integer DEFAULT 0 NOT NULL,
    date integer
);


ALTER TABLE public.character_achievement OWNER TO arcemu;

--
-- Name: character_achievement_progress; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE character_achievement_progress (
    guid integer DEFAULT 0 NOT NULL,
    criteria integer DEFAULT 0 NOT NULL,
    counter integer,
    date integer
);


ALTER TABLE public.character_achievement_progress OWNER TO arcemu;

--
-- Name: characters; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE characters (
    guid integer DEFAULT 0 NOT NULL,
    acct integer DEFAULT 0 NOT NULL,
    name text NOT NULL,
    race smallint NOT NULL,
    class smallint NOT NULL,
    gender numeric(3,0) NOT NULL,
    custom_faction integer DEFAULT 0 NOT NULL,
    level integer NOT NULL,
    xp integer NOT NULL,
    active_cheats integer DEFAULT 0 NOT NULL,
    exploration_data text NOT NULL,
    skills text NOT NULL,
    watched_faction_index bigint NOT NULL,
    selected_pvp_title integer NOT NULL,
    available_pvp_titles bigint DEFAULT 0 NOT NULL,
    available_pvp_titles1 bigint NOT NULL,
    gold integer NOT NULL,
    ammo_id integer NOT NULL,
    available_prof_points integer NOT NULL,
    current_hp integer NOT NULL,
    current_power integer NOT NULL,
    pvprank integer NOT NULL,
    bytes integer NOT NULL,
    bytes2 integer NOT NULL,
    player_flags integer NOT NULL,
    player_bytes integer NOT NULL,
    positionx real DEFAULT 0 NOT NULL,
    positiony real DEFAULT 0 NOT NULL,
    positionz real DEFAULT 0 NOT NULL,
    orientation real DEFAULT 0 NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    zoneid integer DEFAULT 0 NOT NULL,
    taximask text NOT NULL,
    banned integer DEFAULT 0 NOT NULL,
    banreason text NOT NULL,
    "timestamp" integer,
    online integer,
    bindpositionx real DEFAULT 0 NOT NULL,
    bindpositiony real DEFAULT 0 NOT NULL,
    bindpositionz real DEFAULT 0 NOT NULL,
    bindmapid integer DEFAULT 0 NOT NULL,
    bindzoneid integer DEFAULT 0 NOT NULL,
    isresting integer DEFAULT 0 NOT NULL,
    reststate integer DEFAULT 0 NOT NULL,
    resttime integer DEFAULT 0 NOT NULL,
    playedtime text NOT NULL,
    deathstate integer DEFAULT 0 NOT NULL,
    talentresettimes integer DEFAULT 0 NOT NULL,
    first_login numeric(3,0) DEFAULT 0 NOT NULL,
    forced_rename_pending numeric(3,0) DEFAULT 0 NOT NULL,
    arenapoints integer NOT NULL,
    totalstableslots integer DEFAULT 0 NOT NULL,
    instance_id integer NOT NULL,
    entrypointmap integer NOT NULL,
    entrypointx real NOT NULL,
    entrypointy real NOT NULL,
    entrypointz real NOT NULL,
    entrypointo real NOT NULL,
    entrypointinstance integer NOT NULL,
    taxi_path integer NOT NULL,
    taxi_lastnode integer NOT NULL,
    taxi_mountid integer NOT NULL,
    transporter integer NOT NULL,
    transporter_xdiff real NOT NULL,
    transporter_ydiff real NOT NULL,
    transporter_zdiff real NOT NULL,
    spells text NOT NULL,
    deleted_spells text NOT NULL,
    reputation text NOT NULL,
    actions1 text NOT NULL,
    actions2 text NOT NULL,
    auras text NOT NULL,
    finished_quests text NOT NULL,
    finisheddailies text NOT NULL,
    honorrollovertime integer DEFAULT 0 NOT NULL,
    killstoday integer DEFAULT 0 NOT NULL,
    killsyesterday integer DEFAULT 0 NOT NULL,
    killslifetime integer DEFAULT 0 NOT NULL,
    honortoday integer DEFAULT 0 NOT NULL,
    honoryesterday integer DEFAULT 0 NOT NULL,
    honorpoints integer DEFAULT 0 NOT NULL,
    difficulty integer DEFAULT 0 NOT NULL,
    drunkvalue integer DEFAULT 0 NOT NULL,
    glyphs1 text NOT NULL,
    talents1 text NOT NULL,
    glyphs2 text NOT NULL,
    talents2 text NOT NULL,
    numspecs integer DEFAULT 1 NOT NULL,
    currentspec integer DEFAULT 0 NOT NULL,
    talentpoint_override text NOT NULL,
    phase integer DEFAULT 1 NOT NULL,
    cangainxp integer DEFAULT 1 NOT NULL,
    data text
);


ALTER TABLE public.characters OWNER TO arcemu;

--
-- Name: characters_insert_queue; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE characters_insert_queue (
    insert_temp_guid integer DEFAULT 0 NOT NULL,
    acct integer DEFAULT 0 NOT NULL,
    name text NOT NULL,
    race numeric(3,0) DEFAULT 0 NOT NULL,
    class numeric(3,0) DEFAULT 0 NOT NULL,
    gender numeric(3,0) DEFAULT 0 NOT NULL,
    custom_faction integer DEFAULT 0 NOT NULL,
    level integer DEFAULT 0 NOT NULL,
    xp integer DEFAULT 0 NOT NULL,
    exploration_data text NOT NULL,
    skills text NOT NULL,
    watched_faction_index integer DEFAULT 0 NOT NULL,
    selected_pvp_title integer DEFAULT 0 NOT NULL,
    available_pvp_titles integer DEFAULT 0 NOT NULL,
    gold integer DEFAULT 0 NOT NULL,
    ammo_id integer DEFAULT 0 NOT NULL,
    available_prof_points integer DEFAULT 0 NOT NULL,
    available_talent_points integer DEFAULT 0 NOT NULL,
    current_hp integer DEFAULT 0 NOT NULL,
    current_power integer DEFAULT 0 NOT NULL,
    pvprank numeric(3,0) DEFAULT 0 NOT NULL,
    bytes integer DEFAULT 0 NOT NULL,
    bytes2 integer DEFAULT 0 NOT NULL,
    player_flags integer DEFAULT 0 NOT NULL,
    player_bytes integer DEFAULT 0 NOT NULL,
    positionx real DEFAULT 0 NOT NULL,
    positiony real DEFAULT 0 NOT NULL,
    positionz real DEFAULT 0 NOT NULL,
    orientation real DEFAULT 0 NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    zoneid integer DEFAULT 0 NOT NULL,
    taximask text NOT NULL,
    banned integer NOT NULL,
    banreason text NOT NULL,
    "timestamp" integer NOT NULL,
    online integer,
    bindpositionx real DEFAULT 0 NOT NULL,
    bindpositiony real DEFAULT 0 NOT NULL,
    bindpositionz real DEFAULT 0 NOT NULL,
    bindmapid integer DEFAULT 0 NOT NULL,
    bindzoneid integer DEFAULT 0 NOT NULL,
    isresting numeric(3,0) DEFAULT 0 NOT NULL,
    reststate numeric(3,0) DEFAULT 0 NOT NULL,
    resttime integer DEFAULT 0 NOT NULL,
    playedtime text NOT NULL,
    deathstate integer DEFAULT 0 NOT NULL,
    talentresettimes integer DEFAULT 0 NOT NULL,
    first_login numeric(3,0) DEFAULT 0 NOT NULL,
    forced_rename_pending numeric(3,0) DEFAULT 0 NOT NULL,
    publicnote text NOT NULL,
    officernote text NOT NULL,
    guildid integer DEFAULT 0 NOT NULL,
    guildrank integer DEFAULT 0 NOT NULL,
    arenapoints integer NOT NULL,
    totalstableslots integer DEFAULT 0 NOT NULL,
    instance_id integer DEFAULT 0 NOT NULL,
    entrypointmap integer DEFAULT 0 NOT NULL,
    entrypointx real DEFAULT 0 NOT NULL,
    entrypointy real DEFAULT 0 NOT NULL,
    entrypointz real DEFAULT 0 NOT NULL,
    entrypointo real DEFAULT 0 NOT NULL,
    entrypointinstance integer DEFAULT 0 NOT NULL,
    taxi_path integer DEFAULT 0 NOT NULL,
    taxi_lastnode integer DEFAULT 0 NOT NULL,
    taxi_mountid integer DEFAULT 0 NOT NULL,
    transporter integer DEFAULT 0 NOT NULL,
    transporter_xdiff real DEFAULT 0 NOT NULL,
    transporter_ydiff real DEFAULT 0 NOT NULL,
    transporter_zdiff real DEFAULT 0 NOT NULL,
    spells text NOT NULL,
    deleted_spells text NOT NULL,
    reputation text NOT NULL,
    actions text NOT NULL,
    auras text NOT NULL,
    finished_quests text NOT NULL,
    honorpointstoadd integer NOT NULL,
    killstoday integer DEFAULT 0 NOT NULL,
    killsyesterday integer DEFAULT 0 NOT NULL,
    killslifetime integer DEFAULT 0 NOT NULL,
    honortoday integer DEFAULT 0 NOT NULL,
    honoryesterday integer DEFAULT 0 NOT NULL,
    honorpoints integer DEFAULT 0 NOT NULL,
    difficulty integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.characters_insert_queue OWNER TO arcemu;

--
-- Name: charters; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE charters (
    charterid integer NOT NULL,
    chartertype integer DEFAULT 0 NOT NULL,
    leaderguid integer DEFAULT 0 NOT NULL,
    guildname text NOT NULL,
    itemguid bigint DEFAULT 0 NOT NULL,
    signer1 integer DEFAULT 0 NOT NULL,
    signer2 integer DEFAULT 0 NOT NULL,
    signer3 integer DEFAULT 0 NOT NULL,
    signer4 integer DEFAULT 0 NOT NULL,
    signer5 integer DEFAULT 0 NOT NULL,
    signer6 integer DEFAULT 0 NOT NULL,
    signer7 integer DEFAULT 0 NOT NULL,
    signer8 integer DEFAULT 0 NOT NULL,
    signer9 integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.charters OWNER TO arcemu;

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
-- Name: corpses; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE corpses (
    guid bigint DEFAULT 0 NOT NULL,
    positionx real DEFAULT 0 NOT NULL,
    positiony real DEFAULT 0 NOT NULL,
    positionz real DEFAULT 0 NOT NULL,
    orientation real DEFAULT 0 NOT NULL,
    zoneid integer DEFAULT 38 NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    instanceid integer DEFAULT 0 NOT NULL,
    data text NOT NULL
);


ALTER TABLE public.corpses OWNER TO arcemu;

--
-- Name: gm_tickets; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE gm_tickets (
    ticketid integer NOT NULL,
    playerguid integer DEFAULT 1 NOT NULL,
    name character varying(200) DEFAULT ''::character varying NOT NULL,
    level integer DEFAULT 1 NOT NULL,
    map integer DEFAULT 0 NOT NULL,
    posx double precision DEFAULT 0.0 NOT NULL,
    posy double precision DEFAULT 0.0 NOT NULL,
    posz double precision DEFAULT 0.0 NOT NULL,
    message text DEFAULT ''::text NOT NULL,
    "timestamp" text DEFAULT ''::text NOT NULL,
    deleted integer DEFAULT '0' NOT NULL,
    assignedto integer DEFAULT 1 NOT NULL,
    comment text DEFAULT ''::text NOT NULL
);


ALTER TABLE public.gm_tickets OWNER TO arcemu;

--
-- Name: gm_tickets_ticketid_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE gm_tickets_ticketid_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.gm_tickets_ticketid_seq OWNER TO arcemu;

--
-- Name: gm_tickets_ticketid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE gm_tickets_ticketid_seq OWNED BY gm_tickets.ticketid;


--
-- Name: groups; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE groups (
    group_id integer NOT NULL,
    group_type numeric(3,0) NOT NULL,
    subgroup_count numeric(3,0) NOT NULL,
    loot_method numeric(3,0) NOT NULL,
    loot_threshold numeric(3,0) NOT NULL,
    difficulty integer DEFAULT 0 NOT NULL,
    raiddifficulty integer DEFAULT 0 NOT NULL,
    assistant_leader integer DEFAULT 0 NOT NULL,
    main_tank integer DEFAULT 0 NOT NULL,
    main_assist integer DEFAULT 0 NOT NULL,
    group1member1 integer NOT NULL,
    group1member2 integer NOT NULL,
    group1member3 integer NOT NULL,
    group1member4 integer NOT NULL,
    group1member5 integer NOT NULL,
    group2member1 integer NOT NULL,
    group2member2 integer NOT NULL,
    group2member3 integer NOT NULL,
    group2member4 integer NOT NULL,
    group2member5 integer NOT NULL,
    group3member1 integer NOT NULL,
    group3member2 integer NOT NULL,
    group3member3 integer NOT NULL,
    group3member4 integer NOT NULL,
    group3member5 integer NOT NULL,
    group4member1 integer NOT NULL,
    group4member2 integer NOT NULL,
    group4member3 integer NOT NULL,
    group4member4 integer NOT NULL,
    group4member5 integer NOT NULL,
    group5member1 integer NOT NULL,
    group5member2 integer NOT NULL,
    group5member3 integer NOT NULL,
    group5member4 integer NOT NULL,
    group5member5 integer NOT NULL,
    group6member1 integer NOT NULL,
    group6member2 integer NOT NULL,
    group6member3 integer NOT NULL,
    group6member4 integer NOT NULL,
    group6member5 integer NOT NULL,
    group7member1 integer NOT NULL,
    group7member2 integer NOT NULL,
    group7member3 integer NOT NULL,
    group7member4 integer NOT NULL,
    group7member5 integer NOT NULL,
    group8member1 integer NOT NULL,
    group8member2 integer NOT NULL,
    group8member3 integer NOT NULL,
    group8member4 integer NOT NULL,
    group8member5 integer NOT NULL,
    "timestamp" integer NOT NULL,
    instanceids text NOT NULL
);


ALTER TABLE public.groups OWNER TO arcemu;

--
-- Name: guild_bankitems; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE guild_bankitems (
    guildid integer NOT NULL,
    tabid integer NOT NULL,
    slotid integer NOT NULL,
    itemguid integer NOT NULL
);


ALTER TABLE public.guild_bankitems OWNER TO arcemu;

--
-- Name: guild_banklogs; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE guild_banklogs (
    log_id integer NOT NULL,
    guildid integer NOT NULL,
    tabid integer NOT NULL,
    action integer NOT NULL,
    player_guid integer NOT NULL,
    item_entry integer NOT NULL,
    stack_count integer NOT NULL,
    "timestamp" integer NOT NULL
);


ALTER TABLE public.guild_banklogs OWNER TO arcemu;

--
-- Name: guild_banktabs; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE guild_banktabs (
    guildid integer NOT NULL,
    tabid integer NOT NULL,
    tabname text NOT NULL,
    tabicon text NOT NULL,
    tabinfo text NOT NULL
);


ALTER TABLE public.guild_banktabs OWNER TO arcemu;

--
-- Name: guild_data; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE guild_data (
    guildid integer NOT NULL,
    playerid integer NOT NULL,
    guildrank integer NOT NULL,
    publicnote text NOT NULL,
    officernote text NOT NULL,
    lastwithdrawreset integer DEFAULT 0 NOT NULL,
    withdrawlssincelastreset integer DEFAULT 0 NOT NULL,
    lastitemwithdrawreset0 integer DEFAULT 0 NOT NULL,
    itemwithdrawlssincelastreset0 integer DEFAULT 0 NOT NULL,
    lastitemwithdrawreset1 integer NOT NULL,
    itemwithdrawlssincelastreset1 integer NOT NULL,
    lastitemwithdrawreset2 integer NOT NULL,
    itemwithdrawlssincelastreset2 integer NOT NULL,
    lastitemwithdrawreset3 integer NOT NULL,
    itemwithdrawlssincelastreset3 integer NOT NULL,
    lastitemwithdrawreset4 integer NOT NULL,
    itemwithdrawlssincelastreset4 integer NOT NULL,
    lastitemwithdrawreset5 integer NOT NULL,
    itemwithdrawlssincelastreset5 integer NOT NULL
);


ALTER TABLE public.guild_data OWNER TO arcemu;

--
-- Name: guild_logs; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE guild_logs (
    log_id integer NOT NULL,
    guildid integer NOT NULL,
    "timestamp" integer NOT NULL,
    event_type integer NOT NULL,
    misc1 integer NOT NULL,
    misc2 integer NOT NULL,
    misc3 integer NOT NULL
);


ALTER TABLE public.guild_logs OWNER TO arcemu;

--
-- Name: guild_ranks; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE guild_ranks (
    guildid integer DEFAULT 0 NOT NULL,
    rankid integer DEFAULT 0 NOT NULL,
    rankname text NOT NULL,
    rankrights integer DEFAULT 0 NOT NULL,
    goldlimitperday integer DEFAULT 0 NOT NULL,
    banktabflags0 integer DEFAULT 0 NOT NULL,
    itemstacksperday0 integer DEFAULT 0 NOT NULL,
    banktabflags1 integer DEFAULT 0 NOT NULL,
    itemstacksperday1 integer DEFAULT 0 NOT NULL,
    banktabflags2 integer DEFAULT 0 NOT NULL,
    itemstacksperday2 integer DEFAULT 0 NOT NULL,
    banktabflags3 integer DEFAULT 0 NOT NULL,
    itemstacksperday3 integer DEFAULT 0 NOT NULL,
    banktabflags4 integer DEFAULT 0 NOT NULL,
    itemstacksperday4 integer DEFAULT 0 NOT NULL,
    banktabflags5 integer DEFAULT 0 NOT NULL,
    itemstacksperday5 integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.guild_ranks OWNER TO arcemu;

--
-- Name: guilds; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE guilds (
    guildid bigint NOT NULL,
    guildname text NOT NULL,
    leaderguid bigint DEFAULT 0 NOT NULL,
    emblemstyle integer DEFAULT 0 NOT NULL,
    emblemcolor integer DEFAULT 0 NOT NULL,
    borderstyle integer DEFAULT 0 NOT NULL,
    bordercolor integer DEFAULT 0 NOT NULL,
    backgroundcolor integer DEFAULT 0 NOT NULL,
    guildinfo text NOT NULL,
    motd text NOT NULL,
    createdate integer NOT NULL,
    bankbalance bigint NOT NULL
);


ALTER TABLE public.guilds OWNER TO arcemu;

--
-- Name: guilds_guildid_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE guilds_guildid_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.guilds_guildid_seq OWNER TO arcemu;

--
-- Name: guilds_guildid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE guilds_guildid_seq OWNED BY guilds.guildid;


--
-- Name: instanceids; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE instanceids (
    playerguid integer DEFAULT 0 NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    mode integer DEFAULT 0 NOT NULL,
    instanceid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.instanceids OWNER TO arcemu;

--
-- Name: instances; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE instances (
    id integer NOT NULL,
    mapid integer NOT NULL,
    creation integer NOT NULL,
    expiration integer NOT NULL,
    killed_npc_guids text NOT NULL,
    difficulty integer NOT NULL,
    creator_group integer NOT NULL,
    creator_guid integer NOT NULL,
    persistent numeric(3,0) DEFAULT 0 NOT NULL
);


ALTER TABLE public.instances OWNER TO arcemu;

--
-- Name: mailbox; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE mailbox (
    message_id integer DEFAULT 0 NOT NULL,
    message_type integer DEFAULT 0 NOT NULL,
    player_guid integer DEFAULT 0 NOT NULL,
    sender_guid integer DEFAULT 0 NOT NULL,
    subject text NOT NULL,
    body text NOT NULL,
    money integer DEFAULT 0 NOT NULL,
    attached_item_guids text NOT NULL,
    cod integer DEFAULT 0 NOT NULL,
    stationary integer DEFAULT 0 NOT NULL,
    expiry_time integer DEFAULT 0 NOT NULL,
    delivery_time integer DEFAULT 0 NOT NULL,
    copy_made integer DEFAULT 0 NOT NULL,
    read_flag integer DEFAULT 0 NOT NULL,
    deleted_flag integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.mailbox OWNER TO arcemu;

--
-- Name: mailbox_insert_queue; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE mailbox_insert_queue (
    sender_guid integer NOT NULL,
    receiver_guid integer NOT NULL,
    subject text NOT NULL,
    body text NOT NULL,
    stationary integer NOT NULL,
    money integer NOT NULL,
    item_id integer NOT NULL,
    item_stack integer NOT NULL
);


ALTER TABLE public.mailbox_insert_queue OWNER TO arcemu;

--
-- Name: playercooldowns; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playercooldowns (
    player_guid integer NOT NULL,
    cooldown_type integer NOT NULL,
    cooldown_misc integer NOT NULL,
    cooldown_expire_time integer NOT NULL,
    cooldown_spellid integer NOT NULL,
    cooldown_itemid integer NOT NULL
);


ALTER TABLE public.playercooldowns OWNER TO arcemu;

--
-- Name: playeritems; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playeritems (
    ownerguid integer DEFAULT 0 NOT NULL,
    guid bigint NOT NULL,
    entry integer DEFAULT 0 NOT NULL,
    wrapped_item_id integer DEFAULT 0 NOT NULL,
    wrapped_creator integer DEFAULT 0 NOT NULL,
    creator integer DEFAULT 0 NOT NULL,
    count integer DEFAULT 0 NOT NULL,
    charges integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    randomprop integer DEFAULT 0 NOT NULL,
    randomsuffix integer NOT NULL,
    itemtext integer DEFAULT 0 NOT NULL,
    durability integer DEFAULT 0 NOT NULL,
    containerslot integer DEFAULT (-1),
    slot integer DEFAULT 0 NOT NULL,
    enchantments text NOT NULL,
    duration_expireson integer DEFAULT 0 NOT NULL,
    refund_purchasedon integer DEFAULT 0 NOT NULL,
    refund_costid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.playeritems OWNER TO arcemu;

--
-- Name: playeritems_guid_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE playeritems_guid_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.playeritems_guid_seq OWNER TO arcemu;

--
-- Name: playeritems_guid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE playeritems_guid_seq OWNED BY playeritems.guid;


--
-- Name: playeritems_insert_queue; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playeritems_insert_queue (
    ownerguid integer DEFAULT 0 NOT NULL,
    entry integer DEFAULT 0 NOT NULL,
    wrapped_item_id integer DEFAULT 0 NOT NULL,
    wrapped_creator integer DEFAULT 0 NOT NULL,
    creator integer DEFAULT 0 NOT NULL,
    count integer DEFAULT 0 NOT NULL,
    charges integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    randomprop integer DEFAULT 0 NOT NULL,
    randomsuffix integer NOT NULL,
    itemtext integer DEFAULT 0 NOT NULL,
    durability integer DEFAULT 0 NOT NULL,
    containerslot integer DEFAULT (-1) NOT NULL,
    slot numeric(3,0) DEFAULT 0 NOT NULL,
    enchantments text NOT NULL
);


ALTER TABLE public.playeritems_insert_queue OWNER TO arcemu;

--
-- Name: playerpets; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playerpets (
    ownerguid bigint DEFAULT 0 NOT NULL,
    petnumber integer DEFAULT 0 NOT NULL,
    name text NOT NULL,
    entry integer DEFAULT 0 NOT NULL,
    fields text NOT NULL,
    xp integer DEFAULT 0 NOT NULL,
    active numeric(3,0) DEFAULT 0 NOT NULL,
    level integer DEFAULT 0 NOT NULL,
    actionbar text NOT NULL,
    happinessupdate integer DEFAULT 0 NOT NULL,
    summon integer DEFAULT 0 NOT NULL,
    reset_time integer DEFAULT 0 NOT NULL,
    reset_cost integer DEFAULT 0 NOT NULL,
    spellid integer DEFAULT 0 NOT NULL,
    petstate integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.playerpets OWNER TO arcemu;

--
-- Name: playerpetspells; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playerpetspells (
    ownerguid bigint DEFAULT 0 NOT NULL,
    petnumber integer DEFAULT 0 NOT NULL,
    spellid integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.playerpetspells OWNER TO arcemu;

--
-- Name: playersummons; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playersummons (
    ownerguid integer DEFAULT 0 NOT NULL,
    entry integer DEFAULT 0 NOT NULL,
    name text NOT NULL
);


ALTER TABLE public.playersummons OWNER TO arcemu;

--
-- Name: playersummonspells; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE playersummonspells (
    ownerguid bigint DEFAULT 0 NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    spellid integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.playersummonspells OWNER TO arcemu;

--
-- Name: questlog; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE questlog (
    player_guid bigint DEFAULT 0 NOT NULL,
    quest_id bigint DEFAULT 0 NOT NULL,
    slot integer DEFAULT 0 NOT NULL,
    time_left bigint DEFAULT 0 NOT NULL,
    explored_area1 bigint DEFAULT 0 NOT NULL,
    explored_area2 bigint DEFAULT 0 NOT NULL,
    explored_area3 bigint DEFAULT 0 NOT NULL,
    explored_area4 bigint DEFAULT 0 NOT NULL,
    mob_kill1 bigint DEFAULT 0 NOT NULL,
    mob_kill2 bigint DEFAULT 0 NOT NULL,
    mob_kill3 bigint DEFAULT 0 NOT NULL,
    mob_kill4 bigint DEFAULT 0 NOT NULL,
    completed integer DEFAULT 0 NOT NULL
);


ALTER TABLE public.questlog OWNER TO arcemu;

--
-- Name: server_settings; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE server_settings (
    setting_id text NOT NULL,
    setting_value integer NOT NULL
);


ALTER TABLE public.server_settings OWNER TO arcemu;

--
-- Name: social_friends; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE social_friends (
    character_guid integer NOT NULL,
    friend_guid integer NOT NULL,
    note text NOT NULL
);


ALTER TABLE public.social_friends OWNER TO arcemu;

--
-- Name: social_ignores; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE social_ignores (
    character_guid integer NOT NULL,
    ignore_guid integer NOT NULL
);


ALTER TABLE public.social_ignores OWNER TO arcemu;

--
-- Name: tutorials; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE tutorials (
    playerid bigint DEFAULT 0 NOT NULL,
    tut0 bigint DEFAULT 0 NOT NULL,
    tut1 bigint DEFAULT 0 NOT NULL,
    tut2 bigint DEFAULT 0 NOT NULL,
    tut3 bigint DEFAULT 0 NOT NULL,
    tut4 bigint DEFAULT 0 NOT NULL,
    tut5 bigint DEFAULT 0 NOT NULL,
    tut6 bigint DEFAULT 0 NOT NULL,
    tut7 bigint DEFAULT 0 NOT NULL
);


ALTER TABLE public.tutorials OWNER TO arcemu;

--
-- Name: auctionid; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE auctions ALTER COLUMN auctionid SET DEFAULT nextval('auctions_auctionid_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE clientaddons ALTER COLUMN id SET DEFAULT nextval('clientaddons_id_seq'::regclass);


--
-- Name: ticketid; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE gm_tickets ALTER COLUMN ticketid SET DEFAULT nextval('gm_tickets_ticketid_seq'::regclass);


--
-- Name: guildid; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE guilds ALTER COLUMN guildid SET DEFAULT nextval('guilds_guildid_seq'::regclass);


--
-- Name: guid; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE playeritems ALTER COLUMN guid SET DEFAULT nextval('playeritems_guid_seq'::regclass);


--
-- Name: pk_account_data; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY account_data
    ADD CONSTRAINT pk_account_data PRIMARY KEY (acct);


--
-- Name: pk_account_forced_permissions; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY account_forced_permissions
    ADD CONSTRAINT pk_account_forced_permissions PRIMARY KEY (login);


--
-- Name: pk_arenateams; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY arenateams
    ADD CONSTRAINT pk_arenateams PRIMARY KEY (id);


--
-- Name: pk_auctions; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY auctions
    ADD CONSTRAINT pk_auctions PRIMARY KEY (auctionid);


--
-- Name: pk_character_achievement; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY character_achievement
    ADD CONSTRAINT pk_character_achievement PRIMARY KEY (guid, achievement);


--
-- Name: pk_character_achievement_progress; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY character_achievement_progress
    ADD CONSTRAINT pk_character_achievement_progress PRIMARY KEY (guid, criteria);


--
-- Name: pk_characters; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY characters
    ADD CONSTRAINT pk_characters PRIMARY KEY (guid);


--
-- Name: pk_charters; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY charters
    ADD CONSTRAINT pk_charters PRIMARY KEY (charterid);


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
-- Name: pk_corpses; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY corpses
    ADD CONSTRAINT pk_corpses PRIMARY KEY (guid);


--
-- Name: pk_groups; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY groups
    ADD CONSTRAINT pk_groups PRIMARY KEY (group_id);


--
-- Name: pk_guild_bankitems; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY guild_bankitems
    ADD CONSTRAINT pk_guild_bankitems PRIMARY KEY (guildid, tabid, slotid);


--
-- Name: pk_guild_banklogs; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY guild_banklogs
    ADD CONSTRAINT pk_guild_banklogs PRIMARY KEY (log_id, guildid);


--
-- Name: pk_guild_banktabs; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY guild_banktabs
    ADD CONSTRAINT pk_guild_banktabs PRIMARY KEY (guildid, tabid);


--
-- Name: pk_guild_logs; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY guild_logs
    ADD CONSTRAINT pk_guild_logs PRIMARY KEY (log_id, guildid);


--
-- Name: pk_guild_ranks; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY guild_ranks
    ADD CONSTRAINT pk_guild_ranks PRIMARY KEY (guildid, rankid);


--
-- Name: pk_guilds; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY guilds
    ADD CONSTRAINT pk_guilds PRIMARY KEY (guildid);


--
-- Name: pk_instanceids; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY instanceids
    ADD CONSTRAINT pk_instanceids PRIMARY KEY (playerguid, mapid, mode);


--
-- Name: pk_instances; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY instances
    ADD CONSTRAINT pk_instances PRIMARY KEY (id);


--
-- Name: pk_mailbox; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY mailbox
    ADD CONSTRAINT pk_mailbox PRIMARY KEY (message_id);


--
-- Name: pk_playeritems; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY playeritems
    ADD CONSTRAINT pk_playeritems PRIMARY KEY (guid);


--
-- Name: pk_playerpets; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY playerpets
    ADD CONSTRAINT pk_playerpets PRIMARY KEY (ownerguid, petnumber);


--
-- Name: pk_questlog; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY questlog
    ADD CONSTRAINT pk_questlog PRIMARY KEY (player_guid, quest_id);


--
-- Name: pk_server_settings; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY server_settings
    ADD CONSTRAINT pk_server_settings PRIMARY KEY (setting_id);


--
-- Name: pk_social_friends; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY social_friends
    ADD CONSTRAINT pk_social_friends PRIMARY KEY (character_guid, friend_guid);


--
-- Name: pk_social_ignores; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY social_ignores
    ADD CONSTRAINT pk_social_ignores PRIMARY KEY (character_guid, ignore_guid);


--
-- Name: pk_tutorials; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY tutorials
    ADD CONSTRAINT pk_tutorials PRIMARY KEY (playerid);


--
-- Name: ticketid; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY gm_tickets
    ADD CONSTRAINT ticketid PRIMARY KEY (ticketid);


--
-- Name: auctions_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX auctions_b ON auctions USING btree (auctionhouse);


--
-- Name: characters_acct; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX characters_acct ON characters USING btree (acct);


--
-- Name: characters_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX characters_b ON characters USING btree (banned);


--
-- Name: characters_c; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX characters_c ON characters USING btree (online);


--
-- Name: characters_d; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX characters_d ON characters USING btree (forced_rename_pending);


--
-- Name: characters_insert_queue_acct; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX characters_insert_queue_acct ON characters_insert_queue USING btree (acct);


--
-- Name: characters_insert_queue_guid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX characters_insert_queue_guid ON characters_insert_queue USING btree (insert_temp_guid);


--
-- Name: characters_insert_queue_guildid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX characters_insert_queue_guildid ON characters_insert_queue USING btree (guildid);


--
-- Name: characters_name; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX characters_name ON characters USING btree (name);


--
-- Name: charters_leaderguid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX charters_leaderguid ON charters USING btree (leaderguid);


--
-- Name: clientaddons_index; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX clientaddons_index ON clientaddons USING btree (name);


--
-- Name: corpses_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX corpses_b ON corpses USING btree (instanceid);


--
-- Name: guild_bankitems_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_bankitems_a ON guild_bankitems USING btree (guildid);


--
-- Name: guild_bankitems_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_bankitems_b ON guild_bankitems USING btree (tabid);


--
-- Name: guild_bankitems_c; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_bankitems_c ON guild_bankitems USING btree (slotid);


--
-- Name: guild_banklogs_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_banklogs_a ON guild_banklogs USING btree (guildid);


--
-- Name: guild_banklogs_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_banklogs_b ON guild_banklogs USING btree (tabid);


--
-- Name: guild_banktabs_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_banktabs_a ON guild_banktabs USING btree (guildid);


--
-- Name: guild_banktabs_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_banktabs_b ON guild_banktabs USING btree (tabid);


--
-- Name: guild_data_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_data_a ON guild_data USING btree (guildid);


--
-- Name: guild_data_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX guild_data_b ON guild_data USING btree (playerid);


--
-- Name: guilds_guildid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX guilds_guildid ON guilds USING btree (guildid);


--
-- Name: instanceids_ix_instanceid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX instanceids_ix_instanceid ON instanceids USING btree (playerguid);


--
-- Name: instances_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX instances_a ON instances USING btree (mapid);


--
-- Name: mailbox_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX mailbox_b ON mailbox USING btree (player_guid);


--
-- Name: playeritems_insert_queue_ownerguid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX playeritems_insert_queue_ownerguid ON playeritems_insert_queue USING btree (ownerguid);


--
-- Name: playeritems_itemtext; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX playeritems_itemtext ON playeritems USING btree (itemtext);


--
-- Name: playeritems_ownerguid; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX playeritems_ownerguid ON playeritems USING btree (ownerguid);


--
-- Name: playerpetspells_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX playerpetspells_a ON playerpetspells USING btree (ownerguid);


--
-- Name: playerpetspells_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX playerpetspells_b ON playerpetspells USING btree (petnumber);


--
-- Name: playersummons_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX playersummons_a ON playersummons USING btree (ownerguid);


--
-- Name: playersummonspells_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX playersummonspells_a ON playersummonspells USING btree (ownerguid);


--
-- Name: social_friends_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX social_friends_a ON social_friends USING btree (character_guid);


--
-- Name: social_friends_b; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX social_friends_b ON social_friends USING btree (friend_guid);


--
-- Name: social_ignores_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE INDEX social_ignores_a ON social_ignores USING btree (character_guid);


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

