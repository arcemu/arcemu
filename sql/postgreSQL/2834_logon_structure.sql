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
    acct integer NOT NULL,
    login text NOT NULL,
    password text NOT NULL,
    encrypted_password text NOT NULL,
    gm text NOT NULL,
    banned integer NOT NULL,
    lastlogin timestamp without time zone NOT NULL,
    lastip text NOT NULL,
    email text NOT NULL,
    flags numeric(3,0) DEFAULT 0 NOT NULL,
    forcelanguage text NOT NULL,
    muted integer DEFAULT 0 NOT NULL,
    banreason text
);


ALTER TABLE public.accounts OWNER TO arcemu;

--
-- Name: accounts_acct_seq; Type: SEQUENCE; Schema: public; Owner: arcemu
--

CREATE SEQUENCE accounts_acct_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.accounts_acct_seq OWNER TO arcemu;

--
-- Name: accounts_acct_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: arcemu
--

ALTER SEQUENCE accounts_acct_seq OWNED BY accounts.acct;


--
-- Name: ipbans; Type: TABLE; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE TABLE ipbans (
    ip text NOT NULL,
    expire integer NOT NULL,
    banreason text
);


ALTER TABLE public.ipbans OWNER TO arcemu;

--
-- Name: acct; Type: DEFAULT; Schema: public; Owner: arcemu
--

ALTER TABLE accounts ALTER COLUMN acct SET DEFAULT nextval('accounts_acct_seq'::regclass);


--
-- Name: pk_accounts; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY accounts
    ADD CONSTRAINT pk_accounts PRIMARY KEY (acct);


--
-- Name: pk_ipbans; Type: CONSTRAINT; Schema: public; Owner: arcemu; Tablespace: 
--

ALTER TABLE ONLY ipbans
    ADD CONSTRAINT pk_ipbans PRIMARY KEY (ip);


--
-- Name: accounts_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX accounts_a ON accounts USING btree (login);


--
-- Name: ipbans_a; Type: INDEX; Schema: public; Owner: arcemu; Tablespace: 
--

CREATE UNIQUE INDEX ipbans_a ON ipbans USING btree (ip);


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

