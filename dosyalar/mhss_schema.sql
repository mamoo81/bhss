--
-- PostgreSQL database dump
--


-- Dumped from database version 13.8 (Debian 13.8-0+deb11u1)
-- Dumped by pg_dump version 13.8 (Debian 13.8-0+deb11u1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: borchesaplamaturleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.borchesaplamaturleri (
    id integer NOT NULL,
    ad character varying NOT NULL
);


--
-- Name: borchesaplamaturleri_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.borchesaplamaturleri_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: borchesaplamaturleri_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.borchesaplamaturleri_id_seq OWNED BY public.borchesaplamaturleri.id;


--
-- Name: carikartlar_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.carikartlar_id_seq
    START WITH 2
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: carikartlar; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.carikartlar (
    id bigint DEFAULT nextval('public.carikartlar_id_seq'::regclass) NOT NULL,
    ad text NOT NULL,
    tip bigint NOT NULL,
    vergi_no text NOT NULL,
    vergi_daire text NOT NULL,
    il text,
    ilce text,
    adres text,
    mail text,
    telefon character varying(11) DEFAULT 0,
    tarih date DEFAULT CURRENT_TIMESTAMP NOT NULL,
    aciklama text,
    yetkili text,
    borc_hesaplama integer
);


--
-- Name: carikartlar_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.carikartlar_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: carikartlar_tip_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.carikartlar_tip_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: carikartlar_tip_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.carikartlar_tip_seq OWNED BY public.carikartlar.tip;


--
-- Name: caritipleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.caritipleri (
    id bigint NOT NULL,
    tip text NOT NULL
);


--
-- Name: caritipleri_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.caritipleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: caritipleri_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.caritipleri_id_seq OWNED BY public.caritipleri.id;


--
-- Name: caritipleri_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.caritipleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: caritipleri_sequence; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.caritipleri_sequence OWNED BY public.caritipleri.id;


--
-- Name: faturalar; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.faturalar (
    id bigint NOT NULL,
    fatura_no text NOT NULL,
    kullanici bigint NOT NULL,
    durum boolean DEFAULT false,
    toplamtutar numeric(18,3) DEFAULT 0 NOT NULL,
    odenentutar numeric(18,3) DEFAULT 0 NOT NULL,
    kalantutar numeric(18,3) DEFAULT 0 NOT NULL,
    cari bigint NOT NULL,
    tarih timestamp without time zone NOT NULL,
    evrakno text,
    odemetipi bigint NOT NULL,
    tipi bigint NOT NULL,
    aciklama text
);


--
-- Name: faturalar_cari_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturalar_cari_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturalar_cari_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.faturalar_cari_seq OWNED BY public.faturalar.cari;


--
-- Name: faturalar_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturalar_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturalar_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.faturalar_id_seq OWNED BY public.faturalar.id;


--
-- Name: faturalar_kullanici_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturalar_kullanici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturalar_kullanici_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.faturalar_kullanici_seq OWNED BY public.faturalar.kullanici;


--
-- Name: faturalar_odemetipi_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturalar_odemetipi_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturalar_odemetipi_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.faturalar_odemetipi_seq OWNED BY public.faturalar.odemetipi;


--
-- Name: faturalar_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturalar_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturalar_tipi_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturalar_tipi_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturalar_tipi_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.faturalar_tipi_seq OWNED BY public.faturalar.tipi;


--
-- Name: faturatipleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.faturatipleri (
    id bigint NOT NULL,
    tip text NOT NULL
);


--
-- Name: faturatipleri_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturatipleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturatipleri_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.faturatipleri_id_seq OWNED BY public.faturatipleri.id;


--
-- Name: faturatipleri_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.faturatipleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: faturatipleri_sequence; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.faturatipleri_sequence OWNED BY public.faturatipleri.id;


--
-- Name: ilceler; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ilceler (
    id bigint NOT NULL,
    ilid bigint NOT NULL,
    ilce text NOT NULL
);


--
-- Name: ilceler_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.ilceler_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: ilceler_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.ilceler_id_seq OWNED BY public.ilceler.id;


--
-- Name: ilceler_ilid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.ilceler_ilid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: ilceler_ilid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.ilceler_ilid_seq OWNED BY public.ilceler.ilid;


--
-- Name: iller; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.iller (
    id bigint NOT NULL,
    il text NOT NULL
);


--
-- Name: iller_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.iller_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: iller_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.iller_id_seq OWNED BY public.iller.id;


--
-- Name: kasa; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.kasa (
    id bigint NOT NULL,
    para numeric(18,3) NOT NULL
);


--
-- Name: kasa_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.kasa_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: kasa_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.kasa_id_seq OWNED BY public.kasa.id;


--
-- Name: kasahareketadlari; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.kasahareketadlari (
    id integer NOT NULL,
    ad character varying NOT NULL
);


--
-- Name: kasahareketadlari_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.kasahareketadlari_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: kasahareketadlari_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.kasahareketadlari_id_seq OWNED BY public.kasahareketadlari.id;


--
-- Name: kasahareketleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.kasahareketleri (
    id bigint NOT NULL,
    miktar money NOT NULL,
    kullanici bigint NOT NULL,
    islem integer,
    tarih timestamp without time zone NOT NULL,
    kar money,
    evrakno text,
    aciklama text
);


--
-- Name: kasahareketleri_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.kasahareketleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: kasahareketleri_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.kasahareketleri_id_seq OWNED BY public.kasahareketleri.id;


--
-- Name: kasahareketleri_kullanici_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.kasahareketleri_kullanici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: kasahareketleri_kullanici_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.kasahareketleri_kullanici_seq OWNED BY public.kasahareketleri.kullanici;


--
-- Name: kasahareketleri_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.kasahareketleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: kullanicilar; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.kullanicilar (
    id bigint NOT NULL,
    username character varying(32) NOT NULL,
    password character varying(32) NOT NULL,
    ad text,
    soyad text,
    cepno character varying(11),
    tarih timestamp without time zone NOT NULL,
    kasayetki boolean DEFAULT false,
    iadeyetki boolean DEFAULT false,
    stokyetki boolean DEFAULT false,
    cariyetki boolean,
    ayaryetki boolean
);


--
-- Name: kullanicilar_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.kullanicilar_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: kullanicilar_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.kullanicilar_id_seq OWNED BY public.kullanicilar.id;


--
-- Name: kullanicilar_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.kullanicilar_sequence
    START WITH 2
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: odemetipleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.odemetipleri (
    id bigint NOT NULL,
    tip text NOT NULL
);


--
-- Name: odemetipleri_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.odemetipleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: odemetipleri_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.odemetipleri_id_seq OWNED BY public.odemetipleri.id;


--
-- Name: odemetipleri_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.odemetipleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: odemetipleri_sequence; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.odemetipleri_sequence OWNED BY public.odemetipleri.id;


--
-- Name: oturum; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.oturum (
    id bigint NOT NULL,
    username text NOT NULL,
    giristarihi timestamp without time zone NOT NULL
);


--
-- Name: oturum_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.oturum_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: oturum_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.oturum_id_seq OWNED BY public.oturum.id;


--
-- Name: stokbirimleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.stokbirimleri (
    id bigint NOT NULL,
    birim text NOT NULL
);


--
-- Name: stokbirimleri_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokbirimleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokbirimleri_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokbirimleri_id_seq OWNED BY public.stokbirimleri.id;


--
-- Name: stokbirimleri_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokbirimleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokgruplari; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.stokgruplari (
    id bigint NOT NULL,
    grup text NOT NULL
);


--
-- Name: stokgruplari_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokgruplari_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokgruplari_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokgruplari_id_seq OWNED BY public.stokgruplari.id;


--
-- Name: stokgruplari_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokgruplari_sequence
    START WITH 100
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokgruplari_sequence; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokgruplari_sequence OWNED BY public.stokgruplari.id;


--
-- Name: stokhareketleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.stokhareketleri (
    barkod character varying(13) NOT NULL,
    islem_no character varying,
    islem_turu character varying NOT NULL,
    islem_miktari numeric(18,3) NOT NULL,
    tarih timestamp without time zone NOT NULL,
    kullanici bigint NOT NULL,
    aciklama text,
    birim_f money,
    toplam_f money,
    kdv integer
);


--
-- Name: stokhareketleri_kullanici_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokhareketleri_kullanici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokhareketleri_kullanici_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokhareketleri_kullanici_seq OWNED BY public.stokhareketleri.kullanici;


--
-- Name: stokkartlari; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.stokkartlari (
    id bigint NOT NULL,
    barkod character varying(13) NOT NULL,
    ad text NOT NULL,
    birim integer NOT NULL,
    miktar numeric(18,3) NOT NULL,
    grup integer NOT NULL,
    afiyat money NOT NULL,
    sfiyat money NOT NULL,
    kdv integer NOT NULL,
    tarih timestamp without time zone NOT NULL,
    aciklama text,
    kdvdahil boolean DEFAULT false,
    otv integer,
    otvdahil boolean DEFAULT false,
    kod text,
    tedarikci bigint,
    uretici bigint,
    mensei text
);


--
-- Name: stokkartlari_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokkartlari_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokkartlari_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokkartlari_id_seq OWNED BY public.stokkartlari.id;


--
-- Name: stokkartlari_kdv_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokkartlari_kdv_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokkartlari_kdv_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokkartlari_kdv_seq OWNED BY public.stokkartlari.kdv;


--
-- Name: stokkartlari_sequence; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokkartlari_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokkartlari_tedarikci_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokkartlari_tedarikci_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokkartlari_tedarikci_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokkartlari_tedarikci_seq OWNED BY public.stokkartlari.tedarikci;


--
-- Name: stokkartlari_uretici_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.stokkartlari_uretici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: stokkartlari_uretici_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.stokkartlari_uretici_seq OWNED BY public.stokkartlari.uretici;


--
-- Name: teraziler; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.teraziler (
    id bigint NOT NULL,
    marka text NOT NULL
);


--
-- Name: teraziler_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.teraziler_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: teraziler_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.teraziler_id_seq OWNED BY public.teraziler.id;


--
-- Name: terazimodel; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.terazimodel (
    id bigint NOT NULL,
    model text NOT NULL
);


--
-- Name: terazimodel_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.terazimodel_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: terazimodel_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.terazimodel_id_seq OWNED BY public.terazimodel.id;


--
-- Name: ureticiler; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.ureticiler (
    id bigint NOT NULL,
    ad text NOT NULL
);


--
-- Name: ureticiler_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.ureticiler_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: ureticiler_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.ureticiler_id_seq OWNED BY public.ureticiler.id;


--
-- Name: vergidaireleri; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.vergidaireleri (
    id bigint NOT NULL,
    ad text NOT NULL,
    kod text NOT NULL,
    il bigint NOT NULL,
    ilce text NOT NULL
);


--
-- Name: vergidaireleri_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.vergidaireleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: vergidaireleri_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.vergidaireleri_id_seq OWNED BY public.vergidaireleri.id;


--
-- Name: vergidaireleri_il_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.vergidaireleri_il_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: vergidaireleri_il_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.vergidaireleri_il_seq OWNED BY public.vergidaireleri.il;


--
-- Name: borchesaplamaturleri id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.borchesaplamaturleri ALTER COLUMN id SET DEFAULT nextval('public.borchesaplamaturleri_id_seq'::regclass);


--
-- Name: carikartlar tip; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.carikartlar ALTER COLUMN tip SET DEFAULT nextval('public.carikartlar_tip_seq'::regclass);


--
-- Name: caritipleri id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.caritipleri ALTER COLUMN id SET DEFAULT nextval('public.caritipleri_id_seq'::regclass);


--
-- Name: faturalar id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturalar ALTER COLUMN id SET DEFAULT nextval('public.faturalar_id_seq'::regclass);


--
-- Name: faturalar kullanici; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturalar ALTER COLUMN kullanici SET DEFAULT nextval('public.faturalar_kullanici_seq'::regclass);


--
-- Name: faturalar cari; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturalar ALTER COLUMN cari SET DEFAULT nextval('public.faturalar_cari_seq'::regclass);


--
-- Name: faturalar odemetipi; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturalar ALTER COLUMN odemetipi SET DEFAULT nextval('public.faturalar_odemetipi_seq'::regclass);


--
-- Name: faturalar tipi; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturalar ALTER COLUMN tipi SET DEFAULT nextval('public.faturalar_tipi_seq'::regclass);


--
-- Name: faturatipleri id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturatipleri ALTER COLUMN id SET DEFAULT nextval('public.faturatipleri_id_seq'::regclass);


--
-- Name: ilceler id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ilceler ALTER COLUMN id SET DEFAULT nextval('public.ilceler_id_seq'::regclass);


--
-- Name: ilceler ilid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ilceler ALTER COLUMN ilid SET DEFAULT nextval('public.ilceler_ilid_seq'::regclass);


--
-- Name: iller id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.iller ALTER COLUMN id SET DEFAULT nextval('public.iller_id_seq'::regclass);


--
-- Name: kasa id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kasa ALTER COLUMN id SET DEFAULT nextval('public.kasa_id_seq'::regclass);


--
-- Name: kasahareketadlari id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kasahareketadlari ALTER COLUMN id SET DEFAULT nextval('public.kasahareketadlari_id_seq'::regclass);


--
-- Name: kasahareketleri id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kasahareketleri ALTER COLUMN id SET DEFAULT nextval('public.kasahareketleri_id_seq'::regclass);


--
-- Name: kasahareketleri kullanici; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kasahareketleri ALTER COLUMN kullanici SET DEFAULT nextval('public.kasahareketleri_kullanici_seq'::regclass);


--
-- Name: kullanicilar id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kullanicilar ALTER COLUMN id SET DEFAULT nextval('public.kullanicilar_id_seq'::regclass);


--
-- Name: odemetipleri id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.odemetipleri ALTER COLUMN id SET DEFAULT nextval('public.odemetipleri_id_seq'::regclass);


--
-- Name: oturum id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.oturum ALTER COLUMN id SET DEFAULT nextval('public.oturum_id_seq'::regclass);


--
-- Name: stokbirimleri id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokbirimleri ALTER COLUMN id SET DEFAULT nextval('public.stokbirimleri_id_seq'::regclass);


--
-- Name: stokgruplari id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokgruplari ALTER COLUMN id SET DEFAULT nextval('public.stokgruplari_id_seq'::regclass);


--
-- Name: stokhareketleri kullanici; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokhareketleri ALTER COLUMN kullanici SET DEFAULT nextval('public.stokhareketleri_kullanici_seq'::regclass);


--
-- Name: stokkartlari id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokkartlari ALTER COLUMN id SET DEFAULT nextval('public.stokkartlari_id_seq'::regclass);


--
-- Name: stokkartlari kdv; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokkartlari ALTER COLUMN kdv SET DEFAULT nextval('public.stokkartlari_kdv_seq'::regclass);


--
-- Name: stokkartlari tedarikci; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokkartlari ALTER COLUMN tedarikci SET DEFAULT nextval('public.stokkartlari_tedarikci_seq'::regclass);


--
-- Name: stokkartlari uretici; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokkartlari ALTER COLUMN uretici SET DEFAULT nextval('public.stokkartlari_uretici_seq'::regclass);


--
-- Name: teraziler id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.teraziler ALTER COLUMN id SET DEFAULT nextval('public.teraziler_id_seq'::regclass);


--
-- Name: terazimodel id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.terazimodel ALTER COLUMN id SET DEFAULT nextval('public.terazimodel_id_seq'::regclass);


--
-- Name: ureticiler id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ureticiler ALTER COLUMN id SET DEFAULT nextval('public.ureticiler_id_seq'::regclass);


--
-- Name: vergidaireleri id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.vergidaireleri ALTER COLUMN id SET DEFAULT nextval('public.vergidaireleri_id_seq'::regclass);


--
-- Name: vergidaireleri il; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.vergidaireleri ALTER COLUMN il SET DEFAULT nextval('public.vergidaireleri_il_seq'::regclass);


--
-- Data for Name: borchesaplamaturleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.borchesaplamaturleri (id, ad) FROM stdin;
1	NORMAL
2	GÜNCEL FİYATTAN
\.


--
-- Data for Name: carikartlar; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.carikartlar (id, ad, tip, vergi_no, vergi_daire, il, ilce, adres, mail, telefon, tarih, aciklama, yetkili, borc_hesaplama) FROM stdin;
1	DİREKT	1								2021-12-04		\N	\N
\.


--
-- Data for Name: caritipleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.caritipleri (id, tip) FROM stdin;
1	MÜŞTERİ
2	TOPTANCI
3	PERSONEL
\.


--
-- Data for Name: faturalar; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.faturalar (id, fatura_no, kullanici, durum, toplamtutar, odenentutar, kalantutar, cari, tarih, evrakno, odemetipi, tipi, aciklama) FROM stdin;
\.


--
-- Data for Name: faturatipleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.faturatipleri (id, tip) FROM stdin;
1	ALIŞ
2	SATIŞ
3	İADE
4	ÖDEME
5	TAHSİLAT
\.


--
-- Data for Name: ilceler; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.ilceler (id, ilid, ilce) FROM stdin;
1	1	ALADAĞ
2	1	CEYHAN
3	1	ÇUKUROVA
4	1	FEKE
5	1	İMAMOĞLU
6	1	KARAİSALI
7	1	KARATAŞ
8	1	KOZAN
9	1	POZANTI
10	1	SAİMBEYLİ
11	1	SARIÇAM
12	1	SEYHAN
13	1	TUFANBEYLİ
14	1	YUMURTALIK
15	1	YÜREĞİR
16	2	BESNİ
17	2	ÇELİKHAN
18	2	GERGER
19	2	GÖLBAŞI
20	2	KAHTA
21	2	MERKEZ
22	2	SAMSAT
23	2	SİNCİK
24	2	TUT
25	3	BAŞMAKCI
26	3	BAYAT
27	3	BOLDAVİN
28	3	ÇAY
29	3	ÇOBANLAR
30	3	DAZKIRI
31	3	DİNAR
32	3	EMİRDAĞ
33	3	EVCİLER
34	3	HOCALAR
35	3	İHSANİYE
36	3	İSCEHİSAR
37	3	KIZILÖREN
38	3	MERKEZ
39	3	SANDIKLI
40	3	SİNANPAŞA
41	3	SULTANDAĞI
42	3	ŞUHUT
43	4	DİYADİN
44	4	DOĞUBAYAZIT
45	4	ELEŞKİRT
46	4	HAMUR
47	4	MERKEZ
48	4	PATNOS
49	4	TAŞLIÇAY
50	4	TUTAK
51	5	GÖYNÜCEK
52	5	GÜMÜŞHACIKÖY
53	5	HAMAMÖZÜ
54	5	MERKEZ
55	5	ORTAKÖY
56	5	SARIYAHŞİ
57	5	SULTANHANI
58	6	AKYURT
59	6	ALTINDAĞ
60	6	AYAŞ
61	6	BALA
62	6	BEYPAZARI
63	6	ÇAMLIDERE
64	6	ÇANKAYA
65	6	ÇUBUK
66	6	ELMADAĞ
67	6	ETİMESGUT
68	6	EVREN
69	6	GÖLBAŞI
70	6	GÜDÜL
71	6	HAYMANA
72	6	KAHRAMANKAZAN
73	6	KALECİK
74	6	KEÇİÖREN
75	6	KIZILCAHAMAM
76	6	MAMAK
77	6	NALLIHAN
78	6	POLATLI
79	6	PURSAKLAR
80	6	SİNCAN
81	6	ŞEREFLİKOÇHİSAR
82	6	YENİMAHALLE
83	7	AKSEKİ
84	7	AKSU
85	7	ALANYA
86	7	DEMRE
87	7	DÖŞEMEALTI
88	7	ELMALI
89	7	FİNİKE
90	7	GAZİPAŞA
91	7	GÜNDOĞMUŞ
92	7	İBRADI
93	7	KAŞ
94	7	KEMER
95	7	KEPEZ
96	7	KONYAALTI
97	7	KORKUTELİ
98	7	KUMLUCA
99	7	MANAVGAT
100	7	MURATPAŞA
101	7	SERİK
102	8	ARDANUÇ
103	8	ARHAVİ
104	8	BORÇKA
105	8	HOPA
106	8	KEMALPAŞA
107	8	MERKEZ
108	8	MURGUL
109	8	ŞAVŞAT
110	8	YUSUFELİ
111	9	BOZDOĞAN
112	9	BUHARKENT
113	9	ÇİNE
114	9	DİDİM
115	9	EFELER
116	9	GERMENCİK
117	9	İNCİRLİOVA
118	9	KARACASU
119	9	KARPUZLU
120	9	KOÇARLI
121	9	KÖŞK
122	9	KUŞADASI
123	9	KUYUCAK
124	9	NAZİLLİ
125	9	SÖKE
126	9	SULTANHİSAR
127	9	YENİPAZAR
128	10	ALTIEYLÜL
129	10	AYVALIK
130	10	BALYA
131	10	BANDIRMA
132	10	BİGADİÇ
133	10	BURHANİYE
134	10	DURSUNBEY
135	10	EDREMİT
136	10	ERDEK
137	10	GÖMEÇ
138	10	GÖNEN
139	10	HAVRAN
140	10	İVRİNDİ
141	10	KARESİ
142	10	KEPSUT
143	10	MANYAS
144	10	MARMARA
145	10	SAVAŞTEPE
146	10	SINDIRGI
147	10	SUSURLUK
148	11	BOZÜYÜK
149	11	GÖLPAZARI
150	11	İNHİSAR
151	11	MERKEZ
152	11	OSMANELİ
153	11	PAZARYERİ
154	11	SÖĞÜT
155	11	YENİPAZAR
156	12	ADAKLI
157	12	GENÇ
158	12	KARLIOVA
159	12	KİĞI
160	12	MERKEZ
161	12	SOLHAN
162	12	YAYLADERE
163	12	YEDİSU
164	13	ADİLCEVAZ
165	13	AHLAT
166	13	GÜROYMAK
167	13	HİZAN
168	13	MERKEZ
169	13	MUTKİ
170	13	TATVAN
180	14	DÖRTDİVAN
181	14	GEREDE
182	14	GÖYNÜK
183	14	KIBRISCIK
184	14	MENGEN
185	14	MERKEZ
186	14	MUDURNU
187	14	SEBEN
188	14	YENİÇAĞA
189	15	AĞLASUN
190	15	ALTINYAYLA
191	15	BUCAK
192	15	ÇAVDIR
193	15	ÇELTİKÇİ
194	15	GÖLHİSAR
195	15	KARAMANLI
196	15	KEMER
197	15	MERKEZ
198	15	TEFENNİ
199	15	YEŞİLOVA
200	16	BÜYÜKORHAN
201	16	GEMLİK
202	16	GÜRSU
203	16	HARMANCIK
204	16	İNEGÖL
205	16	İZNİK
206	16	KARACABEY
207	16	KELEŞ
208	16	KESTEL
209	16	MUDANYA
210	16	MUSTAFAKEMALPAŞA
211	16	NİLÜFER
212	16	ORHANELİ
213	16	ORHANGAZİ
214	16	OSMANGAZİ
215	16	YENİŞEHİR
216	16	YILDIRIM
217	17	AYVACIK
218	17	BAYRAMİÇ
219	17	BİGA
220	17	BOZCAADA
221	17	ÇAN
222	17	ECEABAT
223	17	EZİNE
224	17	GELİBOLU
225	17	GÖKÇEADA
226	17	LAPSEKİ
227	17	MERKEZ
228	17	YENİCE
229	18	ATKARACALAR
230	18	BAYRAMÖREN
231	18	ÇERKEŞ
232	18	ELDİVAN
233	18	ILGAZ
234	18	KIZILIRMAK
235	18	KORGUN
236	18	KURŞUNLU
237	18	MERKEZ
238	18	ORTA
239	18	ŞABANÖZÜ
240	18	YAPRAKLI
241	19	ALACA
242	19	BAYAT
243	19	BOĞAZKALE
244	19	DODURGA
245	19	İSKİLİP
246	19	KARGI
247	19	LAÇİN
248	19	MECİTÖZÜ
249	19	MERKEZ
250	19	OĞUZLAR
251	19	ORTAKÖY
252	19	OSMANCIK
253	19	SUNGURLU
254	19	UĞURLUDAĞ
255	20	ACIPAYAM
256	20	BABADAĞ
257	20	BAKLAN
258	20	BEKİLLİ
259	20	BEYAĞAÇ
260	20	BOZKURT
261	20	BULDAN
262	20	ÇAL
263	20	ÇAMELİ
264	20	ÇARDAK
265	20	ÇİVRİL
266	20	GÜNEY
267	20	HONAZ
268	20	KALE
269	20	MERKEZEFENDİ
270	20	PAMUKKALE
271	20	SARAYKÖY
272	20	SERİNHİSAR
273	20	TAVAS
274	21	BAĞLAR
275	21	BİSMİL
276	21	ÇERMİK
277	21	ÇINAR
278	21	ÇÜNGÜŞ
279	21	DİCLE
280	21	EĞİL
281	21	ERGANİ
282	21	HANİ
283	21	HAZRO
284	21	KAYAPINAR
285	21	KOCAKÖY
286	21	KULP
287	21	LİCE
288	21	SİLVAN
289	21	SUR
290	21	YENİŞEHİR
291	22	ENEZ
292	22	HAVSA
293	22	İPSALA
294	22	KEŞAN
295	22	LALAPAŞA
296	22	MERİÇ
297	22	MERKEZ
298	22	SÜLOĞLU
299	22	UZUNKÖPRÜ
300	23	AĞIN
301	23	ALACAKAYA
302	23	ARICAK
303	23	BASKİL
304	23	KARAKOÇAN
305	23	KEBAN
306	23	KOVANCILAR
307	23	MADEN
308	23	MERKEZ
309	23	PALU
310	23	SİVRİCE
311	24	ÇAYIRLI
312	24	İLİÇ
313	24	KEMAH
314	24	KEMALİYE
315	24	MERKEZ
316	24	OTLUKBEYİ
317	24	REFAHİYE
318	24	TERCAN
319	24	ÜZÜMLÜ
320	25	AŞKALE
321	25	AİZZİYE
322	25	ÇAT
323	25	HINIS
324	25	HORASAN
325	25	İSPİR
326	25	KARAÇOBAN
327	25	KARAYAZI
328	25	KÖPRÜKÖY
329	25	NARMAN
330	25	OLTU
331	25	OLUR
332	25	PALANDÖKEN
333	25	PASİNLER
334	25	PAZARYOLU
335	25	ŞENKAYA
336	25	TEKMAN
337	25	TORTUM
338	25	UZUNDERE
339	25	YAKUTİYE
340	26	ALPU
341	26	BEYLİKOVA
342	26	ÇİFTELER
343	26	GÜNYÜZÜ
344	26	HAN
345	26	İNÖNÜ
346	26	MAHMUDİYE
347	26	MİHALGAZİ
348	26	MİHALIÇÇIK
349	26	ODUNPAZARI
350	26	SARICAKAYA
351	26	SEYİTGAZİ
352	26	SİVRİHİSAR
353	26	TEPEBAŞI
354	27	ARABAN
355	27	İSLAHİYE
356	27	KARKAMIŞ
357	27	NİZİP
358	27	NURDAĞI
359	27	OĞUZELİ
360	27	ŞAHİNBEY
361	27	ŞEHİTKAMİL
362	27	YAVUZELİ
363	28	ALUCRA
364	28	BULANCAK
365	28	ÇAMOLUK
366	28	ÇANAKÇI
367	28	DERELİ
368	28	DOĞANKENT
369	28	ESPİYE
370	28	EYNESİL
371	28	GÖRELE
372	28	GÜCE
373	28	KEŞAP
374	28	MERKEZ
375	28	PİRAZİZ
376	28	ŞEBİNKARAHİSAR
377	28	TİREBOLU
378	28	YAĞLIDERE
379	29	KELKİT
380	29	KÖSE
381	29	KÜRTÜN
382	29	MERKEZ
383	29	ŞİRAN
384	29	TORUL
385	30	ÇUKURCA
386	30	DERECİK
387	30	MERKEZ
388	30	ŞEMDİNLİ
389	30	YÜKSEKOVA
390	31	ALTINÖZÜ
391	31	ANTAKYA
392	31	ARSUZ
393	31	BELEN
394	31	DEFNE
395	31	DÖRTYOL
396	31	ERZİN
397	31	HASSA
398	31	İSKENDERUN
399	31	KIRIKHAN
400	31	KUMLU
401	31	PAYAS
402	31	REYHANLI
403	31	SAMANDAĞ
404	31	YAYLADAĞI
405	32	AKSU
406	32	ATABEY
407	32	EĞİRDİR
408	32	GELENDOST
409	32	GÖNEN
410	32	KEÇİBORLU
411	32	MERKEZ
412	32	SENİRKENT
413	32	SÜTÇÜLER
414	32	ŞARKİKARAAĞAÇ
415	32	ULUBORLU
416	32	YALVAÇ
417	32	YENİŞARBADEMLİ
418	33	AKDENİZ
419	33	ANAMUR
420	33	AYDINCIK
421	33	BOZYAZI
422	33	ÇAMLIYAYLA
423	33	ERDEMLİ
424	33	GÜLNAR
425	33	MEZİTLİ
426	33	MUT
427	33	SİLİFKE
428	33	TARSUS
429	33	TOROSLAR
430	33	YENİŞEHİR
431	34	ADALAR
432	34	ARNAVUTKÖY
433	34	ATAŞEHİR
434	34	AVCILAR
435	34	BAĞCILAR
436	34	BAHÇELİEVLER
437	34	BAKIRKÖY
438	34	BAŞAKŞEHİR
439	34	BAYRAMPAŞA
440	34	BEŞİKTAŞ
441	34	BEYKOZ
442	34	BEYLİKDÜZÜ
443	34	BEYOĞLU
444	34	BÜYÜKÇEKMECE
445	34	ÇATALCA
446	34	ÇEKMEKÖY
447	34	ESENLER
448	34	ESENYURT
449	34	EYÜPSULTAN
450	34	FATİH
451	34	GAZİOSMANPAŞA
452	34	GÜNGÖREN
453	34	KADIKÖY
454	34	KAĞITHANE
455	34	KARTAL
456	34	KÜÇÜKÇEKMECE
457	34	MALTEPE
458	34	PENDİK
459	34	SANCAKTEPE
460	34	SARIYER
461	34	SİLİVRİ
462	34	SULTANBEYLİ
463	34	SULTANGAZİ
464	34	ŞİLE
465	34	ŞİŞLİ
466	34	TUZLA
467	34	ÜMRANİYE
468	34	ÜSKÜDAR
469	34	ZEYTİNBURNU
470	35	ALİAĞA
471	35	BALÇOVA
472	35	BAYINDIR
473	35	BAYRAKLI
474	35	BERGAMA
475	35	BEYDAĞ
476	35	BORNOVA
477	35	BUCA
478	35	ÇEŞME
479	35	ÇİĞLİ
480	35	DİKİLİ
481	35	FOÇA
482	35	GAZİEMİR
483	35	GÜZELBAHÇE
484	35	KARABAĞLAR
485	35	KARABURUN
486	35	KARŞIYAKA
487	35	KEMALPAŞA
488	35	KINIK
489	35	KİRAZ
490	35	KONAK
491	35	MENDERES
492	35	MENEMEN
493	35	NARLIDERE
494	35	ÖDEMİŞ
495	35	SEFERİHİSAR
496	35	SELÇUK
497	35	TİRE
498	35	TORBALI
499	35	URLA
500	36	AKYAKA
501	36	ARPAÇAY
502	36	DİGOR
503	36	KAĞIZMAN
504	36	MERKEZ
505	36	SARIKAMIŞ
506	36	SELİM
507	36	SUSUZ
508	37	ABANA
509	37	AĞLI
510	37	ARAÇ
511	37	AZDAVAY
512	37	BOZKURT
513	37	CİDE
514	37	ÇATALZEYTİN
515	37	DADAY
516	37	DEVREKANİ
517	37	DOĞANYURT
518	37	HANÖNÜ
519	37	İHSANGAZİ
520	37	İNEBOLU
521	37	KÜRE
522	37	MERKEZ
523	37	PINARBAŞI
524	37	SEYDİLER
525	37	ŞENPAZAR
526	37	TAŞKÖPRÜ
527	37	TOSYA
528	38	AKKIŞLA
529	38	BÜNYAN
530	38	DEVELİ
531	38	FELAHİYE
532	38	HACILAR
533	38	İNCESU
534	38	KOCASİNAN
535	38	MELİKGAZİ
536	38	ÖZVATAN
537	38	PINARBAŞI
538	38	SARIOĞLAN
539	38	SARIZ
540	38	TALAS
541	38	TOMARZA
542	38	YAHYALI
543	38	YEŞİLHİSAR
544	39	BABAESKİ
545	39	DEMİRKÖY
546	39	KOFÇAZ
547	39	LÜLEBURGAZ
548	39	MERKEZ
549	39	PEHLİVANKÖY
550	39	PINARHİSAR
551	39	VİZE
552	40	AKÇAKENT
553	40	AKPINAR
554	40	BOZTEPE
555	40	ÇİÇEKDAĞI
556	40	KAMAN
557	40	MERKEZ
558	40	MUCUR
559	41	BAŞİSKELE
560	41	ÇAYIROVA
561	41	DARICA
562	41	DERİNCE
563	41	DİLOVASI
564	41	GEBZE
565	41	GÖLCÜK
566	41	İZMİT
567	41	KANDIRA
568	41	KARAMÜRSEL
569	41	KARTEPE
570	41	KÖRFEZ
571	42	AHIRLI
572	42	AKÖREN
573	42	AKŞEHİR
574	42	ALTINEKİN
575	42	BEYŞEHİR
576	42	BOZKIR
577	42	CİHANBEYLİ
578	42	ÇELTİK
579	42	ÇUMRA
580	42	DERBENT
581	42	DEREBUCAK
582	42	DOĞANHİSAR
583	42	EMİRGAZİ
584	42	EREĞLİ
585	42	GÜNEYSINIR
586	42	HADİM
587	42	HALKAPINAR
588	42	HÜYÜK
589	42	ILGIN
590	42	KADINHANI
591	42	KARAPINAR
592	42	KARATAY
593	42	KULU
594	42	MERAM
595	42	SARAYÖNÜ
596	42	SELÇUKLU
597	42	SEYDİŞEHİR
598	42	TAŞKENT
599	42	TUZLUKÇU
600	42	YALIHÜYÜK
601	42	YUNAK
602	43	ALTINTAŞ
603	43	ASLANAPA
604	43	ÇAVDARHİSAR
605	43	DOMANİÇ
606	43	DUMLUPINAR
607	43	EMET
608	43	GEDİZ
609	43	HİSARCIK
610	43	MERKEZ
611	43	PAZARLAR
612	43	SİMAV
613	43	ŞAPHANE
614	43	TAVŞANLI
615	44	AKÇADAĞ
616	44	ARAPGİR
617	44	ARGUVAN
618	44	BATTALGAZİ
619	44	DARENDE
620	44	DOĞANŞEHİR
621	44	DOĞANYOL
622	44	HEKİMHAN
623	44	KALE
624	44	KULUNCAK
625	44	PÜTÜRGE
626	44	YAZIHAN
627	44	YEŞİLYURT
628	45	AHMETLİ
629	45	AKHİSAR
630	45	ALAŞEHİR
631	45	DEMİRCİ
632	45	GÖLMARMARA
633	45	GÖRDEŞ
634	45	KIRKAĞAÇ
635	45	KÖPRÜBAŞI
636	45	KULA
637	45	SALİHLİ
638	45	SARIGÖL
639	45	SARUHANLI
640	45	SELENDİ
641	45	SOMA
642	45	ŞEHZADELER
643	45	TURGUTLU
644	45	YUNUSEMRE
645	46	AFŞİN
646	46	ANDIRIN
647	46	ÇAĞLAYANCERİT
648	46	DULKADİROĞLU
649	46	EKİNÖZÜ
650	46	ELBİSTAN
651	46	GÖKSUN
652	46	NURHAK
653	46	ONİKİŞUBAT
654	46	PAZARCIK
655	46	TÜRKOĞLU
656	47	ARTUKLU
657	47	DARGEÇİT
658	47	DERİK
659	47	KIZILTEPE
660	47	MAZIDAĞI
661	47	MİDYAT
662	47	NUSAYBİN
663	47	ÖMERLİ
664	47	SAVUR
665	47	YEŞİLLİ
666	48	BODRUM
667	48	DALAMAN
668	48	DATÇA
669	48	FETHİYE
670	48	KAVAKLIDERE
671	48	KÖYCEĞİZ
672	48	MARMARİS
673	48	MENTEŞE
674	48	MİLAS
675	48	ORTACA
676	48	SEYDİKEMER
677	48	ULA
678	48	YATAĞAN
679	49	BULANIK
680	49	HASKÖY
681	49	KORKUT
682	49	MALAZGİRT
683	49	MERKEZ
684	49	VARTO
685	50	ACIGÖL
686	50	AVANOS
687	50	DERİNKUYU
688	50	GÜLŞEHİR
689	50	HACIBEKTAŞ
690	50	KOZAKLI
691	50	MERKEZ
692	50	ÜRGÜP
693	51	ALTUNHİSAR
694	51	BOR
695	51	ÇAMARDI
696	51	ÇİFTLİK
697	51	MERKEZ
698	51	ULUKIŞLA
699	52	AKKUŞ
700	52	ALTINORDU
701	52	AYBASTI
702	52	ÇAMAŞ
703	52	ÇATALPINAR
704	52	ÇAYBAŞI
705	52	FATSA
706	52	GÖLKÖY
707	52	GÜLYALI
708	52	GÜRGENTEPE
709	52	İKİZCE
710	52	KABADÜZ
711	52	KABATAŞ
712	52	KORGAN
713	52	KUMRU
714	52	MESUDİYE
715	52	PERŞEMBE
716	52	ULUBEY
717	52	ÜNYE
718	53	ARDEŞEN
719	53	ÇAMLIHEMŞİN
720	53	ÇAYELİ
721	53	DEREPAZARI
722	53	FINDIKLI
723	53	GÜNEYSU
724	53	HEMŞİN
725	53	İKİZDERE
726	53	İYİDERE
727	53	KALKANDERE
728	53	MERKEZ
729	53	PAZAR
730	54	ADAPAZARI
731	54	AKYAZI
732	54	ARİFİYE
733	54	ERENLER
734	54	FERİZLİ
735	54	GEYVE
736	54	HENDEK
737	54	KARAPÜRÇEK
738	54	KARASU
739	54	KAYNARCA
740	54	KOCAALİ
741	54	PAMUKOVA
742	54	SAPANCA
743	54	SERDİVAN
744	54	SÖĞÜTLÜ
745	54	TARAKLI
746	55	ONDOKUZMAYIS
747	55	ALAÇAM
748	55	ASARCIK
749	55	ATAKUM
750	55	AYVACIK
751	55	BAFRA
752	55	CANİK
753	55	ÇARŞAMBA
754	55	HAVZA
755	55	İLKADIM
756	55	KAVAK
757	55	LADİK
758	55	SALIPAZARI
759	55	TEKKEKÖY
760	55	TERME
761	55	VEZİRKÖPRÜ
762	55	YAKAKENT
763	56	BAYKAN
764	56	ERUH
765	56	KURTALAN
766	56	MERKEZ
767	56	PERVARİ
768	56	ŞİRVAN
769	56	TİLLO
770	57	AYANCIK
771	57	BOYABAT
772	57	DİKMEN
773	57	DURAĞAN
774	57	ERFELEK
775	57	GERZE
776	57	MERKEZ
777	57	SARAYDÜZÜ
778	57	TÜRKELİ
779	58	AKINCILAR
780	58	ALTINYAYLA
781	58	DİVRİĞİ
782	58	DOĞANŞAR
783	58	GEMEREK
784	58	GÖLOVA
785	58	GÜRÜN
786	58	HAFİK
787	58	İMRANLI
788	58	KANGAL
789	58	KOYULHİSAR
790	58	MERKEZ
791	58	SUŞEHRİ
792	58	ŞARKIŞLA
793	58	ULAŞ
794	58	YILDIZELİ
795	58	ZARA
796	59	ÇERKEZKÖY
797	59	ÇORLU
798	59	ERGENE
799	59	HAYRABOLU
800	59	KAPAKLI
801	59	MALKARA
802	59	MARMARAEREĞLİSİ
803	59	MURATLI
804	59	SARAY
805	59	SÜLEYMANPAŞA
806	59	ŞARKÖY
807	60	ALMUS
808	60	ARTOVA
809	60	BAŞÇİFTLİK
810	60	ERBAA
811	60	MERKEZ
812	60	NİKSAR
813	60	PAZAR
814	60	REŞADİYE
815	60	SULUSARAY
816	60	TURHAL
817	60	YEŞİLYURT
818	60	ZİLE
819	61	AKÇAABAT
820	61	ARAKLI
821	61	ARSİN
822	61	BEŞİKDÜZÜ
823	61	ÇARŞIBAŞI
824	61	ÇAYKARA
825	61	DERKENPAZARI
826	61	DÜZKÖY
827	61	HAYRAT
828	61	KÖPRÜBAŞI
829	61	MAÇKA
830	61	OF
831	61	ORTAHİSAR
832	61	SÜRMENE
833	61	ŞALPAZARI
834	61	TONYA
835	61	VAKFIKEBİR
836	61	YOMRA
837	62	ÇEMİŞGEZEK
838	62	HOZAT
839	62	MAZGİRT
840	62	MERKEZ
841	62	NAZIMİYE
842	62	OVACIK
843	62	PERTEK
844	62	PÜLÜMÜR
845	63	AKÇAKALE
846	63	BİRECİK
847	63	BOZOVA
848	63	CEYLANPINAR
849	63	EYYÜBİYE
850	63	HALFETİ
851	63	HALİLİYE
852	63	HARRAN
853	63	HİLVAN
854	63	KARAKÖPRÜ
855	63	SİVEREK
856	63	SURUÇ
857	63	VİRANŞEHİR
858	64	BANAZ
859	64	EŞME
860	64	KARAHALLI
861	64	MERKEZ
862	64	SİVASLI
863	64	ULUBEY
864	65	BAHÇESARAY
865	65	BAŞKALE
866	65	ÇALDIRAN
867	65	ÇATAK
868	65	EDREMİT
869	65	ERCİŞ
870	65	GEVAŞ
871	65	GÜRPINAR
872	65	İPEKYOLU
873	65	MURADİYE
874	65	ÖZALP
875	65	SARAY
876	65	TUŞBA
877	66	AKDAĞMADENİ
878	66	AYDINCIK
879	66	BOĞAZLIYAN
880	66	ÇANDIR
881	66	ÇAYIRALAN
882	66	ÇEKEREK
883	66	KADIŞEHRİ
884	66	MERKEZ
885	66	SARAYKENT
886	66	SARIKAYA
887	66	SORGUN
888	66	ŞEFAATLİ
889	66	YENİFAKILI
890	66	YERKÖY
891	67	ALAPLI
892	67	ÇAYCUMA
893	67	DEVREK
894	67	EREĞLİ
895	67	GÖKÇEBEY
896	67	KİLİMLİ
897	67	KOZLU
898	67	MERKEZ
899	68	AĞAÇÖREN
900	68	ESKİL
901	68	GÜLAĞAÇ
902	68	GÜZELYURT
903	68	MERKEZ
904	68	ORTAKÖY
905	68	SARIYAHŞİ
906	68	SULTANHANI
907	69	AYDINTEPE
908	69	DEMİRÖZÜ
909	69	MERKEZ
910	70	AYRANCI
911	70	BAŞYAYLA
912	70	ERMENEK
913	70	KAZIMKARABEKİR
914	70	MERKEZ
915	70	SARIVELİLER
916	71	BAHŞILI
917	71	BALIŞEYH
918	71	ÇELEBİ
919	71	DELİCE
920	71	KARAKEÇİLİ
921	71	KESKİN
922	71	MERKEZ
923	71	SULAKYURT
924	71	YAHŞİHAN
925	72	BEŞİRİ
926	72	GERCÜŞ
927	72	HASANKEYF
928	72	KOZLUK
929	72	MERKEZ
930	72	SASON
931	73	BEYTÜŞŞEBAP
932	73	CİZRE
933	73	GÜÇLÜKONAK
934	73	İDİL
935	73	MERKEZ
936	73	SİLOPİ
937	73	ULUDERE
938	74	AMASRA
939	74	KURUCAŞİLE
940	74	MERKEZ
941	74	ULUS
942	75	ÇILDIR
943	75	DAMAL
944	75	GÖLE
945	75	HANAK
946	75	MERKEZ
947	75	POSOF
948	76	ARALIK
949	76	KARAKOYUNLU
950	76	MERKEZ
951	76	TUZLUCA
952	77	ALTINOVA
953	77	ARMUTLU
954	77	ÇINARCIK
955	77	ÇİFTLİKKÖY
956	77	MERKEZ
957	77	TERMAL
958	78	EFLANİ
959	78	ESKİPAZAR
960	78	MERKEZ
961	78	OVACIK
962	78	SAFRANBOLU
963	78	YENİCE
964	79	ERBEYLİ
965	79	MERKEZ
966	79	MUSABEYLİ
967	79	POLATELİ
968	80	BAHÇE
969	80	DÜZİÇİ
970	80	HASANBEYLİ
971	80	KADİRLİ
972	80	MERKEZ
973	80	SUMBAS
974	80	TOPRAKKALE
975	81	AKÇAKOCA
976	81	CUMAYERİ
977	81	ÇİLİMLİ
978	81	GÖLYAKA
979	81	GÜMÜŞOVA
980	81	KAYNAŞLI
981	81	MERKEZ
982	81	YIĞILCA
\.


--
-- Data for Name: iller; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.iller (id, il) FROM stdin;
1	ADANA
2	ADIYAMAN
3	AFYON
4	AĞRI
5	AMASYA
6	ANKARA
7	ANTALYA
8	ARTVİN
9	AYDIN
10	BALIKESİR
11	BİLECİK
12	BİNGÖL
13	BİTLİS
14	BOLU
15	BURDUR
16	BURSA
17	ÇANAKKALE
18	ÇANKIRI
19	ÇORUM
20	DENİZLİ
21	DİYARBAKIR
22	EDİRNE
23	ELAZIĞ
24	ERZİNCAN
25	ERZURUM
26	ESKİŞEHİR
27	GAZİANTEP
28	GİRESUN
29	GÜMÜŞHANE
30	HAKKARİ
31	HATAY
32	ISPARTA
33	İÇEL
34	İSTANBUL
35	İZMİR
36	KARS
37	KASTAMONU
38	KAYSERİ
39	KIRKLARELİ
40	KIRŞEHİR
41	KOCAELİ
42	KONYA
43	KÜTAHYA
44	MALATYA
45	MANİSA
46	KAHRAMANMARAŞ
47	MARDİN
48	MUĞLA
49	MUŞ
50	NEVŞEHİR
51	NİĞDE
52	ORDU
53	RİZE
54	SAKARYA
55	SAMSUN
56	SİİRT
57	SİNOP
58	SİVAS
59	TEKİRDAĞ
60	TOKAT
61	TRABZON
62	TUNCELİ
63	ŞANLIURFA
64	UŞAK
65	VAN
66	YOZGAT
67	ZONGULDAK
68	AKSARAY
69	BAYBURT
70	KARAMAN
71	KIRIKKALE
72	BATMAN
73	ŞIRNAK
74	BARTIN
75	ARDAHAN
76	IĞDIR
77	YALOVA
78	KARABÜK
79	KİLİS
80	OSMANİYE
81	DÜZCE
\.


--
-- Data for Name: kasa; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.kasa (id, para) FROM stdin;
1	0.000
\.


--
-- Data for Name: kasahareketadlari; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.kasahareketadlari (id, ad) FROM stdin;
1	GİRİŞ
2	ÇIKIŞ
3	SATIŞ
4	İADE
\.


--
-- Data for Name: kasahareketleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.kasahareketleri (id, miktar, kullanici, islem, tarih, kar, evrakno, aciklama) FROM stdin;
\.


--
-- Data for Name: kullanicilar; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.kullanicilar (id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki, cariyetki, ayaryetki) FROM stdin;
1	admin	admin	ADMİN	ADMİN	0000000000	2021-12-04 00:00:00	t	t	t	t	t
\.


--
-- Data for Name: odemetipleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.odemetipleri (id, tip) FROM stdin;
1	NAKİT
2	KREDİ KARTI
3	HAVALE
4	ÇEK
5	SENET
\.


--
-- Data for Name: oturum; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.oturum (id, username, giristarihi) FROM stdin;
1	admin	2022-12-20 13:18:56.99
\.


--
-- Data for Name: stokbirimleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.stokbirimleri (id, birim) FROM stdin;
1	ADET
2	KİLOGRAM
3	KOLİ
4	PALET
5	PAKET
6	METRE
7	METREKARE
8	METREKÜP
9	LİTRE
\.


--
-- Data for Name: stokgruplari; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.stokgruplari (id, grup) FROM stdin;
1	GIDA
2	TÜTÜN
3	ALKOLSÜZ İÇECEK
4	ALKOLLÜ İÇECEK
5	TEMİZLİK
6	MANAV
7	HIRDAVAT
8	KİŞİSEL BAKIM
9	ELEKTRONİK
10	ŞARKÜTERİ
11	BAHARAT
12	OYUNCAK
13	GİYİM
14	KIRTASİYE
\.


--
-- Data for Name: stokhareketleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.stokhareketleri (barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama, birim_f, toplam_f, kdv) FROM stdin;
\.


--
-- Data for Name: stokkartlari; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.stokkartlari (id, barkod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, tarih, aciklama, kdvdahil, otv, otvdahil, kod, tedarikci, uretici, mensei) FROM stdin;
\.


--
-- Data for Name: teraziler; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.teraziler (id, marka) FROM stdin;
1	ACLAS
2	CAS
3	HANA
4	TEM
\.


--
-- Data for Name: terazimodel; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.terazimodel (id, model) FROM stdin;
1	PS1X
\.


--
-- Data for Name: ureticiler; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.ureticiler (id, ad) FROM stdin;
1	ÜLKER
2	ETİ GIDA SANAYİ VE TİCARET ANONİM ŞİRKETİ
3	SAGRA GRUP GIDA ÜRETİM VE TİCARET\nANONİM ŞİRKETİ
4	HARİBO ŞEKERLEME SANAYİ VE TİCARET LİMİTED ŞİRKETİ
5	TOYBOX GIDA PAZARLAMA SANAYİ VE TİCARET ANONİM ŞİRKETİ
6	FRİTO LAY GIDA SANAYİ VE TİCARET ANONİM ŞİRKETİ
7	NESTLE TÜRKİYE GIDA SANAYİ ANONİM ŞİRKETİ
8	KENT GIDA MADDELERİ SANAYİ VE TİCARET ANONİM ŞİRKETİ
9	PEYMAN KURUYEMİŞ GIDA AKTARİYE KİMYEVİ MADDELER TARIM ÜRÜNLERİ SANAYİ VE TİCARET ANONİM ŞİRKETİ
10	TADIM GIDA MADDELERİ SANAYİ VE TİCARET ANONİM ŞİRKETİ
11	MEHMETLER YAĞ SANAYİ VE TİCARET ANONİM ŞİRKETİ
12	AJİNOMOTO İSTANBUL GIDA SANAYİ VE TİCARET LİMİTED ŞİRKETİ
13	VEFA BOZACISI BOZA SİRKE ŞIRA VE GIDA MADDELERİ SANAYİ VE TİCARET ANONİM ŞİRKETİ
14	ATA AMBALAJ GIDA SANAYİ TİCARET LİMİTED ŞİRKETİ
15	TUKAŞ GIDA SANAYİ VE TİCARET ANONİM ŞİRKETİ
16	TAMEK GIDA VE KONSANTRE SANAYİ VE TİCARET ANONİM ŞİRKETİ
17	GOLD HARVEST KURUYEMİŞ VE TARIM ÜRÜNLERİ SANAYİ VE TİCARET ANONİM ŞİRKETİ
18	UNİLEVER SANAYİ VE TİCARET TÜRK ANONİM ŞİRKETİ
19	ÇAY İŞLETMELERİ GENEL MÜDÜRLÜĞÜ
20	BARİLLA GIDA SANAYİ ANONİM ŞİRKETİ
21	MARMARAÇAY GIDA PAZ. VE REK. HİZ. SANAYİ VE TİCARET ANONİM ŞİRKETİ
22	ACEMOĞLU GIDA
23	YAYLA AGRO GIDA SANAYİ VE NAKLİYAT ANONİM ŞİRKETİ
24	ÇEVİK TİCARET KOLL. ŞİRKETİ
25	SAFİR TUZ GIDA MADDELERİ PAZARLAMA NAK. İNŞ. İTH. İHR. SAN. VE TİC A.Ş.
26	MED-MAR SAĞ.HİZ. GIDA İNŞ.TUR.TUZ.İŞL.NAK. VE ELK.ÜRT.SAN.TİC.AŞ
27	RAFİNE BİLLUR TUZ SANAYİ A.Ş
28	NAZİF VE MURAT ŞİŞMANOĞLU ORTAKLIĞI
29	MUTLULAR NAKLİYE AKARYAKIT GIDA OTO. TARIM VE HAYVANCILIK SAN. TİC.LTD.ŞTİ.
30	MEMİŞOĞLU TARIM ÜRÜNLERİ TİCARET LTD.ŞTİ.
31	HEKİMOĞLU UN FABRİKASI TİCARET VE SANAYİ A.Ş
32	TURKŞEKER TARIM VE GIDA SAN.TİC A.Ş
33	YEŞİLLER KURUYEMİŞ TİC. VE SAN.LTD.ŞTİ
34	VELİ YUMURTA
35	PROCTER&GAMBLE TÜKETİM MALLARI SANAYİ A.Ş.
36	AYKİM TEM. MAD. SAN. VE TİC. A.Ş.
37	HAYAT KİMYA SAN. A.Ş.
39	BEYAZ KAĞIT VE HİJYENİK ÜRÜNLER TEM. İNŞ. SAN.TİC.A.Ş.
40	TETİKOĞLU KİMYA GIDA OTO PAZ.SAN.TİC.LTD.ŞTİ.
42	RECKİTT BENCKİSER EV VE HİJYEN ÜRÜNLERİ A.Ş.
43	KOPAŞ KOZMATİK PAZ. VE SAN.A.Ş.
44	DALAN KİMYA END. A.Ş.
45	EVYAP SABUN YAĞ GLİSERİN SANAYİ VE TİC.A.Ş.
46	COLGATE PALMOLİVE TEMİZLİK ÜRÜNLERİ SANAYİ TİC.A.Ş.
47	YONCA ÜRETİM PAZARLAMA
48	AKTAŞ SAĞLIK KOZMETİK VE TEMİZLİK ÜRÜNLERİ SAN.TİC.LTD.ŞTİ.
49	TÜRK HENKEL KİMYA SAN. VE TİC.A.Ş.
51	LİBEL İTH.İHR. VE KOZ. SAN.A.Ş.
52	CANAN KOZMETİK SAN. VE YİC A.Ş.
53	E.S.T. EYÜP SABRİ TUNCER KOZMETİK SANAYİ A.Ş.
54	HOBİ KOZMETİK İMALAT VE SAN.TİC.A.Ş.
55	YAZGAN LTD.ŞTİ.
56	MSC KİMYA SAN. VE TİC.A.Ş.
57	KORUMA TEMİZLİK A.Ş.
58	ABC DETERJAN SAN. VE TİC.A.Ş.
59	FATER TEMİZLİK ÜRÜNLERİ LTD.ŞTİ.
60	KİMBERLY CLARK
61	COSTER AEROSOL VALF SAN. A.Ş.
62	BEDA YAPI SAN.TİC.LTD.ŞTİ.
63	İLKA PLASTİK AMBALAJ GIDA SAN. VE TİC.LTD.ŞTİ.
64	KAMAR GLOBAL PAZARLAMA LTD.ŞTİ
65	RENKSAN PLASTİK SÜNGER SAN. VE TİC.LTD.ŞTİ.
66	TESLA PLASTİK VE AMBALAJ SANAYİ DIŞ TİCARET A.Ş.
67	VERA PLASTİK
68	MAKSİMUM EV İTH.MAD.PAZ. SANAYİ VE TİC.LTD.ŞTİ.
69	PLUS FİLM PLASTİK AMBALAJ SAN. VE TİC.LTD.ŞTİ.
70	ŞAKA İNŞAAT TURİZM VE GIDA SAN.DIŞ TİC.LTD.ŞTİ.
71	AR KAĞIT SANAYİ VE TİCARET LİMİTED ŞİRKETİ
72	LİLA KAĞIT SAN. VE TİC.A.Ş.
73	PURETEX PLASTİK VE TEKSTİL ÜRÜNLERİ SANAYİ VE TİCARET A.Ş.
74	ONTEX TÜKETİM ÜRÜNLERİ SAN. VE TİC.A.Ş.
75	ERUSLU SAĞLIK ÜRÜNLERİ SANAYİ VE TİCARET A.Ş.
76	ART İNTERNATİONAL HİJYEN SAĞLIK ÜRÜNLERİ SA. VE TİC.A.Ş.
77	KARDEŞLER UÇAN YAĞLAR SANAYİ A.Ş.
78	FİBRİL TÜKETİM ÜRÜNLERİ SAN A.Ş.
79	SEPA MENSUCAT SAN. TİCARET A.Ş.
80	KISMET LÜKS HIRDAVAT KIRT.KOZM.PROTEKNİK ÜRÜNL.SAN.TİC.LTD.ŞTİ
81	SEFA ENDÜSTRİYEL SAN.TİC.LTD.ŞTİ
82	ONURER KİMYA
83	BEYBİ PLASTİK FABRİKASI SANAYİ A.Ş.
84	ALBİ LATEKS ÜRÜNLERİ SAN. VE TİC.LTD.ŞTİ
85	SELİ KAYNAK SUYU VE MEYVE SULARI İŞLETMELERİ SAN.TİC.A.Ş.
86	GESAŞ GRNEL GIDA SAN. VE TİC.A.Ş.
87	KÜKRER FERMANTASYON TÜKETİM MADDELERİ VE TEMİZLİK ÜRÜNLERİ SAN. VE TİC.LTD.ŞTİ.
88	DOĞANAY GIDA TARIM VE HAYVANCILIK SAN.TİC.A.Ş.
89	PINAR SÜT MAMÜLLERİ SANAYİ A.Ş.
90	KONYA ŞEKER SAN. VE TİC.A.Ş.
91	ABDURRAHMAN TATLICI GIDA SAN. VE TİC.LTD.ŞTİ.
92	TAT GISA SANAYİ A.Ş.
93	DARDANEL ÖNENTAŞ GIDA SAN.A.Ş.
94	KELLOGG MED GIDA TİC.LTD.ŞTİ.
95	CHİPİTA GIDA ÜRETİM A.Ş.
96	KURUKAHVECİ MEHMET EFENDİ MAHDUMLARI LTD.ŞTİ.
41	SC JOHNSON WAX A.Ş.
97	FORSA KAĞITÇILIK SAN. TİC. LTD. ŞTİ.
38	LEVENT KİMYA SANAYİ VE TİCARET A.Ş.
98	ADKOTURK GIDA SANAYİ VE TİC.LTD.ŞTİ.
99	NİĞDE MEŞRUBAT VE GIDA SAN.TİC.LTD.ŞTİ.
100	BEYPAZARI KARAKOCA MADEN SUYU İŞL.A.Ş.
101	DR. OETKER GIDA SAN. VE TİC.A.Ş.
102	PAK GIDA ÜRETİM VE PAZARLAMA A.Ş.
103	BANAT FIRÇA VE PLASTİK SAN.A.Ş.
104	DİFAŞ FIRÇA VE PLASTİK SAN. VE TİC.A.Ş.
105	ECZACIBAŞI TÜKETİM ÜRÜNLERİ SANAYİ VE TİCARET A.Ş.
106	ALTIN İPLİK VE ÇORAP SANAYİ A.Ş.
107	DURACELL SATIŞ VE DAĞITIM LİMİTED ŞİRKETİ
108	TİBET İTH.İHR. VE KOZ.SAN.A.Ş.
109	ARI GIDA SANAYİ A.Ş.
110	PHİLSA PHİLİP MORRİS SABANCI A.Ş.
111	COCA COLA İÇECEK A.Ş.
112	FRUKO MEŞRUBAT SAN.LTD.ŞTİ
113	FREŞA İÇEÇEK SAN. VE TİC.A.Ş.
114	OĞUZ GIDA SAN VE TİC.A.Ş.
115	VERSAGE CONVERT MANAGEMENT GMBH
116	RED BULL GIDA DAĞITIM VE PAZ.TİC.LTD.ŞTİ.
117	AK GIDA SAN. VE TİC.A.Ş.
118	ŞUAYP DEMİREL AVŞAR MADENSUYU İŞLETMESİ
119	DELTA GIDA SAN. VE TİC.A.Ş.
120	GÜNEŞOĞLU SÜT GIDA SAN. VE TİC.A.Ş.
121	BESLER GIDA VE KİMYA SAN. VE TİC. A.Ş.
122	PINAR ENTEGRE ET VE UN SAN.A.Ş.
123	NAMET GIDA SAN. VE TİC. A.Ş.
124	EKİCİLER SÜT GIDA TARIM HAYVANCILIK SANAYİ VE TİCARET A.Ş.
125	AKAY ZEYTİNCİLİK GIDA MARKETÇİLİK AKARYAK.NAK.İTH.İHR.SAN. VE TİC.LTD.ŞTİ.
126	YNS ET MAMÜLLERİ GIDA MAD.PAZ.SAN.TİC.LTD.ŞTİ
127	NATURA GIDA SAN. VE TİC.A.Ş.
128	ŞENPİLİÇ GIDA SAN.A.Ş.
129	EUROPEAN TOBACCO A.Ş.
130	BRİTİSH AMERİCAN TABACCO TÜRKİYE
131	DEMİR MAKİNE TİC. VE SAN.LTD.ŞTİ.
132	GÜNEŞ YAMANLAR TİCARET VE GIDA SANAYİ A.Ş.
\.


--
-- Data for Name: vergidaireleri; Type: TABLE DATA; Schema: public; Owner: -
--

COPY public.vergidaireleri (id, ad, kod, il, ilce) FROM stdin;
1	ADANA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	1250	1	MERKEZ
8	5 OCAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	1251	1	MERKEZ
9	ÇUKUROVA VERGİ DAİRESİ MÜDÜRLÜĞÜ	1255	1	MERKEZ
10	ZİYAPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	1254	1	MERKEZ
12	YÜREĞİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	1252	1	MERKEZ
11	SEYHAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	1253	1	MERKEZ
13	İMAMOĞLU MALMÜDÜRLÜĞÜ	1115	1	İMAMOĞLU
14	ALADAĞ MALMÜDÜRLÜĞÜ	1114	1	ALADAĞ
15	YUMURTALIK MALMÜDÜRLÜĞÜ	1112	1	YUMURTALIK
16	TUFANBEYLİ MALMÜDÜRLÜĞÜ	1111	1	TUFANBEYLİ
17	SAİMBEYLİ MALMÜDÜRLÜĞÜ	1110	1	SAİMBEYLİ
18	POZANTI MALMÜDÜRLÜĞÜ	1109	1	POZANTI
19	KARAİSALI MALMÜDÜRLÜĞÜ	1105	1	KARAİSALI
20	FEKE MAL MÜDÜRLÜĞÜ	1103	1	FEKE
21	KARATAŞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	1205	1	KARATAŞ
22	KOZAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	1203	1	KOZAN
23	CEYHAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	1201	1	CEYHAN
24	TUT MALMÜDÜRLÜĞÜ	2108	2	TUT
25	SİNCİK MALMÜDÜRLÜĞÜ	2107	2	SİNCİK
26	SAMSAT MALMÜDÜRLÜĞÜ	2106	2	SAMSAT
27	GÖLBAŞI MALMÜDÜRLÜĞÜ	2104	2	GÖLBAŞI
28	GERGER MALMÜDÜRLÜĞÜ	2103	2	GERGER
29	ÇELİKHAN MALMÜDÜRLÜĞÜ	2102	2	ÇELİKHAN
30	BESNİ MALMÜDÜRLÜĞÜ	2101	2	BESNİ
31	KAHTA VERGİ DAİRESİ MÜDÜRLÜĞÜ	2261	2	KAHTA
32	ADIYAMAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	2260	2	MERKEZ
33	KIZILÖREN MALMÜDÜRLÜĞÜ	3117	3	KIZILÖREN
34	HOCALAR MALMÜDÜRLÜĞÜ	3116	3	HOCALAR
35	EVCİLER MALMÜDÜRLÜĞÜ	3115	3	EVCİLER
36	ÇOBANLAR MALMÜDÜRLÜĞÜ	3114	3	ÇOBANLAR
37	BAYAT MALMÜDÜRLÜĞÜ	3112	3	BAYAT
38	BAŞMAKÇI MALMÜDÜRLÜĞÜ	3111	3	BAŞMAKÇI
39	ŞUHUT MALMÜDÜRLÜĞÜ	3109	3	ŞUHUT
40	SULTANDAĞI MALMÜDÜRLÜĞÜ	3108	3	SULTANDAĞI
41	SİNANPAŞA MALMÜDÜRLÜĞÜ	3107	3	SİNANPAŞA
42	İHSANİYE MALMÜDÜRLÜĞÜ	3105	3	İHSANİYE
43	DAZKIRI MALMÜDÜRLÜĞÜ	3103	3	DAZKIRI
44	ÇAY VERGİ DAİRESİ MÜDÜRLÜĞÜ	3202	3	ÇAY
45	İSCEHİSAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	3264	3	İSCEHİSAR
46	SANDIKLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	3263	3	SANDIKLI
47	EMİRDAĞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	3262	3	EMİRDAĞ
48	BOLVADİN VERGİ DAİRESİ MÜDÜRLÜĞÜ	3261	3	BOLVADİN
49	DİNAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	3260	3	DİNAR
50	KOCATEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	3280	3	MERKEZ
51	TINAZTEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	3201	3	MERKEZ
52	TUTAK MALMÜDÜRLÜĞÜ	4107	4	TUTAK
53	TAŞLIÇAY MALMÜDÜRLÜĞÜ	4106	4	TAŞLIÇAY
54	HAMUR MALMÜDÜRLÜĞÜ	4104	4	HAMUR
55	ELEŞKİRT MALMÜDÜRLÜĞÜ	4103	4	ELEŞKİRT
56	DİYADİN MALMÜDÜRLÜĞÜ	4101	4	DİYADİN
57	PATNOS VERGİ DAİRESİ MÜDÜRLÜĞÜ	4262	4	PATNOS
58	DOĞUBEYAZIT VERGİ DAİRESİ MÜDÜRLÜĞÜ	4261	4	DOĞUBEYAZIT
59	AĞRI VERGİ DAİRESİ MÜDÜRLÜĞÜ	4260	4	MERKEZ
60	HAMAMÖZÜ MALMÜDÜRLÜĞÜ	5106	5	HAMAMÖZÜ
61	GÖYNÜCEK MALMÜDÜRLÜĞÜ	5101	5	GÖYNÜCEK
62	SULUOVA VERGİ DAİRESİ MÜDÜRLÜĞÜ	5264	5	SULUOVA
63	TAŞOVA VERGİ DAİRESİ MÜDÜRLÜĞÜ	5263	5	TAŞOVA
64	GÜMÜŞHACIKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	5262	5	GÜMÜŞHACIKÖY
65	MERZİFON VERGİ DAİRESİ MÜDÜRLÜĞÜ	5261	5	MERZİFON
66	AMASYA VERGİ DAİRESİ MÜDÜRLÜĞÜ	5260	5	MERKEZ
67	AKYURT MALMÜDÜRLÜĞÜ	6125	6	MERKEZ- (5216)
68	NALLIHAN MALMÜDÜRLÜĞÜ	6117	6	NALLIHAN
69	KIZILCAHAMAM MALMÜDÜRLÜĞÜ	6115	6	KIZILCAHAMAM
70	KALECİK MALMÜDÜRLÜĞÜ	6112	6	MERKEZ- (5216)
71	GÜDÜL MALMÜDÜRLÜĞÜ	6110	6	GÜDÜL
72	ÇAMLIDERE MALMÜDÜRLÜĞÜ	6106	6	ÇAMLIDERE
73	BALÂ MALMÜDÜRLÜĞÜ	6103	6	MERKEZ- (5216)
74	AYAŞ MALMÜDÜRLÜĞÜ	6102	6	MERKEZ- (5216)
75	ELMADAĞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	6215	6	MERKEZ- (5216)
76	HAYMANA VERGİ DAİRESİ MÜDÜRLÜĞÜ	6213	6	HAYMANA
77	ÇUBUK VERGİ DAİRESİ MÜDÜRLÜĞÜ	6211	6	MERKEZ- (5216)
78	BEYPAZARI VERGİ DAİRESİ MÜDÜRLÜĞÜ	6209	6	BEYPAZARI
79	ŞEREFLİKOÇHİSAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	6207	6	ŞEREFLİKOÇHİSAR
80	POLATLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	6205	6	POLATLI
81	KAHRAMANKAZAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	6279	6	MERKEZ- (5216)
82	KEÇİÖREN VERGİ DAİRESİ MÜDÜRLÜĞÜ	6278	6	MERKEZ
83	CUMHURİYET VERGİ DAİRESİ MÜDÜRLÜĞÜ	6277	6	MERKEZ
84	BAŞKENT VERGİ DAİRESİ MÜDÜRLÜĞÜ	6276	6	MERKEZ
85	ETİMESGUT VERGİ DAİRESİ MÜDÜRLÜĞÜ	6275	6	MERKEZ
86	DIŞKAPI VERGİ DAİRESİ MÜDÜRLÜĞÜ	6274	6	MERKEZ
87	SİNCAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	6273	6	MERKEZ
88	GÖLBAŞI VERGİ DAİRESİ MÜDÜRLÜĞÜ	6272	6	MERKEZ
89	OSTİM VERGİ DAİRESİ MÜDÜRLÜĞÜ	6271	6	MERKEZ
90	MUHAMMET KARAGÜZEL VERGİ DAİRESİ MÜDÜRLÜĞÜ	6270	6	MERKEZ
91	YAHYA GALİP VERGİ DAİRESİ MÜDÜRLÜĞÜ	6269	6	MERKEZ
92	HİTİT VERGİ DAİRESİ MÜDÜRLÜĞÜ	6268	6	MERKEZ
93	YEĞENBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	6266	6	MERKEZ
94	DOĞANBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	6265	6	MERKEZ
95	DİKİMEVİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	6264	6	MERKEZ
96	SEĞMENLER VERGİ DAİRESİ MÜDÜRLÜĞÜ	6262	6	MERKEZ
97	YILDIRIM BEYAZIT VERGİ DAİRESİ MÜDÜRLÜĞÜ	6261	6	MERKEZ
98	ULUS VERGİ DAİRESİ MÜDÜRLÜĞÜ	6260	6	MERKEZ
99	MİTHATPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	6259	6	MERKEZ
100	KIZILBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	6258	6	MERKEZ
101	ÇANKAYA VERGİ DAİRESİ MÜDÜRLÜĞÜ	6257	6	MERKEZ
102	YENİMAHALLE VERGİ DAİRESİ MÜDÜRLÜĞÜ	6255	6	MERKEZ
103	MALTEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	6254	6	MERKEZ
104	KAVAKLIDERE VERGİ DAİRESİ MÜDÜRLÜĞÜ	6253	6	MERKEZ
105	VERASET VE HARÇLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	6252	6	MERKEZ
106	ANKARA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	6281	6	MERKEZ
107	ANADOLU İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	6280	6	MERKEZ
108	EVREN MALMÜDÜRLÜĞÜ	6127	6	EVREN
109	İBRADI MALMÜDÜRLÜĞÜ	7113	7	İBRADI
110	DEMRE MALMÜDÜRLÜĞÜ	7112	7	DEMRE
111	KAŞ MALMÜDÜRLÜĞÜ	7107	7	KAŞ
112	GÜNDOĞMUŞ MALMÜDÜRLÜĞÜ	7106	7	GÜNDOĞMUŞ
113	GAZİPAŞA MALMÜDÜRLÜĞÜ	7105	7	GAZİPAŞA
114	FİNİKE MALMÜDÜRLÜĞÜ	7104	7	FİNİKE
115	AKSEKİ MALMÜDÜRLÜĞÜ	7101	7	AKSEKİ
116	KUMLUCA VERGİ DAİRESİ MÜDÜRLÜĞÜ	7206	7	KUMLUCA
117	KEMER VERGİ DAİRESİ MÜDÜRLÜĞÜ	7205	7	KEMER
118	ELMALI VERGİ DAİRESİ MÜDÜRLÜĞÜ	7204	7	ELMALI
119	MANAVGAT VERGİ DAİRESİ MÜDÜRLÜĞÜ	7203	7	MANAVGAT
120	SERİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	7202	7	SERİK
121	ALANYA VERGİ DAİRESİ MÜDÜRLÜĞÜ	7201	7	ALANYA
122	KORKUTELİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	7257	7	KORKUTELİ
123	ANTALYA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	7256	7	MERKEZ
124	ANTALYA KURUMLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	7255	7	MERKEZ
125	DÜDEN VERGİ DAİRESİ MÜDÜRLÜĞÜ	7254	7	MERKEZ
126	MURATPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	7253	7	MERKEZ
127	KALEKAPI VERGİ DAİRESİ MÜDÜRLÜĞÜ	7252	7	MERKEZ
128	ÜÇKAPILAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	7251	7	MERKEZ
129	MURGUL MALMÜDÜRLÜĞÜ	8107	8	MURGUL
130	YUSUFELİ MALMÜDÜRLÜĞÜ	8106	8	YUSUFELİ
131	ŞAVŞAT MALMÜDÜRLÜĞÜ	8105	8	ŞAVŞAT
132	BORÇKA MALMÜDÜRLÜĞÜ	8103	8	BORÇKA
133	ARDANUÇ MALMÜDÜRLÜĞÜ	8101	8	ARDANUÇ
134	ARHAVİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	8262	8	ARHAVİ
135	HOPA VERGİ DAİRESİ MÜDÜRLÜĞÜ	8261	8	HOPA
136	ARTVİN VERGİ DAİRESİ MÜDÜRLÜĞÜ	8260	8	MERKEZ
137	GERMENCİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	9263	9	GERMENCİK
138	KÖŞK MALMÜDÜRLÜĞÜ	9115	9	KÖŞK
139	KARPUZLU MALMÜDÜRLÜĞÜ	9114	9	KARPUZLU
140	İNCİRLİOVA MALMÜDÜRLÜĞÜ	9113	9	İNCİRLİOVA
141	BUHARKENT MALMÜDÜRLÜĞÜ	9112	9	BUHARKENT
142	YENİPAZAR MALMÜDÜRLÜĞÜ	9111	9	YENİPAZAR
143	SULTANHİSAR MALMÜDÜRLÜĞÜ	9110	9	SULTANHİSAR
144	KUYUCAK MALMÜDÜRLÜĞÜ	9107	9	KUYUCAK
145	KOÇARLI MALMÜDÜRLÜĞÜ	9105	9	KOÇARLI
146	KARACASU MALMÜDÜRLÜĞÜ	9104	9	KARACASU
147	BOZDOĞAN MALMÜDÜRLÜĞÜ	9101	9	BOZDOĞAN
148	DİDİM VERGİ DAİRESİ MÜDÜRLÜĞÜ	9281	9	DİDİM
149	KUŞADASI VERGİ DAİRESİ MÜDÜRLÜĞÜ	9265	9	KUŞADASI
150	ÇİNE VERGİ DAİRESİ MÜDÜRLÜĞÜ	9262	9	ÇİNE
151	SÖKE VERGİ DAİRESİ MÜDÜRLÜĞÜ	9261	9	SÖKE
152	NAZİLLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	9260	9	NAZİLLİ
153	GÜZELHİSAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	9280	9	MERKEZ
154	EFELER VERGİ DAİRESİ MÜDÜRLÜĞÜ	9201	9	MERKEZ
155	GÖMEÇ MALMÜDÜRLÜĞÜ	10118	10	GÖMEÇ
156	MARMARA MALMÜDÜRLÜĞÜ	10117	10	MARMARA
157	SAVAŞTEPE MALMÜDÜRLÜĞÜ	10114	10	SAVAŞTEPE
158	MANYAS MALMÜDÜRLÜĞÜ	10113	10	MANYAS
159	KEPSUT MALMÜDÜRLÜĞÜ	10112	10	KEPSUT
160	İVRİNDİ MALMÜDÜRLÜĞÜ	10111	10	İVRİNDİ
161	HAVRAN MALMÜDÜRLÜĞÜ	10110	10	HAVRAN
162	BALYA MALMÜDÜRLÜĞÜ	10102	10	BALYA
163	DURSUNBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	10269	10	DURSUNBEY
164	SINDIRGI VERGİ DAİRESİ MÜDÜRLÜĞÜ	10268	10	SINDIRGI
165	BİGADİÇ VERGİ DAİRESİ MÜDÜRLÜĞÜ	10267	10	BİGADİÇ
166	ERDEK VERGİ DAİRESİ MÜDÜRLÜĞÜ	10266	10	ERDEK
167	SUSURLUK VERGİ DAİRESİ MÜDÜRLÜĞÜ	10265	10	SUSURLUK
168	GÖNEN VERGİ DAİRESİ MÜDÜRLÜĞÜ	10264	10	GÖNEN
169	EDREMİT VERGİ DAİRESİ MÜDÜRLÜĞÜ	10263	10	EDREMİT
170	BURHANİYE VERGİ DAİRESİ MÜDÜRLÜĞÜ	10262	10	BURHANİYE
171	BANDIRMA VERGİ DAİRESİ MÜDÜRLÜĞÜ	10261	10	BANDIRMA
172	AYVALIK VERGİ DAİRESİ MÜDÜRLÜĞÜ	10260	10	AYVALIK
173	KURTDERELİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	10280	10	MERKEZ
174	KARESİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	10201	10	MERKEZ
175	İNHİSAR MALMÜDÜRLÜĞÜ	11107	11	İNHİSAR
176	YENİPAZAR MALMÜDÜRLÜĞÜ	11106	11	YENİPAZAR
177	SÖĞÜT MALMÜDÜRLÜĞÜ	11105	11	SÖĞÜT
178	PAZARYERİ MALMÜDÜRLÜĞÜ	11104	11	PAZARYERİ
179	OSMANELİ MALMÜDÜRLÜĞÜ	11103	11	OSMANELİ
180	GÖLPAZARI MALMÜDÜRLÜĞÜ	11102	11	GÖLPAZARI
181	BOZÜYÜK VERGİ DAİRESİ MÜDÜRLÜĞÜ	11261	11	BOZÜYÜK
182	BİLECİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	11260	11	MERKEZ
183	YEDİSU MALMÜDÜRLÜĞÜ	12107	12	YEDİSU
184	YAYLADERE MALMÜDÜRLÜĞÜ	12106	12	YAYLADERE
185	ADAKLI MALMÜDÜRLÜĞÜ	12105	12	ADAKLI
186	SOLHAN MALMÜDÜRLÜĞÜ	12104	12	SOLHAN
187	KİĞI MALMÜDÜRLÜĞÜ	12103	12	KİĞI
188	KARLIOVA MALMÜDÜRLÜĞÜ	12102	12	KARLIOVA
189	GENÇ MALMÜDÜRLÜĞÜ	12101	12	GENÇ
190	BİNGÖL VERGİ DAİRESİ MÜDÜRLÜĞÜ	12260	12	MERKEZ
191	GÜROYMAK MALMÜDÜRLÜĞÜ	13106	13	GÖROYMAK
192	MUTKİ MALMÜDÜRLÜĞÜ	13104	13	MUTKİ
193	HİZAN MALMÜDÜRLÜĞÜ	13103	13	HİZAN
194	AHLAT MALMÜDÜRLÜĞÜ	13102	13	AHLAT
195	ADİLCEVAZ MALMÜDÜRLÜĞÜ	13101	13	ADİLCEVAZ
196	TATVAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	13261	13	TATVAN
197	BİTLİS VERGİ DAİRESİ MÜDÜRLÜĞÜ	13260	13	MERKEZ
198	YENİÇAĞA MALMÜDÜRLÜĞÜ	14114	14	YENİÇAĞA
199	DÖRTDİVAN MALMÜDÜRLÜĞÜ	14113	14	DÖRTDİVAN
200	SEBEN MALMÜDÜRLÜĞÜ	14108	14	SEBEN
201	MUDURNU MALMÜDÜRLÜĞÜ	14107	14	MUDURNU
202	MENGEN MALMÜDÜRLÜĞÜ	14106	14	MENGEN
203	KIBRISCIK MALMÜDÜRLÜĞÜ	14105	14	KIBRISCIK
204	GÖYNÜK MALMÜDÜRLÜĞÜ	14104	14	GÖYNÜK
205	GEREDE VERGİ DAİRESİ MÜDÜRLÜĞÜ	14262	14	GEREDE
206	BOLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	14260	14	MERKEZ
207	ÇELTİKÇİ MALMÜDÜRLÜĞÜ	15110	15	ÇELTİKÇİ
208	ÇAVDIR MALMÜDÜRLÜĞÜ	15109	15	ÇAVDIR
209	ALTINYAYLA MALMÜDÜRLÜĞÜ	15108	15	ALTINYAYLA
210	KEMER MALMÜDÜRLÜĞÜ	15107	15	KEMER
211	KARAMANLI MALMÜDÜRLÜĞÜ	15106	15	KARAMANLI
212	YEŞİLOVA MALMÜDÜRLÜĞÜ	15105	15	YEŞİLOVA
213	TEFENNİ MALMÜDÜRLÜĞÜ	15104	15	TEFENNİ
214	GÖLHİSAR MALMÜDÜRLÜĞÜ	15103	15	GÖLHİSAR
215	AĞLASUN MALMÜDÜRLÜĞÜ	15101	15	AĞLASUN
216	BUCAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	15261	15	BUCAK
217	BURDUR VERGİ DAİRESİ MÜDÜRLÜĞÜ	15260	15	MERKEZ
218	BÜYÜKORHAN MALMÜDÜRLÜĞÜ	16112	16	BÜYÜKORHAN
219	HARMANCIK MALMÜDÜRLÜĞÜ	16111	16	HARMANCIK
220	ORHANELİ MALMÜDÜRLÜĞÜ	16108	16	ORHANELİ
221	KELES MALMÜDÜRLÜĞÜ	16105	16	KELES
222	YENİŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	16208	16	YENİŞEHİR
223	İZNİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	16207	16	İZNİK
224	ORHANGAZİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	16206	16	ORHANGAZİ
225	MUDANYA VERGİ DAİRESİ MÜDÜRLÜĞÜ	16205	16	MERKEZ- (5216)
226	MUSTAFAKEMALPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	16204	16	MUSTAFAKEMALPAŞA
227	KARACABEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	16203	16	KARACABEY
228	İNEGÖL VERGİ DAİRESİ MÜDÜRLÜĞÜ	16202	16	İNEGÖL
229	GEMLİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	16201	16	MERKEZ- (5216)
230	BURSA VERASET VE HARÇLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	16260	16	MERKEZ
231	GÖKDERE VERGİ DAİRESİ MÜDÜRLÜĞÜ	16259	16	MERKEZ
232	ERTUĞRULGAZİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	16258	16	MERKEZ
233	NİLÜFER VERGİ DAİRESİ MÜDÜRLÜĞÜ	16257	16	MERKEZ
234	YEŞİL VERGİ DAİRESİ MÜDÜRLÜĞÜ	16256	16	MERKEZ
235	ULUDAĞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	16255	16	MERKEZ
236	SETBAŞI VERGİ DAİRESİ MÜDÜRLÜĞÜ	16254	16	MERKEZ
237	ÇEKİRGE VERGİ DAİRESİ MÜDÜRLÜĞÜ	16253	16	MERKEZ
238	YILDIRIM VERGİ DAİRESİ MÜDÜRLÜĞÜ	16252	16	MERKEZ
239	OSMANGAZİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	16251	16	MERKEZ
240	BURSA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	16250	16	MERKEZ
241	YENİCE MALMÜDÜRLÜĞÜ	17111	17	YENİCE
242	LAPSEKİ MALMÜDÜRLÜĞÜ	17110	17	LAPSEKİ
243	GÖKÇEADA MALMÜDÜRLÜĞÜ	17109	17	GÖKÇEADA
244	EZİNE MALMÜDÜRLÜĞÜ	17107	17	EZİNE
245	ECEABAT MALMÜDÜRLÜĞÜ	17106	17	ECEABAT
246	BOZCAADA MALMÜDÜRLÜĞÜ	17104	17	BOZCAADA
247	BAYRAMİÇ MALMÜDÜRLÜĞÜ	17102	17	BAYRAMİÇ
248	AYVACIK MALMÜDÜRLÜĞÜ	17101	17	AYVACIK
249	GELİBOLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	17263	17	GELİBOLU
250	ÇAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	17262	17	ÇAN
251	BİGA VERGİ DAİRESİ MÜDÜRLÜĞÜ	17261	17	BİGA
252	ÇANAKKALE VERGİ DAİRESİ MÜDÜRLÜĞÜ	17260	17	MERKEZ
253	KORGUN MALMÜDÜRLÜĞÜ	18113	18	KORGUN
254	BAYRAMÖREN MALMÜDÜRLÜĞÜ	18112	18	BAYRAMÖREN
255	KIZILIRMAK MALMÜDÜRLÜĞÜ	18111	18	KIZILIRMAK
256	ATKARACALAR MALMÜDÜRLÜĞÜ	18110	18	ATKARACALAR
257	YAPRAKLI MALMÜDÜRLÜĞÜ	18109	18	YAPRAKLI
258	ŞABANÖZÜ MALMÜDÜRLÜĞÜ	18108	18	ŞABANÖZÜ
259	ORTA MALMÜDÜRLÜĞÜ	18106	18	ORTA
260	KURŞUNLU MALMÜDÜRLÜĞÜ	18105	18	KURŞUNLU
261	ILGAZ MALMÜDÜRLÜĞÜ	18104	18	ILGAZ
262	ELDİVAN MALMÜDÜRLÜĞÜ	18102	18	ELDİVAN
263	ÇERKEŞ MALMÜDÜRLÜĞÜ	18101	18	ÇERKEŞ
264	ÇANKIRI VERGİ DAİRESİ MÜDÜRLÜĞÜ	18260	18	MERKEZ
265	LAÇİN MALMÜDÜRLÜĞÜ	19113	19	LAÇİN
266	OĞUZLAR MALMÜDÜRLÜĞÜ	19112	19	OĞUZLAR
267	DODURGA MALMÜDÜRLÜĞÜ	19111	19	DODURGA
268	UĞURLUDAĞ MALMÜDÜRLÜĞÜ	19110	19	UĞURLUDAĞ
269	BOĞAZKALE MALMÜDÜRLÜĞÜ	19109	19	BOĞAZKALE
270	OSMANCIK MALMÜDÜRLÜĞÜ	19107	19	OSMANCIK
271	ORTAKÖY MALMÜDÜRLÜĞÜ	19106	19	ORTAKÖY
272	MECİTÖZÜ MALMÜDÜRLÜĞÜ	19105	19	MECİTÖZÜ
273	KARGI MALMÜDÜRLÜĞÜ	19104	19	KARGI
274	İSKİLİP MALMÜDÜRLÜĞÜ	19103	19	İSKİLİP
275	BAYAT MALMÜDÜRLÜĞÜ	19102	19	BAYAT
276	ALACA MALMÜDÜRLÜĞÜ	19101	19	ALACA
277	SUNGURLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	19261	19	SUNGURLU
278	ÇORUM VERGİ DAİRESİ MÜDÜRLÜĞÜ	19260	19	MERKEZ
279	BOZKURT MALMÜDÜRLÜĞÜ	20118	20	BOZKURT
280	BEYAĞAÇ MALMÜDÜRLÜĞÜ	20117	20	BEYAĞAÇ
281	BAKLAN MALMÜDÜRLÜĞÜ	20116	20	BAKLAN
282	PAMUKKALE MALMÜDÜRLÜĞÜ (AKKÖY)	20115	20	PAMUKKALE
283	SERİNHİSAR MALMÜDÜRLÜĞÜ	20114	20	SERİNHİSAR
284	HONAZ MALMÜDÜRLÜĞÜ	20113	20	HONAZ
285	BEKİLLİ MALMÜDÜRLÜĞÜ	20112	20	BEKİLLİ
286	BABADAĞ MALMÜDÜRLÜĞÜ	20111	20	BABADAĞ
287	KALE MALMÜDÜRLÜĞÜ	20108	20	KALE
288	GÜNEY MALMÜDÜRLÜĞÜ	20107	20	GÜNEY
289	ÇARDAK MALMÜDÜRLÜĞÜ	20105	20	ÇARDAK
290	ÇAMELİ MALMÜDÜRLÜĞÜ	20104	20	ÇAMELİ
291	ÇİVRİL VERGİ DAİRESİ MÜDÜRLÜĞÜ	20265	20	ÇİVRİL
292	ÇAL VERGİ DAİRESİ MÜDÜRLÜĞÜ	20264	20	ÇAL
293	BULDAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	20263	20	BULDAN
294	TAVAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	20262	20	TAVAS
295	ACIPAYAM VERGİ DAİRESİ MÜDÜRLÜĞÜ	20261	20	ACIPAYAM
296	SARAYKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	20260	20	SARAYKÖY
297	PAMUKKALE VERGİ DAİRESİ MÜDÜRLÜĞÜ	20280	20	MERKEZ
298	DENİZLİ İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	20250	20	MERKEZ
299	GÖKPINAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	20203	20	MERKEZ
300	ÇINAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	20202	20	MERKEZ
301	SARAYLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	20201	20	MERKEZ
302	KOCAKÖY MALMÜDÜRLÜĞÜ	21113	21	KOCAKÖY
303	EĞİL MALMÜDÜRLÜĞÜ	21112	21	EĞİL
304	SİLVAN MALMÜDÜRLÜĞÜ	21111	21	SİLVAN
305	LİCE MALMÜDÜRLÜĞÜ	21110	21	LİCE
306	KULP MALMÜDÜRLÜĞÜ	21109	21	KULP
307	HAZRO MALMÜDÜRLÜĞÜ	21108	21	HAZRO
308	HANİ MALMÜDÜRLÜĞÜ	21107	21	HANİ
309	DİCLE MALMÜDÜRLÜĞÜ	21105	21	DİCLE
310	ÇÜNGÜŞ MALMÜDÜRLÜĞÜ	21104	21	ÇÜNGÜŞ
311	ÇINAR MALMÜDÜRLÜĞÜ	21103	21	ÇINAR
312	ÇERMİK MALMÜDÜRLÜĞÜ	21102	21	ÇERMİK
313	BİSMİL VERGİ DAİRESİ MÜDÜRLÜĞÜ	21284	21	BİSMİL
314	ERGANİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	21283	21	ERGANİ
315	CAHİT SITKI TARANCI VERGİ DAİRESİ MÜDÜRLÜĞÜ	21282	21	MERKEZ
316	SÜLEYMAN NAZİF VERGİ DAİRESİ MÜDÜRLÜĞÜ	21281	21	MERKEZ
317	GÖKALP VERGİ DAİRESİ MÜDÜRLÜĞÜ	21251	21	MERKEZ
318	SÜLOĞLU MALMÜDÜRLÜĞÜ	22108	22	SÜLOĞLU
319	MERİÇ MALMÜDÜRLÜĞÜ	22106	22	MERİÇ
320	LALAPAŞA MALMÜDÜRLÜĞÜ	22105	22	LALAPAŞA
321	ENEZ MALMÜDÜRLÜĞÜ	22101	22	ENEZ
322	İPSALA VERGİ DAİRESİ MÜDÜRLÜĞÜ	22264	22	İPSALA
323	HAVSA VERGİ DAİRESİ MÜDÜRLÜĞÜ	22263	22	HAVSA
324	UZUNKÖPRÜ VERGİ DAİRESİ MÜDÜRLÜĞÜ	22262	22	UZUNKÖPRÜ
325	KEŞAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	22261	22	KEŞAN
326	KIRKPINAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	22260	22	MERKEZ
327	ARDA VERGİ DAİRESİ MÜDÜRLÜĞÜ	22201	22	MERKEZ
328	ALACAKAYA MALMÜDÜRLÜĞÜ	23110	23	ALACAKAYA
329	KOVANCILAR MALMÜDÜRLÜĞÜ	23109	23	KOVANCILAR
330	ARICAK MALMÜDÜRLÜĞÜ	23108	23	ARICAK
331	SİVRİCE MALMÜDÜRLÜĞÜ	23107	23	SİVRİCE
332	PALU MALMÜDÜRLÜĞÜ	23106	23	PALU
333	MADEN MALMÜDÜRLÜĞÜ	23105	23	MADEN
334	KEBAN MALMÜDÜRLÜĞÜ	23104	23	KEBAN
335	KARAKOÇAN MALMÜDÜRLÜĞÜ	23103	23	KARAKOÇAN
336	BASKİL MALMÜDÜRLÜĞÜ	23102	23	BASKİL
337	AĞIN MALMÜDÜRLÜĞÜ	23101	23	AĞIN
338	HAZAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	23280	23	MERKEZ
339	HARPUT VERGİ DAİRESİ MÜDÜRLÜĞÜ	23201	23	MERKEZ
340	OTLUKBELİ MALMÜDÜRLÜĞÜ	24108	24	OTLUKBELİ
341	ÜZÜMLÜ MALMÜDÜRLÜĞÜ	24107	24	ÜZÜMLÜ
342	TERCAN MALMÜDÜRLÜĞÜ	24106	24	TERCAN
343	REFAHİYE MALMÜDÜRLÜĞÜ	24105	24	REFAHİYE
344	KEMALİYE MALMÜDÜRLÜĞÜ	24104	24	KEMALİYE
345	KEMAH MALMÜDÜRLÜĞÜ	24103	24	KEMAH
346	İLİÇ MALMÜDÜRLÜĞÜ	24102	24	İLİÇ
347	ÇAYIRLI MALMÜDÜRLÜĞÜ	24101	24	ÇAYIRLI
348	FEVZİPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	24260	24	MERKEZ
349	KÖPRÜKÖY MALMÜDÜRLÜĞÜ	25118	25	KÖPRÜKÖY
350	AZİZİYE MALMÜDÜRLÜĞÜ  (ILICA)	25117	25	MERKEZ- (5216)
351	PAZARYOLU MALMÜDÜRLÜĞÜ	25116	25	PAZARYOLU
352	UZUNDERE MALMÜDÜRLÜĞÜ	25115	25	UZUNDERE
353	KARAÇOBAN MALMÜDÜRLÜĞÜ	25114	25	KARAÇOBAN
354	TORTUM MALMÜDÜRLÜĞÜ	25113	25	TORTUM
355	TEKMAN MALMÜDÜRLÜĞÜ	25112	25	TEKMAN
356	ŞENKAYA MALMÜDÜRLÜĞÜ	25111	25	ŞENKAYA
357	PASİNLER MALMÜDÜRLÜĞÜ	25110	25	PASİNLER
358	OLUR MALMÜDÜRLÜĞÜ	25109	25	OLUR
359	OLTU MALMÜDÜRLÜĞÜ	25108	25	OLTU
360	NARMAN MALMÜDÜRLÜĞÜ	25107	25	NARMAN
361	KARAYAZI MALMÜDÜRLÜĞÜ	25106	25	KARAYAZI
362	İSPİR MALMÜDÜRLÜĞÜ	25105	25	İSPİR
363	HORASAN MALMÜDÜRLÜĞÜ	25104	25	HORASAN
364	HINIS MALMÜDÜRLÜĞÜ	25103	25	HINIS
365	ÇAT MALMÜDÜRLÜĞÜ	25102	25	ÇAT
366	AŞKALE MALMÜDÜRLÜĞÜ	25101	25	AŞKALE
367	KAZIMKARABEKİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	25280	25	MERKEZ
368	AZİZİYE VERGİ DAİRESİ MÜDÜRLÜĞÜ	25251	25	MERKEZ
379	ESKİŞEHİR VERGİ DAİRESİ BAŞKANLIĞI	26250	26	MERKEZ
378	MAHMUDİYE MALMÜDÜRLÜĞÜ	26102	26	MAHMUDİYE
377	MİHALIÇÇIK MALMÜDÜRLÜĞÜ	26103	26	MİHALIÇÇIK
376	SARICAKAYA MALMÜDÜRLÜĞÜ	26104	26	SARICAKAYA
375	SEYİTGAZİ MALMÜDÜRLÜĞÜ	26105	26	SEYİTGAZİ
374	ALPU MALMÜDÜRLÜĞÜ	26107	26	ALPU
373	BEYLİKOVA MALMÜDÜRLÜĞÜ	26108	26	BEYLİKOVA
372	İNÖNÜ MALMÜDÜRLÜĞÜ	26109	26	İNÖNÜ
371	GÜNYÜZÜ MALMÜDÜRLÜĞÜ	26110	26	GÜNYÜZÜ
370	HAN MALMÜDÜRLÜĞÜ	26111	26	HAN
369	MİHALGAZİ MALMÜDÜRLÜĞÜ	26112	26	MİHALGAZİ
380	NURDAĞI MALMÜDÜRLÜĞÜ	27108	27	NURDAĞI
381	KARKAMIŞ MALMÜDÜRLÜĞÜ	27107	27	KARKAMIŞ
382	YAVUZELİ MALMÜDÜRLÜĞÜ	27106	27	YAVUZELİ
383	OĞUZELİ MALMÜDÜRLÜĞÜ	27105	27	MERKEZ- (5216)
384	ARABAN MALMÜDÜRLÜĞÜ	27101	27	ARABAN
385	İSLAHİYE VERGİ DAİRESİ MÜDÜRLÜĞÜ	27203	27	İSLAHİYE
386	NİZİP VERGİ DAİRESİ MÜDÜRLÜĞÜ	27202	27	NİZİP
387	KOZANLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	27255	27	MERKEZ
388	GAZİKENT VERGİ DAİRESİ MÜDÜRLÜĞÜ	27254	27	MERKEZ
389	ŞAHİNBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	27253	27	MERKEZ
390	ŞEHİTKÂMİL VERGİ DAİRESİ MÜDÜRLÜĞÜ	27252	27	MERKEZ
391	SUBURCU VERGİ DAİRESİ MÜDÜRLÜĞÜ	27251	27	MERKEZ
392	GAZİANTEP İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	27250	27	MERKEZ
393	DOĞANKENT MALMÜDÜRLÜĞÜ	28115	28	DOĞANKENT
394	ÇAMOLUK MALMÜDÜRLÜĞÜ	28114	28	ÇAMOLUK
395	GÜCE MALMÜDÜRLÜĞÜ	28113	28	GÜCE
396	ÇANAKÇI MALMÜDÜRLÜĞÜ	28112	28	ÇANAKÇI
397	YAĞLIDERE MALMÜDÜRLÜĞÜ	28111	28	YAĞLIDERE
398	PİRAZİZ MALMÜDÜRLÜĞÜ	28110	28	PİRAZİZ
399	TİREBOLU MALMÜDÜRLÜĞÜ	28109	28	TİREBOLU
400	ŞEBİNKARAHİSAR MALMÜDÜRLÜĞÜ	28108	28	ŞEBİNKARAHİSAR
401	KEŞAP MALMÜDÜRLÜĞÜ	28107	28	KEŞAP
402	GÖRELE MALMÜDÜRLÜĞÜ	28106	28	GÖRELE
403	EYNESİL MALMÜDÜRLÜĞÜ	28105	28	EYNESİL
404	ESPİYE MALMÜDÜRLÜĞÜ	28104	28	ESPİYE
405	DERELİ MALMÜDÜRLÜĞÜ	28103	28	DERELİ
406	ALUCRA MALMÜDÜRLÜĞÜ	28101	28	ALUCRA
407	BULANCAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	28261	28	BULANCAK
408	GİRESUN VERGİ DAİRESİ MÜDÜRLÜĞÜ	28260	28	MERKEZ
409	KÜRTÜN MALMÜDÜRLÜĞÜ	29108	29	KÜRTÜN
410	KÖSE MALMÜDÜRLÜĞÜ	29107	29	KÖSE
411	TORUL MALMÜDÜRLÜĞÜ	29104	29	TORUL
412	ŞİRAN MALMÜDÜRLÜĞÜ	29103	29	ŞİRAN
413	KELKİT MALMÜDÜRLÜĞÜ	29102	29	KELKİT
414	GÜMÜŞHANE VERGİ DAİRESİ MÜDÜRLÜĞÜ\n	29260	29	MERKEZ
415	DERECİK MALMÜDÜRLÜĞÜ	30104	30	DERECİK
416	ŞEMDİNLİ MALMÜDÜRLÜĞÜ	30103	30	ŞEMDİNLİ
417	ÇUKURCA MALMÜDÜRLÜĞÜ	30102	30	ÇUKURCA
418	YÜKSEKOVA VERGİ DAİRESİ MÜDÜRLÜĞÜ	30261	30	YÜKSEKOVA
419	HAKKARİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	30260	30	MERKEZ
420	KUMLU MALMÜDÜRLÜĞÜ	31111	31	KUMLU
421	BELEN MALMÜDÜRLÜĞÜ	31110	31	BELEN
422	ERZİN MALMÜDÜRLÜĞÜ	31109	31	ERZİN
423	YAYLADAĞI MALMÜDÜRLÜĞÜ	31108	31	YAYLADAĞI
424	HASSA MALMÜDÜRLÜĞÜ	31103	31	HASSA
425	ALTINÖZÜ MALMÜDÜRLÜĞÜ	31101	31	ALTINÖZÜ
426	SAMANDAĞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	31263	31	SAMANDAĞ
427	REYHANLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	31262	31	REYHANLI
428	KIRIKHAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	31261	31	KIRIKHAN
429	DÖRTYOL  VERGİ DAİRESİ MÜDÜRLÜĞÜ	31260	31	DÖRTYOL
430	ASIM GÜNDÜZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	31290	31	İSKENDERUN
431	AKDENİZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	31281	31	İSKENDERUN
432	SAHİL VERGİ DAİRESİ MÜDÜRLÜĞÜ	31202	31	İSKENDERUN
433	ŞÜKRÜKANATLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	31280	31	MERKEZ
434	ANTAKYA VERGİ DAİRESİ MÜDÜRLÜĞÜ	31203	31	MERKEZ
435	23 TEMMUZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	31201	31	MERKEZ
436	YENİŞARBADEMLİ MALMÜDÜRLÜĞÜ	32112	32	YENİŞARBADEMLİ
437	GÖNEN MALMÜDÜRLÜĞÜ	32111	32	GÖNEN
438	AKSU MALMÜDÜRLÜĞÜ	32110	32	AKSU
439	ULUBORLU MALMÜDÜRLÜĞÜ	32108	32	ULUBORLU
440	ŞARKİKARAAĞAÇ MALMÜDÜRLÜĞÜ	32107	32	ŞARKİKARAAĞAÇ
441	SÜTÇÜLER MALMÜDÜRLÜĞÜ	32106	32	SÜTÇÜLER
442	SENİRKENT MALMÜDÜRLÜĞÜ	32105	32	SENİRKENT
443	KEÇİBORLU MALMÜDÜRLÜĞÜ	32104	32	KEÇİBORLU
444	GELENDOST MALMÜDÜRLÜĞÜ	32103	32	GELENDOST
445	ATABEY MALMÜDÜRLÜĞÜ	32101	32	ATABEY
446	YALVAÇ VERGİ DAİRESİ MÜDÜRLÜĞÜ	32262	32	YALVAÇ
447	EĞİRDİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	32261	32	EĞİRDİR
448	KAYMAKKAPI VERGİ DAİRESİ MÜDÜRLÜĞÜ	32260	32	MERKEZ
449	DAVRAZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	32201	32	MERKEZ
450	ÇAMLIYAYLA MALMÜDÜRLÜĞÜ	33109	33	ÇAMLIYAYLA
451	BOZYAZI MALMÜDÜRLÜĞÜ	33108	33	BOZYAZI
452	AYDINCIK MALMÜDÜRLÜĞÜ	33107	33	AYDINCIK
453	MUT MALMÜDÜRLÜĞÜ	33104	33	MUT
454	GÜLNAR MALMÜDÜRLÜĞÜ	33103	33	GÜLNAR
455	ŞEHİTKERİM VERGİ DAİRESİ MÜDÜRLÜĞÜ	33205	33	TARSUS
456	KIZILMURAT VERGİ DAİRESİ MÜDÜRLÜĞÜ	33204	33	TARSUS
457	ANAMUR VERGİ DAİRESİ MÜDÜRLÜĞÜ	33203	33	ANAMUR
458	SİLİFKE VERGİ DAİRESİ MÜDÜRLÜĞÜ	33202	33	SİLİFKE
459	ERDEMLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	33201	33	ERDEMLİ
460	MERSİN İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	33256	33	MERKEZ
461	TOROS VERGİ DAİRESİ MÜDÜRLÜĞÜ	33255	33	MERKEZ
462	LİMAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	33254	33	MERKEZ
463	URAY VERGİ DAİRESİ MÜDÜRLÜĞÜ	33252	33	MERKEZ
464	İSTİKLÂL VERGİ DAİRESİ MÜDÜRLÜĞÜ	33250	33	MERKEZ
465	KÜÇÜKYALI VERGİ DAİRESİ MÜDÜRLÜĞÜ	34262	34	MERKEZ
466	GÜNEŞLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34261	34	MERKEZ
467	KOCASİNAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	34260	34	MERKEZ
468	GÜNGÖREN VERGİ DAİRESİ MÜDÜRLÜĞÜ	34259	34	MERKEZ
469	RIHTIM VERASET VE HARÇLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34258	34	MERKEZ
470	TUNA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34257	34	MERKEZ
471	HİSAR VERASET VE HARÇLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34256	34	MERKEZ
472	ERENKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	34255	34	MERKEZ
473	KASIMPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34254	34	MERKEZ
474	YEDİTEPE VERASET VE HARÇLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34253	34	MERKEZ
475	ÜMRANİYE VERGİ DAİRESİ MÜDÜRLÜĞÜ	34252	34	MERKEZ
476	BEŞİKTAŞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34251	34	MERKEZ
477	İKİTELLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34249	34	MERKEZ
478	ZİNCİRLİKUYU VERGİ DAİRESİ MÜDÜRLÜĞÜ	34248	34	MERKEZ
479	MASLAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	34247	34	MERKEZ
480	KOZYATAĞI VERGİ DAİRESİ MÜDÜRLÜĞÜ	34246	34	MERKEZ
481	TUZLA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34245	34	MERKEZ
482	ANADOLU KURUMLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34244	34	MERKEZ
483	SULTANBEYLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34242	34	MERKEZ
484	MERTER VERGİ DAİRESİ MÜDÜRLÜĞÜ	34239	34	MERKEZ
485	KÜÇÜKKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	34237	34	MERKEZ
486	FATİH VERGİ DAİRESİ MÜDÜRLÜĞÜ	34236	34	MERKEZ
487	ESENLER VERGİ DAİRESİ MÜDÜRLÜĞÜ	34235	34	MERKEZ
488	DAVUTPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34234	34	MERKEZ
489	MARMARA KURUMLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34232	34	MERKEZ
490	BOĞAZİÇİ KURUMLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34231	34	MERKEZ
491	BÜYÜK MÜKELLEFLER VERGİ DAİRESİ BAŞKANLIĞI	34230	34	MERKEZ
492	YENİKAPI VERGİ DAİRESİ MÜDÜRLÜĞÜ	34224	34	MERKEZ
493	ALEMDAĞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34223	34	MERKEZ
494	ÇAMLICA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	34222	34	MERKEZ
495	VATAN İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	34221	34	MERKEZ
496	HALİÇ İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	34220	34	MERKEZ
497	ŞİLE VERGİ DAİRESİ MÜDÜRLÜĞÜ	34205	32	MERKEZ- (5216)
498	BÜYÜKÇEKMECE VERGİ DAİRESİ MÜDÜRLÜĞÜ	34204	32	MERKEZ- (5216)
499	SİLİVRİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34203	32	MERKEZ- (5216)
500	ESENYURT VERGİ DAİRESİ MÜDÜRLÜĞÜ	34298	32	MERKEZ
501	BEYLİKDÜZÜ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34297	32	MERKEZ
502	KÜÇÜKÇEKMECE VERGİ DAİRESİ MÜDÜRLÜĞÜ	34296	32	MERKEZ
503	ADALAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34295	32	MERKEZ- (5216)
504	AVCILAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34294	32	MERKEZ
505	YENİBOSNA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34293	32	MERKEZ
506	YAKACIK VERGİ DAİRESİ MÜDÜRLÜĞÜ	34292	32	MERKEZ
507	ATIŞALANI VERGİ DAİRESİ MÜDÜRLÜĞÜ	34291	32	MERKEZ
508	SARIGAZİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34288	32	MERKEZ
509	NAKİL VASITALARI VERGİ DAİRESİ MÜDÜRLÜĞÜ	34287	32	MERKEZ
510	KARTAL VERGİ DAİRESİ MÜDÜRLÜĞÜ	34285	32	MERKEZ
511	BAKIRKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	34284	32	MERKEZ
512	SARIYER VERGİ DAİRESİ MÜDÜRLÜĞÜ	34283	32	MERKEZ
513	BEYKOZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34281	32	MERKEZ
514	ZEYTİNBURNU VERGİ DAİRESİ MÜDÜRLÜĞÜ	34280	32	MERKEZ
515	KAĞITHANE VERGİ DAİRESİ MÜDÜRLÜĞÜ	34279	32	MERKEZ
516	HALKALI VERGİ DAİRESİ MÜDÜRLÜĞÜ	34278	32	MERKEZ
517	ÜSKÜDAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	34277	32	MERKEZ
518	ŞİŞLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	34276	32	MERKEZ
519	MECİDİYEKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	34274	32	MERKEZ
520	KOCAMUSTAFAPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34273	32	MERKEZ
521	KADIKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	34272	32	MERKEZ
522	HOCAPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34271	32	MERKEZ
523	GÖZTEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	34270	32	MERKEZ
524	GAZİOSMANPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34269	32	MERKEZ
525	BEYOĞLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	34266	32	MERKEZ
526	BEYAZIT VERGİ DAİRESİ MÜDÜRLÜĞÜ	34265	32	MERKEZ
527	BAYRAMPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	34264	32	MERKEZ
528	PENDİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	34263	32	MERKEZ
529	BEYDAĞ MALMÜDÜRLÜĞÜ	35120	35	BEYDAĞ
530	SEFERİHİSAR MALMÜDÜRLÜĞÜ	35114	35	MERKEZ- (5216)
531	KARABURUN MALMÜDÜRLÜĞÜ	35107	35	KARABURUN
532	FOÇA MALMÜDÜRLÜĞÜ	35106	35	MERKEZ- (5216)
533	DİKİLİ MALMÜDÜRLÜĞÜ	35105	35	DİKİLİ
534	MENDERES VERGİ DAİRESİ MÜDÜRLÜĞÜ	35215	35	MERKEZ- (5216)
535	ALİAĞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	35213	35	MERKEZ- (5216)
536	ÇEŞME VERGİ DAİRESİ MÜDÜRLÜĞÜ	35212	35	ÇEŞME
537	KİRAZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	35211	35	KİRAZ
538	KINIK VERGİ DAİRESİ MÜDÜRLÜĞÜ	35210	35	KINIK
539	SELÇUK VERGİ DAİRESİ MÜDÜRLÜĞÜ	35209	35	SELÇUK
540	URLA VERGİ DAİRESİ MÜDÜRLÜĞÜ	35208	35	MERKEZ- (5216)
541	KEMALPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	35207	35	MERKEZ- (5216)
542	TORBALI VERGİ DAİRESİ MÜDÜRLÜĞÜ	35206	35	MERKEZ- (5216)
543	TİRE VERGİ DAİRESİ MÜDÜRLÜĞÜ	35205	35	TİRE
544	ÖDEMİŞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	35204	35	ÖDEMİŞ
545	MENEMEN VERGİ DAİRESİ MÜDÜRLÜĞÜ	35203	35	MERKEZ- (5216)
546	BERGAMA VERGİ DAİRESİ MÜDÜRLÜĞÜ	35202	35	BERGAMA
547	BAYINDIR VERGİ DAİRESİ MÜDÜRLÜĞÜ	35201	35	BAYINDIR
548	ÇAKABEY İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	35270	35	MERKEZ
549	ÇİĞLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	35269	35	MERKEZ
550	EGE VERGİ DAİRESİ MÜDÜRLÜĞÜ	35268	35	MERKEZ
551	GAZİEMİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	35267	35	MERKEZ
552	BALÇOVA VERGİ DAİRESİ MÜDÜRLÜĞÜ	35266	35	MERKEZ
553	BORNOVA VERGİ DAİRESİ MÜDÜRLÜĞÜ	35264	35	MERKEZ
554	HASAN TAHSİN VERGİ DAİRESİ MÜDÜRLÜĞÜ	35263	35	MERKEZ
555	TAŞITLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	35262	35	MERKEZ
556	KADİFEKALE VERGİ DAİRESİ MÜDÜRLÜĞÜ	35261	35	MERKEZ
557	ŞİRİNYER VERGİ DAİRESİ MÜDÜRLÜĞÜ	35260	35	MERKEZ
558	KORDON VERGİ DAİRESİ MÜDÜRLÜĞÜ	35259	35	MERKEZ
559	KONAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	35258	35	MERKEZ
560	KEMERALTI VERGİ DAİRESİ MÜDÜRLÜĞÜ	35257	35	MERKEZ
561	KARŞIYAKA VERGİ DAİRESİ MÜDÜRLÜĞÜ	35256	35	MERKEZ
562	BELKAHVE VERGİ DAİRESİ MÜDÜRLÜĞÜ	35254	35	MERKEZ
563	YAMANLAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	35252	35	MERKEZ
564	9 EYLÜL VERGİ DAİRESİ MÜDÜRLÜĞÜ	35251	35	MERKEZ
565	İZMİR İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	35250	35	MERKEZ
566	AKYAKA MALMÜDÜRLÜĞÜ	36115	36	AKYAKA
567	SUSUZ MALMÜDÜRLÜĞÜ	36113	36	SUSUZ
568	SELİM MALMÜDÜRLÜĞÜ	36112	36	SELİM
569	SARIKAMIŞ MALMÜDÜRLÜĞÜ	36111	36	SARIKAMIŞ
570	KAĞIZMAN MALMÜDÜRLÜĞÜ	36109	36	KAĞIZMAN
571	DİGOR MALMÜDÜRLÜĞÜ	36105	36	DİGOR
572	ARPAÇAY MALMÜDÜRLÜĞÜ	36103	36	ARPAÇAY
573	KARS VERGİ DAİRESİ MÜDÜRLÜĞÜ	36260	36	MERKEZ
574	SEYDİLER MALMÜDÜRLÜĞÜ	37119	37	SEYDİLER
575	HANÖNÜ MALMÜDÜRLÜĞÜ	37118	37	HANÖNÜ
576	DOĞANYURT MALMÜDÜRLÜĞÜ	37117	37	DOĞANYURT
577	AĞLI MALMÜDÜRLÜĞÜ	37116	37	AĞLI
578	ŞENPAZAR MALMÜDÜRLÜĞÜ	37115	37	ŞENPAZAR
579	PINARBAŞI MALMÜDÜRLÜĞÜ	37114	37	PINARBAŞI
580	İHSANGAZİ MALMÜDÜRLÜĞÜ	37113	37	İHSANGAZİ
581	ABANA MALMÜDÜRLÜĞÜ	37112	37	ABANA
582	KÜRE MALMÜDÜRLÜĞÜ	37109	37	KÜRE
583	İNEBOLU MALMÜDÜRLÜĞÜ	37108	37	İNEBOLU
584	DEVREKANİ MALMÜDÜRLÜĞÜ	37107	37	DEVREKANİ
585	DADAY MALMÜDÜRLÜĞÜ	37106	37	DADAY
586	ÇATALZEYTİN MALMÜDÜRLÜĞÜ	37105	37	ÇATALZEYTİN
587	CİDE MALMÜDÜRLÜĞÜ	37104	37	CİDE
588	BOZKURT MALMÜDÜRLÜĞÜ	37103	37	BOZKURT
589	AZDAVAY MALMÜDÜRLÜĞÜ	37102	37	AZDAVAY
590	ARAÇ MALMÜDÜRLÜĞÜ	37101	37	ARAÇ
591	TAŞKÖPRÜ VERGİ DAİRESİ MÜDÜRLÜĞÜ	37262	37	TAŞKÖPRÜ
592	TOSYA VERGİ DAİRESİ MÜDÜRLÜĞÜ	37261	37	TOSYA
593	KASTAMONU VERGİ DAİRESİ MÜDÜRLÜĞÜ	37260	37	MERKEZ
594	ÖZVATAN MALMÜDÜRLÜĞÜ	38114	38	ÖZVATAN
595	HACILAR MALMÜDÜRLÜĞÜ	38113	38	MERKEZ- (5216)
596	AKKIŞLA MALMÜDÜRLÜĞÜ	38111	38	AKKIŞLA
597	YEŞİLHİSAR MALMÜDÜRLÜĞÜ	38110	38	YEŞİLHİSAR
598	YAHYALI MALMÜDÜRLÜĞÜ	38109	38	YAHYALI
599	TOMARZA MALMÜDÜRLÜĞÜ	38108	38	TOMARZA
600	SARIZ MALMÜDÜRLÜĞÜ	38107	38	SARIZ
601	SARIOĞLAN MALMÜDÜRLÜĞÜ	38106	38	SARIOĞLAN
602	İNCESU MALMÜDÜRLÜĞÜ	38104	38	İNCESU
603	FELAHİYE MALMÜDÜRLÜĞÜ	38103	38	FELAHİYE
604	BÜNYAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	38203	38	BÜNYAN
605	PINARBAŞI VERGİ DAİRESİ MÜDÜRLÜĞÜ	38202	38	PINARBAŞI
606	DEVELİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	38201	38	DEVELİ
607	GEVHER NESİBE VERGİ DAİRESİ MÜDÜRLÜĞÜ	38254	38	MERKEZ
608	KALEÖNÜ VERGİ DAİRESİ MÜDÜRLÜĞÜ	38253	38	MERKEZ
609	ERCİYES VERGİ DAİRESİ MÜDÜRLÜĞÜ	38252	38	38252
610	MİMAR SİNAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	38251	38	MERKEZ
611	KAYSERİ İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	38250	38	MERKEZ
612	VİZE MALMÜDÜRLÜĞÜ	39107	39	VİZE
613	PINARHİSAR MALMÜDÜRLÜĞÜ	39106	39	PINARHİSAR
614	PEHLİVANKÖY MALMÜDÜRLÜĞÜ	39105	39	PEHLİVANKÖY
615	KOFÇAZ MALMÜDÜRLÜĞÜ	39103	39	KOFÇAZ
616	DEMİRKÖY MALMÜDÜRLÜĞÜ	39102	39	DEMİRKÖY
617	BABAESKİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	39262	39	BABAESKİ
618	LÜLEBURGAZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	39261	39	LÜLEBURGAZ
619	KIRKLARELİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	39260	39	MERKEZ
620	BOZTEPE MALMÜDÜRLÜĞÜ	40106	40	BOZTEPE
621	AKÇAKENT MALMÜDÜRLÜĞÜ	40105	40	AKÇAKENT
622	AKPINAR MALMÜDÜRLÜĞÜ	40104	40	AKPINAR
623	MUCUR MALMÜDÜRLÜĞÜ	40103	40	MUCUR
624	ÇİÇEKDAĞI MALMÜDÜRLÜĞÜ	40101	40	ÇİÇEKDAĞI
625	KAMAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	40261	40	KAMAN
626	KIRŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	40260	40	MERKEZ
627	KANDIRA MALMÜDÜRLÜĞÜ	41103	41	MERKEZ- (5216)
628	DERİNCE VERGİ DAİRESİ MÜDÜRLÜĞÜ	41207	41	MERKEZ- (5216)
629	KÖRFEZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	41206	41	MERKEZ- (5216)
630	KARAMÜRSEL VERGİ DAİRESİ MÜDÜRLÜĞÜ	41205	41	MERKEZ- (5216)
631	GÖLCÜK VERGİ DAİRESİ MÜDÜRLÜĞÜ	41204	41	MERKEZ- (5216)
632	İLYASBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	41203	41	MERKEZ- (5216)
633	ULUÇINAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	41202	41	MERKEZ- (5216)
634	ACISU VERGİ DAİRESİ MÜDÜRLÜĞÜ	41290	41	MERKEZ
635	GEBZE İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	41254	41	MERKEZ GEBZE
636	ALEMDAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	41253	41	MERKEZ
637	TEPECİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	41252	41	MERKEZ
638	KOCAELİ İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	41250	41	MERKEZ
639	YALIHÜYÜK MALMÜDÜRLÜĞÜ	42131	42	YALIHÜYÜK
640	TUZLUKÇU MALMÜDÜRLÜĞÜ	42130	42	TUZLUKÇU
641	HALKAPINAR MALMÜDÜRLÜĞÜ	42129	42	HALKAPINAR
642	GÜNEYSINIR MALMÜDÜRLÜĞÜ	42128	42	GÜNEYSINIR
643	EMİRGAZİ MALMÜDÜRLÜĞÜ	42127	42	EMİRGAZİ
644	DERBENT MALMÜDÜRLÜĞÜ	42126	42	DERBENT
645	ÇELTİK MALMÜDÜRLÜĞÜ	42125	42	ÇELTİK
646	AHIRLI MALMÜDÜRLÜĞÜ	42124	42	AHIRLI
647	TAŞKENT MALMÜDÜRLÜĞÜ	42123	42	TAŞKENT
648	HÜYÜK MALMÜDÜRLÜĞÜ	42122	42	HÜYÜK
649	DEREBUCAK MALMÜDÜRLÜĞÜ	42121	42	DEREBUCAK
650	ALTINEKİN MALMÜDÜRLÜĞÜ	42119	42	ALTINEKİN
651	AKÖREN MALMÜDÜRLÜĞÜ	42118	42	AKÖREN
652	YUNAK MALMÜDÜRLÜĞÜ	42117	42	YUNAK
653	SARAYÖNÜ MALMÜDÜRLÜĞÜ	42115	42	SARAYÖNÜ
654	KADINHANI MALMÜDÜRLÜĞÜ	42111	42	KADINHANI
655	HADİM MALMÜDÜRLÜĞÜ	42109	42	HADİM
656	DOĞANHİSAR MALMÜDÜRLÜĞÜ	42106	42	DOĞANHİSAR
657	BOZKIR MALMÜDÜRLÜĞÜ	42103	42	BOZKIR
658	KARAPINAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	42210	42	KARAPINAR
659	KULU VERGİ DAİRESİ MÜDÜRLÜĞÜ	42209	42	KULU
660	ILGIN VERGİ DAİRESİ MÜDÜRLÜĞÜ	42208	42	ILGIN
661	SEYDİŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	42207	42	SEYDİŞEHİR
662	ÇUMRA VERGİ DAİRESİ MÜDÜRLÜĞÜ	42206	42	ÇUMRA
663	CİHANBEYLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	42205	42	CİHANBEYLİ
745	DATÇA MALMÜDÜRLÜĞÜ	48102	48	DATÇA
664	BEYŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	42204	42	BEYŞEHİR
665	EREĞLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	42202	42	EREĞLİ
666	AKŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	42201	42	AKŞEHİR
667	ALAADDİN VERGİ DAİRESİ MÜDÜRLÜĞÜ	42254	42	MERKEZ
668	MERAM VERGİ DAİRESİ MÜDÜRLÜĞÜ	42253	42	MERKEZ
669	MEVLANA VERGİ DAİRESİ MÜDÜRLÜĞÜ	42252	42	MERKEZ
670	SELÇUK VERGİ DAİRESİ MÜDÜRLÜĞÜ	42251	42	MERKEZ
671	KONYA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	42250	42	MERKEZ
672	PAZARLAR MALMÜDÜRLÜĞÜ	43112	43	PAZARLAR
673	ÇAVDARHİSAR MALMÜDÜRLÜĞÜ	43111	43	ÇAVDARHİSAR
674	ŞAPHANE MALMÜDÜRLÜĞÜ	43110	43	ŞAPHANE
675	HİSARCIK MALMÜDÜRLÜĞÜ	43109	43	HİSARCIK
676	DUMLUPINAR MALMÜDÜRLÜĞÜ	43108	43	DUMLUPINAR
677	ASLANAPA MALMÜDÜRLÜĞÜ	43107	43	ASLANAPA
678	DOMANİÇ MALMÜDÜRLÜĞÜ	43102	43	DOMANİÇ
679	ALTINTAŞ MALMÜDÜRLÜĞÜ	43101	43	ALTINTAŞ
680	EMET VERGİ DAİRESİ MÜDÜRLÜĞÜ	43263	43	EMET
681	TAVŞANLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	43262	43	TAVŞANLI
682	SİMAV VERGİ DAİRESİ MÜDÜRLÜĞÜ	43261	43	SİMAV
683	GEDİZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	43260	43	GEDİZ
684	ÇİNİLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	43280	43	MERKEZ
685	30 AĞUSTOS VERGİ DAİRESİ MÜDÜRLÜĞÜ	43201	43	MERKEZ
686	YAZIHAN MALMÜDÜRLÜĞÜ	44113	44	YAZIHAN
687	KULUNCAK MALMÜDÜRLÜĞÜ	44112	44	KULUNCAK
688	KALE MALMÜDÜRLÜĞÜ	44111	44	KALE
689	DOĞANYOL MALMÜDÜRLÜĞÜ	44110	44	DOĞANYOL
690	BATTALGAZİ MALMÜDÜRLÜĞÜ	44109	44	BATTALGAZİ
691	YEŞİLYURT MALMÜDÜRLÜĞÜ	44108	44	YEŞİLYURT
692	PÜTÜRGE MALMÜDÜRLÜĞÜ	44107	44	PÜTÜRGE
693	HEKİMHAN MALMÜDÜRLÜĞÜ	44106	44	HEKİMHAN
694	DOĞANŞEHİR MALMÜDÜRLÜĞÜ	44105	44	DOĞANŞEHİR
695	DARENDE MALMÜDÜRLÜĞÜ	44104	44	DARENDE
696	ARGUVAN MALMÜDÜRLÜĞÜ	44103	44	ARGUVAN
697	ARAPGİR MALMÜDÜRLÜĞÜ	44102	44	ARAPGİR
698	AKÇADAĞ MALMÜDÜRLÜĞÜ	44101	44	AKÇADAĞ
699	BEYDAĞI VERGİ DAİRESİ MÜDÜRLÜĞÜ	44252	44	MERKEZ
700	FIRAT VERGİ DAİRESİ MÜDÜRLÜĞÜ	44251	44	MERKEZ
701	KÖPRÜBAŞI MALMÜDÜRLÜĞÜ	45115	45	KÖPRÜBAŞI
702	GÖLMARMARA MALMÜDÜRLÜĞÜ	45114	45	GÖLMARMARA
703	AHMETLİ MALMÜDÜRLÜĞÜ	45113	45	AHMETLİ
704	SELENDİ MALMÜDÜRLÜĞÜ	45110	45	SELENDİ
705	KULA VERGİ DAİRESİ MÜDÜRLÜĞÜ	45211	45	KULA
706	GÖRDES VERGİ DAİRESİ MÜDÜRLÜĞÜ	45210	45	GÖRDES
707	TURGUTLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	45209	45	TURGUTLU
708	SOMA VERGİ DAİRESİ MÜDÜRLÜĞÜ	45208	45	SOMA
709	SARUHANLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	45207	45	SARUHANLI
710	SARIGÖL VERGİ DAİRESİ MÜDÜRLÜĞÜ	45206	45	SARIGÖL
711	SALİHLİ ADİL ORAL VERGİ DAİRESİ MÜDÜRLÜĞÜ	45205	45	SALİHLİ
712	KIRKAĞAÇ VERGİ DAİRESİ MÜDÜRLÜĞÜ	45204	45	KIRKAĞAÇ
713	DEMİRCİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	45203	45	DEMİRCİ
714	ALAŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	45202	45	ALAŞEHİR
715	AKHİSAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	45201	45	AKHİSAR
716	MESİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	45252	45	MERKEZ
717	ŞEHİT CİHAN GÜNEŞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	45251	45	MERKEZ
718	MANİSA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	45250	45	MERKEZ
719	NURHAK MALMÜDÜRLÜĞÜ	46109	46	NURHAK
720	EKİNÖZÜ MALMÜDÜRLÜĞÜ	46108	46	EKİNÖZÜ
721	ÇAĞLAYANCERİT MALMÜDÜRLÜĞÜ	46107	46	ÇAĞLAYANCERİT
722	TÜRKOĞLU MALMÜDÜRLÜĞÜ	46106	46	TÜRKOĞLU
723	GÖKSUN MALMÜDÜRLÜĞÜ	46104	46	GÖKSUN
724	ANDIRIN MALMÜDÜRLÜĞÜ	46102	46	ANDIRIN
725	PAZARCIK VERGİ DAİRESİ MÜDÜRLÜĞÜ	46262	46	PAZARCIK
726	AFŞİN VERGİ DAİRESİ MÜDÜRLÜĞÜ	46261	46	AFŞİN
727	ELBİSTAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	46260	46	ELBİSTAN
728	AKSU VERGİ DAİRESİ MÜDÜRLÜĞÜ	46280	46	MERKEZ
729	ASLANBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	46201	46	MERKEZ
730	YEŞİLLİ MALMÜDÜRLÜĞÜ	47113	47	YEŞİLLİ
731	DARGEÇİT MALMÜDÜRLÜĞÜ	47112	47	DARGEÇİT
732	SAVUR MALMÜDÜRLÜĞÜ	47110	47	SAVUR
733	ÖMERLİ MALMÜDÜRLÜĞÜ	47109	47	ÖMERLİ
734	MİDYAT MALMÜDÜRLÜĞÜ	47107	47	MİDYAT
735	MAZIDAĞI MALMÜDÜRLÜĞÜ	47106	47	MAZIDAĞI
736	DERİK MALMÜDÜRLÜĞÜ	47102	47	DERİK
737	NUSAYBİN VERGİ DAİRESİ MÜDÜRLÜĞÜ	47262	47	NUSAYBİN
738	KIZILTEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	47261	47	KIZILTEPE
739	MARDİN VERGİ DAİRESİ MÜDÜRLÜĞÜ	47260	47	MERKEZ
740	SEYDİKEMER MALMÜDÜRLÜĞÜ	48113	48	SEYDİEMER
741	KAVAKLIDERE MALMÜDÜRLÜĞÜ	48111	48	KAVAKLIDERE
742	ORTACA MALMÜDÜRLÜĞÜ	48110	48	ORTACA
743	DALAMAN MALMÜDÜRLÜĞÜ	48109	48	DALAMAN
744	ULA MALMÜDÜRLÜĞÜ	48108	48	ULA
746	YATAĞAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	48266	48	YATAĞAN
747	MARMARİS VERGİ DAİRESİ MÜDÜRLÜĞÜ	48265	48	MARMARİS
748	MİLAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	48264	48	MİLAS
749	KÖYCEĞİZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	48263	48	KÖYCEĞİZ
750	FETHİYE VERGİ DAİRESİ MÜDÜRLÜĞÜ	48262	48	FETHİYE
751	BODRUM VERGİ DAİRESİ MÜDÜRLÜĞÜ	48261	48	BODRUM
752	MUĞLA VERGİ DAİRESİ MÜDÜRLÜĞÜ	48260	48	MERKEZ
753	KORKUT MALMÜDÜRLÜĞÜ	49105	49	KORKUT
754	HASKÖY MALMÜDÜRLÜĞÜ	49104	49	HASKÖY
755	VARTO MALMÜDÜRLÜĞÜ	49103	49	VARTO
756	MALAZGİRT MALMÜDÜRLÜĞÜ	49102	49	MALAZGİRT
757	BULANIK MALMÜDÜRLÜĞÜ	49101	49	BULANIK
758	MUŞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	49260	49	MERKEZ
759	ACIGÖL MALMÜDÜRLÜĞÜ	50107	50	ACIGÖL
760	ÜRGÜP MALMÜDÜRLÜĞÜ	50106	50	ÜRGÜP
761	KOZAKLI MALMÜDÜRLÜĞÜ	50105	50	KOZAKLI
762	HACIBEKTAŞ MALMÜDÜRLÜĞÜ	50104	50	HACIBEKTAŞ
763	GÜLŞEHİR MALMÜDÜRLÜĞÜ	50103	50	GÜLŞEHİR
764	DERİNKUYU MALMÜDÜRLÜĞÜ	50102	50	DERİNKUYU
765	AVANOS MALMÜDÜRLÜĞÜ	50101	50	AVANOS
766	NEVŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	50260	50	MERKEZ
767	ÇİFTLİK MALMÜDÜRLÜĞÜ	51107	51	ÇİFTLİK
768	ALTUNHİSAR MALMÜDÜRLÜĞÜ	51106	51	ALTUNHİSAR
769	ULUKIŞLA MALMÜDÜRLÜĞÜ	51105	51	ULUKIŞLA
770	ÇAMARDI MALMÜDÜRLÜĞÜ	51103	51	ÇAMARDI
771	BOR VERGİ DAİRESİ MÜDÜRLÜĞÜ	51262	51	BOR
772	NİĞDE VERGİ DAİRESİ MÜDÜRLÜĞÜ	51260	51	MERKEZ
773	KABATAŞ MALMÜDÜRLÜĞÜ	52118	52	KABATAŞ
774	KABADÜZ MALMÜDÜRLÜĞÜ	52117	52	KABADÜZ
775	İKİZCE MALMÜDÜRLÜĞÜ	52116	52	İKİZCE
776	ÇAYBAŞI MALMÜDÜRLÜĞÜ	52115	52	ÇAYBAŞI
777	ÇATALPINAR MALMÜDÜRLÜĞÜ	52114	52	ÇATALPINAR
778	ÇAMAŞ MALMÜDÜRLÜĞÜ	52113	52	ÇAMAŞ
779	GÜRGENTEPE MALMÜDÜRLÜĞÜ	52112	52	GÜRGENTEPE
780	GÜLYALI MALMÜDÜRLÜĞÜ	52111	52	GÜLYALI
781	ULUBEY MALMÜDÜRLÜĞÜ	52109	52	ULUBEY
782	PERŞEMBE MALMÜDÜRLÜĞÜ	52108	52	PERŞEMBE
783	MESUDİYE MALMÜDÜRLÜĞÜ	52107	52	MESUDİYE
784	KUMRU MALMÜDÜRLÜĞÜ	52106	52	KUMRU
785	KORGAN MALMÜDÜRLÜĞÜ	52105	52	KORGAN
786	GÖLKÖY MALMÜDÜRLÜĞÜ	52104	52	GÖLKÖY
787	AYBASTI MALMÜDÜRLÜĞÜ	52102	52	AYBASTI
788	AKKUŞ MALMÜDÜRLÜĞÜ	52101	52	AKKUŞ
789	ÜNYE VERGİ DAİRESİ MÜDÜRLÜĞÜ	52262	52	ÜNYE
790	FATSA VERGİ DAİRESİ MÜDÜRLÜĞÜ	52261	52	FATSA
791	KÖPRÜBAŞI VERGİ DAİRESİ MÜDÜRLÜĞÜ	52260	52	MERKEZ
792	BOZTEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	52201	52	MERKEZ
793	İYİDERE MALMÜDÜRLÜĞÜ	53111	53	İYİDERE
794	HEMŞİN MALMÜDÜRLÜĞÜ	53110	53	HEMŞİN
795	DEREPAZARI MALMÜDÜRLÜĞÜ	53109	53	DEREPAZARI
796	GÜNEYSU MALMÜDÜRLÜĞÜ	53108	53	GÜNEYSU
797	KALKANDERE MALMÜDÜRLÜĞÜ	53106	53	KALKANDERE
798	İKİZDERE MALMÜDÜRLÜĞÜ	53105	53	İKİZDERE
799	FINDIKLI MALMÜDÜRLÜĞÜ	53104	53	FINDIKLI
800	ÇAMLIHEMŞİN MALMÜDÜRLÜĞÜ	53102	53	ÇAMLIHEMŞİN
801	ARDEŞEN VERGİ DAİRESİ MÜDÜRLÜĞÜ	53263	53	ARDEŞEN
802	PAZAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	53262	53	PAZAR
803	ÇAYELİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	53261	53	ÇAYELİ
804	YEŞİLÇAY VERGİ DAİRESİ MÜDÜRLÜĞÜ	53260	53	MERKEZ
805	KAÇKAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	53201	53	MERKEZ
806	KARAPÜRÇEK MALMÜDÜRLÜĞÜ	54111	54	MERKEZ- (5216)
807	TARAKLI MALMÜDÜRLÜĞÜ	54109	54	TARAKLI
808	PAMUKOVA MALMÜDÜRLÜĞÜ	54108	54	PAMUKOVA
809	KOCAALİ MALMÜDÜRLÜĞÜ	54107	54	KOCAALİ
810	KAYNARCA MALMÜDÜRLÜĞÜ	54105	54	KAYNARCA
811	KARASU VERGİ DAİRESİ MÜDÜRLÜĞÜ	54204	54	KARASU
812	HENDEK VERGİ DAİRESİ MÜDÜRLÜĞÜ	54203	54	MERKEZ- (5216)
813	GEYVE VERGİ DAİRESİ MÜDÜRLÜĞÜ	54202	54	GEYVE
814	AKYAZI VERGİ DAİRESİ MÜDÜRLÜĞÜ	54201	54	MERKEZ- (5216)
815	SAPANCA VERGİ DAİRESİ MÜDÜRLÜĞÜ	54253	54	MERKEZ- (5216)
816	ALİ FUAT CEBESOY VERGİ DAİRESİ MÜDÜRLÜĞÜ	54252	54	MERKEZ
817	GÜMRÜKÖNÜ VERGİ DAİRESİ MÜDÜRLÜĞÜ	54251	54	MERKEZ
818	SAKARYA İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	54250	54	MERKEZ
819	YAKAKENT MALMÜDÜRLÜĞÜ	55114	55	YAKAKENT
820	AYVACIK MALMÜDÜRLÜĞÜ	55113	55	AYVACIK
821	TEKKEKÖY MALMÜDÜRLÜĞÜ	55112	55	MERKEZ- (5216)
822	SALIPAZARI MALMÜDÜRLÜĞÜ	55111	55	SALIPAZARI
823	ONDOKUZ MAYIS MALMÜDÜRLÜĞÜ	55110	55	ONDOKUZ MAYIS
824	ASARCIK MALMÜDÜRLÜĞÜ	55109	55	ASARCIK
825	VEZİRKÖPRÜ MALMÜDÜRLÜĞÜ	55108	55	VEZİRKÖPRÜ
826	LADİK MALMÜDÜRLÜĞÜ	55106	55	LADİK
827	KAVAK MALMÜDÜRLÜĞÜ	55105	55	KAVAK
828	ALAÇAM MALMÜDÜRLÜĞÜ	55101	55	ALAÇAM
829	HAVZA VERGİ DAİRESİ MÜDÜRLÜĞÜ	55205	55	HAVZA
830	TERME VERGİ DAİRESİ MÜDÜRLÜĞÜ	55204	55	TERME
831	ÇARŞAMBA VERGİ DAİRESİ MÜDÜRLÜĞÜ	55203	55	ÇARŞAMBA
832	BAFRA VERGİ DAİRESİ MÜDÜRLÜĞÜ	55202	55	55202
833	ZAFER VERGİ DAİRESİ MÜDÜRLÜĞÜ	55290	55	MERKEZ
834	GAZİLER VERGİ DAİRESİ MÜDÜRLÜĞÜ	55252	55	MERKEZ
835	19 MAYIS VERGİ DAİRESİ MÜDÜRLÜĞÜ	55251	55	MERKEZ
836	ŞİRVAN MALMÜDÜRLÜĞÜ	56110	56	ŞİRVAN
837	PERVARİ MALMÜDÜRLÜĞÜ	56107	56	PERVARİ
838	KURTALAN MALMÜDÜRLÜĞÜ	56106	56	KURTALAN
839	ERUH MALMÜDÜRLÜĞÜ	56104	56	ERUH
840	BAYKAN MALMÜDÜRLÜĞÜ	56102	56	BAYKAN
841	SİİRT VERGİ DAİRESİ MÜDÜRLÜĞÜ	56260	56	MERKEZ
842	SARAYDÜZÜ MALMÜDÜRLÜĞÜ	57108	57	SARAYDÜZÜ
843	DİKMEN MALMÜDÜRLÜĞÜ	57107	57	DİKMEN
844	TÜRKELİ MALMÜDÜRLÜĞÜ	57106	57	TÜRKELİ
845	GERZE MALMÜDÜRLÜĞÜ	57105	57	GERZE
846	ERFELEK MALMÜDÜRLÜĞÜ	57104	57	ERFELEK
847	DURAĞAN MALMÜDÜRLÜĞÜ	57103	57	DURAĞAN
848	AYANCIK MALMÜDÜRLÜĞÜ	57101	57	AYANCIK
849	BOYABAT VERGİ DAİRESİ MÜDÜRLÜĞÜ	57261	57	BOYABAT
850	SİNOP VERGİ DAİRESİ MÜDÜRLÜĞÜ	57260	57	MERKEZ
851	ULAŞ MALMÜDÜRLÜĞÜ	58116	58	ULAŞ
852	GÖLOVA MALMÜDÜRLÜĞÜ	58115	58	GÖLOVA
853	DOĞANŞAR MALMÜDÜRLÜĞÜ	58114	58	DOĞANŞAR
854	ALTINYAYLA MALMÜDÜRLÜĞÜ	58113	58	ALTINYAYLA
855	AKINCILAR MALMÜDÜRLÜĞÜ	58112	58	AKINCILAR
856	ZARA MALMÜDÜRLÜĞÜ	58111	58	ZARA
857	YILDIZELİ MALMÜDÜRLÜĞÜ	58110	58	YILDIZELİ
858	SUŞEHRİ MALMÜDÜRLÜĞÜ	58109	58	SUŞEHRİ
859	KOYULHİSAR MALMÜDÜRLÜĞÜ	58107	58	KOYULHİSAR
860	KANGAL MALMÜDÜRLÜĞÜ	58106	58	KANGAL
861	İMRANLI MALMÜDÜRLÜĞÜ	58105	58	İMRANLI
862	HAFİK MALMÜDÜRLÜĞÜ	58104	58	HAFİK
863	GÜRÜN MALMÜDÜRLÜĞÜ	58103	58	GÜRÜN
864	GEMEREK MALMÜDÜRLÜĞÜ	58102	58	GEMEREK
865	DİVRİĞİ MALMÜDÜRLÜĞÜ	58101	58	DİVRİĞİ
866	ŞARKIŞLA VERGİ DAİRESİ MÜDÜRLÜĞÜ	58260	58	ŞARKIŞLA
867	SİTE VERGİ DAİRESİ MÜDÜRLÜĞÜ	58280	58	MERKEZ
868	KALE VERGİ DAİRESİ MÜDÜRLÜĞÜ	58201	58	MERKEZ
869	SARAY MALMÜDÜRLÜĞÜ	59106	59	SARAY
870	KAPAKLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	59267	59	KAPAKLI
871	ŞARKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	59266	59	ŞARKÖY
872	MURATLI VERGİ DAİRESİ MÜDÜRLÜĞÜ	59265	59	MURATLI
873	MALKARA VERGİ DAİRESİ MÜDÜRLÜĞÜ	59264	59	MALKARA
874	HAYRABOLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	59263	59	HAYRABOLU
875	ÇORLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	59262	59	ÇORLU
876	ÇERKEZKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	59261	59	ÇERKEZKÖY
877	ÇORLU İHTİSAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	59250	59	ÇORLU
878	NAMIK KEMAL VERGİ DAİRESİ MÜDÜRLÜĞÜ	59260	59	MERKEZ
879	SÜLEYMANPAŞA VERGİ DAİRESİ MÜDÜRLÜĞÜ	59201	59	MERKEZ
880	MARMARA EREĞLİSİ MALMÜDÜRLÜĞÜ	59108	59	MARMARA EREĞLİSİ
881	SULUSARAY MALMÜDÜRLÜĞÜ	60111	60	SULUSARAY
882	BAŞÇİFTLİK MALMÜDÜRLÜĞÜ	60110	60	BAŞÇİFTLİK
883	YEŞİLYURT MALMÜDÜRLÜĞÜ	60109	60	YEŞİLYURT
884	PAZAR MALMÜDÜRLÜĞÜ	60108	60	PAZAR
885	REŞADİYE MALMÜDÜRLÜĞÜ	60105	60	REŞADİYE
886	ARTOVA MALMÜDÜRLÜĞÜ	60102	60	ARTOVA
887	ALMUS MALMÜDÜRLÜĞÜ	60101	60	ALMUS
888	ZİLE VERGİ DAİRESİ MÜDÜRLÜĞÜ	60264	60	ZİLE
889	TURHAL VERGİ DAİRESİ MÜDÜRLÜĞÜ	60263	60	TURHAL
890	NİKSAR VERGİ DAİRESİ MÜDÜRLÜĞÜ	60262	60	NİKSAR
891	ERBAA VERGİ DAİRESİ MÜDÜRLÜĞÜ	60261	60	ERBAA
892	TOKAT VERGİ DAİRESİ MÜDÜRLÜĞÜ	60260	60	MERKEZ
893	KÖPRÜBAŞI MALMÜDÜRLÜĞÜ	61117	61	KÖPRÜBAŞI
894	HAYRAT MALMÜDÜRLÜĞÜ	61116	61	HAYRAT
895	DÜZKÖY MALMÜDÜRLÜĞÜ	61115	61	DÜZKÖY
896	DERNEKPAZARI MALMÜDÜRLÜĞÜ	61114	61	DERNEKPAZARI
897	ÇARŞIBAŞI MALMÜDÜRLÜĞÜ	61113	61	ÇARŞIBAŞI
898	ŞALPAZARI MALMÜDÜRLÜĞÜ	61112	61	ŞALPAZARI
899	BEŞİKDÜZÜ MALMÜDÜRLÜĞÜ	61111	61	BEŞİKDÜZÜ
900	YOMRA MALMÜDÜRLÜĞÜ	61110	61	YOMRA
901	TONYA MALMÜDÜRLÜĞÜ	61108	61	TONYA
902	SÜRMENE MALMÜDÜRLÜĞÜ	61107	61	SÜRMENE
903	MAÇKA MALMÜDÜRLÜĞÜ	61105	61	MAÇKA
904	ÇAYKARA MALMÜDÜRLÜĞÜ	61104	61	ÇAYKARA
905	ARSİN MALMÜDÜRLÜĞÜ	61103	61	ARSİN
906	ARAKLI MALMÜDÜRLÜĞÜ	61102	61	ARAKLI
907	VAKFIKEBİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	61262	61	VAKFIKEBİR
908	OF VERGİ DAİRESİ MÜDÜRLÜĞÜ	61261	61	OF
909	AKÇAABAT VERGİ DAİRESİ MÜDÜRLÜĞÜ	61260	61	AKÇAABAT
910	KARADENİZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	61280	61	MERKEZ
911	HIZIRBEY VERGİ DAİRESİ MÜDÜRLÜĞÜ	61201	61	MERKEZ
912	PÜLÜMÜR MALMÜDÜRLÜĞÜ	62107	62	PÜLÜMÜR
913	PERTEK MALMÜDÜRLÜĞÜ	62106	62	PERTEK
914	OVACIK MALMÜDÜRLÜĞÜ	62105	62	OVACIK
915	NAZİMİYE MALMÜDÜRLÜĞÜ	62104	62	NAZİMİYE
916	MAZGİRT MALMÜDÜRLÜĞÜ	62103	62	MAZGİRT
917	HOZAT MALMÜDÜRLÜĞÜ	62102	62	HOZAT
918	ÇEMİŞGEZEK MALMÜDÜRLÜĞÜ	62101	62	ÇEMİŞGEZEK
919	TUNCELİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	62260	62	MERKEZ
920	HARRAN MALMÜDÜRLÜĞÜ	63110	63	HARRAN
921	CEYLANPINAR MALMÜDÜRLÜĞÜ	63109	63	CEYLANPINAR
922	HİLVAN MALMÜDÜRLÜĞÜ	63105	63	HİLVAN
923	HALFETİ MALMÜDÜRLÜĞÜ	63104	63	HALFETİ
924	BOZOVA MALMÜDÜRLÜĞÜ	63103	63	BOZOVA
925	AKÇAKALE MALMÜDÜRLÜĞÜ	63101	63	AKÇAKALE
926	SURUÇ  VERGİ DAİRESİ MÜDÜRLÜĞÜ	63263	63	SURUÇ
927	BİRECİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	63262	63	BİRECİK
928	VİRANŞEHİR VERGİ DAİRESİ MÜDÜRLÜĞÜ	63261	63	VİRANŞEHİR
929	SİVEREK VERGİ DAİRESİ MÜDÜRLÜĞÜ	63260	63	SİVEREK
930	GÖBEKLİTEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	63281	63	MERKEZ
931	TOPÇU MEYDANI VERGİ DAİRESİ MÜDÜRLÜĞÜ	63280	63	MERKEZ
932	ŞEHİTLİK VERGİ DAİRESİ MÜDÜRLÜĞÜ	63201	63	MERKEZ
933	SİVASLI MALMÜDÜRLÜĞÜ	64105	64	SİVASLI
934	ULUBEY MALMÜDÜRLÜĞÜ	64104	64	ULUBEY
935	KARAHALLI MALMÜDÜRLÜĞÜ	64103	64	KARAHALLI
936	EŞME VERGİ DAİRESİ MÜDÜRLÜĞÜ	64262	64	EŞME
937	BANAZ VERGİ DAİRESİ MÜDÜRLÜĞÜ	64261	64	BANAZ
938	UŞAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	64260	64	MERKEZ
939	SARAY MALMÜDÜRLÜĞÜ	65111	65	SARAY
940	EDREMİT MALMÜDÜRLÜĞÜ	65110	65	EDREMİT
941	ÇALDIRAN MALMÜDÜRLÜĞÜ	65109	65	ÇALDIRAN
942	BAHÇESARAY MALMÜDÜRLÜĞÜ	65108	65	BAHÇESARAY
943	ÖZALP MALMÜDÜRLÜĞÜ	65107	65	ÖZALP
944	MURADİYE MALMÜDÜRLÜĞÜ	65106	65	MURADİYE
945	GÜRPINAR MALMÜDÜRLÜĞÜ	65105	65	GÜRPINAR
946	GEVAŞ MALMÜDÜRLÜĞÜ	65104	65	GEVAŞ
947	ÇATAK MALMÜDÜRLÜĞÜ	65102	65	ÇATAK
948	BAŞKALE MALMÜDÜRLÜĞÜ	65101	65	BAŞKALE
949	ERCİŞ VERGİ DAİRESİ MÜDÜRLÜĞÜ	65201	65	ERCİŞ
950	VAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	65260	65	MERKEZ
951	YENİFAKILI MALMÜDÜRLÜĞÜ	66113	66	YENİFAKILI
952	SARAYKENT MALMÜDÜRLÜĞÜ	66112	66	SARAYKENT
953	KADIŞEHRİ MALMÜDÜRLÜĞÜ	66111	66	KADIŞEHRİ
954	ÇANDIR MALMÜDÜRLÜĞÜ	66110	66	ÇANDIR
955	AYDINCIK MALMÜDÜRLÜĞÜ	66109	66	AYDINCIK
956	ŞEFAATLİ MALMÜDÜRLÜĞÜ	66106	66	ŞEFAATLİ
957	SARIKAYA MALMÜDÜRLÜĞÜ	66105	66	SARIKAYA
958	ÇEKEREK MALMÜDÜRLÜĞÜ	66104	66	ÇEKEREK
959	ÇAYIRALAN MALMÜDÜRLÜĞÜ	66103	66	ÇAYIRALAN
960	AKDAĞMADENİ MALMÜDÜRLÜĞÜ	66101	66	AKDAĞMADENİ
961	YERKÖY VERGİ DAİRESİ MÜDÜRLÜĞÜ	66263	66	YERKÖY
962	SORGUN VERGİ DAİRESİ MÜDÜRLÜĞÜ	66262	66	SORGUN
963	BOĞAZLIYAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	66261	66	BOĞAZLIYAN
964	YOZGAT VERGİ DAİRESİ MÜDÜRLÜĞÜ	66260	66	MERKEZ
965	GÖKÇEBEY MALMÜDÜRLÜĞÜ	67113	67	GÖKÇEBEY
966	ALAPLI MALMÜDÜRLÜĞÜ	67110	67	ALAPLI
967	DEVREK VERGİ DAİRESİ MÜDÜRLÜĞÜ	67264	67	DEVREK
968	ÇAYCUMA VERGİ DAİRESİ MÜDÜRLÜĞÜ	67263	67	ÇAYCUMA
969	EREĞLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	67261	67	EREĞLİ
970	KARA ELMAS VERGİ DAİRESİ MÜDÜRLÜĞÜ	67280	67	MERKEZ
971	UZUNMEHMET VERGİ DAİRESİ MÜDÜRLÜĞÜ	67201	67	MERKEZ
972	GÜLAĞAÇ MALMÜDÜRLÜĞÜ	68106	68	GÜLAĞAÇ
973	ESKİL MALMÜDÜRLÜĞÜ	68105	68	ESKİL
974	SARIYAHŞİ MALMÜDÜRLÜĞÜ	68104	68	SARIYAHŞİ
975	ORTAKÖY MALMÜDÜRLÜĞÜ	68103	68	ORTAKÖY
976	GÜZELYURT MALMÜDÜRLÜĞÜ	68102	68	GÜZELYURT
977	AĞAÇÖREN MALMÜDÜRLÜĞÜ	68101	68	AĞAÇÖREN
978	AKSARAY VERGİ DAİRESİ MÜDÜRLÜĞÜ	68201	68	MERKEZ
979	DEMİRÖZÜ MALMÜDÜRLÜĞÜ	69102	69	DEMİRÖZÜ
980	AYDINTEPE MALMÜDÜRLÜĞÜ	69101	69	AYDINTEPE
981	BAYBURT VERGİ DAİRESİ MÜDÜRLÜĞÜ	69201	69	MERKEZ
982	SARIVELİLER MALMÜDÜRLÜĞÜ	70105	70	SARIVELİLER
983	BAŞYAYLA MALMÜDÜRLÜĞÜ	70104	70	BAŞYAYLA
984	KAZIM KARABEKİR MALMÜDÜRLÜĞÜ	70103	70	KAZIM KARABEKİR
985	ERMENEK MALMÜDÜRLÜĞÜ	70102	70	ERMENEK
986	AYRANCI MALMÜDÜRLÜĞÜ	70101	70	AYRANCI
987	KARAMAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	70201	70	MERKEZ
988	KARAKEÇİLİ MALMÜDÜRLÜĞÜ	71107	71	KARAKEÇİLİ
989	ÇELEBİ MALMÜDÜRLÜĞÜ	71106	71	ÇELEBİ
990	BALIŞEYH MALMÜDÜRLÜĞÜ	71105	71	BALIŞEYH
991	SULAKYURT MALMÜDÜRLÜĞÜ	71103	71	SULAKYURT
992	KESKİN MALMÜDÜRLÜĞÜ	71102	71	KESKİN
993	DELİCE MALMÜDÜRLÜĞÜ	71101	71	DELİCE
994	KALETEPE VERGİ DAİRESİ MÜDÜRLÜĞÜ	71202	71	MERKEZ
995	IRMAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	71201	71	MERKEZ
996	SASON MALMÜDÜRLÜĞÜ	72105	72	SASON
997	KOZLUK MALMÜDÜRLÜĞÜ	72104	72	KOZLUK
998	HASANKEYF MALMÜDÜRLÜĞÜ	72103	72	HASANKEYF
999	GERCÜŞ MALMÜDÜRLÜĞÜ	72102	72	GERCÜŞ
1000	BEŞİRİ MALMÜDÜRLÜĞÜ	72101	72	BEŞİRİ
1001	BATMAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	72260	72	MERKEZ
1002	ULUDERE MALMÜDÜRLÜĞÜ	73106	73	ULUDERE
1003	İDİL MALMÜDÜRLÜĞÜ	73104	73	İDİL
1004	GÜÇLÜKONAK MALMÜDÜRLÜĞÜ	73103	73	GÜÇLÜKONAK
1005	BEYTÜŞŞEBAP MALMÜDÜRLÜĞÜ	73101	73	BEYTÜŞŞEBAP
1006	SİLOPİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	73262	73	SİLOPİ
1007	CİZRE VERGİ DAİRESİ MÜDÜRLÜĞÜ	73261	73	CİZRE
1008	ŞIRNAK VERGİ DAİRESİ MÜDÜRLÜĞÜ	73260	73	MERKEZ
1009	ULUS MALMÜDÜRLÜĞÜ	74103	74	ULUS
1010	KURUCAŞİLE MALMÜDÜRLÜĞÜ	74102	74	KURUCAŞİLE
1011	AMASRA MALMÜDÜRLÜĞÜ	74101	74	AMASRA
1012	BARTIN VERGİ DAİRESİ MÜDÜRLÜĞÜ	74260	74	MERKEZ
1013	POSOF MALMÜDÜRLÜĞÜ	75105	75	POSOF
1014	HANAK MALMÜDÜRLÜĞÜ	75104	75	HANAK
1015	GÖLE MALMÜDÜRLÜĞÜ	75103	75	GÖLE
1016	DAMAL MALMÜDÜRLÜĞÜ	75102	75	DAMAL
1017	ÇILDIR MALMÜDÜRLÜĞÜ	75101	75	ÇILDIR
1018	ARDAHAN VERGİ DAİRESİ MÜDÜRLÜĞÜ	75201	75	MERKEZ
1019	TUZLUCA MALMÜDÜRLÜĞÜ	76103	76	TUZLUCA
1020	KARAKOYUNLU MALMÜDÜRLÜĞÜ	76102	76	KARAKOYUNLU
1021	ARALIK MALMÜDÜRLÜĞÜ	76101	76	ARALIK
1022	IĞDIR VERGİ DAİRESİ MÜDÜRLÜĞÜ	76201	76	MERKEZ
1023	ÇINARCIK MALMÜDÜRLÜĞÜ	77103	77	ÇINARCIK
1024	ARMUTLU MALMÜDÜRLÜĞÜ	77102	77	ARMUTLU
1025	ALTINOVA MALMÜDÜRLÜĞÜ	77101	77	ALTINOVA
1026	YALOVA VERGİ DAİRESİ MÜDÜRLÜĞÜ	77201	77	MERKEZ
1027	YENİCE MALMÜDÜRLÜĞÜ	78105	78	YENİCE
1028	OVACIK MALMÜDÜRLÜĞÜ	78103	78	OVACIK
1029	ESKİPAZAR MALMÜDÜRLÜĞÜ	78102	78	ESKİPAZAR
1030	EFLANİ MALMÜDÜRLÜĞÜ	78101	78	EFLANİ
1031	SAFRANBOLU VERGİ DAİRESİ MÜDÜRLÜĞÜ	78260	78	SAFRANBOLU
1032	KARABÜK VERGİ DAİRESİ MÜDÜRLÜĞÜ	78201	78	MERKEZ
1033	KİLİS VERGİ DAİRESİ MÜDÜRLÜĞÜ	79201	79	MERKEZ
1034	TOPRAKKALE MALMÜDÜRLÜĞÜ	80106	80	TOPRAKKALE
1035	SUMBAS MALMÜDÜRLÜĞÜ	80105	80	SUMBAS
1036	HASANBEYLİ MALMÜDÜRLÜĞÜ	80104	80	HASANBEYLİ
1037	DÜZİÇİ MALMÜDÜRLÜĞÜ	80102	80	DÜZİÇİ
1038	BAHÇE  MALMÜDÜRLÜĞÜ	80101	80	BAHÇE
1039	KADİRLİ VERGİ DAİRESİ MÜDÜRLÜĞÜ	80260	80	KADİRLİ
1040	OSMANİYE VERGİ DAİRESİ MÜDÜRLÜĞÜ	80201	80	MERKEZ
1041	YIĞILCA MALMÜDÜRLÜĞÜ	81107	81	YIĞILCA
1042	KAYNAŞLI MALMÜDÜRLÜĞÜ	81106	81	KAYNAŞLI
1043	GÜMÜŞOVA MALMÜDÜRLÜĞÜ	81105	81	GÜMÜŞOVA
1044	GÖLYAKA MALMÜDÜRLÜĞÜ	81104	81	GÖLYAKA
1045	ÇİLİMLİ MALMÜDÜRLÜĞÜ	81103	81	ÇİLİMLİ
1046	CUMAYERİ MALMÜDÜRLÜĞÜ	81102	81	CUMAYERİ
1047	AKÇAKOCA VERGİ DAİRESİ MÜDÜRLÜĞÜ	81261	81	AKÇAKOCA
1048	DÜZCE VERGİ DAİRESİ MÜDÜRLÜĞÜ	81260	81	MERKEZ
\.


--
-- Name: borchesaplamaturleri_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.borchesaplamaturleri_id_seq', 2, true);


--
-- Name: carikartlar_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.carikartlar_id_seq', 1, false);


--
-- Name: carikartlar_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.carikartlar_sequence', 1, true);


--
-- Name: carikartlar_tip_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.carikartlar_tip_seq', 1, true);


--
-- Name: caritipleri_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.caritipleri_id_seq', 1, true);


--
-- Name: caritipleri_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.caritipleri_sequence', 2, true);


--
-- Name: faturalar_cari_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturalar_cari_seq', 1, false);


--
-- Name: faturalar_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturalar_id_seq', 1, false);


--
-- Name: faturalar_kullanici_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturalar_kullanici_seq', 1, false);


--
-- Name: faturalar_odemetipi_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturalar_odemetipi_seq', 1, false);


--
-- Name: faturalar_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturalar_sequence', 1, false);


--
-- Name: faturalar_tipi_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturalar_tipi_seq', 1, false);


--
-- Name: faturatipleri_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturatipleri_id_seq', 1, false);


--
-- Name: faturatipleri_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.faturatipleri_sequence', 5, true);


--
-- Name: ilceler_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.ilceler_id_seq', 982, true);


--
-- Name: ilceler_ilid_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.ilceler_ilid_seq', 1, false);


--
-- Name: iller_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.iller_id_seq', 81, true);


--
-- Name: kasa_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.kasa_id_seq', 1, false);


--
-- Name: kasahareketadlari_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.kasahareketadlari_id_seq', 4, true);


--
-- Name: kasahareketleri_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.kasahareketleri_id_seq', 1, false);


--
-- Name: kasahareketleri_kullanici_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.kasahareketleri_kullanici_seq', 1, false);


--
-- Name: kasahareketleri_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.kasahareketleri_sequence', 1, false);


--
-- Name: kullanicilar_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.kullanicilar_id_seq', 1, false);


--
-- Name: kullanicilar_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.kullanicilar_sequence', 2, true);


--
-- Name: odemetipleri_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.odemetipleri_id_seq', 1, false);


--
-- Name: odemetipleri_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.odemetipleri_sequence', 5, true);


--
-- Name: oturum_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.oturum_id_seq', 1, false);


--
-- Name: stokbirimleri_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokbirimleri_id_seq', 1, false);


--
-- Name: stokbirimleri_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokbirimleri_sequence', 9, true);


--
-- Name: stokgruplari_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokgruplari_id_seq', 14, true);


--
-- Name: stokgruplari_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokgruplari_sequence', 107, true);


--
-- Name: stokhareketleri_kullanici_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokhareketleri_kullanici_seq', 1, false);


--
-- Name: stokkartlari_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokkartlari_id_seq', 1, false);


--
-- Name: stokkartlari_kdv_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokkartlari_kdv_seq', 1, false);


--
-- Name: stokkartlari_sequence; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokkartlari_sequence', 1, false);


--
-- Name: stokkartlari_tedarikci_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokkartlari_tedarikci_seq', 1, false);


--
-- Name: stokkartlari_uretici_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.stokkartlari_uretici_seq', 1, false);


--
-- Name: teraziler_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.teraziler_id_seq', 33, true);


--
-- Name: terazimodel_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.terazimodel_id_seq', 1, false);


--
-- Name: ureticiler_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.ureticiler_id_seq', 132, true);


--
-- Name: vergidaireleri_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.vergidaireleri_id_seq', 1048, true);


--
-- Name: vergidaireleri_il_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.vergidaireleri_il_seq', 3, true);


--
-- Name: borchesaplamaturleri borchesaplamaturleri_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.borchesaplamaturleri
    ADD CONSTRAINT borchesaplamaturleri_pkey PRIMARY KEY (id);


--
-- Name: carikartlar carikartlar_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.carikartlar
    ADD CONSTRAINT carikartlar_pkey PRIMARY KEY (id);


--
-- Name: caritipleri caritipleri_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.caritipleri
    ADD CONSTRAINT caritipleri_pkey PRIMARY KEY (id);


--
-- Name: faturalar faturalar_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturalar
    ADD CONSTRAINT faturalar_pkey PRIMARY KEY (id);


--
-- Name: faturatipleri faturatipleri_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.faturatipleri
    ADD CONSTRAINT faturatipleri_pkey PRIMARY KEY (id);


--
-- Name: ilceler ilceler_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ilceler
    ADD CONSTRAINT ilceler_pkey PRIMARY KEY (id);


--
-- Name: iller iller_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.iller
    ADD CONSTRAINT iller_pkey PRIMARY KEY (id);


--
-- Name: kasa kasa_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kasa
    ADD CONSTRAINT kasa_pkey PRIMARY KEY (id);


--
-- Name: kasahareketadlari kasahareketadlari_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kasahareketadlari
    ADD CONSTRAINT kasahareketadlari_pkey PRIMARY KEY (id);


--
-- Name: kasahareketleri kasahareketleri_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kasahareketleri
    ADD CONSTRAINT kasahareketleri_pkey PRIMARY KEY (id);


--
-- Name: kullanicilar kullanicilar_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.kullanicilar
    ADD CONSTRAINT kullanicilar_pkey PRIMARY KEY (id);


--
-- Name: odemetipleri odemetipleri_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.odemetipleri
    ADD CONSTRAINT odemetipleri_pkey PRIMARY KEY (id);


--
-- Name: oturum oturum_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.oturum
    ADD CONSTRAINT oturum_pkey PRIMARY KEY (id);


--
-- Name: stokbirimleri stokbirimleri_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokbirimleri
    ADD CONSTRAINT stokbirimleri_pkey PRIMARY KEY (id);


--
-- Name: stokgruplari stokgruplari_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokgruplari
    ADD CONSTRAINT stokgruplari_pkey PRIMARY KEY (id);


--
-- Name: stokkartlari stokkartlari_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.stokkartlari
    ADD CONSTRAINT stokkartlari_pkey PRIMARY KEY (id);


--
-- Name: teraziler teraziler_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.teraziler
    ADD CONSTRAINT teraziler_pkey PRIMARY KEY (id);


--
-- Name: ureticiler ureticiler_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.ureticiler
    ADD CONSTRAINT ureticiler_pkey PRIMARY KEY (id);


--
-- Name: vergidaireleri vergidaireleri_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.vergidaireleri
    ADD CONSTRAINT vergidaireleri_pkey PRIMARY KEY (id);


--
-- PostgreSQL database dump complete
--


