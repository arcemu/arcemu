--
-- PostgreSQL database dump
--

-- Started on 2009-12-15 21:45:48

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- TOC entry 514 (class 2612 OID 16386)
-- Name: plpgsql; Type: PROCEDURAL LANGUAGE; Schema: -; Owner: -
--

CREATE PROCEDURAL LANGUAGE plpgsql;


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 1701 (class 1259 OID 35308)
-- Dependencies: 2084 2085 2086 2087 2088 2089 2090 2091 2092 2093 2094 2095 2096 2097 2098 2099 2100 2101 2102 2103 2104 3
-- Name: ai_agents; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE ai_agents (
    entry integer DEFAULT 0 NOT NULL,
    instance_mode integer DEFAULT 4 NOT NULL,
    type smallint DEFAULT 0::smallint NOT NULL,
    event integer DEFAULT 0 NOT NULL,
    chance integer DEFAULT 0 NOT NULL,
    maxcount integer DEFAULT 0 NOT NULL,
    spell integer DEFAULT 0 NOT NULL,
    spelltype integer DEFAULT 0 NOT NULL,
    targettype integer DEFAULT 0 NOT NULL,
    cooldown integer DEFAULT 0 NOT NULL,
    floatmisc1 double precision DEFAULT 0::double precision NOT NULL,
    misc2 integer DEFAULT 0 NOT NULL,
    CONSTRAINT ai_agents_chance_check CHECK ((chance >= 0)),
    CONSTRAINT ai_agents_entry_check CHECK ((entry >= 0)),
    CONSTRAINT ai_agents_event_check CHECK ((event >= 0)),
    CONSTRAINT ai_agents_instance_mode_check CHECK ((instance_mode >= 0)),
    CONSTRAINT ai_agents_maxcount_check CHECK ((maxcount >= 0)),
    CONSTRAINT ai_agents_misc2_check CHECK ((misc2 >= 0)),
    CONSTRAINT ai_agents_spell_check CHECK ((spell >= 0)),
    CONSTRAINT ai_agents_spelltype_check CHECK ((spelltype >= 0)),
    CONSTRAINT ai_agents_type_check CHECK ((type >= 0))
);


--
-- TOC entry 3503 (class 0 OID 0)
-- Dependencies: 1701
-- Name: TABLE ai_agents; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE ai_agents IS 'AI System';


--
-- TOC entry 1702 (class 1259 OID 35334)
-- Dependencies: 2105 2106 2107 2108 3
-- Name: ai_threattospellid; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE ai_threattospellid (
    spell integer DEFAULT 0 NOT NULL,
    mod integer DEFAULT 0 NOT NULL,
    modcoef double precision DEFAULT 1::double precision NOT NULL,
    CONSTRAINT ai_threattospellid_spell_check CHECK ((spell >= 0))
);


--
-- TOC entry 3504 (class 0 OID 0)
-- Dependencies: 1702
-- Name: TABLE ai_threattospellid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE ai_threattospellid IS 'AI System';


--
-- TOC entry 1703 (class 1259 OID 35343)
-- Dependencies: 2109 2110 2111 2112 2113 2114 2115 2116 2117 2118 2119 2120 2121 2122 2123 3
-- Name: areatriggers; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE areatriggers (
    entry integer DEFAULT 0 NOT NULL,
    type smallint DEFAULT 0::smallint,
    map integer,
    screen integer,
    name character varying(100) DEFAULT '0'::character varying,
    position_x double precision DEFAULT 0::double precision NOT NULL,
    position_y double precision DEFAULT 0::double precision NOT NULL,
    position_z double precision DEFAULT 0::double precision NOT NULL,
    orientation double precision DEFAULT 0::double precision NOT NULL,
    required_honor_rank integer DEFAULT 0 NOT NULL,
    required_level smallint DEFAULT 0::smallint NOT NULL,
    CONSTRAINT areatriggers_entry_check CHECK ((entry >= 0)),
    CONSTRAINT areatriggers_map_check CHECK ((map >= 0)),
    CONSTRAINT areatriggers_required_honor_rank_check CHECK ((required_honor_rank >= 0)),
    CONSTRAINT areatriggers_required_level_check CHECK ((required_level >= 0)),
    CONSTRAINT areatriggers_screen_check CHECK ((screen >= 0)),
    CONSTRAINT areatriggers_type_check CHECK ((type >= 0))
);


--
-- TOC entry 3505 (class 0 OID 0)
-- Dependencies: 1703
-- Name: TABLE areatriggers; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE areatriggers IS 'Trigger System';


--
-- TOC entry 1704 (class 1259 OID 35363)
-- Dependencies: 3
-- Name: auctionhouse_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE auctionhouse_id_seq
    START WITH 39
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1705 (class 1259 OID 35365)
-- Dependencies: 2124 2125 2126 2127 3
-- Name: auctionhouse; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE auctionhouse (
    id integer DEFAULT nextval('auctionhouse_id_seq'::regclass) NOT NULL,
    creature_entry bigint DEFAULT 0::bigint NOT NULL,
    ahgroup integer DEFAULT 0 NOT NULL,
    CONSTRAINT auctionhouse_creature_entry_check CHECK ((creature_entry >= 0))
);


--
-- TOC entry 3506 (class 0 OID 0)
-- Dependencies: 1705
-- Name: TABLE auctionhouse; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE auctionhouse IS 'Auction House';


--
-- TOC entry 1706 (class 1259 OID 35374)
-- Dependencies: 3
-- Name: banned_phrases; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE banned_phrases (
    phrase character varying(250) NOT NULL
);


--
-- TOC entry 1707 (class 1259 OID 35379)
-- Dependencies: 3
-- Name: clientaddons_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE clientaddons_id_seq
    START WITH 153
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1708 (class 1259 OID 35381)
-- Dependencies: 2128 2129 2130 2131 2132 2133 2134 3
-- Name: clientaddons; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE clientaddons (
    id integer DEFAULT nextval('clientaddons_id_seq'::regclass) NOT NULL,
    name character varying(50) DEFAULT NULL::character varying,
    crc bigint,
    banned integer DEFAULT 0 NOT NULL,
    showinlist integer DEFAULT 0 NOT NULL,
    CONSTRAINT clientaddons_banned_check CHECK ((banned >= 0)),
    CONSTRAINT clientaddons_crc_check CHECK ((crc >= 0)),
    CONSTRAINT clientaddons_showinlist_check CHECK ((showinlist >= 0))
);


--
-- TOC entry 3507 (class 0 OID 0)
-- Dependencies: 1708
-- Name: TABLE clientaddons; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE clientaddons IS 'Client Addons';


--
-- TOC entry 1709 (class 1259 OID 35393)
-- Dependencies: 3
-- Name: command_overrides; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE command_overrides (
    command_name character varying(100) NOT NULL,
    access_level character varying(10) NOT NULL
);


--
-- TOC entry 3508 (class 0 OID 0)
-- Dependencies: 1709
-- Name: TABLE command_overrides; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE command_overrides IS 'Commands System';


--
-- TOC entry 1710 (class 1259 OID 35398)
-- Dependencies: 2135 2136 2137 2138 2139 2140 3
-- Name: creature_formations; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_formations (
    spawn_id integer DEFAULT 0 NOT NULL,
    target_spawn_id integer DEFAULT 0 NOT NULL,
    follow_angle double precision DEFAULT 0::double precision NOT NULL,
    follow_dist double precision DEFAULT 0::double precision NOT NULL,
    CONSTRAINT creature_formations_spawn_id_check CHECK ((spawn_id >= 0)),
    CONSTRAINT creature_formations_target_spawn_id_check CHECK ((target_spawn_id >= 0))
);


--
-- TOC entry 3509 (class 0 OID 0)
-- Dependencies: 1710
-- Name: TABLE creature_formations; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_formations IS 'Creature System';


--
-- TOC entry 1711 (class 1259 OID 35409)
-- Dependencies: 2141 2142 2143 2144 2145 2146 2147 2148 2149 2150 2151 2152 2153 2154 2155 2156 2157 2158 2159 2160 2161 2162 2163 2164 2165 2166 2167 2168 2169 2170 2171 2172 2173 2174 2175 2176 2177 2178 2179 2180 3
-- Name: creature_names; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_names (
    entry integer DEFAULT 0 NOT NULL,
    name character varying(100) NOT NULL,
    subname character varying(100) NOT NULL,
    info_str character varying(500) NOT NULL,
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
    unknown_float1 double precision DEFAULT 1::double precision NOT NULL,
    unknown_float2 double precision DEFAULT 1::double precision NOT NULL,
    leader smallint DEFAULT 0::smallint NOT NULL,
    questitem1 integer DEFAULT 0 NOT NULL,
    questitem2 integer DEFAULT 0 NOT NULL,
    questitem3 integer DEFAULT 0 NOT NULL,
    questitem4 integer DEFAULT 0 NOT NULL,
    questitem5 integer DEFAULT 0 NOT NULL,
    questitem6 integer DEFAULT 0 NOT NULL,
    waypointid integer DEFAULT 0 NOT NULL,
    CONSTRAINT creature_names_entry_check CHECK ((entry >= 0)),
    CONSTRAINT creature_names_family_check CHECK ((family >= 0)),
    CONSTRAINT creature_names_female_displayid2_check CHECK ((female_displayid2 >= 0)),
    CONSTRAINT creature_names_female_displayid_check CHECK ((female_displayid >= 0)),
    CONSTRAINT creature_names_flags1_check CHECK ((flags1 >= 0)),
    CONSTRAINT creature_names_killcredit1_check CHECK ((killcredit1 >= 0)),
    CONSTRAINT creature_names_killcredit2_check CHECK ((killcredit2 >= 0)),
    CONSTRAINT creature_names_leader_check CHECK ((leader >= 0)),
    CONSTRAINT creature_names_male_displayid2_check CHECK ((male_displayid2 >= 0)),
    CONSTRAINT creature_names_male_displayid_check CHECK ((male_displayid >= 0)),
    CONSTRAINT creature_names_questitem1_check CHECK ((questitem1 >= 0)),
    CONSTRAINT creature_names_questitem2_check CHECK ((questitem2 >= 0)),
    CONSTRAINT creature_names_questitem3_check CHECK ((questitem3 >= 0)),
    CONSTRAINT creature_names_questitem4_check CHECK ((questitem4 >= 0)),
    CONSTRAINT creature_names_questitem5_check CHECK ((questitem5 >= 0)),
    CONSTRAINT creature_names_questitem6_check CHECK ((questitem6 >= 0)),
    CONSTRAINT creature_names_rank_check CHECK ((rank >= 0)),
    CONSTRAINT creature_names_type_check CHECK ((type >= 0)),
    CONSTRAINT creature_names_waypointid_check CHECK ((waypointid >= 0))
);


--
-- TOC entry 3510 (class 0 OID 0)
-- Dependencies: 1711
-- Name: TABLE creature_names; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_names IS 'Creature System';


--
-- TOC entry 1712 (class 1259 OID 35457)
-- Dependencies: 2181 3
-- Name: creature_names_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_names_localized (
    id integer NOT NULL,
    language_code character varying(5) NOT NULL,
    name character varying(100) NOT NULL,
    subname character varying(100) NOT NULL,
    CONSTRAINT creature_names_localized_id_check CHECK ((id >= 0))
);


--
-- TOC entry 1713 (class 1259 OID 35463)
-- Dependencies: 2182 2183 2184 2185 2186 2187 2188 2189 2190 2191 2192 2193 2194 2195 2196 2197 2198 2199 2200 2201 2202 2203 2204 2205 2206 2207 2208 2209 2210 2211 2212 2213 2214 2215 2216 2217 2218 2219 2220 2221 2222 2223 2224 2225 2226 2227 2228 2229 2230 2231 2232 2233 2234 2235 2236 2237 2238 2239 2240 2241 2242 2243 2244 2245 2246 2247 2248 2249 2250 2251 2252 2253 3
-- Name: creature_proto; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_proto (
    entry integer DEFAULT 0 NOT NULL,
    minlevel integer DEFAULT 1 NOT NULL,
    maxlevel integer DEFAULT 1 NOT NULL,
    faction integer DEFAULT 35 NOT NULL,
    minhealth integer DEFAULT 1 NOT NULL,
    maxhealth integer DEFAULT 1 NOT NULL,
    mana integer DEFAULT 0 NOT NULL,
    scale double precision DEFAULT 1::double precision NOT NULL,
    npcflags integer DEFAULT 1 NOT NULL,
    attacktime integer DEFAULT 1000 NOT NULL,
    attacktype integer DEFAULT 0 NOT NULL,
    mindamage double precision DEFAULT 1::double precision NOT NULL,
    maxdamage double precision DEFAULT 1::double precision NOT NULL,
    can_ranged integer DEFAULT 0 NOT NULL,
    rangedattacktime integer DEFAULT 0 NOT NULL,
    rangedmindamage double precision DEFAULT 0::double precision NOT NULL,
    rangedmaxdamage double precision DEFAULT 0::double precision NOT NULL,
    respawntime integer DEFAULT 30000 NOT NULL,
    armor integer DEFAULT 0 NOT NULL,
    resistance1 integer DEFAULT 0 NOT NULL,
    resistance2 integer DEFAULT 0 NOT NULL,
    resistance3 integer DEFAULT 0 NOT NULL,
    resistance4 integer DEFAULT 0 NOT NULL,
    resistance5 integer DEFAULT 0 NOT NULL,
    resistance6 integer DEFAULT 0 NOT NULL,
    combat_reach double precision DEFAULT 1::double precision NOT NULL,
    bounding_radius double precision DEFAULT 1::double precision NOT NULL,
    auras text NOT NULL,
    boss integer DEFAULT 0 NOT NULL,
    money integer DEFAULT 0 NOT NULL,
    invisibility_type integer DEFAULT 0 NOT NULL,
    death_state integer DEFAULT 0 NOT NULL,
    walk_speed double precision DEFAULT 2.5::double precision NOT NULL,
    run_speed double precision DEFAULT 8::double precision NOT NULL,
    fly_speed double precision DEFAULT 14::double precision NOT NULL,
    extra_a9_flags integer DEFAULT 0 NOT NULL,
    spell1 integer DEFAULT 0 NOT NULL,
    spell2 integer DEFAULT 0 NOT NULL,
    spell3 integer DEFAULT 0 NOT NULL,
    spell4 integer DEFAULT 0 NOT NULL,
    spell_flags integer DEFAULT 0 NOT NULL,
    modimmunities integer DEFAULT 0 NOT NULL,
    istrainingdummy integer DEFAULT 0 NOT NULL,
    guardtype integer DEFAULT 0 NOT NULL,
    summonguard integer DEFAULT 0 NOT NULL,
    CONSTRAINT creature_proto_armor_check CHECK ((armor >= 0)),
    CONSTRAINT creature_proto_attacktime_check CHECK ((attacktime >= 0)),
    CONSTRAINT creature_proto_boss_check CHECK ((boss >= 0)),
    CONSTRAINT creature_proto_can_ranged_check CHECK ((can_ranged >= 0)),
    CONSTRAINT creature_proto_death_state_check CHECK ((death_state >= 0)),
    CONSTRAINT creature_proto_entry_check CHECK ((entry >= 0)),
    CONSTRAINT creature_proto_faction_check CHECK ((faction >= 0)),
    CONSTRAINT creature_proto_guardtype_check CHECK ((guardtype >= 0)),
    CONSTRAINT creature_proto_invisibility_type_check CHECK ((invisibility_type >= 0)),
    CONSTRAINT creature_proto_istrainingdummy_check CHECK ((istrainingdummy >= 0)),
    CONSTRAINT creature_proto_mana_check CHECK ((mana >= 0)),
    CONSTRAINT creature_proto_maxhealth_check CHECK ((maxhealth >= 0)),
    CONSTRAINT creature_proto_maxlevel_check CHECK ((maxlevel >= 0)),
    CONSTRAINT creature_proto_minhealth_check CHECK ((minhealth >= 0)),
    CONSTRAINT creature_proto_minlevel_check CHECK ((minlevel >= 0)),
    CONSTRAINT creature_proto_modimmunities_check CHECK ((modimmunities >= 0)),
    CONSTRAINT creature_proto_npcflags_check CHECK ((npcflags >= 0)),
    CONSTRAINT creature_proto_rangedattacktime_check CHECK ((rangedattacktime >= 0)),
    CONSTRAINT creature_proto_rangedmaxdamage_check CHECK ((rangedmaxdamage >= (0)::double precision)),
    CONSTRAINT creature_proto_rangedmindamage_check CHECK ((rangedmindamage >= (0)::double precision)),
    CONSTRAINT creature_proto_resistance1_check CHECK ((resistance1 >= 0)),
    CONSTRAINT creature_proto_resistance2_check CHECK ((resistance2 >= 0)),
    CONSTRAINT creature_proto_resistance3_check CHECK ((resistance3 >= 0)),
    CONSTRAINT creature_proto_resistance4_check CHECK ((resistance4 >= 0)),
    CONSTRAINT creature_proto_resistance5_check CHECK ((resistance5 >= 0)),
    CONSTRAINT creature_proto_resistance6_check CHECK ((resistance6 >= 0)),
    CONSTRAINT creature_proto_respawntime_check CHECK ((respawntime >= 0)),
    CONSTRAINT creature_proto_summonguard_check CHECK ((summonguard >= 0))
);


--
-- TOC entry 3511 (class 0 OID 0)
-- Dependencies: 1713
-- Name: TABLE creature_proto; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_proto IS 'Creature System';


--
-- TOC entry 1714 (class 1259 OID 35543)
-- Dependencies: 2254 2255 2256 2257 3
-- Name: creature_quest_finisher; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_quest_finisher (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    CONSTRAINT creature_quest_finisher_id_check CHECK ((id >= 0)),
    CONSTRAINT creature_quest_finisher_quest_check CHECK ((quest >= 0))
);


--
-- TOC entry 3512 (class 0 OID 0)
-- Dependencies: 1714
-- Name: TABLE creature_quest_finisher; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_quest_finisher IS 'Creature System';


--
-- TOC entry 1715 (class 1259 OID 35552)
-- Dependencies: 2258 2259 2260 2261 3
-- Name: creature_quest_starter; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_quest_starter (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    CONSTRAINT creature_quest_starter_id_check CHECK ((id >= 0)),
    CONSTRAINT creature_quest_starter_quest_check CHECK ((quest >= 0))
);


--
-- TOC entry 3513 (class 0 OID 0)
-- Dependencies: 1715
-- Name: TABLE creature_quest_starter; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_quest_starter IS 'Creature System';


--
-- TOC entry 1716 (class 1259 OID 35561)
-- Dependencies: 3
-- Name: creature_spawns_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE creature_spawns_id_seq
    START WITH 389976
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1717 (class 1259 OID 35563)
-- Dependencies: 2262 2263 2264 2265 2266 2267 2268 2269 2270 2271 2272 2273 2274 2275 2276 2277 2278 2279 2280 2281 2282 2283 2284 2285 2286 2287 3
-- Name: creature_spawns; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_spawns (
    id integer DEFAULT nextval('creature_spawns_id_seq'::regclass) NOT NULL,
    entry integer NOT NULL,
    map integer NOT NULL,
    position_x double precision NOT NULL,
    position_y double precision NOT NULL,
    position_z double precision NOT NULL,
    orientation double precision NOT NULL,
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
    canfly smallint DEFAULT 0::smallint NOT NULL,
    phase integer DEFAULT 1 NOT NULL,
    CONSTRAINT creature_spawns_displayid_check CHECK ((displayid >= 0)),
    CONSTRAINT creature_spawns_mountdisplayid_check CHECK ((mountdisplayid >= 0)),
    CONSTRAINT creature_spawns_phase_check CHECK ((phase >= 0)),
    CONSTRAINT creature_spawns_slot1item_check CHECK ((slot1item >= 0)),
    CONSTRAINT creature_spawns_slot2item_check CHECK ((slot2item >= 0)),
    CONSTRAINT creature_spawns_slot3item_check CHECK ((slot3item >= 0))
);


--
-- TOC entry 3514 (class 0 OID 0)
-- Dependencies: 1717
-- Name: TABLE creature_spawns; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_spawns IS 'Spawn System';


--
-- TOC entry 1718 (class 1259 OID 35594)
-- Dependencies: 3
-- Name: creature_staticspawns_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE creature_staticspawns_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1719 (class 1259 OID 35596)
-- Dependencies: 2288 2289 2290 2291 2292 2293 2294 2295 2296 2297 2298 2299 2300 2301 2302 2303 2304 2305 2306 2307 2308 2309 2310 2311 2312 2313 3
-- Name: creature_staticspawns; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_staticspawns (
    id integer DEFAULT nextval('creature_staticspawns_id_seq'::regclass) NOT NULL,
    entry integer NOT NULL,
    map integer NOT NULL,
    x double precision NOT NULL,
    y double precision NOT NULL,
    z double precision NOT NULL,
    o double precision NOT NULL,
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
    canfly smallint DEFAULT 0::smallint NOT NULL,
    phase integer DEFAULT 1 NOT NULL,
    CONSTRAINT creature_staticspawns_displayid_check CHECK ((displayid >= 0)),
    CONSTRAINT creature_staticspawns_mountdisplayid_check CHECK ((mountdisplayid >= 0)),
    CONSTRAINT creature_staticspawns_phase_check CHECK ((phase >= 0)),
    CONSTRAINT creature_staticspawns_slot1item_check CHECK ((slot1item >= 0)),
    CONSTRAINT creature_staticspawns_slot2item_check CHECK ((slot2item >= 0)),
    CONSTRAINT creature_staticspawns_slot3item_check CHECK ((slot3item >= 0))
);


--
-- TOC entry 3515 (class 0 OID 0)
-- Dependencies: 1719
-- Name: TABLE creature_staticspawns; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_staticspawns IS 'Spawn System';


--
-- TOC entry 1720 (class 1259 OID 35627)
-- Dependencies: 2314 2315 2316 2317 2318 2319 2320 2321 2322 2323 2324 2325 2326 2327 3
-- Name: creature_timed_emotes; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_timed_emotes (
    spawnid integer DEFAULT 0 NOT NULL,
    rowid integer DEFAULT 0 NOT NULL,
    type smallint DEFAULT 1::smallint NOT NULL,
    value integer DEFAULT 0 NOT NULL,
    msg text,
    msg_type integer DEFAULT 0 NOT NULL,
    msg_lang integer DEFAULT 0 NOT NULL,
    expire_after integer DEFAULT 0 NOT NULL,
    CONSTRAINT creature_timed_emotes_expire_after_check CHECK ((expire_after >= 0)),
    CONSTRAINT creature_timed_emotes_msg_lang_check CHECK ((msg_lang >= 0)),
    CONSTRAINT creature_timed_emotes_msg_type_check CHECK ((msg_type >= 0)),
    CONSTRAINT creature_timed_emotes_rowid_check CHECK ((rowid >= 0)),
    CONSTRAINT creature_timed_emotes_spawnid_check CHECK ((spawnid >= 0)),
    CONSTRAINT creature_timed_emotes_type_check CHECK ((type >= 0)),
    CONSTRAINT creature_timed_emotes_value_check CHECK ((value >= 0))
);


--
-- TOC entry 1721 (class 1259 OID 35649)
-- Dependencies: 2328 2329 2330 2331 2332 2333 2334 2335 2336 2337 2338 2339 2340 2341 2342 2343 2344 2345 2346 2347 2348 2349 2350 3
-- Name: creature_waypoints; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE creature_waypoints (
    spawnid integer DEFAULT 0 NOT NULL,
    waypointid integer DEFAULT 0 NOT NULL,
    position_x double precision DEFAULT 0::double precision NOT NULL,
    position_y double precision DEFAULT 0::double precision NOT NULL,
    position_z double precision DEFAULT 0::double precision NOT NULL,
    waittime integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    forwardemoteoneshot smallint DEFAULT 0::smallint NOT NULL,
    forwardemoteid integer DEFAULT 0 NOT NULL,
    backwardemoteoneshot smallint DEFAULT 0::smallint NOT NULL,
    backwardemoteid integer DEFAULT 0 NOT NULL,
    forwardskinid integer DEFAULT 0 NOT NULL,
    backwardskinid integer DEFAULT 0 NOT NULL,
    CONSTRAINT creature_waypoints_backwardemoteid_check CHECK ((backwardemoteid >= 0)),
    CONSTRAINT creature_waypoints_backwardemoteoneshot_check CHECK ((backwardemoteoneshot >= 0)),
    CONSTRAINT creature_waypoints_backwardskinid_check CHECK ((backwardskinid >= 0)),
    CONSTRAINT creature_waypoints_flags_check CHECK ((flags >= 0)),
    CONSTRAINT creature_waypoints_forwardemoteid_check CHECK ((forwardemoteid >= 0)),
    CONSTRAINT creature_waypoints_forwardemoteoneshot_check CHECK ((forwardemoteoneshot >= 0)),
    CONSTRAINT creature_waypoints_forwardskinid_check CHECK ((forwardskinid >= 0)),
    CONSTRAINT creature_waypoints_spawnid_check CHECK ((spawnid >= 0)),
    CONSTRAINT creature_waypoints_waittime_check CHECK ((waittime >= 0)),
    CONSTRAINT creature_waypoints_waypointid_check CHECK ((waypointid >= 0))
);


--
-- TOC entry 3516 (class 0 OID 0)
-- Dependencies: 1721
-- Name: TABLE creature_waypoints; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE creature_waypoints IS 'Creature System';


--
-- TOC entry 1722 (class 1259 OID 35688)
-- Dependencies: 2351 2352 2353 2354 3
-- Name: fishing; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE fishing (
    zone integer DEFAULT 0 NOT NULL,
    minskill integer,
    maxskill integer,
    CONSTRAINT fishing_maxskill_check CHECK ((maxskill >= 0)),
    CONSTRAINT fishing_minskill_check CHECK ((minskill >= 0)),
    CONSTRAINT fishing_zone_check CHECK ((zone >= 0))
);


--
-- TOC entry 3517 (class 0 OID 0)
-- Dependencies: 1722
-- Name: TABLE fishing; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE fishing IS 'Fishing System';


--
-- TOC entry 1806 (class 1259 OID 37736)
-- Dependencies: 3295 3296 3297 3298 3299 3300 3301 3302 3303 3304 3305 3306 3307 3308 3309 3310 3311 3312 3313 3314 3315 3316 3317 3318 3319 3320 3321 3322 3323 3324 3325 3326 3327 3328 3329 3330 3331 3332 3
-- Name: gameobject_names; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_names (
    entry integer DEFAULT 0 NOT NULL,
    type integer DEFAULT 0 NOT NULL,
    displayid integer DEFAULT 0 NOT NULL,
    name character varying(100) DEFAULT ''::character varying NOT NULL,
    category character varying(100) DEFAULT ''::character varying NOT NULL,
    castbartext character varying(100) DEFAULT ''::character varying NOT NULL,
    unkstr character varying(100) DEFAULT ''::character varying NOT NULL,
    spellfocus integer DEFAULT 0 NOT NULL,
    sound1 bigint DEFAULT 0::bigint NOT NULL,
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
    size double precision DEFAULT 1::double precision NOT NULL,
    questitem1 integer DEFAULT 0 NOT NULL,
    questitem2 integer DEFAULT 0 NOT NULL,
    questitem3 integer DEFAULT 0 NOT NULL,
    questitem4 integer DEFAULT 0 NOT NULL,
    questitem5 integer DEFAULT 0 NOT NULL,
    questitem6 integer DEFAULT 0 NOT NULL
);


--
-- TOC entry 3518 (class 0 OID 0)
-- Dependencies: 1806
-- Name: TABLE gameobject_names; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_names IS 'Gameobject System';


--
-- TOC entry 1723 (class 1259 OID 35769)
-- Dependencies: 3
-- Name: gameobject_names_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_names_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
    name character varying(100) NOT NULL
);


--
-- TOC entry 1724 (class 1259 OID 35774)
-- Dependencies: 2355 2356 2357 2358 3
-- Name: gameobject_quest_finisher; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_quest_finisher (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    CONSTRAINT gameobject_quest_finisher_id_check CHECK ((id >= 0)),
    CONSTRAINT gameobject_quest_finisher_quest_check CHECK ((quest >= 0))
);


--
-- TOC entry 3519 (class 0 OID 0)
-- Dependencies: 1724
-- Name: TABLE gameobject_quest_finisher; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_quest_finisher IS 'Quest System';


--
-- TOC entry 1725 (class 1259 OID 35783)
-- Dependencies: 2359 2360 2361 2362 3
-- Name: gameobject_quest_item_binding; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_quest_item_binding (
    entry integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    item integer DEFAULT 0 NOT NULL,
    item_count integer DEFAULT 0 NOT NULL
);


--
-- TOC entry 3520 (class 0 OID 0)
-- Dependencies: 1725
-- Name: TABLE gameobject_quest_item_binding; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_quest_item_binding IS 'Quest System';


--
-- TOC entry 1726 (class 1259 OID 35792)
-- Dependencies: 2363 2364 2365 3
-- Name: gameobject_quest_pickup_binding; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_quest_pickup_binding (
    entry integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    required_count integer DEFAULT 0 NOT NULL
);


--
-- TOC entry 3521 (class 0 OID 0)
-- Dependencies: 1726
-- Name: TABLE gameobject_quest_pickup_binding; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_quest_pickup_binding IS 'Quest System';


--
-- TOC entry 1727 (class 1259 OID 35800)
-- Dependencies: 2366 2367 2368 2369 3
-- Name: gameobject_quest_starter; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_quest_starter (
    id integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    CONSTRAINT gameobject_quest_starter_id_check CHECK ((id >= 0)),
    CONSTRAINT gameobject_quest_starter_quest_check CHECK ((quest >= 0))
);


--
-- TOC entry 3522 (class 0 OID 0)
-- Dependencies: 1727
-- Name: TABLE gameobject_quest_starter; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_quest_starter IS 'Quest System';


--
-- TOC entry 1728 (class 1259 OID 35809)
-- Dependencies: 3
-- Name: gameobject_spawns_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gameobject_spawns_id_seq
    START WITH 56993
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1729 (class 1259 OID 35811)
-- Dependencies: 2370 2371 2372 2373 2374 2375 2376 2377 2378 2379 2380 2381 2382 2383 2384 2385 2386 2387 2388 2389 2390 2391 2392 2393 2394 2395 3
-- Name: gameobject_spawns; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_spawns (
    id integer DEFAULT nextval('gameobject_spawns_id_seq'::regclass) NOT NULL,
    entry integer DEFAULT 0 NOT NULL,
    map integer DEFAULT 0 NOT NULL,
    position_x double precision DEFAULT 0::double precision NOT NULL,
    position_y double precision DEFAULT 0::double precision NOT NULL,
    position_z double precision DEFAULT 0::double precision NOT NULL,
    facing double precision DEFAULT 0::double precision NOT NULL,
    orientation1 double precision DEFAULT 0::double precision NOT NULL,
    orientation2 double precision DEFAULT 0::double precision NOT NULL,
    orientation3 double precision DEFAULT 0::double precision NOT NULL,
    orientation4 double precision DEFAULT 0::double precision NOT NULL,
    state integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 0 NOT NULL,
    scale double precision DEFAULT 0::double precision NOT NULL,
    statenpclink integer DEFAULT 0 NOT NULL,
    phase integer DEFAULT 1 NOT NULL,
    overrides integer DEFAULT 0 NOT NULL,
    CONSTRAINT gameobject_spawns_entry_check CHECK ((entry >= 0)),
    CONSTRAINT gameobject_spawns_faction_check CHECK ((faction >= 0)),
    CONSTRAINT gameobject_spawns_flags_check CHECK ((flags >= 0)),
    CONSTRAINT gameobject_spawns_map_check CHECK ((map >= 0)),
    CONSTRAINT gameobject_spawns_overrides_check CHECK ((overrides >= 0)),
    CONSTRAINT gameobject_spawns_phase_check CHECK ((phase >= 0)),
    CONSTRAINT gameobject_spawns_state_check CHECK ((state >= 0)),
    CONSTRAINT gameobject_spawns_statenpclink_check CHECK ((statenpclink >= 0))
);


--
-- TOC entry 3523 (class 0 OID 0)
-- Dependencies: 1729
-- Name: TABLE gameobject_spawns; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_spawns IS 'Spawn System';


--
-- TOC entry 1730 (class 1259 OID 35842)
-- Dependencies: 3
-- Name: gameobject_staticspawns_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gameobject_staticspawns_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1731 (class 1259 OID 35844)
-- Dependencies: 2396 2397 2398 2399 2400 2401 2402 2403 2404 2405 3
-- Name: gameobject_staticspawns; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_staticspawns (
    id integer DEFAULT nextval('gameobject_staticspawns_id_seq'::regclass) NOT NULL,
    entry integer NOT NULL,
    map integer DEFAULT 0 NOT NULL,
    x double precision NOT NULL,
    y double precision NOT NULL,
    z double precision NOT NULL,
    facing double precision NOT NULL,
    o double precision NOT NULL,
    o1 double precision NOT NULL,
    o2 double precision NOT NULL,
    o3 double precision NOT NULL,
    state integer DEFAULT 0 NOT NULL,
    flags integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 0 NOT NULL,
    scale double precision NOT NULL,
    respawnnpclink integer DEFAULT 0 NOT NULL,
    phase integer DEFAULT 1 NOT NULL,
    overrides integer DEFAULT 0 NOT NULL,
    CONSTRAINT gameobject_staticspawns_overrides_check CHECK ((overrides >= 0)),
    CONSTRAINT gameobject_staticspawns_phase_check CHECK ((phase >= 0))
);


--
-- TOC entry 3524 (class 0 OID 0)
-- Dependencies: 1731
-- Name: TABLE gameobject_staticspawns; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_staticspawns IS 'Spawn System';


--
-- TOC entry 1732 (class 1259 OID 35859)
-- Dependencies: 3
-- Name: gameobject_teleports_entry_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gameobject_teleports_entry_seq
    START WITH 800213
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1733 (class 1259 OID 35861)
-- Dependencies: 2406 2407 2408 3
-- Name: gameobject_teleports; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gameobject_teleports (
    entry integer DEFAULT nextval('gameobject_teleports_entry_seq'::regclass) NOT NULL,
    mapid integer NOT NULL,
    x_pos double precision NOT NULL,
    y_pos double precision NOT NULL,
    z_pos double precision NOT NULL,
    orientation double precision NOT NULL,
    required_level integer NOT NULL,
    CONSTRAINT gameobject_teleports_mapid_check CHECK ((mapid >= 0)),
    CONSTRAINT gameobject_teleports_required_level_check CHECK ((required_level >= 0))
);


--
-- TOC entry 3525 (class 0 OID 0)
-- Dependencies: 1733
-- Name: TABLE gameobject_teleports; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gameobject_teleports IS 'Optional table to create custom portals';


--
-- TOC entry 1734 (class 1259 OID 35869)
-- Dependencies: 3
-- Name: graveyards_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE graveyards_id_seq
    START WITH 1475
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1735 (class 1259 OID 35871)
-- Dependencies: 2409 2410 2411 2412 2413 2414 2415 2416 2417 2418 2419 2420 2421 3
-- Name: graveyards; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE graveyards (
    id integer DEFAULT nextval('graveyards_id_seq'::regclass) NOT NULL,
    position_x double precision DEFAULT 0::double precision NOT NULL,
    position_y double precision DEFAULT 0::double precision NOT NULL,
    position_z double precision DEFAULT 0::double precision NOT NULL,
    orientation double precision DEFAULT 0::double precision NOT NULL,
    zoneid integer DEFAULT 0 NOT NULL,
    adjacentzoneid integer DEFAULT 0 NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    faction integer DEFAULT 0 NOT NULL,
    name character varying(255) NOT NULL,
    CONSTRAINT graveyards_adjacentzoneid_check CHECK ((adjacentzoneid >= 0)),
    CONSTRAINT graveyards_faction_check CHECK ((faction >= 0)),
    CONSTRAINT graveyards_mapid_check CHECK ((mapid >= 0)),
    CONSTRAINT graveyards_zoneid_check CHECK ((zoneid >= 0))
);


--
-- TOC entry 3526 (class 0 OID 0)
-- Dependencies: 1735
-- Name: TABLE graveyards; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE graveyards IS 'Graveyard System';


--
-- TOC entry 1736 (class 1259 OID 35889)
-- Dependencies: 2422 2423 2424 2425 2426 2427 3
-- Name: instance_bosses; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE instance_bosses (
    mapid integer DEFAULT 0 NOT NULL,
    creatureid integer DEFAULT 0 NOT NULL,
    trash text NOT NULL,
    trash_respawn_override integer DEFAULT 0 NOT NULL,
    CONSTRAINT instance_bosses_creatureid_check CHECK ((creatureid >= 0)),
    CONSTRAINT instance_bosses_mapid_check CHECK ((mapid >= 0)),
    CONSTRAINT instance_bosses_trash_respawn_override_check CHECK ((trash_respawn_override >= 0))
);


--
-- TOC entry 3527 (class 0 OID 0)
-- Dependencies: 1736
-- Name: TABLE instance_bosses; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE instance_bosses IS 'Instance Boss Definitions';


--
-- TOC entry 1737 (class 1259 OID 35903)
-- Dependencies: 2428 2429 2430 3
-- Name: item_quest_association; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE item_quest_association (
    item integer DEFAULT 0 NOT NULL,
    quest integer DEFAULT 0 NOT NULL,
    item_count integer DEFAULT 0 NOT NULL
);


--
-- TOC entry 3528 (class 0 OID 0)
-- Dependencies: 1737
-- Name: TABLE item_quest_association; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE item_quest_association IS 'Quest System';


--
-- TOC entry 1738 (class 1259 OID 35911)
-- Dependencies: 3
-- Name: item_randomprop_groups; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE item_randomprop_groups (
    entry_id integer NOT NULL,
    randomprops_entryid integer NOT NULL,
    chance double precision NOT NULL
);


--
-- TOC entry 3529 (class 0 OID 0)
-- Dependencies: 1738
-- Name: TABLE item_randomprop_groups; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE item_randomprop_groups IS 'Item System';


--
-- TOC entry 1739 (class 1259 OID 35916)
-- Dependencies: 3
-- Name: item_randomsuffix_groups; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE item_randomsuffix_groups (
    entry_id integer NOT NULL,
    randomsuffix_entryid integer NOT NULL,
    chance double precision NOT NULL
);


--
-- TOC entry 3530 (class 0 OID 0)
-- Dependencies: 1739
-- Name: TABLE item_randomsuffix_groups; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE item_randomsuffix_groups IS 'Item System';


--
-- TOC entry 1740 (class 1259 OID 35921)
-- Dependencies: 2431 2432 2433 2434 2435 3
-- Name: itemnames; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE itemnames (
    entry integer DEFAULT 0 NOT NULL,
    name character varying(100) DEFAULT ''::character varying NOT NULL,
    slotid integer DEFAULT 0 NOT NULL,
    CONSTRAINT itemnames_entry_check CHECK ((entry >= 0)),
    CONSTRAINT itemnames_slotid_check CHECK ((slotid >= 0))
);


--
-- TOC entry 1741 (class 1259 OID 35931)
-- Dependencies: 2436 2437 2438 2439 3
-- Name: itempages; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE itempages (
    entry integer DEFAULT 0 NOT NULL,
    text text NOT NULL,
    next_page integer DEFAULT 0 NOT NULL,
    CONSTRAINT itempages_entry_check CHECK ((entry >= 0)),
    CONSTRAINT itempages_next_page_check CHECK ((next_page >= 0))
);


--
-- TOC entry 3531 (class 0 OID 0)
-- Dependencies: 1741
-- Name: TABLE itempages; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE itempages IS 'Item System';


--
-- TOC entry 1742 (class 1259 OID 35943)
-- Dependencies: 3
-- Name: itempages_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE itempages_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
    text text NOT NULL
);


--
-- TOC entry 1743 (class 1259 OID 35951)
-- Dependencies: 3
-- Name: itempetfood; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE itempetfood (
    entry integer NOT NULL,
    food_type integer NOT NULL
);


--
-- TOC entry 1744 (class 1259 OID 35956)
-- Dependencies: 2440 2441 2442 2443 2444 2445 2446 2447 2448 2449 2450 2451 2452 2453 2454 2455 2456 2457 2458 2459 2460 2461 2462 2463 2464 2465 2466 2467 2468 2469 2470 2471 2472 2473 2474 2475 2476 2477 2478 2479 2480 2481 2482 2483 2484 2485 2486 2487 2488 2489 2490 2491 2492 2493 2494 2495 2496 2497 2498 2499 2500 2501 2502 2503 2504 2505 2506 2507 2508 2509 2510 2511 2512 2513 2514 2515 2516 2517 2518 2519 2520 2521 2522 2523 2524 2525 2526 2527 2528 2529 2530 2531 2532 2533 2534 2535 2536 2537 2538 2539 2540 2541 2542 2543 2544 2545 2546 2547 2548 2549 2550 2551 2552 2553 2554 2555 2556 2557 2558 2559 2560 2561 2562 2563 2564 2565 2566 3
-- Name: items; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE items (
    entry integer DEFAULT 0 NOT NULL,
    class integer DEFAULT 0 NOT NULL,
    subclass integer DEFAULT 0 NOT NULL,
    field4 integer DEFAULT (-1) NOT NULL,
    name1 character varying(255) NOT NULL,
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
    uniquecount integer DEFAULT 0 NOT NULL,
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
    dmg_min1 double precision DEFAULT 0::double precision NOT NULL,
    dmg_max1 double precision DEFAULT 0::double precision NOT NULL,
    dmg_type1 integer DEFAULT 0 NOT NULL,
    dmg_min2 double precision DEFAULT 0::double precision NOT NULL,
    dmg_max2 double precision DEFAULT 0::double precision NOT NULL,
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
    range double precision DEFAULT 0::double precision NOT NULL,
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
    description character varying(255) DEFAULT ''::character varying NOT NULL,
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
    holidayid integer DEFAULT 0 NOT NULL,
    CONSTRAINT items_displayid_check CHECK ((displayid >= 0)),
    CONSTRAINT items_entry_check CHECK ((entry >= 0)),
    CONSTRAINT items_existingduration_check CHECK ((existingduration >= 0)),
    CONSTRAINT items_faction_check CHECK ((faction >= 0)),
    CONSTRAINT items_holidayid_check CHECK ((holidayid >= 0)),
    CONSTRAINT items_itemlimitcategoryid_check CHECK ((itemlimitcategoryid >= 0)),
    CONSTRAINT items_itemstatscount_check CHECK ((itemstatscount >= 0)),
    CONSTRAINT items_scaledstatsdistributionflags_check CHECK ((scaledstatsdistributionflags >= 0)),
    CONSTRAINT items_scaledstatsdistributionid_check CHECK ((scaledstatsdistributionid >= 0))
);


--
-- TOC entry 3532 (class 0 OID 0)
-- Dependencies: 1744
-- Name: TABLE items; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE items IS 'Item System';


--
-- TOC entry 1745 (class 1259 OID 36092)
-- Dependencies: 3
-- Name: items_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE items_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
    name character varying(255) NOT NULL,
    description character varying(255) NOT NULL
);


--
-- TOC entry 1746 (class 1259 OID 36100)
-- Dependencies: 3
-- Name: loot_creatures_index_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE loot_creatures_index_seq
    START WITH 851164
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1747 (class 1259 OID 36102)
-- Dependencies: 2567 2568 2569 2570 2571 2572 2573 2574 2575 2576 2577 2578 2579 3
-- Name: loot_creatures; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE loot_creatures (
    index integer DEFAULT nextval('loot_creatures_index_seq'::regclass) NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance double precision DEFAULT 0::double precision NOT NULL,
    normal25percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic10percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic25percentchance double precision DEFAULT 0::double precision NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL,
    CONSTRAINT loot_creatures_ffa_loot_check CHECK ((ffa_loot >= 0)),
    CONSTRAINT loot_creatures_maxcount_check CHECK ((maxcount >= 0)),
    CONSTRAINT loot_creatures_mincount_check CHECK ((mincount >= 0))
);


--
-- TOC entry 3533 (class 0 OID 0)
-- Dependencies: 1747
-- Name: TABLE loot_creatures; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE loot_creatures IS 'Loot System';


--
-- TOC entry 1748 (class 1259 OID 36125)
-- Dependencies: 3
-- Name: loot_fishing_index_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE loot_fishing_index_seq
    START WITH 2703
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1749 (class 1259 OID 36127)
-- Dependencies: 2580 2581 2582 2583 2584 2585 2586 2587 2588 2589 2590 2591 2592 2593 2594 3
-- Name: loot_fishing; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE loot_fishing (
    index integer DEFAULT nextval('loot_fishing_index_seq'::regclass) NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance double precision DEFAULT 0::double precision NOT NULL,
    normal25percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic10percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic25percentchance double precision DEFAULT 0::double precision NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL,
    CONSTRAINT loot_fishing_entryid_check CHECK ((entryid >= 0)),
    CONSTRAINT loot_fishing_ffa_loot_check CHECK ((ffa_loot >= 0)),
    CONSTRAINT loot_fishing_itemid_check CHECK ((itemid >= 0)),
    CONSTRAINT loot_fishing_maxcount_check CHECK ((maxcount >= 0)),
    CONSTRAINT loot_fishing_mincount_check CHECK ((mincount >= 0))
);


--
-- TOC entry 3534 (class 0 OID 0)
-- Dependencies: 1749
-- Name: TABLE loot_fishing; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE loot_fishing IS 'Loot System';


--
-- TOC entry 1750 (class 1259 OID 36149)
-- Dependencies: 3
-- Name: loot_gameobjects_index_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE loot_gameobjects_index_seq
    START WITH 162542
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1751 (class 1259 OID 36151)
-- Dependencies: 2595 2596 2597 2598 2599 2600 2601 2602 2603 2604 2605 2606 2607 2608 2609 3
-- Name: loot_gameobjects; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE loot_gameobjects (
    index integer DEFAULT nextval('loot_gameobjects_index_seq'::regclass) NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance double precision DEFAULT 0::double precision NOT NULL,
    normal25percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic10percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic25percentchance double precision DEFAULT 0::double precision NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL,
    CONSTRAINT loot_gameobjects_entryid_check CHECK ((entryid >= 0)),
    CONSTRAINT loot_gameobjects_ffa_loot_check CHECK ((ffa_loot >= 0)),
    CONSTRAINT loot_gameobjects_itemid_check CHECK ((itemid >= 0)),
    CONSTRAINT loot_gameobjects_maxcount_check CHECK ((maxcount >= 0)),
    CONSTRAINT loot_gameobjects_mincount_check CHECK ((mincount >= 0))
);


--
-- TOC entry 3535 (class 0 OID 0)
-- Dependencies: 1751
-- Name: TABLE loot_gameobjects; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE loot_gameobjects IS 'Loot System';


--
-- TOC entry 1752 (class 1259 OID 36178)
-- Dependencies: 3
-- Name: loot_items_index_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE loot_items_index_seq
    START WITH 35798
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1753 (class 1259 OID 36180)
-- Dependencies: 2610 2611 2612 2613 2614 2615 2616 2617 2618 2619 2620 2621 2622 2623 2624 3
-- Name: loot_items; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE loot_items (
    index integer DEFAULT nextval('loot_items_index_seq'::regclass) NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 25 NOT NULL,
    normal10percentchance double precision DEFAULT 0::double precision NOT NULL,
    normal25percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic10percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic25percentchance double precision DEFAULT 0::double precision NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL,
    CONSTRAINT loot_items_entryid_check CHECK ((entryid >= 0)),
    CONSTRAINT loot_items_ffa_loot_check CHECK ((ffa_loot >= 0)),
    CONSTRAINT loot_items_itemid_check CHECK ((itemid >= 0)),
    CONSTRAINT loot_items_maxcount_check CHECK ((maxcount >= 0)),
    CONSTRAINT loot_items_mincount_check CHECK ((mincount >= 0))
);


--
-- TOC entry 3536 (class 0 OID 0)
-- Dependencies: 1753
-- Name: TABLE loot_items; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE loot_items IS 'Loot System';


--
-- TOC entry 1754 (class 1259 OID 36202)
-- Dependencies: 3
-- Name: loot_pickpocketing_index_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE loot_pickpocketing_index_seq
    START WITH 18607
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1755 (class 1259 OID 36204)
-- Dependencies: 2625 2626 2627 2628 2629 2630 2631 2632 2633 2634 2635 2636 2637 2638 2639 3
-- Name: loot_pickpocketing; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE loot_pickpocketing (
    index integer DEFAULT nextval('loot_pickpocketing_index_seq'::regclass) NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 25 NOT NULL,
    normal10percentchance double precision DEFAULT 0::double precision NOT NULL,
    normal25percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic10percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic25percentchance double precision DEFAULT 0::double precision NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL,
    CONSTRAINT loot_pickpocketing_entryid_check CHECK ((entryid >= 0)),
    CONSTRAINT loot_pickpocketing_ffa_loot_check CHECK ((ffa_loot >= 0)),
    CONSTRAINT loot_pickpocketing_itemid_check CHECK ((itemid >= 0)),
    CONSTRAINT loot_pickpocketing_maxcount_check CHECK ((maxcount >= 0)),
    CONSTRAINT loot_pickpocketing_mincount_check CHECK ((mincount >= 0))
);


--
-- TOC entry 3537 (class 0 OID 0)
-- Dependencies: 1755
-- Name: TABLE loot_pickpocketing; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE loot_pickpocketing IS 'Loot System';


--
-- TOC entry 1756 (class 1259 OID 36226)
-- Dependencies: 3
-- Name: loot_skinning_index_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE loot_skinning_index_seq
    START WITH 5413
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1757 (class 1259 OID 36228)
-- Dependencies: 2640 2641 2642 2643 2644 2645 2646 2647 2648 2649 2650 2651 2652 2653 2654 3
-- Name: loot_skinning; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE loot_skinning (
    index integer DEFAULT nextval('loot_skinning_index_seq'::regclass) NOT NULL,
    entryid integer DEFAULT 0 NOT NULL,
    itemid integer DEFAULT 0 NOT NULL,
    normal10percentchance double precision DEFAULT 0::double precision NOT NULL,
    normal25percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic10percentchance double precision DEFAULT 0::double precision NOT NULL,
    heroic25percentchance double precision DEFAULT 0::double precision NOT NULL,
    mincount integer DEFAULT 1 NOT NULL,
    maxcount integer DEFAULT 1 NOT NULL,
    ffa_loot integer DEFAULT 0 NOT NULL,
    CONSTRAINT loot_skinning_entryid_check CHECK ((entryid >= 0)),
    CONSTRAINT loot_skinning_ffa_loot_check CHECK ((ffa_loot >= 0)),
    CONSTRAINT loot_skinning_itemid_check CHECK ((itemid >= 0)),
    CONSTRAINT loot_skinning_maxcount_check CHECK ((maxcount >= 0)),
    CONSTRAINT loot_skinning_mincount_check CHECK ((mincount >= 0))
);


--
-- TOC entry 3538 (class 0 OID 0)
-- Dependencies: 1757
-- Name: TABLE loot_skinning; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE loot_skinning IS 'Loot System';


--
-- TOC entry 1758 (class 1259 OID 36250)
-- Dependencies: 3
-- Name: map_checkpoint; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE map_checkpoint (
    entry integer NOT NULL,
    prereq_checkpoint_id integer NOT NULL,
    creature_id integer NOT NULL,
    name character varying(255) NOT NULL
);


--
-- TOC entry 3539 (class 0 OID 0)
-- Dependencies: 1758
-- Name: TABLE map_checkpoint; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE map_checkpoint IS 'Map System';


--
-- TOC entry 1759 (class 1259 OID 36255)
-- Dependencies: 2655 2656 2657 2658 3
-- Name: npc_gossip_textid; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE npc_gossip_textid (
    creatureid integer DEFAULT 0 NOT NULL,
    textid integer DEFAULT 0 NOT NULL,
    CONSTRAINT npc_gossip_textid_creatureid_check CHECK ((creatureid >= 0)),
    CONSTRAINT npc_gossip_textid_textid_check CHECK ((textid >= 0))
);


--
-- TOC entry 3540 (class 0 OID 0)
-- Dependencies: 1759
-- Name: TABLE npc_gossip_textid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE npc_gossip_textid IS 'NPC System';


--
-- TOC entry 1760 (class 1259 OID 36264)
-- Dependencies: 2659 2660 2661 2662 2663 2664 2665 2666 2667 3
-- Name: npc_monstersay; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE npc_monstersay (
    entry integer DEFAULT 0 NOT NULL,
    event integer DEFAULT 0 NOT NULL,
    chance double precision DEFAULT 0::double precision NOT NULL,
    language integer DEFAULT 0 NOT NULL,
    type integer DEFAULT 0 NOT NULL,
    monstername text,
    text0 text,
    text1 text,
    text2 text,
    text3 text,
    text4 text,
    CONSTRAINT npc_monstersay_entry_check CHECK ((entry >= 0)),
    CONSTRAINT npc_monstersay_event_check CHECK ((event >= 0)),
    CONSTRAINT npc_monstersay_language_check CHECK ((language >= 0)),
    CONSTRAINT npc_monstersay_type_check CHECK ((type >= 0))
);


--
-- TOC entry 3541 (class 0 OID 0)
-- Dependencies: 1760
-- Name: TABLE npc_monstersay; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE npc_monstersay IS 'NPC System';


--
-- TOC entry 1761 (class 1259 OID 36281)
-- Dependencies: 2668 2669 2670 2671 2672 2673 2674 2675 2676 2677 2678 2679 2680 2681 2682 2683 2684 2685 2686 2687 2688 2689 2690 2691 2692 2693 2694 2695 2696 2697 2698 2699 2700 2701 2702 2703 2704 2705 2706 2707 2708 2709 2710 2711 2712 2713 2714 2715 2716 2717 2718 2719 2720 2721 2722 2723 2724 2725 2726 2727 2728 2729 2730 2731 2732 2733 2734 2735 2736 2737 2738 2739 2740 2741 2742 2743 2744 2745 2746 2747 2748 2749 2750 2751 2752 2753 2754 2755 2756 2757 2758 2759 2760 2761 2762 2763 2764 2765 2766 2767 2768 2769 2770 2771 2772 2773 2774 2775 2776 2777 2778 2779 2780 2781 2782 2783 2784 2785 2786 2787 2788 2789 3
-- Name: npc_text; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE npc_text (
    entry integer DEFAULT 0 NOT NULL,
    prob0 double precision DEFAULT 0::double precision NOT NULL,
    text0_0 text NOT NULL,
    text0_1 text NOT NULL,
    lang0 integer DEFAULT 0 NOT NULL,
    em0_0 integer DEFAULT 0 NOT NULL,
    em0_1 integer DEFAULT 0 NOT NULL,
    em0_2 integer DEFAULT 0 NOT NULL,
    em0_3 integer DEFAULT 0 NOT NULL,
    em0_4 integer DEFAULT 0 NOT NULL,
    em0_5 integer DEFAULT 0 NOT NULL,
    prob1 double precision DEFAULT 0::double precision NOT NULL,
    text1_0 text NOT NULL,
    text1_1 text NOT NULL,
    lang1 integer DEFAULT 0 NOT NULL,
    em1_0 integer DEFAULT 0 NOT NULL,
    em1_1 integer DEFAULT 0 NOT NULL,
    em1_2 integer DEFAULT 0 NOT NULL,
    em1_3 integer DEFAULT 0 NOT NULL,
    em1_4 integer DEFAULT 0 NOT NULL,
    em1_5 integer DEFAULT 0 NOT NULL,
    prob2 double precision DEFAULT 0::double precision NOT NULL,
    text2_0 text NOT NULL,
    text2_1 text NOT NULL,
    lang2 integer DEFAULT 0 NOT NULL,
    em2_0 integer DEFAULT 0 NOT NULL,
    em2_1 integer DEFAULT 0 NOT NULL,
    em2_2 integer DEFAULT 0 NOT NULL,
    em2_3 integer DEFAULT 0 NOT NULL,
    em2_4 integer DEFAULT 0 NOT NULL,
    em2_5 integer DEFAULT 0 NOT NULL,
    prob3 double precision DEFAULT 0::double precision NOT NULL,
    text3_0 text NOT NULL,
    text3_1 text NOT NULL,
    lang3 integer DEFAULT 0 NOT NULL,
    em3_0 integer DEFAULT 0 NOT NULL,
    em3_1 integer DEFAULT 0 NOT NULL,
    em3_2 integer DEFAULT 0 NOT NULL,
    em3_3 integer DEFAULT 0 NOT NULL,
    em3_4 integer DEFAULT 0 NOT NULL,
    em3_5 integer DEFAULT 0 NOT NULL,
    prob4 double precision DEFAULT 0::double precision NOT NULL,
    text4_0 text NOT NULL,
    text4_1 text NOT NULL,
    lang4 integer DEFAULT 0 NOT NULL,
    em4_0 integer DEFAULT 0 NOT NULL,
    em4_1 integer DEFAULT 0 NOT NULL,
    em4_2 integer DEFAULT 0 NOT NULL,
    em4_3 integer DEFAULT 0 NOT NULL,
    em4_4 integer DEFAULT 0 NOT NULL,
    em4_5 integer DEFAULT 0 NOT NULL,
    prob5 double precision DEFAULT 0::double precision NOT NULL,
    text5_0 text NOT NULL,
    text5_1 text NOT NULL,
    lang5 integer DEFAULT 0 NOT NULL,
    em5_0 integer DEFAULT 0 NOT NULL,
    em5_1 integer DEFAULT 0 NOT NULL,
    em5_2 integer DEFAULT 0 NOT NULL,
    em5_3 integer DEFAULT 0 NOT NULL,
    em5_4 integer DEFAULT 0 NOT NULL,
    em5_5 integer DEFAULT 0 NOT NULL,
    prob6 double precision DEFAULT 0::double precision NOT NULL,
    text6_0 text NOT NULL,
    text6_1 text NOT NULL,
    lang6 integer DEFAULT 0 NOT NULL,
    em6_0 integer DEFAULT 0 NOT NULL,
    em6_1 integer DEFAULT 0 NOT NULL,
    em6_2 integer DEFAULT 0 NOT NULL,
    em6_3 integer DEFAULT 0 NOT NULL,
    em6_4 integer DEFAULT 0 NOT NULL,
    em6_5 integer DEFAULT 0 NOT NULL,
    prob7 double precision DEFAULT 0::double precision NOT NULL,
    text7_0 text NOT NULL,
    text7_1 text NOT NULL,
    lang7 integer DEFAULT 0 NOT NULL,
    em7_0 integer DEFAULT 0 NOT NULL,
    em7_1 integer DEFAULT 0 NOT NULL,
    em7_2 integer DEFAULT 0 NOT NULL,
    em7_3 integer DEFAULT 0 NOT NULL,
    em7_4 integer DEFAULT 0 NOT NULL,
    em7_5 integer DEFAULT 0 NOT NULL,
    CONSTRAINT npc_text_em0_0_check CHECK ((em0_0 >= 0)),
    CONSTRAINT npc_text_em0_1_check CHECK ((em0_1 >= 0)),
    CONSTRAINT npc_text_em0_2_check CHECK ((em0_2 >= 0)),
    CONSTRAINT npc_text_em0_3_check CHECK ((em0_3 >= 0)),
    CONSTRAINT npc_text_em0_4_check CHECK ((em0_4 >= 0)),
    CONSTRAINT npc_text_em0_5_check CHECK ((em0_5 >= 0)),
    CONSTRAINT npc_text_em1_0_check CHECK ((em1_0 >= 0)),
    CONSTRAINT npc_text_em1_1_check CHECK ((em1_1 >= 0)),
    CONSTRAINT npc_text_em1_2_check CHECK ((em1_2 >= 0)),
    CONSTRAINT npc_text_em1_3_check CHECK ((em1_3 >= 0)),
    CONSTRAINT npc_text_em1_4_check CHECK ((em1_4 >= 0)),
    CONSTRAINT npc_text_em1_5_check CHECK ((em1_5 >= 0)),
    CONSTRAINT npc_text_em2_0_check CHECK ((em2_0 >= 0)),
    CONSTRAINT npc_text_em2_1_check CHECK ((em2_1 >= 0)),
    CONSTRAINT npc_text_em2_2_check CHECK ((em2_2 >= 0)),
    CONSTRAINT npc_text_em2_3_check CHECK ((em2_3 >= 0)),
    CONSTRAINT npc_text_em2_4_check CHECK ((em2_4 >= 0)),
    CONSTRAINT npc_text_em2_5_check CHECK ((em2_5 >= 0)),
    CONSTRAINT npc_text_em3_0_check CHECK ((em3_0 >= 0)),
    CONSTRAINT npc_text_em3_1_check CHECK ((em3_1 >= 0)),
    CONSTRAINT npc_text_em3_2_check CHECK ((em3_2 >= 0)),
    CONSTRAINT npc_text_em3_3_check CHECK ((em3_3 >= 0)),
    CONSTRAINT npc_text_em3_4_check CHECK ((em3_4 >= 0)),
    CONSTRAINT npc_text_em3_5_check CHECK ((em3_5 >= 0)),
    CONSTRAINT npc_text_em4_0_check CHECK ((em4_0 >= 0)),
    CONSTRAINT npc_text_em4_1_check CHECK ((em4_1 >= 0)),
    CONSTRAINT npc_text_em4_2_check CHECK ((em4_2 >= 0)),
    CONSTRAINT npc_text_em4_3_check CHECK ((em4_3 >= 0)),
    CONSTRAINT npc_text_em4_4_check CHECK ((em4_4 >= 0)),
    CONSTRAINT npc_text_em4_5_check CHECK ((em4_5 >= 0)),
    CONSTRAINT npc_text_em5_0_check CHECK ((em5_0 >= 0)),
    CONSTRAINT npc_text_em5_1_check CHECK ((em5_1 >= 0)),
    CONSTRAINT npc_text_em5_2_check CHECK ((em5_2 >= 0)),
    CONSTRAINT npc_text_em5_3_check CHECK ((em5_3 >= 0)),
    CONSTRAINT npc_text_em5_4_check CHECK ((em5_4 >= 0)),
    CONSTRAINT npc_text_em5_5_check CHECK ((em5_5 >= 0)),
    CONSTRAINT npc_text_em6_0_check CHECK ((em6_0 >= 0)),
    CONSTRAINT npc_text_em6_1_check CHECK ((em6_1 >= 0)),
    CONSTRAINT npc_text_em6_2_check CHECK ((em6_2 >= 0)),
    CONSTRAINT npc_text_em6_3_check CHECK ((em6_3 >= 0)),
    CONSTRAINT npc_text_em6_4_check CHECK ((em6_4 >= 0)),
    CONSTRAINT npc_text_em6_5_check CHECK ((em6_5 >= 0)),
    CONSTRAINT npc_text_em7_0_check CHECK ((em7_0 >= 0)),
    CONSTRAINT npc_text_em7_1_check CHECK ((em7_1 >= 0)),
    CONSTRAINT npc_text_em7_2_check CHECK ((em7_2 >= 0)),
    CONSTRAINT npc_text_em7_3_check CHECK ((em7_3 >= 0)),
    CONSTRAINT npc_text_em7_4_check CHECK ((em7_4 >= 0)),
    CONSTRAINT npc_text_em7_5_check CHECK ((em7_5 >= 0)),
    CONSTRAINT npc_text_entry_check CHECK ((entry >= 0)),
    CONSTRAINT npc_text_lang0_check CHECK ((lang0 >= 0)),
    CONSTRAINT npc_text_lang1_check CHECK ((lang1 >= 0)),
    CONSTRAINT npc_text_lang2_check CHECK ((lang2 >= 0)),
    CONSTRAINT npc_text_lang3_check CHECK ((lang3 >= 0)),
    CONSTRAINT npc_text_lang4_check CHECK ((lang4 >= 0)),
    CONSTRAINT npc_text_lang5_check CHECK ((lang5 >= 0)),
    CONSTRAINT npc_text_lang6_check CHECK ((lang6 >= 0)),
    CONSTRAINT npc_text_lang7_check CHECK ((lang7 >= 0))
);


--
-- TOC entry 3542 (class 0 OID 0)
-- Dependencies: 1761
-- Name: TABLE npc_text; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE npc_text IS 'NPC System';


--
-- TOC entry 1762 (class 1259 OID 36415)
-- Dependencies: 3
-- Name: npc_text_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE npc_text_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
    text0 character varying(200) NOT NULL,
    text0_1 character varying(200) NOT NULL,
    text1 character varying(200) NOT NULL,
    text1_1 character varying(200) NOT NULL,
    text2 character varying(200) NOT NULL,
    text2_1 character varying(200) NOT NULL,
    text3 character varying(200) NOT NULL,
    text3_1 character varying(200) NOT NULL,
    text4 character varying(200) NOT NULL,
    text4_1 character varying(200) NOT NULL,
    text5 character varying(200) NOT NULL,
    text5_1 character varying(200) NOT NULL,
    text6 character varying(200) NOT NULL,
    text6_1 character varying(200) NOT NULL,
    text7 character varying(200) NOT NULL,
    text7_1 character varying(200) NOT NULL
);


--
-- TOC entry 1763 (class 1259 OID 36423)
-- Dependencies: 2790 2791 3
-- Name: petdefaultspells; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE petdefaultspells (
    entry integer DEFAULT 0 NOT NULL,
    spell integer DEFAULT 0 NOT NULL
);


--
-- TOC entry 3543 (class 0 OID 0)
-- Dependencies: 1763
-- Name: TABLE petdefaultspells; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE petdefaultspells IS 'Pet System';


--
-- TOC entry 1764 (class 1259 OID 36428)
-- Dependencies: 3
-- Name: playercreateinfo_index_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE playercreateinfo_index_seq
    START WITH 66
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1765 (class 1259 OID 36430)
-- Dependencies: 2792 2793 2794 2795 2796 2797 2798 2799 2800 2801 2802 2803 2804 2805 2806 2807 2808 2809 2810 2811 2812 2813 2814 2815 2816 2817 2818 2819 2820 2821 2822 2823 2824 2825 2826 2827 2828 2829 2830 2831 2832 2833 3
-- Name: playercreateinfo; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE playercreateinfo (
    index integer DEFAULT nextval('playercreateinfo_index_seq'::regclass) NOT NULL,
    race smallint DEFAULT 0::smallint NOT NULL,
    factiontemplate integer DEFAULT 0 NOT NULL,
    class smallint DEFAULT 0::smallint NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    zoneid integer DEFAULT 0 NOT NULL,
    positionx double precision DEFAULT 0::double precision NOT NULL,
    positiony double precision DEFAULT 0::double precision NOT NULL,
    positionz double precision DEFAULT 0::double precision NOT NULL,
    displayid smallint DEFAULT 0::smallint NOT NULL,
    basestrength smallint DEFAULT 0::smallint NOT NULL,
    baseagility smallint DEFAULT 0::smallint NOT NULL,
    basestamina smallint DEFAULT 0::smallint NOT NULL,
    baseintellect smallint DEFAULT 0::smallint NOT NULL,
    basespirit smallint DEFAULT 0::smallint NOT NULL,
    basehealth integer DEFAULT 0 NOT NULL,
    basemana integer DEFAULT 0 NOT NULL,
    baserage integer DEFAULT 0 NOT NULL,
    basefocus integer DEFAULT 0 NOT NULL,
    baseenergy integer DEFAULT 0 NOT NULL,
    attackpower integer DEFAULT 0 NOT NULL,
    mindmg double precision DEFAULT 0::double precision NOT NULL,
    maxdmg double precision DEFAULT 0::double precision NOT NULL,
    introid integer DEFAULT 0 NOT NULL,
    taximask text,
    CONSTRAINT playercreateinfo_attackpower_check CHECK ((attackpower >= 0)),
    CONSTRAINT playercreateinfo_baseagility_check CHECK ((baseagility >= 0)),
    CONSTRAINT playercreateinfo_baseenergy_check CHECK ((baseenergy >= 0)),
    CONSTRAINT playercreateinfo_basefocus_check CHECK ((basefocus >= 0)),
    CONSTRAINT playercreateinfo_basehealth_check CHECK ((basehealth >= 0)),
    CONSTRAINT playercreateinfo_baseintellect_check CHECK ((baseintellect >= 0)),
    CONSTRAINT playercreateinfo_basemana_check CHECK ((basemana >= 0)),
    CONSTRAINT playercreateinfo_baserage_check CHECK ((baserage >= 0)),
    CONSTRAINT playercreateinfo_basespirit_check CHECK ((basespirit >= 0)),
    CONSTRAINT playercreateinfo_basestamina_check CHECK ((basestamina >= 0)),
    CONSTRAINT playercreateinfo_basestrength_check CHECK ((basestrength >= 0)),
    CONSTRAINT playercreateinfo_class_check CHECK ((class >= 0)),
    CONSTRAINT playercreateinfo_displayid_check CHECK ((displayid >= 0)),
    CONSTRAINT playercreateinfo_factiontemplate_check CHECK ((factiontemplate >= 0)),
    CONSTRAINT playercreateinfo_introid_check CHECK ((introid >= 0)),
    CONSTRAINT playercreateinfo_mapid_check CHECK ((mapid >= 0)),
    CONSTRAINT playercreateinfo_race_check CHECK ((race >= 0)),
    CONSTRAINT playercreateinfo_zoneid_check CHECK ((zoneid >= 0))
);


--
-- TOC entry 3544 (class 0 OID 0)
-- Dependencies: 1765
-- Name: TABLE playercreateinfo; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE playercreateinfo IS 'Player System';


--
-- TOC entry 1766 (class 1259 OID 36480)
-- Dependencies: 2834 2835 2836 2837 2838 2839 3
-- Name: playercreateinfo_bars; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE playercreateinfo_bars (
    race smallint,
    class smallint,
    button integer,
    action integer,
    type integer,
    misc integer,
    CONSTRAINT playercreateinfo_bars_action_check CHECK ((action >= 0)),
    CONSTRAINT playercreateinfo_bars_button_check CHECK ((button >= 0)),
    CONSTRAINT playercreateinfo_bars_class_check CHECK ((class >= 0)),
    CONSTRAINT playercreateinfo_bars_misc_check CHECK ((misc >= 0)),
    CONSTRAINT playercreateinfo_bars_race_check CHECK ((race >= 0)),
    CONSTRAINT playercreateinfo_bars_type_check CHECK ((type >= 0))
);


--
-- TOC entry 3545 (class 0 OID 0)
-- Dependencies: 1766
-- Name: TABLE playercreateinfo_bars; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE playercreateinfo_bars IS 'Player System';


--
-- TOC entry 1767 (class 1259 OID 36489)
-- Dependencies: 2840 2841 2842 2843 2844 2845 2846 2847 3
-- Name: playercreateinfo_items; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE playercreateinfo_items (
    indexid smallint DEFAULT 0::smallint NOT NULL,
    protoid integer DEFAULT 0 NOT NULL,
    slotid smallint DEFAULT 0::smallint NOT NULL,
    amount integer DEFAULT 0 NOT NULL,
    CONSTRAINT playercreateinfo_items_amount_check CHECK ((amount >= 0)),
    CONSTRAINT playercreateinfo_items_indexid_check CHECK ((indexid >= 0)),
    CONSTRAINT playercreateinfo_items_protoid_check CHECK ((protoid >= 0)),
    CONSTRAINT playercreateinfo_items_slotid_check CHECK ((slotid >= 0))
);


--
-- TOC entry 3546 (class 0 OID 0)
-- Dependencies: 1767
-- Name: TABLE playercreateinfo_items; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE playercreateinfo_items IS 'Player System';


--
-- TOC entry 1768 (class 1259 OID 36500)
-- Dependencies: 2848 2849 2850 2851 2852 2853 2854 2855 3
-- Name: playercreateinfo_skills; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE playercreateinfo_skills (
    indexid smallint DEFAULT 0::smallint NOT NULL,
    skillid integer DEFAULT 0 NOT NULL,
    level integer DEFAULT 0 NOT NULL,
    maxlevel integer DEFAULT 0 NOT NULL,
    CONSTRAINT playercreateinfo_skills_indexid_check CHECK ((indexid >= 0)),
    CONSTRAINT playercreateinfo_skills_level_check CHECK ((level >= 0)),
    CONSTRAINT playercreateinfo_skills_maxlevel_check CHECK ((maxlevel >= 0)),
    CONSTRAINT playercreateinfo_skills_skillid_check CHECK ((skillid >= 0))
);


--
-- TOC entry 3547 (class 0 OID 0)
-- Dependencies: 1768
-- Name: TABLE playercreateinfo_skills; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE playercreateinfo_skills IS 'Player System';


--
-- TOC entry 1769 (class 1259 OID 36511)
-- Dependencies: 2856 2857 2858 2859 3
-- Name: playercreateinfo_spells; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE playercreateinfo_spells (
    indexid smallint DEFAULT 0::smallint NOT NULL,
    spellid smallint DEFAULT 0::smallint NOT NULL,
    CONSTRAINT playercreateinfo_spells_indexid_check CHECK ((indexid >= 0)),
    CONSTRAINT playercreateinfo_spells_spellid_check CHECK ((spellid >= 0))
);


--
-- TOC entry 3548 (class 0 OID 0)
-- Dependencies: 1769
-- Name: TABLE playercreateinfo_spells; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE playercreateinfo_spells IS 'Player System';


--
-- TOC entry 1770 (class 1259 OID 36518)
-- Dependencies: 2860 2861 2862 2863 2864 2865 2866 3
-- Name: professiondiscoveries; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE professiondiscoveries (
    spellid integer DEFAULT 0 NOT NULL,
    spelltodiscover integer DEFAULT 0 NOT NULL,
    skillvalue integer DEFAULT 0 NOT NULL,
    chance double precision DEFAULT 0::double precision NOT NULL,
    CONSTRAINT professiondiscoveries_skillvalue_check CHECK ((skillvalue >= 0)),
    CONSTRAINT professiondiscoveries_spellid_check CHECK ((spellid >= 0)),
    CONSTRAINT professiondiscoveries_spelltodiscover_check CHECK ((spelltodiscover >= 0))
);


--
-- TOC entry 1771 (class 1259 OID 36530)
-- Dependencies: 2867 2868 2869 2870 2871 2872 2873 2874 2875 2876 2877 2878 2879 2880 2881 2882 2883 2884 2885 2886 2887 2888 2889 2890 2891 2892 2893 2894 2895 2896 2897 2898 2899 2900 2901 2902 2903 2904 2905 2906 2907 2908 2909 2910 2911 2912 2913 2914 2915 2916 2917 2918 2919 2920 2921 2922 2923 2924 2925 2926 2927 2928 2929 2930 2931 2932 2933 2934 2935 2936 2937 2938 2939 2940 2941 2942 2943 2944 2945 2946 2947 2948 2949 2950 2951 2952 2953 2954 2955 2956 2957 2958 2959 2960 2961 2962 2963 2964 2965 2966 2967 2968 2969 2970 2971 2972 2973 2974 2975 2976 2977 2978 2979 2980 2981 2982 2983 2984 2985 2986 2987 2988 2989 2990 2991 2992 2993 2994 2995 2996 2997 2998 2999 3000 3001 3002 3003 3004 3005 3006 3007 3008 3009 3010 3011 3012 3013 3014 3015 3016 3017 3018 3019 3020 3021 3022 3023 3024 3025 3026 3027 3028 3029 3030 3031 3032 3033 3034 3035 3036 3037 3038 3039 3040 3041 3042 3043 3044 3045 3046 3047 3048 3049 3050 3051 3052 3053 3054 3055 3056 3057 3058 3059 3060 3061 3062 3063 3064 3065 3066 3067 3068 3069 3070 3071 3072 3073 3074 3075 3076 3077 3078 3079 3080 3081 3082 3083 3084 3085 3086 3087 3088 3089 3090 3091 3092 3093 3094 3095 3096 3097 3098 3099 3100 3101 3102 3103 3104 3105 3106 3107 3108 3109 3110 3111 3112 3113 3114 3
-- Name: quests; Type: TABLE; Schema: public; Owner: -; Tablespace: 
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
    pointx double precision DEFAULT 0::double precision NOT NULL,
    pointy double precision DEFAULT 0::double precision NOT NULL,
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
    iscompletedbyspelleffect integer DEFAULT 0 NOT NULL,
    CONSTRAINT quests_bonushonor_check CHECK ((bonushonor >= 0)),
    CONSTRAINT quests_castspell_check CHECK ((castspell >= 0)),
    CONSTRAINT quests_completeemote_check CHECK ((completeemote >= 0)),
    CONSTRAINT quests_completionemote1_check CHECK ((completionemote1 >= 0)),
    CONSTRAINT quests_completionemote2_check CHECK ((completionemote2 >= 0)),
    CONSTRAINT quests_completionemote3_check CHECK ((completionemote3 >= 0)),
    CONSTRAINT quests_completionemote4_check CHECK ((completionemote4 >= 0)),
    CONSTRAINT quests_completionemotecnt_check CHECK ((completionemotecnt >= 0)),
    CONSTRAINT quests_completionemotedelay1_check CHECK ((completionemotedelay1 >= 0)),
    CONSTRAINT quests_completionemotedelay2_check CHECK ((completionemotedelay2 >= 0)),
    CONSTRAINT quests_completionemotedelay3_check CHECK ((completionemotedelay3 >= 0)),
    CONSTRAINT quests_completionemotedelay4_check CHECK ((completionemotedelay4 >= 0)),
    CONSTRAINT quests_detailemote1_check CHECK ((detailemote1 >= 0)),
    CONSTRAINT quests_detailemote2_check CHECK ((detailemote2 >= 0)),
    CONSTRAINT quests_detailemote3_check CHECK ((detailemote3 >= 0)),
    CONSTRAINT quests_detailemote4_check CHECK ((detailemote4 >= 0)),
    CONSTRAINT quests_detailemotecount_check CHECK ((detailemotecount >= 0)),
    CONSTRAINT quests_detailemotedelay1_check CHECK ((detailemotedelay1 >= 0)),
    CONSTRAINT quests_detailemotedelay2_check CHECK ((detailemotedelay2 >= 0)),
    CONSTRAINT quests_detailemotedelay3_check CHECK ((detailemotedelay3 >= 0)),
    CONSTRAINT quests_detailemotedelay4_check CHECK ((detailemotedelay4 >= 0)),
    CONSTRAINT quests_entry_check CHECK ((entry >= 0)),
    CONSTRAINT quests_exploretrigger1_check CHECK ((exploretrigger1 >= 0)),
    CONSTRAINT quests_exploretrigger2_check CHECK ((exploretrigger2 >= 0)),
    CONSTRAINT quests_exploretrigger3_check CHECK ((exploretrigger3 >= 0)),
    CONSTRAINT quests_exploretrigger4_check CHECK ((exploretrigger4 >= 0)),
    CONSTRAINT quests_flags_check CHECK ((flags >= 0)),
    CONSTRAINT quests_incompleteemote_check CHECK ((incompleteemote >= 0)),
    CONSTRAINT quests_iscompletedbyspelleffect_check CHECK ((iscompletedbyspelleffect >= 0)),
    CONSTRAINT quests_limittime_check CHECK ((limittime >= 0)),
    CONSTRAINT quests_minlevel_check CHECK ((minlevel >= 0)),
    CONSTRAINT quests_nextquestid_check CHECK ((nextquestid >= 0)),
    CONSTRAINT quests_pointmapid_check CHECK ((pointmapid >= 0)),
    CONSTRAINT quests_pointopt_check CHECK ((pointopt >= 0)),
    CONSTRAINT quests_prevquestid_check CHECK ((prevquestid >= 0)),
    CONSTRAINT quests_receiveitemcount1_check CHECK ((receiveitemcount1 >= 0)),
    CONSTRAINT quests_receiveitemcount2_check CHECK ((receiveitemcount2 >= 0)),
    CONSTRAINT quests_receiveitemcount3_check CHECK ((receiveitemcount3 >= 0)),
    CONSTRAINT quests_receiveitemcount4_check CHECK ((receiveitemcount4 >= 0)),
    CONSTRAINT quests_receiveitemid1_check CHECK ((receiveitemid1 >= 0)),
    CONSTRAINT quests_receiveitemid2_check CHECK ((receiveitemid2 >= 0)),
    CONSTRAINT quests_receiveitemid3_check CHECK ((receiveitemid3 >= 0)),
    CONSTRAINT quests_receiveitemid4_check CHECK ((receiveitemid4 >= 0)),
    CONSTRAINT quests_reqemoteid1_check CHECK ((reqemoteid1 >= 0)),
    CONSTRAINT quests_reqemoteid2_check CHECK ((reqemoteid2 >= 0)),
    CONSTRAINT quests_reqemoteid3_check CHECK ((reqemoteid3 >= 0)),
    CONSTRAINT quests_reqemoteid4_check CHECK ((reqemoteid4 >= 0)),
    CONSTRAINT quests_reqitemcount1_check CHECK ((reqitemcount1 >= 0)),
    CONSTRAINT quests_reqitemcount2_check CHECK ((reqitemcount2 >= 0)),
    CONSTRAINT quests_reqitemcount3_check CHECK ((reqitemcount3 >= 0)),
    CONSTRAINT quests_reqitemcount4_check CHECK ((reqitemcount4 >= 0)),
    CONSTRAINT quests_reqitemcount5_check CHECK ((reqitemcount5 >= 0)),
    CONSTRAINT quests_reqitemcount6_check CHECK ((reqitemcount6 >= 0)),
    CONSTRAINT quests_reqitemid1_check CHECK ((reqitemid1 >= 0)),
    CONSTRAINT quests_reqitemid2_check CHECK ((reqitemid2 >= 0)),
    CONSTRAINT quests_reqitemid3_check CHECK ((reqitemid3 >= 0)),
    CONSTRAINT quests_reqitemid4_check CHECK ((reqitemid4 >= 0)),
    CONSTRAINT quests_reqitemid5_check CHECK ((reqitemid5 >= 0)),
    CONSTRAINT quests_reqitemid6_check CHECK ((reqitemid6 >= 0)),
    CONSTRAINT quests_reqkillmoborgocount1_check CHECK ((reqkillmoborgocount1 >= 0)),
    CONSTRAINT quests_reqkillmoborgocount2_check CHECK ((reqkillmoborgocount2 >= 0)),
    CONSTRAINT quests_reqkillmoborgocount3_check CHECK ((reqkillmoborgocount3 >= 0)),
    CONSTRAINT quests_reqkillmoborgocount4_check CHECK ((reqkillmoborgocount4 >= 0)),
    CONSTRAINT quests_requiredclass_check CHECK ((requiredclass >= 0)),
    CONSTRAINT quests_requiredquest1_check CHECK ((requiredquest1 >= 0)),
    CONSTRAINT quests_requiredquest2_check CHECK ((requiredquest2 >= 0)),
    CONSTRAINT quests_requiredquest3_check CHECK ((requiredquest3 >= 0)),
    CONSTRAINT quests_requiredquest4_check CHECK ((requiredquest4 >= 0)),
    CONSTRAINT quests_requiredraces_check CHECK ((requiredraces >= 0)),
    CONSTRAINT quests_requiredrepfaction_check CHECK ((requiredrepfaction >= 0)),
    CONSTRAINT quests_requiredrepvalue_check CHECK ((requiredrepvalue >= 0)),
    CONSTRAINT quests_requiredtradeskill_check CHECK ((requiredtradeskill >= 0)),
    CONSTRAINT quests_requiredtradeskillvalue_check CHECK ((requiredtradeskillvalue >= 0)),
    CONSTRAINT quests_rewardmoneyatmaxlevel_check CHECK ((rewardmoneyatmaxlevel >= 0)),
    CONSTRAINT quests_rewardtalents_check CHECK ((rewardtalents >= 0)),
    CONSTRAINT quests_rewardtitleid_check CHECK ((rewardtitleid >= 0)),
    CONSTRAINT quests_rewchoiceitemcount1_check CHECK ((rewchoiceitemcount1 >= 0)),
    CONSTRAINT quests_rewchoiceitemcount2_check CHECK ((rewchoiceitemcount2 >= 0)),
    CONSTRAINT quests_rewchoiceitemcount3_check CHECK ((rewchoiceitemcount3 >= 0)),
    CONSTRAINT quests_rewchoiceitemcount4_check CHECK ((rewchoiceitemcount4 >= 0)),
    CONSTRAINT quests_rewchoiceitemcount5_check CHECK ((rewchoiceitemcount5 >= 0)),
    CONSTRAINT quests_rewchoiceitemcount6_check CHECK ((rewchoiceitemcount6 >= 0)),
    CONSTRAINT quests_rewchoiceitemid1_check CHECK ((rewchoiceitemid1 >= 0)),
    CONSTRAINT quests_rewchoiceitemid2_check CHECK ((rewchoiceitemid2 >= 0)),
    CONSTRAINT quests_rewchoiceitemid3_check CHECK ((rewchoiceitemid3 >= 0)),
    CONSTRAINT quests_rewchoiceitemid4_check CHECK ((rewchoiceitemid4 >= 0)),
    CONSTRAINT quests_rewchoiceitemid5_check CHECK ((rewchoiceitemid5 >= 0)),
    CONSTRAINT quests_rewchoiceitemid6_check CHECK ((rewchoiceitemid6 >= 0)),
    CONSTRAINT quests_rewitemcount1_check CHECK ((rewitemcount1 >= 0)),
    CONSTRAINT quests_rewitemcount2_check CHECK ((rewitemcount2 >= 0)),
    CONSTRAINT quests_rewitemcount3_check CHECK ((rewitemcount3 >= 0)),
    CONSTRAINT quests_rewitemcount4_check CHECK ((rewitemcount4 >= 0)),
    CONSTRAINT quests_rewitemid1_check CHECK ((rewitemid1 >= 0)),
    CONSTRAINT quests_rewitemid2_check CHECK ((rewitemid2 >= 0)),
    CONSTRAINT quests_rewitemid3_check CHECK ((rewitemid3 >= 0)),
    CONSTRAINT quests_rewitemid4_check CHECK ((rewitemid4 >= 0)),
    CONSTRAINT quests_rewmoney_check CHECK ((rewmoney >= 0)),
    CONSTRAINT quests_rewrepfaction1_check CHECK ((rewrepfaction1 >= 0)),
    CONSTRAINT quests_rewrepfaction2_check CHECK ((rewrepfaction2 >= 0)),
    CONSTRAINT quests_rewrepfaction3_check CHECK ((rewrepfaction3 >= 0)),
    CONSTRAINT quests_rewrepfaction4_check CHECK ((rewrepfaction4 >= 0)),
    CONSTRAINT quests_rewrepfaction5_check CHECK ((rewrepfaction5 >= 0)),
    CONSTRAINT quests_rewrepfaction6_check CHECK ((rewrepfaction6 >= 0)),
    CONSTRAINT quests_rewreplimit_check CHECK ((rewreplimit >= 0)),
    CONSTRAINT quests_rewrepvalue3_check CHECK ((rewrepvalue3 >= 0)),
    CONSTRAINT quests_rewrepvalue4_check CHECK ((rewrepvalue4 >= 0)),
    CONSTRAINT quests_rewrepvalue5_check CHECK ((rewrepvalue5 >= 0)),
    CONSTRAINT quests_rewrepvalue6_check CHECK ((rewrepvalue6 >= 0)),
    CONSTRAINT quests_rewspell_check CHECK ((rewspell >= 0)),
    CONSTRAINT quests_rewxp_check CHECK ((rewxp >= 0)),
    CONSTRAINT quests_sort_check CHECK ((sort >= 0)),
    CONSTRAINT quests_specialflags_check CHECK ((specialflags >= 0)),
    CONSTRAINT quests_srcitem_check CHECK ((srcitem >= 0)),
    CONSTRAINT quests_srcitemcount_check CHECK ((srcitemcount >= 0)),
    CONSTRAINT quests_suggestedplayers_check CHECK ((suggestedplayers >= 0)),
    CONSTRAINT quests_type_check CHECK ((type >= 0)),
    CONSTRAINT quests_zoneid_check CHECK ((zoneid >= 0))
);


--
-- TOC entry 3549 (class 0 OID 0)
-- Dependencies: 1771
-- Name: TABLE quests; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE quests IS 'Quests System';


--
-- TOC entry 1772 (class 1259 OID 36829)
-- Dependencies: 3
-- Name: quests_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE quests_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
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


--
-- TOC entry 1773 (class 1259 OID 36879)
-- Dependencies: 3
-- Name: recall_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE recall_id_seq
    START WITH 829
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1774 (class 1259 OID 36881)
-- Dependencies: 3115 3116 3117 3118 3119 3120 3121 3122 3
-- Name: recall; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE recall (
    id integer DEFAULT nextval('recall_id_seq'::regclass) NOT NULL,
    name character varying(100) NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    positionx double precision DEFAULT 0::double precision NOT NULL,
    positiony double precision DEFAULT 0::double precision NOT NULL,
    positionz double precision DEFAULT 0::double precision NOT NULL,
    orientation double precision DEFAULT 0::double precision NOT NULL,
    CONSTRAINT recall_mapid_check CHECK ((mapid >= 0)),
    CONSTRAINT recall_orientation_check CHECK ((orientation >= (0)::double precision))
);


--
-- TOC entry 3550 (class 0 OID 0)
-- Dependencies: 1774
-- Name: TABLE recall; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE recall IS 'Tele Command';


--
-- TOC entry 1775 (class 1259 OID 36894)
-- Dependencies: 3
-- Name: reputation_creature_onkill; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE reputation_creature_onkill (
    creature_id integer NOT NULL,
    faction_change_alliance integer NOT NULL,
    faction_change_horde integer NOT NULL,
    change_value integer NOT NULL,
    rep_limit integer NOT NULL
);


--
-- TOC entry 3551 (class 0 OID 0)
-- Dependencies: 1775
-- Name: TABLE reputation_creature_onkill; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE reputation_creature_onkill IS 'Creature System';


--
-- TOC entry 1776 (class 1259 OID 36899)
-- Dependencies: 3
-- Name: reputation_faction_onkill; Type: TABLE; Schema: public; Owner: -; Tablespace: 
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


--
-- TOC entry 3552 (class 0 OID 0)
-- Dependencies: 1776
-- Name: TABLE reputation_faction_onkill; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE reputation_faction_onkill IS 'Creature System';


--
-- TOC entry 1777 (class 1259 OID 36904)
-- Dependencies: 3
-- Name: reputation_instance_onkill; Type: TABLE; Schema: public; Owner: -; Tablespace: 
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


--
-- TOC entry 3553 (class 0 OID 0)
-- Dependencies: 1777
-- Name: TABLE reputation_instance_onkill; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE reputation_instance_onkill IS 'Creature System';


--
-- TOC entry 1778 (class 1259 OID 36909)
-- Dependencies: 3123 3
-- Name: spell_coef_override; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE spell_coef_override (
    id double precision,
    name character varying(300) DEFAULT NULL::character varying,
    dspell_coef_override double precision,
    otspell_coef_override double precision
);


--
-- TOC entry 1779 (class 1259 OID 36913)
-- Dependencies: 3
-- Name: spell_disable; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE spell_disable (
    spellid integer NOT NULL,
    replacement_spellid integer NOT NULL
);


--
-- TOC entry 3554 (class 0 OID 0)
-- Dependencies: 1779
-- Name: TABLE spell_disable; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE spell_disable IS 'Spell System';


--
-- TOC entry 1780 (class 1259 OID 36916)
-- Dependencies: 3
-- Name: spell_disable_trainers; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE spell_disable_trainers (
    spellid integer NOT NULL,
    replacement_spellid integer NOT NULL
);


--
-- TOC entry 3555 (class 0 OID 0)
-- Dependencies: 1780
-- Name: TABLE spell_disable_trainers; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE spell_disable_trainers IS 'Trainer System';


--
-- TOC entry 1781 (class 1259 OID 36919)
-- Dependencies: 3124 3125 3126 3127 3128 3129 3130 3131 3132 3
-- Name: spell_effects_override; Type: TABLE; Schema: public; Owner: -; Tablespace: 
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


--
-- TOC entry 1805 (class 1259 OID 37241)
-- Dependencies: 3284 3285 3286 3287 3288 3289 3290 3291 3292 3293 3294 3
-- Name: spell_proc; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE spell_proc (
    spellid integer DEFAULT 0 NOT NULL,
    proconnamehash bigint DEFAULT 0::bigint NOT NULL,
    procflag integer DEFAULT 0 NOT NULL,
    targetself smallint DEFAULT 0::smallint NOT NULL,
    procchance integer DEFAULT (-1) NOT NULL,
    proccharges smallint DEFAULT (-1)::smallint NOT NULL,
    procinterval integer DEFAULT 0 NOT NULL,
    effecttriggerspell0 integer DEFAULT (-1) NOT NULL,
    effecttriggerspell1 integer DEFAULT (-1) NOT NULL,
    effecttriggerspell2 integer DEFAULT (-1) NOT NULL,
    CONSTRAINT spell_proc_proconnamehash_check CHECK ((proconnamehash >= 0))
);


--
-- TOC entry 1782 (class 1259 OID 36949)
-- Dependencies: 3133 3134 3135 3136 3
-- Name: spelloverride; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE spelloverride (
    overrideid integer DEFAULT 0 NOT NULL,
    spellid integer DEFAULT 0 NOT NULL,
    CONSTRAINT spelloverride_overrideid_check CHECK ((overrideid >= 0)),
    CONSTRAINT spelloverride_spellid_check CHECK ((spellid >= 0))
);


--
-- TOC entry 3556 (class 0 OID 0)
-- Dependencies: 1782
-- Name: TABLE spelloverride; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE spelloverride IS 'Spell System';


--
-- TOC entry 1783 (class 1259 OID 36958)
-- Dependencies: 3137 3138 3139 3140 3141 3142 3143 3144 3145 3
-- Name: teleport_coords; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE teleport_coords (
    id integer DEFAULT 0 NOT NULL,
    name character(255) NOT NULL,
    mapid integer DEFAULT 0 NOT NULL,
    position_x double precision DEFAULT 0::double precision NOT NULL,
    position_y double precision DEFAULT 0::double precision NOT NULL,
    position_z double precision DEFAULT 0::double precision NOT NULL,
    totrigger integer DEFAULT 0 NOT NULL,
    CONSTRAINT teleport_coords_id_check CHECK ((id >= 0)),
    CONSTRAINT teleport_coords_mapid_check CHECK ((mapid >= 0)),
    CONSTRAINT teleport_coords_totrigger_check CHECK ((totrigger >= 0))
);


--
-- TOC entry 3557 (class 0 OID 0)
-- Dependencies: 1783
-- Name: TABLE teleport_coords; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE teleport_coords IS 'World System';


--
-- TOC entry 1784 (class 1259 OID 36972)
-- Dependencies: 3146 3147 3148 3149 3150 3151 3152 3153 3
-- Name: totemspells; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE totemspells (
    spell integer DEFAULT 0 NOT NULL,
    castspell1 integer DEFAULT 0 NOT NULL,
    castspell2 integer DEFAULT 0 NOT NULL,
    castspell3 integer DEFAULT 0 NOT NULL,
    CONSTRAINT totemspells_castspell1_check CHECK ((castspell1 >= 0)),
    CONSTRAINT totemspells_castspell2_check CHECK ((castspell2 >= 0)),
    CONSTRAINT totemspells_castspell3_check CHECK ((castspell3 >= 0)),
    CONSTRAINT totemspells_spell_check CHECK ((spell >= 0))
);


--
-- TOC entry 3558 (class 0 OID 0)
-- Dependencies: 1784
-- Name: TABLE totemspells; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE totemspells IS 'Spell System';


--
-- TOC entry 1785 (class 1259 OID 36985)
-- Dependencies: 3154 3155 3156 3157 3158 3159 3160 3161 3162 3163 3164 3165 3166 3
-- Name: trainer_defs; Type: TABLE; Schema: public; Owner: -; Tablespace: 
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
    cannot_train_gossip_textid integer NOT NULL,
    CONSTRAINT trainer_defs_entry_check CHECK ((entry >= 0)),
    CONSTRAINT trainer_defs_req_class_check CHECK ((req_class >= 0)),
    CONSTRAINT trainer_defs_required_skill_check CHECK ((required_skill >= 0)),
    CONSTRAINT trainer_defs_required_skillvalue_check CHECK ((required_skillvalue >= 0)),
    CONSTRAINT trainer_defs_trainer_type_check CHECK ((trainer_type >= 0))
);


--
-- TOC entry 3559 (class 0 OID 0)
-- Dependencies: 1785
-- Name: TABLE trainer_defs; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE trainer_defs IS 'Trainer System';


--
-- TOC entry 1786 (class 1259 OID 37006)
-- Dependencies: 3167 3168 3169 3170 3171 3172 3173 3174 3175 3176 3177 3178 3179 3180 3181 3182 3183 3184 3185 3
-- Name: trainer_spells; Type: TABLE; Schema: public; Owner: -; Tablespace: 
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
    is_prof smallint DEFAULT 0::smallint NOT NULL,
    CONSTRAINT trainer_spells_cast_spell_check CHECK ((cast_spell >= 0)),
    CONSTRAINT trainer_spells_deletespell_check CHECK ((deletespell >= 0)),
    CONSTRAINT trainer_spells_entry_check CHECK ((entry >= 0)),
    CONSTRAINT trainer_spells_is_prof_check CHECK ((is_prof >= 0)),
    CONSTRAINT trainer_spells_learn_spell_check CHECK ((learn_spell >= 0)),
    CONSTRAINT trainer_spells_reqlevel_check CHECK ((reqlevel >= 0)),
    CONSTRAINT trainer_spells_reqskill_check CHECK ((reqskill >= 0)),
    CONSTRAINT trainer_spells_reqskillvalue_check CHECK ((reqskillvalue >= 0)),
    CONSTRAINT trainer_spells_reqspell_check CHECK ((reqspell >= 0)),
    CONSTRAINT trainer_spells_spellcost_check CHECK ((spellcost >= 0))
);


--
-- TOC entry 3560 (class 0 OID 0)
-- Dependencies: 1786
-- Name: TABLE trainer_spells; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE trainer_spells IS 'Trainer System';


--
-- TOC entry 1787 (class 1259 OID 37030)
-- Dependencies: 3186 3187 3188 3189 3190 3191 3192 3193 3194 3195 3196 3197 3198 3199 3200 3201 3
-- Name: trainerspelloverride; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE trainerspelloverride (
    spellid integer DEFAULT 0 NOT NULL,
    cost integer DEFAULT 0 NOT NULL,
    requiredspell integer DEFAULT 0 NOT NULL,
    deletespell integer DEFAULT 0 NOT NULL,
    requiredskill integer DEFAULT 0 NOT NULL,
    requiredskillvalue integer DEFAULT 0 NOT NULL,
    reqlevel integer DEFAULT 0 NOT NULL,
    requiredclass integer DEFAULT 0 NOT NULL,
    CONSTRAINT trainerspelloverride_cost_check CHECK ((cost >= 0)),
    CONSTRAINT trainerspelloverride_deletespell_check CHECK ((deletespell >= 0)),
    CONSTRAINT trainerspelloverride_reqlevel_check CHECK ((reqlevel >= 0)),
    CONSTRAINT trainerspelloverride_requiredclass_check CHECK ((requiredclass >= 0)),
    CONSTRAINT trainerspelloverride_requiredskill_check CHECK ((requiredskill >= 0)),
    CONSTRAINT trainerspelloverride_requiredskillvalue_check CHECK ((requiredskillvalue >= 0)),
    CONSTRAINT trainerspelloverride_requiredspell_check CHECK ((requiredspell >= 0)),
    CONSTRAINT trainerspelloverride_spellid_check CHECK ((spellid >= 0))
);


--
-- TOC entry 3561 (class 0 OID 0)
-- Dependencies: 1787
-- Name: TABLE trainerspelloverride; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE trainerspelloverride IS 'Trainer System';


--
-- TOC entry 1788 (class 1259 OID 37051)
-- Dependencies: 3202 3203 3
-- Name: transport_creatures; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE transport_creatures (
    transport_entry integer NOT NULL,
    creature_entry integer NOT NULL,
    position_x double precision NOT NULL,
    position_y double precision NOT NULL,
    position_z double precision NOT NULL,
    orientation double precision NOT NULL,
    CONSTRAINT transport_creatures_creature_entry_check CHECK ((creature_entry >= 0)),
    CONSTRAINT transport_creatures_transport_entry_check CHECK ((transport_entry >= 0))
);


--
-- TOC entry 1789 (class 1259 OID 37056)
-- Dependencies: 3204 3205 3206 3207 3
-- Name: transport_data; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE transport_data (
    entry integer DEFAULT 0 NOT NULL,
    name text,
    period integer DEFAULT 0 NOT NULL,
    CONSTRAINT transport_data_entry_check CHECK ((entry >= 0)),
    CONSTRAINT transport_data_period_check CHECK ((period >= 0))
);


--
-- TOC entry 3562 (class 0 OID 0)
-- Dependencies: 1789
-- Name: TABLE transport_data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE transport_data IS 'Transports';


--
-- TOC entry 1790 (class 1259 OID 37068)
-- Dependencies: 3208 3209 3210 3211 3
-- Name: unit_display_sizes; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE unit_display_sizes (
    displayid integer NOT NULL,
    halfsize double precision DEFAULT 1::double precision NOT NULL,
    modelid integer DEFAULT 0 NOT NULL,
    CONSTRAINT unit_display_sizes_displayid_check CHECK ((displayid >= 0)),
    CONSTRAINT unit_display_sizes_modelid_check CHECK ((modelid >= 0))
);


--
-- TOC entry 1791 (class 1259 OID 37078)
-- Dependencies: 3212 3213 3214 3215 3216 3217 3218 3219 3220 3221 3
-- Name: vendor_restrictions; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE vendor_restrictions (
    entry integer NOT NULL,
    racemask integer DEFAULT (-1) NOT NULL,
    reqrepfaction integer DEFAULT 0 NOT NULL,
    reqrepfactionvalue integer DEFAULT 0 NOT NULL,
    canbuyattextid integer DEFAULT 0 NOT NULL,
    cannotbuyattextid integer DEFAULT 0 NOT NULL,
    CONSTRAINT vendor_restrictions_canbuyattextid_check CHECK ((canbuyattextid >= 0)),
    CONSTRAINT vendor_restrictions_cannotbuyattextid_check CHECK ((cannotbuyattextid >= 0)),
    CONSTRAINT vendor_restrictions_entry_check CHECK ((entry >= 0)),
    CONSTRAINT vendor_restrictions_reqrepfaction_check CHECK ((reqrepfaction >= 0)),
    CONSTRAINT vendor_restrictions_reqrepfactionvalue_check CHECK ((reqrepfactionvalue >= 0))
);


--
-- TOC entry 1792 (class 1259 OID 37093)
-- Dependencies: 3222 3223 3224 3225 3226 3227 3228 3229 3
-- Name: vendors; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE vendors (
    entry integer DEFAULT 0 NOT NULL,
    item integer DEFAULT 0 NOT NULL,
    amount integer DEFAULT 0 NOT NULL,
    max_amount integer DEFAULT 0 NOT NULL,
    inctime bigint DEFAULT 0::bigint NOT NULL,
    extended_cost integer DEFAULT 0 NOT NULL,
    CONSTRAINT vendors_entry_check CHECK ((entry >= 0)),
    CONSTRAINT vendors_item_check CHECK ((item >= 0))
);


--
-- TOC entry 3563 (class 0 OID 0)
-- Dependencies: 1792
-- Name: TABLE vendors; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE vendors IS 'NPC System';


--
-- TOC entry 1793 (class 1259 OID 37106)
-- Dependencies: 3230 3231 3232 3233 3234 3235 3236 3237 3238 3239 3240 3241 3242 3243 3
-- Name: weather; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE weather (
    zoneid integer DEFAULT 0 NOT NULL,
    high_chance integer DEFAULT 0 NOT NULL,
    high_type integer DEFAULT 0 NOT NULL,
    med_chance integer DEFAULT 0 NOT NULL,
    med_type integer DEFAULT 0 NOT NULL,
    low_chance integer DEFAULT 0 NOT NULL,
    low_type integer DEFAULT 0 NOT NULL,
    CONSTRAINT weather_high_chance_check CHECK ((high_chance >= 0)),
    CONSTRAINT weather_high_type_check CHECK ((high_type >= 0)),
    CONSTRAINT weather_low_chance_check CHECK ((low_chance >= 0)),
    CONSTRAINT weather_low_type_check CHECK ((low_type >= 0)),
    CONSTRAINT weather_med_chance_check CHECK ((med_chance >= 0)),
    CONSTRAINT weather_med_type_check CHECK ((med_type >= 0)),
    CONSTRAINT weather_zoneid_check CHECK ((zoneid >= 0))
);


--
-- TOC entry 3564 (class 0 OID 0)
-- Dependencies: 1793
-- Name: TABLE weather; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE weather IS 'Weather System';


--
-- TOC entry 1794 (class 1259 OID 37125)
-- Dependencies: 3244 3
-- Name: wordfilter_character_names; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE wordfilter_character_names (
    regex_match character varying(500) NOT NULL,
    regex_ignore_if_matched character varying(500) DEFAULT ''::character varying NOT NULL
);


--
-- TOC entry 1795 (class 1259 OID 37132)
-- Dependencies: 3245 3
-- Name: wordfilter_chat; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE wordfilter_chat (
    regex_match character varying(500) NOT NULL,
    regex_ignore_if_matched character varying(500) DEFAULT ''::character varying NOT NULL
);


--
-- TOC entry 1796 (class 1259 OID 37139)
-- Dependencies: 3
-- Name: worldbroadcast_entry_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE worldbroadcast_entry_seq
    START WITH 3
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1797 (class 1259 OID 37141)
-- Dependencies: 3246 3247 3
-- Name: worldbroadcast; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE worldbroadcast (
    entry integer DEFAULT nextval('worldbroadcast_entry_seq'::regclass) NOT NULL,
    text character varying(255) NOT NULL,
    percent integer DEFAULT 100 NOT NULL
);


--
-- TOC entry 1798 (class 1259 OID 37148)
-- Dependencies: 3
-- Name: worldbroadcast_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE worldbroadcast_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
    text character varying(255) NOT NULL
);


--
-- TOC entry 1799 (class 1259 OID 37153)
-- Dependencies: 3248 3249 3250 3251 3252 3253 3254 3255 3256 3257 3258 3259 3260 3261 3262 3263 3264 3265 3266 3267 3268 3269 3270 3271 3272 3273 3274 3275 3276 3277 3278 3279 3
-- Name: worldmap_info; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE worldmap_info (
    entry integer DEFAULT 0 NOT NULL,
    screenid integer DEFAULT 0,
    type integer DEFAULT 0,
    maxplayers integer DEFAULT 0,
    minlevel integer DEFAULT 1,
    minlevel_heroic integer DEFAULT 0 NOT NULL,
    repopx double precision DEFAULT 0::double precision,
    repopy double precision DEFAULT 0::double precision,
    repopz double precision DEFAULT 0::double precision,
    repopentry integer DEFAULT 0,
    area_name character varying(100) DEFAULT '0'::character varying,
    flags integer DEFAULT 0 NOT NULL,
    cooldown integer DEFAULT 0 NOT NULL,
    lvl_mod_a integer DEFAULT 0 NOT NULL,
    required_quest integer DEFAULT 0 NOT NULL,
    required_item integer DEFAULT 0 NOT NULL,
    heroic_keyid_1 integer DEFAULT 0 NOT NULL,
    heroic_keyid_2 integer DEFAULT 0 NOT NULL,
    viewingdistance double precision DEFAULT 80::double precision NOT NULL,
    required_checkpoint integer DEFAULT 0 NOT NULL,
    CONSTRAINT worldmap_info_cooldown_check CHECK ((cooldown >= 0)),
    CONSTRAINT worldmap_info_entry_check CHECK ((entry >= 0)),
    CONSTRAINT worldmap_info_flags_check CHECK ((flags >= 0)),
    CONSTRAINT worldmap_info_lvl_mod_a_check CHECK ((lvl_mod_a >= 0)),
    CONSTRAINT worldmap_info_maxplayers_check CHECK ((maxplayers >= 0)),
    CONSTRAINT worldmap_info_minlevel_check CHECK ((minlevel >= 0)),
    CONSTRAINT worldmap_info_minlevel_heroic_check CHECK ((minlevel_heroic >= 0)),
    CONSTRAINT worldmap_info_repopentry_check CHECK ((repopentry >= 0)),
    CONSTRAINT worldmap_info_required_item_check CHECK ((required_item >= 0)),
    CONSTRAINT worldmap_info_required_quest_check CHECK ((required_quest >= 0)),
    CONSTRAINT worldmap_info_screenid_check CHECK ((screenid >= 0)),
    CONSTRAINT worldmap_info_type_check CHECK ((type >= 0))
);


--
-- TOC entry 3565 (class 0 OID 0)
-- Dependencies: 1799
-- Name: TABLE worldmap_info; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE worldmap_info IS 'World System';


--
-- TOC entry 1800 (class 1259 OID 37190)
-- Dependencies: 3
-- Name: worldmap_info_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE worldmap_info_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
    text character varying(255) NOT NULL
);


--
-- TOC entry 1801 (class 1259 OID 37195)
-- Dependencies: 3
-- Name: worldstring_tables_entry_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE worldstring_tables_entry_seq
    START WITH 83
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


--
-- TOC entry 1802 (class 1259 OID 37197)
-- Dependencies: 3280 3
-- Name: worldstring_tables; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE worldstring_tables (
    entry integer DEFAULT nextval('worldstring_tables_entry_seq'::regclass) NOT NULL,
    text character varying(255) NOT NULL
);


--
-- TOC entry 1803 (class 1259 OID 37203)
-- Dependencies: 3
-- Name: worldstring_tables_localized; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE worldstring_tables_localized (
    entry integer NOT NULL,
    language_code character varying(5) NOT NULL,
    text character varying(255) NOT NULL
);


--
-- TOC entry 1804 (class 1259 OID 37208)
-- Dependencies: 3281 3282 3283 3
-- Name: zoneguards; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE zoneguards (
    zone integer NOT NULL,
    horde_entry integer,
    alliance_entry integer,
    CONSTRAINT zoneguards_alliance_entry_check CHECK ((alliance_entry >= 0)),
    CONSTRAINT zoneguards_horde_entry_check CHECK ((horde_entry >= 0)),
    CONSTRAINT zoneguards_zone_check CHECK ((zone >= 0))
);


--
-- TOC entry 3566 (class 0 OID 0)
-- Dependencies: 1804
-- Name: TABLE zoneguards; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE zoneguards IS 'World System';


--
-- TOC entry 3334 (class 2606 OID 35333)
-- Dependencies: 1701 1701 1701
-- Name: ai_agents_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY ai_agents
    ADD CONSTRAINT ai_agents_pkey PRIMARY KEY (entry, spell);


--
-- TOC entry 3336 (class 2606 OID 35342)
-- Dependencies: 1702 1702
-- Name: ai_threattospellid_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY ai_threattospellid
    ADD CONSTRAINT ai_threattospellid_pkey PRIMARY KEY (spell);


--
-- TOC entry 3338 (class 2606 OID 35362)
-- Dependencies: 1703 1703
-- Name: areatriggers_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY areatriggers
    ADD CONSTRAINT areatriggers_pkey PRIMARY KEY (entry);


--
-- TOC entry 3340 (class 2606 OID 35373)
-- Dependencies: 1705 1705
-- Name: auctionhouse_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY auctionhouse
    ADD CONSTRAINT auctionhouse_pkey PRIMARY KEY (id);


--
-- TOC entry 3342 (class 2606 OID 35378)
-- Dependencies: 1706 1706
-- Name: banned_phrases_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY banned_phrases
    ADD CONSTRAINT banned_phrases_pkey PRIMARY KEY (phrase);


--
-- TOC entry 3344 (class 2606 OID 35392)
-- Dependencies: 1708 1708
-- Name: clientaddons_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY clientaddons
    ADD CONSTRAINT clientaddons_pkey PRIMARY KEY (id);


--
-- TOC entry 3346 (class 2606 OID 35397)
-- Dependencies: 1709 1709
-- Name: command_overrides_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY command_overrides
    ADD CONSTRAINT command_overrides_pkey PRIMARY KEY (command_name);


--
-- TOC entry 3348 (class 2606 OID 35408)
-- Dependencies: 1710 1710
-- Name: creature_formations_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_formations
    ADD CONSTRAINT creature_formations_pkey PRIMARY KEY (spawn_id);


--
-- TOC entry 3352 (class 2606 OID 35462)
-- Dependencies: 1712 1712 1712
-- Name: creature_names_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_names_localized
    ADD CONSTRAINT creature_names_localized_pkey PRIMARY KEY (id, language_code);


--
-- TOC entry 3350 (class 2606 OID 35456)
-- Dependencies: 1711 1711
-- Name: creature_names_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_names
    ADD CONSTRAINT creature_names_pkey PRIMARY KEY (entry);


--
-- TOC entry 3354 (class 2606 OID 35542)
-- Dependencies: 1713 1713
-- Name: creature_proto_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_proto
    ADD CONSTRAINT creature_proto_pkey PRIMARY KEY (entry);


--
-- TOC entry 3356 (class 2606 OID 35551)
-- Dependencies: 1714 1714 1714
-- Name: creature_quest_finisher_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_quest_finisher
    ADD CONSTRAINT creature_quest_finisher_pkey PRIMARY KEY (id, quest);


--
-- TOC entry 3358 (class 2606 OID 35560)
-- Dependencies: 1715 1715 1715
-- Name: creature_quest_starter_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_quest_starter
    ADD CONSTRAINT creature_quest_starter_pkey PRIMARY KEY (id, quest);


--
-- TOC entry 3360 (class 2606 OID 35593)
-- Dependencies: 1717 1717
-- Name: creature_spawns_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_spawns
    ADD CONSTRAINT creature_spawns_pkey PRIMARY KEY (id);


--
-- TOC entry 3362 (class 2606 OID 35626)
-- Dependencies: 1719 1719
-- Name: creature_staticspawns_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_staticspawns
    ADD CONSTRAINT creature_staticspawns_pkey PRIMARY KEY (id);


--
-- TOC entry 3364 (class 2606 OID 35648)
-- Dependencies: 1720 1720 1720
-- Name: creature_timed_emotes_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_timed_emotes
    ADD CONSTRAINT creature_timed_emotes_pkey PRIMARY KEY (spawnid, rowid);


--
-- TOC entry 3366 (class 2606 OID 35676)
-- Dependencies: 1721 1721 1721
-- Name: creature_waypoints_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY creature_waypoints
    ADD CONSTRAINT creature_waypoints_pkey PRIMARY KEY (spawnid, waypointid);


--
-- TOC entry 3368 (class 2606 OID 35696)
-- Dependencies: 1722 1722
-- Name: fishing_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY fishing
    ADD CONSTRAINT fishing_pkey PRIMARY KEY (zone);


--
-- TOC entry 3370 (class 2606 OID 35773)
-- Dependencies: 1723 1723 1723
-- Name: gameobject_names_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_names_localized
    ADD CONSTRAINT gameobject_names_localized_pkey PRIMARY KEY (entry, language_code);


--
-- TOC entry 3497 (class 2606 OID 37778)
-- Dependencies: 1806 1806
-- Name: gameobject_names_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_names
    ADD CONSTRAINT gameobject_names_pkey PRIMARY KEY (entry);


--
-- TOC entry 3372 (class 2606 OID 35782)
-- Dependencies: 1724 1724 1724
-- Name: gameobject_quest_finisher_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_finisher
    ADD CONSTRAINT gameobject_quest_finisher_pkey PRIMARY KEY (id, quest);


--
-- TOC entry 3374 (class 2606 OID 35791)
-- Dependencies: 1725 1725 1725 1725
-- Name: gameobject_quest_item_binding_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_item_binding
    ADD CONSTRAINT gameobject_quest_item_binding_pkey PRIMARY KEY (entry, item, quest);


--
-- TOC entry 3376 (class 2606 OID 35799)
-- Dependencies: 1726 1726 1726
-- Name: gameobject_quest_pickup_binding_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_pickup_binding
    ADD CONSTRAINT gameobject_quest_pickup_binding_pkey PRIMARY KEY (entry, quest);


--
-- TOC entry 3378 (class 2606 OID 35808)
-- Dependencies: 1727 1727 1727
-- Name: gameobject_quest_starter_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_quest_starter
    ADD CONSTRAINT gameobject_quest_starter_pkey PRIMARY KEY (id, quest);


--
-- TOC entry 3380 (class 2606 OID 35841)
-- Dependencies: 1729 1729
-- Name: gameobject_spawns_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_spawns
    ADD CONSTRAINT gameobject_spawns_pkey PRIMARY KEY (id);


--
-- TOC entry 3382 (class 2606 OID 35858)
-- Dependencies: 1731 1731
-- Name: gameobject_staticspawns_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_staticspawns
    ADD CONSTRAINT gameobject_staticspawns_pkey PRIMARY KEY (id);


--
-- TOC entry 3384 (class 2606 OID 35868)
-- Dependencies: 1733 1733
-- Name: gameobject_teleports_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gameobject_teleports
    ADD CONSTRAINT gameobject_teleports_pkey PRIMARY KEY (entry);


--
-- TOC entry 3386 (class 2606 OID 35888)
-- Dependencies: 1735 1735
-- Name: graveyards_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY graveyards
    ADD CONSTRAINT graveyards_pkey PRIMARY KEY (id);


--
-- TOC entry 3388 (class 2606 OID 35902)
-- Dependencies: 1736 1736 1736
-- Name: instance_bosses_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY instance_bosses
    ADD CONSTRAINT instance_bosses_pkey PRIMARY KEY (mapid, creatureid);


--
-- TOC entry 3390 (class 2606 OID 35910)
-- Dependencies: 1737 1737 1737
-- Name: item_quest_association_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY item_quest_association
    ADD CONSTRAINT item_quest_association_pkey PRIMARY KEY (item, quest);


--
-- TOC entry 3392 (class 2606 OID 35915)
-- Dependencies: 1738 1738 1738
-- Name: item_randomprop_groups_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY item_randomprop_groups
    ADD CONSTRAINT item_randomprop_groups_pkey PRIMARY KEY (entry_id, randomprops_entryid);


--
-- TOC entry 3394 (class 2606 OID 35920)
-- Dependencies: 1739 1739 1739
-- Name: item_randomsuffix_groups_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY item_randomsuffix_groups
    ADD CONSTRAINT item_randomsuffix_groups_pkey PRIMARY KEY (entry_id, randomsuffix_entryid);


--
-- TOC entry 3396 (class 2606 OID 35930)
-- Dependencies: 1740 1740
-- Name: itemnames_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY itemnames
    ADD CONSTRAINT itemnames_pkey PRIMARY KEY (entry);


--
-- TOC entry 3400 (class 2606 OID 35950)
-- Dependencies: 1742 1742 1742
-- Name: itempages_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY itempages_localized
    ADD CONSTRAINT itempages_localized_pkey PRIMARY KEY (entry, language_code);


--
-- TOC entry 3398 (class 2606 OID 35942)
-- Dependencies: 1741 1741
-- Name: itempages_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY itempages
    ADD CONSTRAINT itempages_pkey PRIMARY KEY (entry);


--
-- TOC entry 3402 (class 2606 OID 35955)
-- Dependencies: 1743 1743
-- Name: itempetfood_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY itempetfood
    ADD CONSTRAINT itempetfood_pkey PRIMARY KEY (entry);


--
-- TOC entry 3406 (class 2606 OID 36099)
-- Dependencies: 1745 1745 1745
-- Name: items_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY items_localized
    ADD CONSTRAINT items_localized_pkey PRIMARY KEY (entry, language_code);


--
-- TOC entry 3404 (class 2606 OID 36090)
-- Dependencies: 1744 1744
-- Name: items_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY items
    ADD CONSTRAINT items_pkey PRIMARY KEY (entry);


--
-- TOC entry 3408 (class 2606 OID 36121)
-- Dependencies: 1747 1747
-- Name: loot_creatures_index_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_creatures
    ADD CONSTRAINT loot_creatures_index_key UNIQUE (index);


--
-- TOC entry 3410 (class 2606 OID 36119)
-- Dependencies: 1747 1747 1747
-- Name: loot_creatures_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_creatures
    ADD CONSTRAINT loot_creatures_pkey PRIMARY KEY (entryid, itemid);


--
-- TOC entry 3412 (class 2606 OID 36148)
-- Dependencies: 1749 1749
-- Name: loot_fishing_index_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_fishing
    ADD CONSTRAINT loot_fishing_index_key UNIQUE (index);


--
-- TOC entry 3414 (class 2606 OID 36146)
-- Dependencies: 1749 1749 1749
-- Name: loot_fishing_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_fishing
    ADD CONSTRAINT loot_fishing_pkey PRIMARY KEY (itemid, entryid);


--
-- TOC entry 3416 (class 2606 OID 36172)
-- Dependencies: 1751 1751
-- Name: loot_gameobjects_index_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_gameobjects
    ADD CONSTRAINT loot_gameobjects_index_key UNIQUE (index);


--
-- TOC entry 3418 (class 2606 OID 36170)
-- Dependencies: 1751 1751 1751
-- Name: loot_gameobjects_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_gameobjects
    ADD CONSTRAINT loot_gameobjects_pkey PRIMARY KEY (entryid, itemid);


--
-- TOC entry 3420 (class 2606 OID 36201)
-- Dependencies: 1753 1753
-- Name: loot_items_index_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_items
    ADD CONSTRAINT loot_items_index_key UNIQUE (index);


--
-- TOC entry 3422 (class 2606 OID 36199)
-- Dependencies: 1753 1753 1753
-- Name: loot_items_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_items
    ADD CONSTRAINT loot_items_pkey PRIMARY KEY (entryid, itemid);


--
-- TOC entry 3424 (class 2606 OID 36225)
-- Dependencies: 1755 1755
-- Name: loot_pickpocketing_index_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_pickpocketing
    ADD CONSTRAINT loot_pickpocketing_index_key UNIQUE (index);


--
-- TOC entry 3426 (class 2606 OID 36223)
-- Dependencies: 1755 1755 1755
-- Name: loot_pickpocketing_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_pickpocketing
    ADD CONSTRAINT loot_pickpocketing_pkey PRIMARY KEY (entryid, itemid);


--
-- TOC entry 3428 (class 2606 OID 36249)
-- Dependencies: 1757 1757
-- Name: loot_skinning_index_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_skinning
    ADD CONSTRAINT loot_skinning_index_key UNIQUE (index);


--
-- TOC entry 3430 (class 2606 OID 36247)
-- Dependencies: 1757 1757 1757
-- Name: loot_skinning_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY loot_skinning
    ADD CONSTRAINT loot_skinning_pkey PRIMARY KEY (itemid, entryid);


--
-- TOC entry 3432 (class 2606 OID 36254)
-- Dependencies: 1758 1758
-- Name: map_checkpoint_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY map_checkpoint
    ADD CONSTRAINT map_checkpoint_pkey PRIMARY KEY (entry);


--
-- TOC entry 3434 (class 2606 OID 36263)
-- Dependencies: 1759 1759
-- Name: npc_gossip_textid_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY npc_gossip_textid
    ADD CONSTRAINT npc_gossip_textid_pkey PRIMARY KEY (creatureid);


--
-- TOC entry 3436 (class 2606 OID 36280)
-- Dependencies: 1760 1760 1760
-- Name: npc_monstersay_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY npc_monstersay
    ADD CONSTRAINT npc_monstersay_pkey PRIMARY KEY (entry, event);


--
-- TOC entry 3440 (class 2606 OID 36422)
-- Dependencies: 1762 1762 1762
-- Name: npc_text_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY npc_text_localized
    ADD CONSTRAINT npc_text_localized_pkey PRIMARY KEY (entry, language_code);


--
-- TOC entry 3438 (class 2606 OID 36410)
-- Dependencies: 1761 1761
-- Name: npc_text_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY npc_text
    ADD CONSTRAINT npc_text_pkey PRIMARY KEY (entry);


--
-- TOC entry 3442 (class 2606 OID 36479)
-- Dependencies: 1765 1765
-- Name: playercreateinfo_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY playercreateinfo
    ADD CONSTRAINT playercreateinfo_pkey PRIMARY KEY (index);


--
-- TOC entry 3444 (class 2606 OID 36529)
-- Dependencies: 1770 1770 1770
-- Name: professiondiscoveries_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY professiondiscoveries
    ADD CONSTRAINT professiondiscoveries_pkey PRIMARY KEY (spellid, spelltodiscover);


--
-- TOC entry 3448 (class 2606 OID 36836)
-- Dependencies: 1772 1772 1772
-- Name: quests_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY quests_localized
    ADD CONSTRAINT quests_localized_pkey PRIMARY KEY (entry, language_code);


--
-- TOC entry 3446 (class 2606 OID 36785)
-- Dependencies: 1771 1771
-- Name: quests_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY quests
    ADD CONSTRAINT quests_pkey PRIMARY KEY (entry);


--
-- TOC entry 3450 (class 2606 OID 36893)
-- Dependencies: 1774 1774
-- Name: recall_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY recall
    ADD CONSTRAINT recall_pkey PRIMARY KEY (id);


--
-- TOC entry 3452 (class 2606 OID 36898)
-- Dependencies: 1775 1775 1775 1775
-- Name: reputation_creature_onkill_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY reputation_creature_onkill
    ADD CONSTRAINT reputation_creature_onkill_pkey PRIMARY KEY (creature_id, faction_change_horde, faction_change_alliance);


--
-- TOC entry 3454 (class 2606 OID 36903)
-- Dependencies: 1776 1776 1776 1776
-- Name: reputation_faction_onkill_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY reputation_faction_onkill
    ADD CONSTRAINT reputation_faction_onkill_pkey PRIMARY KEY (faction_id, change_factionid_horde, change_factionid_alliance);


--
-- TOC entry 3456 (class 2606 OID 36908)
-- Dependencies: 1777 1777 1777 1777
-- Name: reputation_instance_onkill_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY reputation_instance_onkill
    ADD CONSTRAINT reputation_instance_onkill_pkey PRIMARY KEY (mapid, faction_change_horde, faction_change_alliance);


--
-- TOC entry 3458 (class 2606 OID 36932)
-- Dependencies: 1781 1781
-- Name: spell_effects_override_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY spell_effects_override
    ADD CONSTRAINT spell_effects_override_pkey PRIMARY KEY (spellid);


--
-- TOC entry 3495 (class 2606 OID 37256)
-- Dependencies: 1805 1805
-- Name: spell_proc_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY spell_proc
    ADD CONSTRAINT spell_proc_pkey PRIMARY KEY (spellid);


--
-- TOC entry 3460 (class 2606 OID 36957)
-- Dependencies: 1782 1782 1782
-- Name: spelloverride_overrideid_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY spelloverride
    ADD CONSTRAINT spelloverride_overrideid_key UNIQUE (overrideid, spellid);


--
-- TOC entry 3462 (class 2606 OID 36971)
-- Dependencies: 1783 1783
-- Name: teleport_coords_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY teleport_coords
    ADD CONSTRAINT teleport_coords_pkey PRIMARY KEY (id);


--
-- TOC entry 3464 (class 2606 OID 36984)
-- Dependencies: 1784 1784
-- Name: totemspells_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY totemspells
    ADD CONSTRAINT totemspells_pkey PRIMARY KEY (spell);


--
-- TOC entry 3466 (class 2606 OID 37005)
-- Dependencies: 1785 1785
-- Name: trainer_defs_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY trainer_defs
    ADD CONSTRAINT trainer_defs_pkey PRIMARY KEY (entry);


--
-- TOC entry 3468 (class 2606 OID 37029)
-- Dependencies: 1786 1786 1786 1786
-- Name: trainer_spells_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY trainer_spells
    ADD CONSTRAINT trainer_spells_pkey PRIMARY KEY (entry, cast_spell, learn_spell);


--
-- TOC entry 3470 (class 2606 OID 37050)
-- Dependencies: 1787 1787
-- Name: trainerspelloverride_spellid_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY trainerspelloverride
    ADD CONSTRAINT trainerspelloverride_spellid_key UNIQUE (spellid);


--
-- TOC entry 3472 (class 2606 OID 37067)
-- Dependencies: 1789 1789
-- Name: transport_data_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY transport_data
    ADD CONSTRAINT transport_data_pkey PRIMARY KEY (entry);


--
-- TOC entry 3475 (class 2606 OID 37076)
-- Dependencies: 1790 1790
-- Name: unit_display_sizes_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY unit_display_sizes
    ADD CONSTRAINT unit_display_sizes_pkey PRIMARY KEY (displayid);


--
-- TOC entry 3477 (class 2606 OID 37092)
-- Dependencies: 1791 1791
-- Name: vendor_restrictions_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY vendor_restrictions
    ADD CONSTRAINT vendor_restrictions_pkey PRIMARY KEY (entry);


--
-- TOC entry 3479 (class 2606 OID 37105)
-- Dependencies: 1792 1792 1792
-- Name: vendors_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY vendors
    ADD CONSTRAINT vendors_pkey PRIMARY KEY (entry, item);


--
-- TOC entry 3481 (class 2606 OID 37124)
-- Dependencies: 1793 1793
-- Name: weather_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY weather
    ADD CONSTRAINT weather_pkey PRIMARY KEY (zoneid);


--
-- TOC entry 3485 (class 2606 OID 37152)
-- Dependencies: 1798 1798
-- Name: worldbroadcast_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY worldbroadcast_localized
    ADD CONSTRAINT worldbroadcast_localized_pkey PRIMARY KEY (entry);


--
-- TOC entry 3483 (class 2606 OID 37147)
-- Dependencies: 1797 1797
-- Name: worldbroadcast_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY worldbroadcast
    ADD CONSTRAINT worldbroadcast_pkey PRIMARY KEY (entry);


--
-- TOC entry 3489 (class 2606 OID 37194)
-- Dependencies: 1800 1800
-- Name: worldmap_info_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY worldmap_info_localized
    ADD CONSTRAINT worldmap_info_localized_pkey PRIMARY KEY (entry);


--
-- TOC entry 3487 (class 2606 OID 37189)
-- Dependencies: 1799 1799
-- Name: worldmap_info_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY worldmap_info
    ADD CONSTRAINT worldmap_info_pkey PRIMARY KEY (entry);


--
-- TOC entry 3493 (class 2606 OID 37207)
-- Dependencies: 1803 1803
-- Name: worldstring_tables_localized_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY worldstring_tables_localized
    ADD CONSTRAINT worldstring_tables_localized_pkey PRIMARY KEY (entry);


--
-- TOC entry 3491 (class 2606 OID 37202)
-- Dependencies: 1802 1802
-- Name: worldstring_tables_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY worldstring_tables
    ADD CONSTRAINT worldstring_tables_pkey PRIMARY KEY (entry);


--
-- TOC entry 3473 (class 1259 OID 37077)
-- Dependencies: 1790
-- Name: unit_display_sizes_displayid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX unit_display_sizes_displayid_idx ON unit_display_sizes USING btree (displayid);


--
-- TOC entry 3502 (class 0 OID 0)
-- Dependencies: 3
-- Name: public; Type: ACL; Schema: -; Owner: -
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2009-12-15 21:45:51

--
-- PostgreSQL database dump complete
--

