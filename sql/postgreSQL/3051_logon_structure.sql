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
-- Name: accounts; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE accounts (
    acct bigint NOT NULL,
    login character varying(32) NOT NULL,
    password character varying(32) NOT NULL,
    encrypted_password character varying(42) NOT NULL,
    gm character varying(32) NOT NULL,
    banned bigint NOT NULL,
    lastlogin timestamp without time zone DEFAULT '1971-01-01 00:00:00'::timestamp without time zone NOT NULL,
    lastip character varying(16) NOT NULL,
    email character varying(64) NOT NULL,
    flags smallint DEFAULT (0)::smallint NOT NULL,
    forcelanguage character varying(5) DEFAULT 'enUS'::character varying NOT NULL,
    muted integer DEFAULT 0 NOT NULL,
    banreason character varying(255)
);


ALTER TABLE public.accounts OWNER TO arcemu;

--
-- Name: COLUMN accounts.acct; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.acct IS 'Unique ID';


--
-- Name: COLUMN accounts.login; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.login IS 'Login username';


--
-- Name: COLUMN accounts.password; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.password IS 'Login password';


--
-- Name: COLUMN accounts.gm; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.gm IS 'Game permissions';


--
-- Name: COLUMN accounts.lastlogin; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.lastlogin IS 'Last login timestamp';


--
-- Name: COLUMN accounts.lastip; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.lastip IS 'Last remote address';


--
-- Name: COLUMN accounts.email; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.email IS 'Contact e-mail address';


--
-- Name: COLUMN accounts.flags; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN accounts.flags IS 'Client flags';


--
-- Name: ipbans; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE ipbans (
    ip character varying(20) NOT NULL,
    expire integer NOT NULL,
    banreason character varying(255)
);


ALTER TABLE public.ipbans OWNER TO arcemu;

--
-- Name: COLUMN ipbans.expire; Type: COMMENT; Schema: public; Owner: arcemu
--

COMMENT ON COLUMN ipbans.expire IS 'Expiry time (s)';


--
-- Name: a; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY accounts
    ADD CONSTRAINT a UNIQUE (login);


--
-- Name: accounts_pkey; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY accounts
    ADD CONSTRAINT accounts_pkey PRIMARY KEY (acct);


--
-- Name: ipbans_ip_key; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY ipbans
    ADD CONSTRAINT ipbans_ip_key UNIQUE (ip);


--
-- Name: ipbans_pkey; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY ipbans
    ADD CONSTRAINT ipbans_pkey PRIMARY KEY (ip);


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

