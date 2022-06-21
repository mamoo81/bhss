PGDMP         (                z         	   mhss_data    13.7 (Debian 13.7-0+deb11u1)    13.7 (Debian 13.7-0+deb11u1) �    �           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                      false            �           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                      false            �           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                      false            �           1262    26731 	   mhss_data    DATABASE     ^   CREATE DATABASE mhss_data WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE = 'tr_TR.UTF-8';
    DROP DATABASE mhss_data;
                postgres    false            �            1259    26732    carikartlar_id_seq    SEQUENCE     {   CREATE SEQUENCE public.carikartlar_id_seq
    START WITH 2
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.carikartlar_id_seq;
       public          postgres    false            �            1259    26734    carikartlar    TABLE     �  CREATE TABLE public.carikartlar (
    id bigint DEFAULT nextval('public.carikartlar_id_seq'::regclass) NOT NULL,
    ad text NOT NULL,
    tip bigint NOT NULL,
    vergi_no text NOT NULL,
    vergi_daire text NOT NULL,
    il text,
    ilce text,
    adres text,
    mail text,
    telefon character varying(10) DEFAULT 0,
    tarih date DEFAULT CURRENT_TIMESTAMP NOT NULL,
    aciklama text,
    yetkili text
);
    DROP TABLE public.carikartlar;
       public         heap    postgres    false    200            �            1259    26743    carikartlar_sequence    SEQUENCE     }   CREATE SEQUENCE public.carikartlar_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.carikartlar_sequence;
       public          postgres    false            �            1259    26745    carikartlar_tip_seq    SEQUENCE     |   CREATE SEQUENCE public.carikartlar_tip_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE public.carikartlar_tip_seq;
       public          postgres    false    201            �           0    0    carikartlar_tip_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE public.carikartlar_tip_seq OWNED BY public.carikartlar.tip;
          public          postgres    false    203            �            1259    26747    caritipleri    TABLE     S   CREATE TABLE public.caritipleri (
    id bigint NOT NULL,
    tip text NOT NULL
);
    DROP TABLE public.caritipleri;
       public         heap    postgres    false            �            1259    26753    caritipleri_id_seq    SEQUENCE     {   CREATE SEQUENCE public.caritipleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.caritipleri_id_seq;
       public          postgres    false    204            �           0    0    caritipleri_id_seq    SEQUENCE OWNED BY     I   ALTER SEQUENCE public.caritipleri_id_seq OWNED BY public.caritipleri.id;
          public          postgres    false    205            �            1259    26755    caritipleri_sequence    SEQUENCE     }   CREATE SEQUENCE public.caritipleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.caritipleri_sequence;
       public          postgres    false    204            �           0    0    caritipleri_sequence    SEQUENCE OWNED BY     K   ALTER SEQUENCE public.caritipleri_sequence OWNED BY public.caritipleri.id;
          public          postgres    false    206            �            1259    26757 	   faturalar    TABLE     �  CREATE TABLE public.faturalar (
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
    DROP TABLE public.faturalar;
       public         heap    postgres    false            �            1259    26767    faturalar_cari_seq    SEQUENCE     {   CREATE SEQUENCE public.faturalar_cari_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.faturalar_cari_seq;
       public          postgres    false    207            �           0    0    faturalar_cari_seq    SEQUENCE OWNED BY     I   ALTER SEQUENCE public.faturalar_cari_seq OWNED BY public.faturalar.cari;
          public          postgres    false    208            �            1259    26769    faturalar_id_seq    SEQUENCE     y   CREATE SEQUENCE public.faturalar_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 '   DROP SEQUENCE public.faturalar_id_seq;
       public          postgres    false    207            �           0    0    faturalar_id_seq    SEQUENCE OWNED BY     E   ALTER SEQUENCE public.faturalar_id_seq OWNED BY public.faturalar.id;
          public          postgres    false    209            �            1259    26771    faturalar_kullanici_seq    SEQUENCE     �   CREATE SEQUENCE public.faturalar_kullanici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.faturalar_kullanici_seq;
       public          postgres    false    207            �           0    0    faturalar_kullanici_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.faturalar_kullanici_seq OWNED BY public.faturalar.kullanici;
          public          postgres    false    210            �            1259    26773    faturalar_odemetipi_seq    SEQUENCE     �   CREATE SEQUENCE public.faturalar_odemetipi_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.faturalar_odemetipi_seq;
       public          postgres    false    207            �           0    0    faturalar_odemetipi_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.faturalar_odemetipi_seq OWNED BY public.faturalar.odemetipi;
          public          postgres    false    211            �            1259    26775    faturalar_sequence    SEQUENCE     {   CREATE SEQUENCE public.faturalar_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.faturalar_sequence;
       public          postgres    false            �            1259    26777    faturalar_tipi_seq    SEQUENCE     {   CREATE SEQUENCE public.faturalar_tipi_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.faturalar_tipi_seq;
       public          postgres    false    207            �           0    0    faturalar_tipi_seq    SEQUENCE OWNED BY     I   ALTER SEQUENCE public.faturalar_tipi_seq OWNED BY public.faturalar.tipi;
          public          postgres    false    213            �            1259    26779    faturatipleri    TABLE     U   CREATE TABLE public.faturatipleri (
    id bigint NOT NULL,
    tip text NOT NULL
);
 !   DROP TABLE public.faturatipleri;
       public         heap    postgres    false            �            1259    26785    faturatipleri_id_seq    SEQUENCE     }   CREATE SEQUENCE public.faturatipleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.faturatipleri_id_seq;
       public          postgres    false    214            �           0    0    faturatipleri_id_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.faturatipleri_id_seq OWNED BY public.faturatipleri.id;
          public          postgres    false    215            �            1259    26787    faturatipleri_sequence    SEQUENCE        CREATE SEQUENCE public.faturatipleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 -   DROP SEQUENCE public.faturatipleri_sequence;
       public          postgres    false    214            �           0    0    faturatipleri_sequence    SEQUENCE OWNED BY     O   ALTER SEQUENCE public.faturatipleri_sequence OWNED BY public.faturatipleri.id;
          public          postgres    false    216            �            1259    26789    ilceler    TABLE     j   CREATE TABLE public.ilceler (
    id bigint NOT NULL,
    ilid bigint NOT NULL,
    ilce text NOT NULL
);
    DROP TABLE public.ilceler;
       public         heap    postgres    false            �            1259    26795    ilceler_id_seq    SEQUENCE     w   CREATE SEQUENCE public.ilceler_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 %   DROP SEQUENCE public.ilceler_id_seq;
       public          postgres    false    217            �           0    0    ilceler_id_seq    SEQUENCE OWNED BY     A   ALTER SEQUENCE public.ilceler_id_seq OWNED BY public.ilceler.id;
          public          postgres    false    218            �            1259    26797    ilceler_ilid_seq    SEQUENCE     y   CREATE SEQUENCE public.ilceler_ilid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 '   DROP SEQUENCE public.ilceler_ilid_seq;
       public          postgres    false    217            �           0    0    ilceler_ilid_seq    SEQUENCE OWNED BY     E   ALTER SEQUENCE public.ilceler_ilid_seq OWNED BY public.ilceler.ilid;
          public          postgres    false    219            �            1259    26799    iller    TABLE     L   CREATE TABLE public.iller (
    id bigint NOT NULL,
    il text NOT NULL
);
    DROP TABLE public.iller;
       public         heap    postgres    false            �            1259    26805    iller_id_seq    SEQUENCE     u   CREATE SEQUENCE public.iller_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 #   DROP SEQUENCE public.iller_id_seq;
       public          postgres    false    220            �           0    0    iller_id_seq    SEQUENCE OWNED BY     =   ALTER SEQUENCE public.iller_id_seq OWNED BY public.iller.id;
          public          postgres    false    221            �            1259    26807    kasa    TABLE     V   CREATE TABLE public.kasa (
    id bigint NOT NULL,
    para numeric(18,3) NOT NULL
);
    DROP TABLE public.kasa;
       public         heap    postgres    false            �            1259    26810    kasa_id_seq    SEQUENCE     t   CREATE SEQUENCE public.kasa_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 "   DROP SEQUENCE public.kasa_id_seq;
       public          postgres    false    222            �           0    0    kasa_id_seq    SEQUENCE OWNED BY     ;   ALTER SEQUENCE public.kasa_id_seq OWNED BY public.kasa.id;
          public          postgres    false    223            �            1259    26812    kasahareketleri    TABLE     �   CREATE TABLE public.kasahareketleri (
    id bigint NOT NULL,
    miktar money NOT NULL,
    kullanici bigint NOT NULL,
    islem text,
    tarih timestamp without time zone NOT NULL,
    kar money,
    evrakno text,
    aciklama text
);
 #   DROP TABLE public.kasahareketleri;
       public         heap    postgres    false            �            1259    26818    kasahareketleri_id_seq    SEQUENCE        CREATE SEQUENCE public.kasahareketleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 -   DROP SEQUENCE public.kasahareketleri_id_seq;
       public          postgres    false    224            �           0    0    kasahareketleri_id_seq    SEQUENCE OWNED BY     Q   ALTER SEQUENCE public.kasahareketleri_id_seq OWNED BY public.kasahareketleri.id;
          public          postgres    false    225            �            1259    26820    kasahareketleri_kullanici_seq    SEQUENCE     �   CREATE SEQUENCE public.kasahareketleri_kullanici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 4   DROP SEQUENCE public.kasahareketleri_kullanici_seq;
       public          postgres    false    224            �           0    0    kasahareketleri_kullanici_seq    SEQUENCE OWNED BY     _   ALTER SEQUENCE public.kasahareketleri_kullanici_seq OWNED BY public.kasahareketleri.kullanici;
          public          postgres    false    226            �            1259    26822    kasahareketleri_sequence    SEQUENCE     �   CREATE SEQUENCE public.kasahareketleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 /   DROP SEQUENCE public.kasahareketleri_sequence;
       public          postgres    false            �            1259    26824    kullanicilar    TABLE     �  CREATE TABLE public.kullanicilar (
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
     DROP TABLE public.kullanicilar;
       public         heap    postgres    false            �            1259    26833    kullanicilar_id_seq    SEQUENCE     |   CREATE SEQUENCE public.kullanicilar_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE public.kullanicilar_id_seq;
       public          postgres    false    228            �           0    0    kullanicilar_id_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE public.kullanicilar_id_seq OWNED BY public.kullanicilar.id;
          public          postgres    false    229            �            1259    26835    kullanicilar_sequence    SEQUENCE     ~   CREATE SEQUENCE public.kullanicilar_sequence
    START WITH 2
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.kullanicilar_sequence;
       public          postgres    false            �            1259    26837    odemetipleri    TABLE     T   CREATE TABLE public.odemetipleri (
    id bigint NOT NULL,
    tip text NOT NULL
);
     DROP TABLE public.odemetipleri;
       public         heap    postgres    false            �            1259    26843    odemetipleri_id_seq    SEQUENCE     |   CREATE SEQUENCE public.odemetipleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE public.odemetipleri_id_seq;
       public          postgres    false    231            �           0    0    odemetipleri_id_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE public.odemetipleri_id_seq OWNED BY public.odemetipleri.id;
          public          postgres    false    232            �            1259    26845    odemetipleri_sequence    SEQUENCE     ~   CREATE SEQUENCE public.odemetipleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.odemetipleri_sequence;
       public          postgres    false    231            �           0    0    odemetipleri_sequence    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.odemetipleri_sequence OWNED BY public.odemetipleri.id;
          public          postgres    false    233            �            1259    26847    oturum    TABLE     �   CREATE TABLE public.oturum (
    id bigint NOT NULL,
    username text NOT NULL,
    giristarihi timestamp without time zone NOT NULL
);
    DROP TABLE public.oturum;
       public         heap    postgres    false            �            1259    26853    oturum_id_seq    SEQUENCE     v   CREATE SEQUENCE public.oturum_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 $   DROP SEQUENCE public.oturum_id_seq;
       public          postgres    false    234            �           0    0    oturum_id_seq    SEQUENCE OWNED BY     ?   ALTER SEQUENCE public.oturum_id_seq OWNED BY public.oturum.id;
          public          postgres    false    235            �            1259    26855    stokbirimleri    TABLE     W   CREATE TABLE public.stokbirimleri (
    id bigint NOT NULL,
    birim text NOT NULL
);
 !   DROP TABLE public.stokbirimleri;
       public         heap    postgres    false            �            1259    26861    stokbirimleri_id_seq    SEQUENCE     }   CREATE SEQUENCE public.stokbirimleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.stokbirimleri_id_seq;
       public          postgres    false    236            �           0    0    stokbirimleri_id_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.stokbirimleri_id_seq OWNED BY public.stokbirimleri.id;
          public          postgres    false    237            �            1259    26863    stokbirimleri_sequence    SEQUENCE        CREATE SEQUENCE public.stokbirimleri_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 -   DROP SEQUENCE public.stokbirimleri_sequence;
       public          postgres    false            �            1259    26865    stokgruplari    TABLE     U   CREATE TABLE public.stokgruplari (
    id bigint NOT NULL,
    grup text NOT NULL
);
     DROP TABLE public.stokgruplari;
       public         heap    postgres    false            �            1259    26871    stokgruplari_id_seq    SEQUENCE     |   CREATE SEQUENCE public.stokgruplari_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE public.stokgruplari_id_seq;
       public          postgres    false    239            �           0    0    stokgruplari_id_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE public.stokgruplari_id_seq OWNED BY public.stokgruplari.id;
          public          postgres    false    240            �            1259    26873    stokgruplari_sequence    SEQUENCE     �   CREATE SEQUENCE public.stokgruplari_sequence
    START WITH 100
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.stokgruplari_sequence;
       public          postgres    false    239            �           0    0    stokgruplari_sequence    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.stokgruplari_sequence OWNED BY public.stokgruplari.id;
          public          postgres    false    241            �            1259    26875    stokhareketleri    TABLE     *  CREATE TABLE public.stokhareketleri (
    barkod character varying(13) NOT NULL,
    islem_no character varying,
    islem_turu character varying NOT NULL,
    islem_miktari numeric(18,3) NOT NULL,
    tarih timestamp without time zone NOT NULL,
    kullanici bigint NOT NULL,
    aciklama text
);
 #   DROP TABLE public.stokhareketleri;
       public         heap    postgres    false            �            1259    26881    stokhareketleri_kullanici_seq    SEQUENCE     �   CREATE SEQUENCE public.stokhareketleri_kullanici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 4   DROP SEQUENCE public.stokhareketleri_kullanici_seq;
       public          postgres    false    242            �           0    0    stokhareketleri_kullanici_seq    SEQUENCE OWNED BY     _   ALTER SEQUENCE public.stokhareketleri_kullanici_seq OWNED BY public.stokhareketleri.kullanici;
          public          postgres    false    243            �            1259    26883    stokkartlari    TABLE       CREATE TABLE public.stokkartlari (
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
     DROP TABLE public.stokkartlari;
       public         heap    postgres    false            �            1259    26891    stokkartlari_id_seq    SEQUENCE     |   CREATE SEQUENCE public.stokkartlari_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE public.stokkartlari_id_seq;
       public          postgres    false    244            �           0    0    stokkartlari_id_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE public.stokkartlari_id_seq OWNED BY public.stokkartlari.id;
          public          postgres    false    245            �            1259    26893    stokkartlari_kdv_seq    SEQUENCE     �   CREATE SEQUENCE public.stokkartlari_kdv_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.stokkartlari_kdv_seq;
       public          postgres    false    244            �           0    0    stokkartlari_kdv_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.stokkartlari_kdv_seq OWNED BY public.stokkartlari.kdv;
          public          postgres    false    246            �            1259    26895    stokkartlari_sequence    SEQUENCE     ~   CREATE SEQUENCE public.stokkartlari_sequence
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.stokkartlari_sequence;
       public          postgres    false            �            1259    26897    stokkartlari_tedarikci_seq    SEQUENCE     �   CREATE SEQUENCE public.stokkartlari_tedarikci_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 1   DROP SEQUENCE public.stokkartlari_tedarikci_seq;
       public          postgres    false    244            �           0    0    stokkartlari_tedarikci_seq    SEQUENCE OWNED BY     Y   ALTER SEQUENCE public.stokkartlari_tedarikci_seq OWNED BY public.stokkartlari.tedarikci;
          public          postgres    false    248            �            1259    26899    stokkartlari_uretici_seq    SEQUENCE     �   CREATE SEQUENCE public.stokkartlari_uretici_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 /   DROP SEQUENCE public.stokkartlari_uretici_seq;
       public          postgres    false    244            �           0    0    stokkartlari_uretici_seq    SEQUENCE OWNED BY     U   ALTER SEQUENCE public.stokkartlari_uretici_seq OWNED BY public.stokkartlari.uretici;
          public          postgres    false    249            �            1259    26901 	   teraziler    TABLE     S   CREATE TABLE public.teraziler (
    id bigint NOT NULL,
    marka text NOT NULL
);
    DROP TABLE public.teraziler;
       public         heap    postgres    false            �            1259    26907    teraziler_id_seq    SEQUENCE     y   CREATE SEQUENCE public.teraziler_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 '   DROP SEQUENCE public.teraziler_id_seq;
       public          postgres    false    250            �           0    0    teraziler_id_seq    SEQUENCE OWNED BY     E   ALTER SEQUENCE public.teraziler_id_seq OWNED BY public.teraziler.id;
          public          postgres    false    251            �            1259    26909    terazimodel    TABLE     U   CREATE TABLE public.terazimodel (
    id bigint NOT NULL,
    model text NOT NULL
);
    DROP TABLE public.terazimodel;
       public         heap    postgres    false            �            1259    26915    terazimodel_id_seq    SEQUENCE     {   CREATE SEQUENCE public.terazimodel_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 )   DROP SEQUENCE public.terazimodel_id_seq;
       public          postgres    false    252            �           0    0    terazimodel_id_seq    SEQUENCE OWNED BY     I   ALTER SEQUENCE public.terazimodel_id_seq OWNED BY public.terazimodel.id;
          public          postgres    false    253            �            1259    26917 
   ureticiler    TABLE     Q   CREATE TABLE public.ureticiler (
    id bigint NOT NULL,
    ad text NOT NULL
);
    DROP TABLE public.ureticiler;
       public         heap    postgres    false            �            1259    26923    ureticiler_id_seq    SEQUENCE     z   CREATE SEQUENCE public.ureticiler_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 (   DROP SEQUENCE public.ureticiler_id_seq;
       public          postgres    false    254            �           0    0    ureticiler_id_seq    SEQUENCE OWNED BY     G   ALTER SEQUENCE public.ureticiler_id_seq OWNED BY public.ureticiler.id;
          public          postgres    false    255                        1259    26925    vergidaireleri    TABLE     �   CREATE TABLE public.vergidaireleri (
    id bigint NOT NULL,
    ad text NOT NULL,
    kod text NOT NULL,
    il bigint NOT NULL,
    ilce text NOT NULL
);
 "   DROP TABLE public.vergidaireleri;
       public         heap    postgres    false                       1259    26931    vergidaireleri_id_seq    SEQUENCE     ~   CREATE SEQUENCE public.vergidaireleri_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.vergidaireleri_id_seq;
       public          postgres    false    256            �           0    0    vergidaireleri_id_seq    SEQUENCE OWNED BY     O   ALTER SEQUENCE public.vergidaireleri_id_seq OWNED BY public.vergidaireleri.id;
          public          postgres    false    257                       1259    26933    vergidaireleri_il_seq    SEQUENCE     ~   CREATE SEQUENCE public.vergidaireleri_il_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.vergidaireleri_il_seq;
       public          postgres    false    256            �           0    0    vergidaireleri_il_seq    SEQUENCE OWNED BY     O   ALTER SEQUENCE public.vergidaireleri_il_seq OWNED BY public.vergidaireleri.il;
          public          postgres    false    258            �           2604    26935    carikartlar tip    DEFAULT     r   ALTER TABLE ONLY public.carikartlar ALTER COLUMN tip SET DEFAULT nextval('public.carikartlar_tip_seq'::regclass);
 >   ALTER TABLE public.carikartlar ALTER COLUMN tip DROP DEFAULT;
       public          postgres    false    203    201            �           2604    26936    caritipleri id    DEFAULT     p   ALTER TABLE ONLY public.caritipleri ALTER COLUMN id SET DEFAULT nextval('public.caritipleri_id_seq'::regclass);
 =   ALTER TABLE public.caritipleri ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    205    204            �           2604    26937    faturalar id    DEFAULT     l   ALTER TABLE ONLY public.faturalar ALTER COLUMN id SET DEFAULT nextval('public.faturalar_id_seq'::regclass);
 ;   ALTER TABLE public.faturalar ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    209    207            �           2604    26938    faturalar kullanici    DEFAULT     z   ALTER TABLE ONLY public.faturalar ALTER COLUMN kullanici SET DEFAULT nextval('public.faturalar_kullanici_seq'::regclass);
 B   ALTER TABLE public.faturalar ALTER COLUMN kullanici DROP DEFAULT;
       public          postgres    false    210    207            �           2604    26939    faturalar cari    DEFAULT     p   ALTER TABLE ONLY public.faturalar ALTER COLUMN cari SET DEFAULT nextval('public.faturalar_cari_seq'::regclass);
 =   ALTER TABLE public.faturalar ALTER COLUMN cari DROP DEFAULT;
       public          postgres    false    208    207            �           2604    26940    faturalar odemetipi    DEFAULT     z   ALTER TABLE ONLY public.faturalar ALTER COLUMN odemetipi SET DEFAULT nextval('public.faturalar_odemetipi_seq'::regclass);
 B   ALTER TABLE public.faturalar ALTER COLUMN odemetipi DROP DEFAULT;
       public          postgres    false    211    207            �           2604    26941    faturalar tipi    DEFAULT     p   ALTER TABLE ONLY public.faturalar ALTER COLUMN tipi SET DEFAULT nextval('public.faturalar_tipi_seq'::regclass);
 =   ALTER TABLE public.faturalar ALTER COLUMN tipi DROP DEFAULT;
       public          postgres    false    213    207            �           2604    26942    faturatipleri id    DEFAULT     t   ALTER TABLE ONLY public.faturatipleri ALTER COLUMN id SET DEFAULT nextval('public.faturatipleri_id_seq'::regclass);
 ?   ALTER TABLE public.faturatipleri ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    215    214            �           2604    26943 
   ilceler id    DEFAULT     h   ALTER TABLE ONLY public.ilceler ALTER COLUMN id SET DEFAULT nextval('public.ilceler_id_seq'::regclass);
 9   ALTER TABLE public.ilceler ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    218    217            �           2604    26944    ilceler ilid    DEFAULT     l   ALTER TABLE ONLY public.ilceler ALTER COLUMN ilid SET DEFAULT nextval('public.ilceler_ilid_seq'::regclass);
 ;   ALTER TABLE public.ilceler ALTER COLUMN ilid DROP DEFAULT;
       public          postgres    false    219    217            �           2604    26945    iller id    DEFAULT     d   ALTER TABLE ONLY public.iller ALTER COLUMN id SET DEFAULT nextval('public.iller_id_seq'::regclass);
 7   ALTER TABLE public.iller ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    221    220            �           2604    26946    kasa id    DEFAULT     b   ALTER TABLE ONLY public.kasa ALTER COLUMN id SET DEFAULT nextval('public.kasa_id_seq'::regclass);
 6   ALTER TABLE public.kasa ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    223    222            �           2604    26947    kasahareketleri id    DEFAULT     x   ALTER TABLE ONLY public.kasahareketleri ALTER COLUMN id SET DEFAULT nextval('public.kasahareketleri_id_seq'::regclass);
 A   ALTER TABLE public.kasahareketleri ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    225    224            �           2604    26948    kasahareketleri kullanici    DEFAULT     �   ALTER TABLE ONLY public.kasahareketleri ALTER COLUMN kullanici SET DEFAULT nextval('public.kasahareketleri_kullanici_seq'::regclass);
 H   ALTER TABLE public.kasahareketleri ALTER COLUMN kullanici DROP DEFAULT;
       public          postgres    false    226    224            �           2604    26949    kullanicilar id    DEFAULT     r   ALTER TABLE ONLY public.kullanicilar ALTER COLUMN id SET DEFAULT nextval('public.kullanicilar_id_seq'::regclass);
 >   ALTER TABLE public.kullanicilar ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    229    228            �           2604    26950    odemetipleri id    DEFAULT     r   ALTER TABLE ONLY public.odemetipleri ALTER COLUMN id SET DEFAULT nextval('public.odemetipleri_id_seq'::regclass);
 >   ALTER TABLE public.odemetipleri ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    232    231            �           2604    26951 	   oturum id    DEFAULT     f   ALTER TABLE ONLY public.oturum ALTER COLUMN id SET DEFAULT nextval('public.oturum_id_seq'::regclass);
 8   ALTER TABLE public.oturum ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    235    234            �           2604    26952    stokbirimleri id    DEFAULT     t   ALTER TABLE ONLY public.stokbirimleri ALTER COLUMN id SET DEFAULT nextval('public.stokbirimleri_id_seq'::regclass);
 ?   ALTER TABLE public.stokbirimleri ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    237    236            �           2604    26953    stokgruplari id    DEFAULT     r   ALTER TABLE ONLY public.stokgruplari ALTER COLUMN id SET DEFAULT nextval('public.stokgruplari_id_seq'::regclass);
 >   ALTER TABLE public.stokgruplari ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    240    239            �           2604    26954    stokhareketleri kullanici    DEFAULT     �   ALTER TABLE ONLY public.stokhareketleri ALTER COLUMN kullanici SET DEFAULT nextval('public.stokhareketleri_kullanici_seq'::regclass);
 H   ALTER TABLE public.stokhareketleri ALTER COLUMN kullanici DROP DEFAULT;
       public          postgres    false    243    242            �           2604    26955    stokkartlari id    DEFAULT     r   ALTER TABLE ONLY public.stokkartlari ALTER COLUMN id SET DEFAULT nextval('public.stokkartlari_id_seq'::regclass);
 >   ALTER TABLE public.stokkartlari ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    245    244            �           2604    26956    stokkartlari kdv    DEFAULT     t   ALTER TABLE ONLY public.stokkartlari ALTER COLUMN kdv SET DEFAULT nextval('public.stokkartlari_kdv_seq'::regclass);
 ?   ALTER TABLE public.stokkartlari ALTER COLUMN kdv DROP DEFAULT;
       public          postgres    false    246    244            �           2604    26957    stokkartlari tedarikci    DEFAULT     �   ALTER TABLE ONLY public.stokkartlari ALTER COLUMN tedarikci SET DEFAULT nextval('public.stokkartlari_tedarikci_seq'::regclass);
 E   ALTER TABLE public.stokkartlari ALTER COLUMN tedarikci DROP DEFAULT;
       public          postgres    false    248    244            �           2604    26958    stokkartlari uretici    DEFAULT     |   ALTER TABLE ONLY public.stokkartlari ALTER COLUMN uretici SET DEFAULT nextval('public.stokkartlari_uretici_seq'::regclass);
 C   ALTER TABLE public.stokkartlari ALTER COLUMN uretici DROP DEFAULT;
       public          postgres    false    249    244            �           2604    26959    teraziler id    DEFAULT     l   ALTER TABLE ONLY public.teraziler ALTER COLUMN id SET DEFAULT nextval('public.teraziler_id_seq'::regclass);
 ;   ALTER TABLE public.teraziler ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    251    250            �           2604    26960    terazimodel id    DEFAULT     p   ALTER TABLE ONLY public.terazimodel ALTER COLUMN id SET DEFAULT nextval('public.terazimodel_id_seq'::regclass);
 =   ALTER TABLE public.terazimodel ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    253    252            �           2604    26961    ureticiler id    DEFAULT     n   ALTER TABLE ONLY public.ureticiler ALTER COLUMN id SET DEFAULT nextval('public.ureticiler_id_seq'::regclass);
 <   ALTER TABLE public.ureticiler ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    255    254            �           2604    26962    vergidaireleri id    DEFAULT     v   ALTER TABLE ONLY public.vergidaireleri ALTER COLUMN id SET DEFAULT nextval('public.vergidaireleri_id_seq'::regclass);
 @   ALTER TABLE public.vergidaireleri ALTER COLUMN id DROP DEFAULT;
       public          postgres    false    257    256            �           2604    26963    vergidaireleri il    DEFAULT     v   ALTER TABLE ONLY public.vergidaireleri ALTER COLUMN il SET DEFAULT nextval('public.vergidaireleri_il_seq'::regclass);
 @   ALTER TABLE public.vergidaireleri ALTER COLUMN il DROP DEFAULT;
       public          postgres    false    258    256            �          0    26734    carikartlar 
   TABLE DATA           �   COPY public.carikartlar (id, ad, tip, vergi_no, vergi_daire, il, ilce, adres, mail, telefon, tarih, aciklama, yetkili) FROM stdin;
    public          postgres    false    201   .�       �          0    26747    caritipleri 
   TABLE DATA           .   COPY public.caritipleri (id, tip) FROM stdin;
    public          postgres    false    204   g�       �          0    26757 	   faturalar 
   TABLE DATA           �   COPY public.faturalar (id, fatura_no, kullanici, durum, toplamtutar, odenentutar, kalantutar, cari, tarih, evrakno, odemetipi, tipi, aciklama) FROM stdin;
    public          postgres    false    207   ��       �          0    26779    faturatipleri 
   TABLE DATA           0   COPY public.faturatipleri (id, tip) FROM stdin;
    public          postgres    false    214   ��       �          0    26789    ilceler 
   TABLE DATA           1   COPY public.ilceler (id, ilid, ilce) FROM stdin;
    public          postgres    false    217   �       �          0    26799    iller 
   TABLE DATA           '   COPY public.iller (id, il) FROM stdin;
    public          postgres    false    220   �       �          0    26807    kasa 
   TABLE DATA           (   COPY public.kasa (id, para) FROM stdin;
    public          postgres    false    222   l�       �          0    26812    kasahareketleri 
   TABLE DATA           f   COPY public.kasahareketleri (id, miktar, kullanici, islem, tarih, kar, evrakno, aciklama) FROM stdin;
    public          postgres    false    224   ��       �          0    26824    kullanicilar 
   TABLE DATA           �   COPY public.kullanicilar (id, username, password, ad, soyad, cepno, tarih, kasayetki, iadeyetki, stokyetki, cariyetki, ayaryetki) FROM stdin;
    public          postgres    false    228   ��       �          0    26837    odemetipleri 
   TABLE DATA           /   COPY public.odemetipleri (id, tip) FROM stdin;
    public          postgres    false    231   ��       �          0    26847    oturum 
   TABLE DATA           ;   COPY public.oturum (id, username, giristarihi) FROM stdin;
    public          postgres    false    234   @�       �          0    26855    stokbirimleri 
   TABLE DATA           2   COPY public.stokbirimleri (id, birim) FROM stdin;
    public          postgres    false    236   |�       �          0    26865    stokgruplari 
   TABLE DATA           0   COPY public.stokgruplari (id, grup) FROM stdin;
    public          postgres    false    239   ��       �          0    26875    stokhareketleri 
   TABLE DATA           r   COPY public.stokhareketleri (barkod, islem_no, islem_turu, islem_miktari, tarih, kullanici, aciklama) FROM stdin;
    public          postgres    false    242   ��       �          0    26883    stokkartlari 
   TABLE DATA           �   COPY public.stokkartlari (id, barkod, ad, birim, miktar, grup, afiyat, sfiyat, kdv, tarih, aciklama, kdvdahil, otv, otvdahil, kod, tedarikci, uretici, mensei) FROM stdin;
    public          postgres    false    244   ��       �          0    26901 	   teraziler 
   TABLE DATA           .   COPY public.teraziler (id, marka) FROM stdin;
    public          postgres    false    250   ��       �          0    26909    terazimodel 
   TABLE DATA           0   COPY public.terazimodel (id, model) FROM stdin;
    public          postgres    false    252   ��       �          0    26917 
   ureticiler 
   TABLE DATA           ,   COPY public.ureticiler (id, ad) FROM stdin;
    public          postgres    false    254   �       �          0    26925    vergidaireleri 
   TABLE DATA           ?   COPY public.vergidaireleri (id, ad, kod, il, ilce) FROM stdin;
    public          postgres    false    256   �      �           0    0    carikartlar_id_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.carikartlar_id_seq', 1, false);
          public          postgres    false    200            �           0    0    carikartlar_sequence    SEQUENCE SET     B   SELECT pg_catalog.setval('public.carikartlar_sequence', 1, true);
          public          postgres    false    202                        0    0    carikartlar_tip_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.carikartlar_tip_seq', 1, true);
          public          postgres    false    203                       0    0    caritipleri_id_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.caritipleri_id_seq', 1, false);
          public          postgres    false    205                       0    0    caritipleri_sequence    SEQUENCE SET     B   SELECT pg_catalog.setval('public.caritipleri_sequence', 2, true);
          public          postgres    false    206                       0    0    faturalar_cari_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.faturalar_cari_seq', 1, false);
          public          postgres    false    208                       0    0    faturalar_id_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.faturalar_id_seq', 1, false);
          public          postgres    false    209                       0    0    faturalar_kullanici_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.faturalar_kullanici_seq', 1, false);
          public          postgres    false    210                       0    0    faturalar_odemetipi_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.faturalar_odemetipi_seq', 1, false);
          public          postgres    false    211                       0    0    faturalar_sequence    SEQUENCE SET     A   SELECT pg_catalog.setval('public.faturalar_sequence', 1, false);
          public          postgres    false    212                       0    0    faturalar_tipi_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.faturalar_tipi_seq', 1, false);
          public          postgres    false    213            	           0    0    faturatipleri_id_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.faturatipleri_id_seq', 1, false);
          public          postgres    false    215            
           0    0    faturatipleri_sequence    SEQUENCE SET     D   SELECT pg_catalog.setval('public.faturatipleri_sequence', 5, true);
          public          postgres    false    216                       0    0    ilceler_id_seq    SEQUENCE SET     >   SELECT pg_catalog.setval('public.ilceler_id_seq', 982, true);
          public          postgres    false    218                       0    0    ilceler_ilid_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.ilceler_ilid_seq', 1, false);
          public          postgres    false    219                       0    0    iller_id_seq    SEQUENCE SET     ;   SELECT pg_catalog.setval('public.iller_id_seq', 81, true);
          public          postgres    false    221                       0    0    kasa_id_seq    SEQUENCE SET     :   SELECT pg_catalog.setval('public.kasa_id_seq', 1, false);
          public          postgres    false    223                       0    0    kasahareketleri_id_seq    SEQUENCE SET     E   SELECT pg_catalog.setval('public.kasahareketleri_id_seq', 1, false);
          public          postgres    false    225                       0    0    kasahareketleri_kullanici_seq    SEQUENCE SET     L   SELECT pg_catalog.setval('public.kasahareketleri_kullanici_seq', 1, false);
          public          postgres    false    226                       0    0    kasahareketleri_sequence    SEQUENCE SET     G   SELECT pg_catalog.setval('public.kasahareketleri_sequence', 1, false);
          public          postgres    false    227                       0    0    kullanicilar_id_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('public.kullanicilar_id_seq', 1, false);
          public          postgres    false    229                       0    0    kullanicilar_sequence    SEQUENCE SET     C   SELECT pg_catalog.setval('public.kullanicilar_sequence', 1, true);
          public          postgres    false    230                       0    0    odemetipleri_id_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('public.odemetipleri_id_seq', 1, false);
          public          postgres    false    232                       0    0    odemetipleri_sequence    SEQUENCE SET     C   SELECT pg_catalog.setval('public.odemetipleri_sequence', 5, true);
          public          postgres    false    233                       0    0    oturum_id_seq    SEQUENCE SET     <   SELECT pg_catalog.setval('public.oturum_id_seq', 1, false);
          public          postgres    false    235                       0    0    stokbirimleri_id_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.stokbirimleri_id_seq', 1, false);
          public          postgres    false    237                       0    0    stokbirimleri_sequence    SEQUENCE SET     D   SELECT pg_catalog.setval('public.stokbirimleri_sequence', 8, true);
          public          postgres    false    238                       0    0    stokgruplari_id_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('public.stokgruplari_id_seq', 14, true);
          public          postgres    false    240                       0    0    stokgruplari_sequence    SEQUENCE SET     E   SELECT pg_catalog.setval('public.stokgruplari_sequence', 107, true);
          public          postgres    false    241                       0    0    stokhareketleri_kullanici_seq    SEQUENCE SET     L   SELECT pg_catalog.setval('public.stokhareketleri_kullanici_seq', 1, false);
          public          postgres    false    243                       0    0    stokkartlari_id_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('public.stokkartlari_id_seq', 1, false);
          public          postgres    false    245                       0    0    stokkartlari_kdv_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.stokkartlari_kdv_seq', 1, false);
          public          postgres    false    246                       0    0    stokkartlari_sequence    SEQUENCE SET     D   SELECT pg_catalog.setval('public.stokkartlari_sequence', 1, false);
          public          postgres    false    247                       0    0    stokkartlari_tedarikci_seq    SEQUENCE SET     I   SELECT pg_catalog.setval('public.stokkartlari_tedarikci_seq', 1, false);
          public          postgres    false    248                        0    0    stokkartlari_uretici_seq    SEQUENCE SET     G   SELECT pg_catalog.setval('public.stokkartlari_uretici_seq', 1, false);
          public          postgres    false    249            !           0    0    teraziler_id_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.teraziler_id_seq', 33, true);
          public          postgres    false    251            "           0    0    terazimodel_id_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.terazimodel_id_seq', 1, false);
          public          postgres    false    253            #           0    0    ureticiler_id_seq    SEQUENCE SET     A   SELECT pg_catalog.setval('public.ureticiler_id_seq', 132, true);
          public          postgres    false    255            $           0    0    vergidaireleri_id_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.vergidaireleri_id_seq', 1048, true);
          public          postgres    false    257            %           0    0    vergidaireleri_il_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.vergidaireleri_il_seq', 3, true);
          public          postgres    false    258            �           2606    26965    carikartlar carikartlar_pkey 
   CONSTRAINT     Z   ALTER TABLE ONLY public.carikartlar
    ADD CONSTRAINT carikartlar_pkey PRIMARY KEY (id);
 F   ALTER TABLE ONLY public.carikartlar DROP CONSTRAINT carikartlar_pkey;
       public            postgres    false    201            �           2606    26967    caritipleri caritipleri_pkey 
   CONSTRAINT     Z   ALTER TABLE ONLY public.caritipleri
    ADD CONSTRAINT caritipleri_pkey PRIMARY KEY (id);
 F   ALTER TABLE ONLY public.caritipleri DROP CONSTRAINT caritipleri_pkey;
       public            postgres    false    204            �           2606    26969    faturalar faturalar_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.faturalar
    ADD CONSTRAINT faturalar_pkey PRIMARY KEY (id);
 B   ALTER TABLE ONLY public.faturalar DROP CONSTRAINT faturalar_pkey;
       public            postgres    false    207            �           2606    26971     faturatipleri faturatipleri_pkey 
   CONSTRAINT     ^   ALTER TABLE ONLY public.faturatipleri
    ADD CONSTRAINT faturatipleri_pkey PRIMARY KEY (id);
 J   ALTER TABLE ONLY public.faturatipleri DROP CONSTRAINT faturatipleri_pkey;
       public            postgres    false    214                       2606    26973    ilceler ilceler_pkey 
   CONSTRAINT     R   ALTER TABLE ONLY public.ilceler
    ADD CONSTRAINT ilceler_pkey PRIMARY KEY (id);
 >   ALTER TABLE ONLY public.ilceler DROP CONSTRAINT ilceler_pkey;
       public            postgres    false    217                       2606    26975    iller iller_pkey 
   CONSTRAINT     N   ALTER TABLE ONLY public.iller
    ADD CONSTRAINT iller_pkey PRIMARY KEY (id);
 :   ALTER TABLE ONLY public.iller DROP CONSTRAINT iller_pkey;
       public            postgres    false    220                       2606    26977    kasa kasa_pkey 
   CONSTRAINT     L   ALTER TABLE ONLY public.kasa
    ADD CONSTRAINT kasa_pkey PRIMARY KEY (id);
 8   ALTER TABLE ONLY public.kasa DROP CONSTRAINT kasa_pkey;
       public            postgres    false    222                       2606    26979 $   kasahareketleri kasahareketleri_pkey 
   CONSTRAINT     b   ALTER TABLE ONLY public.kasahareketleri
    ADD CONSTRAINT kasahareketleri_pkey PRIMARY KEY (id);
 N   ALTER TABLE ONLY public.kasahareketleri DROP CONSTRAINT kasahareketleri_pkey;
       public            postgres    false    224            	           2606    26981    kullanicilar kullanicilar_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.kullanicilar
    ADD CONSTRAINT kullanicilar_pkey PRIMARY KEY (id);
 H   ALTER TABLE ONLY public.kullanicilar DROP CONSTRAINT kullanicilar_pkey;
       public            postgres    false    228                       2606    26983    odemetipleri odemetipleri_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.odemetipleri
    ADD CONSTRAINT odemetipleri_pkey PRIMARY KEY (id);
 H   ALTER TABLE ONLY public.odemetipleri DROP CONSTRAINT odemetipleri_pkey;
       public            postgres    false    231                       2606    26985    oturum oturum_pkey 
   CONSTRAINT     P   ALTER TABLE ONLY public.oturum
    ADD CONSTRAINT oturum_pkey PRIMARY KEY (id);
 <   ALTER TABLE ONLY public.oturum DROP CONSTRAINT oturum_pkey;
       public            postgres    false    234                       2606    26987     stokbirimleri stokbirimleri_pkey 
   CONSTRAINT     ^   ALTER TABLE ONLY public.stokbirimleri
    ADD CONSTRAINT stokbirimleri_pkey PRIMARY KEY (id);
 J   ALTER TABLE ONLY public.stokbirimleri DROP CONSTRAINT stokbirimleri_pkey;
       public            postgres    false    236                       2606    26989    stokgruplari stokgruplari_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.stokgruplari
    ADD CONSTRAINT stokgruplari_pkey PRIMARY KEY (id);
 H   ALTER TABLE ONLY public.stokgruplari DROP CONSTRAINT stokgruplari_pkey;
       public            postgres    false    239                       2606    26991    stokkartlari stokkartlari_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.stokkartlari
    ADD CONSTRAINT stokkartlari_pkey PRIMARY KEY (id);
 H   ALTER TABLE ONLY public.stokkartlari DROP CONSTRAINT stokkartlari_pkey;
       public            postgres    false    244                       2606    26993    teraziler teraziler_pkey 
   CONSTRAINT     V   ALTER TABLE ONLY public.teraziler
    ADD CONSTRAINT teraziler_pkey PRIMARY KEY (id);
 B   ALTER TABLE ONLY public.teraziler DROP CONSTRAINT teraziler_pkey;
       public            postgres    false    250                       2606    26995    ureticiler ureticiler_pkey 
   CONSTRAINT     X   ALTER TABLE ONLY public.ureticiler
    ADD CONSTRAINT ureticiler_pkey PRIMARY KEY (id);
 D   ALTER TABLE ONLY public.ureticiler DROP CONSTRAINT ureticiler_pkey;
       public            postgres    false    254                       2606    26997 "   vergidaireleri vergidaireleri_pkey 
   CONSTRAINT     `   ALTER TABLE ONLY public.vergidaireleri
    ADD CONSTRAINT vergidaireleri_pkey PRIMARY KEY (id);
 L   ALTER TABLE ONLY public.vergidaireleri DROP CONSTRAINT vergidaireleri_pkey;
       public            postgres    false    256            �   )   x�3�t9�!��;�Ӑ
��u�tL89c��b���� �R�      �   %   x�3��=<��נ#��8C�B��=�b���� �Q	      �      x������ � �      �   9   x�3�t��<:�ˈ3�1�0�<���ŕ˄��4W_W.S�G��#|C�b���� �      �      x�U�]n�:����ż��H�R��ؐl�`㿟����f��������T[.�ͮ�&U<O�����?�����z�λ�˯_�N��nW�����5\|�=u����q���M��߹��������.	\nܫ��CW�qW<f�pz����]Q�0�~u�]!
s�P�ò�u��GE㺞�i�׆]!6ׯ�S�����+�S����g}6r����{�o����Ŀ]�������]�����ʂ�S?�mW�\��i�]Y���������������I79u�+7
�W}!��2�O��+�����W���<vӮz��p�������C�u�<3U%��Ϗ<@U���η�ￇ�㹯��
��kд�q�������3�"�=d%:|s��Zl������b��h^ǅO@�O�ֿ�O_WO��^;q�յ ~�jnG>�`����?��C������ơN\t��2��k��x�p5������xj4k�����\�>}���ϡ{=�m�TX�������������5��'2��x���/��k�~X<�k�Sx�+�kZ]����ςW�y(�|a&v��U��n�u*��z<���߅:���w�1Z_�a��xʋ0D�����s��S3���q������q���]Oݹ�EQ��'���"��Mco�"3�_���f���c'y�<|�(V\0�Q���8���}\�nђ��u��AΘD��&Q����ex���yd�kV$9p�~����X�=��Ku�.5��3#�����P��˯���׬�R¢!�Rj�~�����g�{w���]�-��~�媧�lK,�_�nܵ"2hb[���i�6����Z�.���p�E�f׊ǰ��������;!�x��[2��YT�~҄��Sz�X�����2<(~�j��e��5�{�p��e���x�gİ�{���}-��|�6�D0�׷>����'n�K"1x=t���W`�˹� V=��������>���V����`Y�ۧ:'~IrR�[+�k�,dÏ���d ��MLgQ�٠��n�hL�e�+�+㭿��Y�=��J1�����RĶ�ݰ(E�~�5��z�:Gq��
�,�Ϭ8�k ��,��D��$\�}WD�,�eY���B7�=f��ahd����=�L����~.
��O�o�6x�%����L���̾�~�N�����~64;�x&������fP��1� �9��'>���X�j>�Q����f�g�`1�}�~#�2�2(Vm� �J�O@�p�U�9�2C);$���}��R��t~����3����?b:~�kH�
z!�?k��d��х��J�
z�P����/�3�:��]ɒ�"�=�,�%��84��k�����-�Q�� �L���iA���(�|�$����h]�0�BK�|:��
{��oeS!���Ë�*�%�"�����t�_��B�{~��\���W��<�P�X�ܿ��I�͢�V�!Ȳ�Y�|�0+,��Ha)dy���˟8�ʰѹAQ[-�<�� �],
8�&��+�crA�-�^xAQ����5�k/e5�%�����������?�������	��Zg_IƠ(��&��9;樦�]�epJ�(�b��me��to��yY�!?@8z������,���2��<���3���%�ea���ȌO`�FG:?,!H*��;��DiY�H��.�׎�.���p]����h����$D��ŝ��a&F�@���,c��Ӂ�4�GZnb~�W�V(��]9H� �6�9�)���{h2�	ö�J��t/@-�dH���P�N���UF�D"|�#(L?T�T�N�3�6;f��	iY�ݵ���J��VIC� ��k�hi2��K-���"�$(NK[g�����da,�u�(0vVm`<)\�\�l����'e�7�ccf���lL�~eȉ��. �������$D�)x�����KI��Vp��IaH�Ҭ[�K��W� qZ��mPV^�W�,�N�წ/bt�>�UB�Θ��0�>ob��r�i.g�C0�<���)�Hh�hR6C�T4-bXw�#�̩�]��h�";��~:��o�!�<��xY�}O5$��ǳ?�2����!,�,�{��"�{�B8�e�h �2�M�9�Z�ȿ��j���O��jXG/�ј�d2!��ы+��կ��o B(��Y.�NpMN3�ި ��U^�P�W7E@#�"%�PZ�s�BʟZy ͤt�qQ���z�Y>&C�@�T�R#�Z���*�A��(=�ІR��W�Trڪ�7��/� b��?�Bŧze(*r�����*Li�^��V�T��Q�q����y�
- J?����@ʇ�W��O������W��OTk�a�B�a�'Us��&��o"�iԀ�q�擗��+5O�������A��/\W�>��P\�n֍����!��e���'��5��8O�}�B0�-ֻ�2�˸0�U��q��2�U�{U�h�M*Z�[��lP�)��*��]s�^U��C�)s[���	dfr+��UeZ׎�0�	ѿT'��Kw\��T��28,�-}��T�(.]=�XjYܣ#��NH��� ��Wl?m��K��s�
x���_��
!�����t�jLt���nLw��,۽�����njLYuCnST�3��y2%	�����f��Н���J����j&�u�=�J��N9�WR�(�冮�*	�,
1�����̡�||���T=�$"ILX�Z�҈d	��Ʃ$�g��/xH%ғ&:�-�~v�\I-�2��zN��zVA�DR����2��Z�����hj��ML�E� ���(�Y����3C\o�lU�%�[W��T�JR���������$����w��I	�
�Jr�j!�<RQ=od�
� ��������NIH�u=�h&��$�^HGU�
#g/�!˙�D��v����Y	�0�PC9 ����B8TF���A������С�g�1	(�굞W���I�Z���g�r�`BJuf��L�e��jF�j�>��Y�27W"�;_5���ťD�^�r�{�(Y#��[p���9P�U�u�^.J�j��z(b�r�f)q��ԈGe�\�J(��H&|p�;�X[Vf�����f{�|��t7��&�=�X�֨�ןĢ5>�0�"t��`�(J�6��a�FO�jk��"�FV0�[�T.�Ge,��RV�v�6�{����?�=s[�@L_if��e�bו��O�R�a_�ݐ?QMg�bU�ն���r�Jm�g�j5�k�}wCȆC޻�?=j�ˊ�m7N���?�n�Qu�ֿWG�����2u]d�u�0u�!i�>KL]W�1'Kum�[���"��M^�ލ�������i����e~�:ݡ��um�=w�ȍ�]ݘ�[G�:���b�c{�Ƥߝﺾ�3�}���:c�բJK�-�;�*�M���GjL���m��1����cc���! <g�t����f��$b1��t�6V�[�:����ksG����0�Eu�I¦��X��-3�����E��N��T��|�u�F}�/�H��->����&��p�Sk4HX��Io2��`�	^�s�Y4T�Fs*7{��s�,�c�Λ.��F����ޔIL����0,&���y�`3��9F����<L�Bw��f )!#ӽ��?�:��@��t'3����5́��?��x�,�ڶ*7ܟ䎭)��l��֭�}���3h�f7SC�dNu�q�pM��03�ы�8���(��]#�N������n�A+�������0n�_�Px�I�rַ�b�tHYߨV#
����P"�{юS#��nw���b�Hw �=�.|$<�Q�7R��DV���@�(��bld�f��`����5k#��[ڕ��
�:t[Z����H�zk��l:缨)�Q�,J3�Ii�.�s>۔fI"��۱Mi��r�)�R��^]6�i.��T��ԐxC���$��Us"�Qb��Ĳ�̤����07R���=Oyz#�I�b��ILr���r�4��2l �!�����-�H��SV�(W�
�3��°���<��4�.�S7�   x������*_mj��z���7Ҕ֝�^E4�F/�,r��DAo8�~1
j[�Q�K�$�}�$'-��0���o"Ai��Z��>}��0�h	����s� %6�G����89�7�H�G���5�I��R?�l9'��uսڧ��j;�$�7H]l�"g��`ػ��Fn���q�i��4�ѳ�u�ty�@�l��_o�Zj�Z�w�;e�.�����L�a��1�ӛ��`f��[z!4dn��{6Ҡ2(_WCOidA2~>l���(
��2�ke�x��c� �f���H��y�ԥb�}�vh�9��p��d�v�m-$3�u�Ŀ��3�g�@_2�C��&Ō�ak�5�4��W���8�y��$��l�
�"�����nX�Z3V�����mM�}�-���)�?�A��1̭�9y�Z�S:����.<��V��³I^W�]@C�*ϥ�Ȁ��@j|�>:p-����:�	�=Q�G�*�����9�~�:��]�j7P+�o�b �)��K���]��P��� �S���/��x�[�X@S@^����"S�!�����f@Rj���r�T��$`P���\�>�YyUl��A��1�.u���]�5��D(M�C���]0IC����UK_@X��݁�+�Ex�O	�"<>
���`�a�U_A\�w�L6Qgi�Q�ZY�4l�`@a�GG.H@bd[u4y�u�R�R6LG聾�����P����V_��sƙ�6E=�RjS\t�a����q�u�����H�Eo,�Ng��
�G�(��͑�X;)d�M"yZY�*��So:���r+"蜒��e/=hL��:��G�T&��u��Jb�����_��$4!{���1aM����dFY��$4ʋp�n�l�'J��&�m,�� �с��1�V��D���o
������A�ڐ��â�Il�� fAb�ޗ�?S�O�	�TK��FpQV$4Ji���)9�p?.%�Ir���,zINڎ)0�*j��wjcu�k#��+�4D�ˑ1o!�h�v�hzC��/A:��f�&CBӪ-�"Hh�������n�>��1��$H_�7���yj�#��a$�\`��4ϛ��P�$����4���^�\�$�N[�TL�HA�R��VO��h�]"���.]���M{�*d���7�
�?7OB��-�hK�<@gtvai�$�2�W�חn^t�������	�Y!"ʒM����F��n�;@�Pzb"zb�{t�є�Y$g���l�^z7R�X��-IJJpq��X���~u��0Á�)��D֝���0��~R/�E�S���]av���'�iCI!U�Г���:���"���W�Ek�l����`y;ng8#��T����
�?wl���{�EdŖ��0����f1�m�ci��։HJ�&؝�"��]DN���<F�	��PDK�.�o~B��љ��>����81�J����M��]�}�MN�P����Ze۱6��g���~������G���k�[����h"�弿+��8c��u�y�AJ����Db����E���ڎ]D�rU��ݍ:��<�v>_u���ixWwz��kY��M� �)�ԍM�!`Fc��w2!پ�YcNK?�s�s6L'f!�ԧڨӽ�����spR�F'a��� ��9׍���߂�ݙ�	k�SNN��V� �E"s�E���)�|!"��	W5�P�6LD�)��<��IG��F𽟨�"��<���241g�[S4����y)"���㶩	~�@Et�>7%b�J��<�_�dy�$�j�xW�ǖ��»B��`��B���'Rz��_�@9��B2z~�F�	��z��ck���lMo^%-�������<�&�������P�A���ݔ%FIE����e/�V��M�(�hu�''�
I�u�܃I�
�]���JR
���6���)�V���$�h��Z�������qz6S?�h&T#���i�� ���G[!I�40�A�4_:>�`���&�����Ex�BeB/ �6��`$#��n���db���Saz77����t���rlJ����@�Rh�ؼ:��؂�1+lB1�횇�nB�}{8��q�<�HF(�l�F��r ��z�|�9U&zy��$���O�W&ʤ��Ά�#�v?K�*3\.>^�a��8hTw0��EM��j��z}�o���l`<�1A� ���[B5�c�,��3jvʓ�h`�䈝Ќ`�_t_C��	pA/B�5�T�%����Є\os��P�kO 9�(���+��K��j`�G��� �ذ[��D�-�6g�\M�DNB8�v^e�1Mm�;��,�u�q�Ԙ⧷z�h�*�uJ*�	d�=���#�(�x?��$GO�ޔ�6�g7����%%�(�l[`۪���WaB�1a)��E=�^��GMP��W&��3�I�M�U���$�7A9��5%�8��im�<!!�� Izu�q��Vm4CJ?<dr�hf!�NzqB�y�[>죹���g�3������&���(��"ڊIz�"lMBKh��[my�А�c��򄎄��&d$��H��:!$����*J�:�#�)�#�z�:wR2K���y~��9��(������?�B���R�'�$���Z��rK�Hx��&X��)&�r�;	-	�Nh	s���٢&Ag/|@�EL�Ut�:-z�-(�#k����6��HH?�[�$��y_�EFd�yc����yj]l��{S;�
Z)ʣ6lQ���&%4�BP��3�9�RE�X	�E��Rb�\���g#$��@P�ݢ+Q�g�k�Z��x��\�j�*��[�}����K���|��/��/2z�r&>*�e�:�7n�P��J�y��X-��M�&�0�@F��s�-��h�u����!�``4�����-���/�Ua��I�1��RV�����EW�+��_�΄���"-Q���~��������w/�����[.�Z4��@);#;-��͛��n��m���-��f�a� ����JpϽ ���ܑ�-�Cg�HIl~��>.3��"#Qۀ~o�EAb�(^}h�EB�#>��-a���zĸ���trw�E?b�Ο*�����)�4$Ɵ��B*)�P������"q[�9Yjѐ��Ц�-����N[$$�ǩ��J5�m���QK�����(�_j��q����&B�VHَ��;,=gϼ��NZ�����R����)I����ă��N�[�Z�.m���UAE���K��܋��ĖgNy�Ǜ�-a�ߊ���3��%�^��P�p�������?z�o��o���r��      �   D  x�5SA��0<ۏY�� }4M��(@K����`O-��`��FsJ9���]Q$�a��]�����c�$�[���q.HB'Q����"�����H���2�ŷ��=�RT��w�s�X�d���ϳ2	O��GV9Ճ�YY����+�UI��u�xǪ���+u�ܳΨq��W$c��9�Eb-x�Z���cm�yY�sc���+��Ѯ�ɘ�L��;���={]�C�ݺO�=p��A7z��xoOAr��3���`�$M���	C�~|9�&'	���X�T���M��~3�
�2�+��Q�%��h�%�i���������u�{�g�9��eJ�b^o��>cZ{��~o��p�&m4�������N�Qp�ߚV��[��j�8��@u�EN�����OC�A�ؖ	��m��KF�7��莋�����ES�z�"M��W����M�o��i���qa酥-��H�%!�1�&y��nD�7�t��&.�4�K��JSm�VB�2]שX�C.!V�J/�1����AWW���^�$Օ�>�����ث����⸂Z��zw��3���.      �      x�3�4�300������ N�      �      x������ � �      �   5   x�3�LL��̃��.�G6��(8�4202�54�50Q00�#������ �?      �   =   x�3��s�>�!�ˈ�;����oǠO.cN�0GW.�����\����~�!\1z\\\ ���      �   ,   x�3�LL����4202�50�52T04�2��21�35����� ��K      �   J   x�3�ttq�2��>����=�ї˘�����.� G��)���f���!A�\���ȶ���	������ Iw]      �   �   x�E���0DϻUPA���Z�H�A��4���=��G9D��<���UGX��[��
H󨧸�E
�-[�X���۟]�K�ªS`��!K3�`P���<ށS8�&���X|�Ԓ���g8vr���K���r�%��Ӷ�W�),)5�r���%�N���>8      �      x������ � �      �      x������ � �      �   &   x�3�tt�q�2�t�Ɯ�~�\&�!��\1z\\\ h?�      �      x�3�6������� �      �   �  x��XK����U0z�G�� �B8��kww��;��jv�띔�6����]v����ɓ)bv����A�~��<�a�o��(B�-�Z���J]?���|���SV���N7�x�����5��{Du��\_?7�(p��=VB��iD�kϙ��M��z�'�%/���mq��B���)Q	!Z\?[1TQY�B(��J�eB
����� ڒ��ht�
d���l��P��([q���~�GyIŸ|�u^<a�g��C㘕b_
C^��z��b����SUe��\?k�զ���3��xʎb��Mu�i^��CX��ˁգ���f�$��p�߆���-�)x_����@-JQ8%�����2fa�v��B�����T�%+��oRQ�1���kvyGӑR����N(!���AQ^>���G�L؆ .%�m�%�����|Oy�E���'��0�`LEY]ϲ���d�Z���]�TE�Խ���f�ڳ��YTRF�9�t���iNO �N��<�u�_fO��,�
y��A�@+f�EN��z�\�ig�4�N��Ed-Â�E�"��L����%���*�8Go�Hf�?]?�d�l4�A~��_.��6�T~=�A�fecd#�^�>N��E��%� �@
�{��Js�.\�4���(�N��;���.��b�D�N�Q�o \p��M�(��Nt�.����#��!�9�� ����w��q�8��I4<���AW��?;���>��%Gw�|�bZ0�RL�eDH"$�喘�[;B����5ۈ�����m ��V(�-���w�X3��p����+k���d�0-RHn��O5GχAU�Ӧ��ıEu*9�{c :�����zo����gs&�-?@z�ᵓ�p��H��K+��F�0YV�'��I2屫��dm�Roq��z0[1�_gۿ{��f4���甮�c�P��/�0�y�`q�.�\��q�ً%,E0��7��h>e"�#���|���.lTJ��z��c5�@2� �E��.�16���O;x�;���wQ.�݀O�$v���Mf���ը��mQp���'�	#/6�o�0E��"bjR�BWu%�#�[��	ZM,��\L������	z�I�.�j3Vp;YAܛ*w�Y��|�������ܠ��=E��sB�G}m��c<7s]�K0�����x�u�lJjx�Ab/�ȏ軀=Y6u�	%�紼�|9a_gG`���C.��>P�2ّ���eB�T�ѵ��C�� �F�Y�~]sVf�Rd�>*I��\Пnju�6?<	�F� 0��w+p�D=n��p4$י��P�v8G���vX��L<"_��ˮY-ؤ����������B��uI��:��:�G��9� �������
V1N�ڑr�����ɑ�V	j�뎡�Քf-Qޭ��2��K�$Q�! ��y�-�BQ��VX7mcW4��6v�xX�hv�`;Q���M��g:9�Y�ض�2��|,,����6֯�˰LPB�/�C�'pC�H���qːCP;�5�P)�n�	o���d��|OwrÍ���{�]cS������)v��z�G	e�b$H[HY�v�W�E�xĜ���?���,-��EJ�	�=[`վ6�gMia�/�s�U{U����zcK��t�s���a#�6�~�^1�%*�|A$kLެ�h��&"^3������κٸ�`��
�dB=fG^�5/��ޱ���!�-ȹOb�a�!$�B{<I؁�o�D�#nsr�2��z�u<�v�������`��ul���f�9�'`oir���5��J�5�k<Y2 �bI�fr�
!�@�������uҌ��~�kFe^�ݣx�����o���FX5���7QڬPT,�4�q�NEq{��-:���2�v��x���g<�1F�����i�<
�i���N����:
_K�"���!������FvdzK���3��%��k(�+�2oX]QN�G���'��m�k&6���d�v ��/Ԗ@_K��Z��r�x_�$�X9;-Bw�n��ɔ)NL�|��Ό��=��Q���M�����9r��r-����_��/��ߺW�u�ӯ�?N�U5����N4~����	�+\������87�d�D����A�}?M�>�)������ؑɥԎ3'u{�I��-�۟�d4����x���}2��EA�@�n�      �      x��}In�ȶ�\����IoH))$������"ޤv��D�����}}3s���tOy���͂�<���.��a?���������p�{_��}�8����:�_������?|~/��UE]�V����
w��0lrh�����7������tWf]?��o�??��v�m������n����quy�9<�����߻a�����|���F�5{wŔ�-�-f��8�%����|8��&g�������4��W�-�=F�D-@e�G�v�;��T�]���1�O�(��e���0p�mp��S6U�ڬ��e����?�M ��V2�m��˦)6��"P�1�C�2�8�]6#~������h�ǲq��?��;6� � N����qH4���˦-�?�m�����l�!e����������>_a9l8"Qt�S���_��Utt�{i8W/��π��	\���2�2f�
�q#Sl�����*�Sc�,1������(�=�#�4�X�c޶�Q5<$̌(�ǘ���d��R���.ѥ`xB����t�x7l>��-ó�,0��~�_�K�p���省��y{�<��c��%������}��A_�J��0}9X#�U�±�bJk���c�>$��3���:\�\;��M�p)�ïV/<���wL�>p���{y����x̠�'|i�b�VJ��=J��}1�9l-��Pھ���u��0]B��`c�Vo�fG�K!d����#�Z��N�!v�Z.3�\�v��]k!D���SByp����\B�YmW����{c�r-��t�Cv	�%� ̐ҵ��pڿ~-�Y�;�K���Nθ܇��)��^pJ�XQ��W��Z�Ӵ���a�����{������?��������ͦ㨸�DWq�s�Q���&x��b�d0Gx�f�?��Ϗ��a����4���oy�w-��O�_�RǹYJ���Յ*P53m1l.��R���?"���\S��*}W��v�<oP?#J߇��a�����%�lqv�UBJ��~���+��m�R�2ʖ�ʰۮW�����S��1������@�Pf�b�σc7d�#3�z�ˎ�`F��^�-u����* �u�qì.o�ǐq4�"��ʖ������_�w,��R�PK�&o���q�z�~���$�e�͐�9��������I3��nn���w/�%���i���J�۬�YL�`b�������,�[pq����[Έco\�mPw��,��B�ɠ6j_����u��}U��8�v��8N>VۜF�E#P����[��vS`���sf�/Uo�����9����0�R�w\6\U��f�K��ČOޭ�Ԩ2��)����>B����
<��굅��|��y�^Y}���2��[�q�٭�*\�y#��%��}�S+C �]R��=�W/�����=�y:��a�%�{��L���}،q�,ٮ c8ꌼ{�m���,[���n���o�z�������=�Yղ��X�ǭ��8[�sI}������1��5��v��zW����R�jßK�6qi�e�Z��$�ޞ�����A�����j��r.�����	����c�3��9l��M���rs����fs�m�_QwFxI�v��eGm�.	Z'�.C�=hq�`m�?_�_-���/��z��A�bhe��g�$m�Vhip=l�'3�NKkf���t��?َh��4{����|��e
-��E�� ��Z�h"v���{��<��VjO���[P�����_�fv�^�{~��9�:u������6Uq9�O������j���;��1;��nF ����$�6E7 �j9<���_qt� ��2�H"��n��ꊗ׷�g���	����=���Q�����wH>9���U�_0�@Wl>�A"��K�Ӳ��<7o珨o��I|E������8`zi3�r�=70����A�I�Ԁ4C@���qF��d{u�(�i�r��r~�{n�~ �
���!��깠���?�a8Ƈ��W�[,� o������������[�	*{�m�8�>pz
�BY������Zp빛�b]?�m�x�`�Ak�=%�m���sC�W�
[�}�����[��
GZ���7���(3�n=�0���$G�A�Îi�>B �_�����8ÒqE��QJ��j�L�	" Ƥ�ގQ���Q֞��;���c�~ �&��.p������� ��%��J_# ��x<��,�T(��'
��q������ _391@w��Ͽ�i����4���r�����*#6����� b]�>l3�z�:}�� ]֕}� D�W�

Fq�x���k��d-��Q�-��k7��rS>s�n�ΰE��Ú	Лb��+#���9@68"'�9����tZ�T�4�[�7\:)�W#<���+��;�O��m[��{�L����+D�P�HP,)WH���JH�u�����
��!:�iȹ�@�y�~~�|~Ϙ�Z�eAA����i�bӤu4�a�GŃZ!�y
�mq.�!ṨB��1@l�i4Y͈ ���mܹS+��� 
�Ͽ� ��H �;���:.3֊#!|Ā@�v�6GBke����sZ3�v4sPH��stx���4)��o
��S��@�%>  6��~ćT�	�� :��6�+�+ ��W,���������(�|qN�9�M
�͈�� ���2"P�� w��X�3j�~@A�.a�A\Ű�Ѓ�� Z�+v	+������5��#A��G��;�� x����@�zN6���=f��?����>��g���֟ѩ�bh�&LI3��c�]�x ��/iں��2�X�~i2�xc��	e�؇:�m�bG#�;.�8�!�n��t��z��,��ȸ��F��.�E|K|0�5u���?�@��g�??���Ӝ������a��#Œ2a@�o�W�ӱ���>al�-����T�������xp�������:�����pq��������* �}b/y)�
*��*DNI^��_A�`E@ɋ*F^3� ��<���xe�o��-Yh�ab�,�A.o�0@���e9������	��-��[�Y^���nd�<���k�f�u{vu����$���yE��+t�rWy�穮a�)z춹�R��H)9j ��IU˭8���$�n��d���Kr��t���p�f/��4a
,9��)�R���d��<�����#6+.��grɥ��#P�9�$�c��sޘ�vw�@琛���OV���i��AJ�WNȢ�Q�{���N�v� ���-�#�"~ �r�
�G�Bi�]�@���C&�8�%!  v��a5�ǅi9*��)}<�ϱ�O��@�8�eF���V����c� ΃�P���ʚ�^)e##m���J8l�.��d����/���°�'+��u1�:�!\��q	���ߍ���5m*�������� Z{���iqwT�3i8�&�OVE�V��o����i�w3�䩋�~اbe���Y�J����St��%<�^r�7��??��x*�r��*9�k^	�%\��Kh�M�j�6D7cJ�W�m�}4�����fLI[��\�����j��V��s;��1�� f���U?$&\+�'LI���+����D�_�)i/ƝL��c"SL�*P��f�{:>�Ţ,��%�d�n���#'W�Ϙ��o\1�i��#�d�ݎ6�Sr��7ƈ�+�dL��st{�;2˞�7Zˌ
�
���5�WT�0�T����9�hH�ڇ��U�<J�\B�z�|��ƪ��Q2�����9C4�#���I7����l7r'A�!%�ˌ�B��Q�J�T�Ix��(��U1z�x! n��aw�����p`u������	'i�u#���2Y���YY����}� �b�)pl�:`#��?�@� ��5�'y�4B�ۈ�+����N�Q�؎"T3 �a���h�`Ž�T�lz�{��u��#~Tݾ�C�n����F.���R    Ѐ��<��ۤ��ǜ�=y�8�ʔ��i��[L�At�7l�w\#��t�n�KV�G#���DA�b�d4Щ�i��]�H9��s�/ңw�N/a�m���velSq��������6�j-ɍ�QSk��3S(=<��(��� �2�|X%R7�DB'j�A� �2�p���N��w ��e�8�&�� ��r&�m�*��R��C�X��؀(M͹Ź�̔���pE�TO"N0�@�J>�R���"�g�*�q�u:ռ�zf�MB�>C}&[�:@v���8�����Yߝ�x!e��nLkw����v�4�;Ţ<e5R�4t�a�u�@�Kn�HU.hBjzs�"Y#��6!��� ��N�B(J��6r06�
�z����!?���d��̯{�蒙	 ƌ�T�A�����a�v��CoH_������JQ�}ӌxP�w�=�C��5Ži�CC=� �*� 25FǛ��@a��;�F\Ej�>ޘ�47�a��k�@�`��8�g���s�B�4������y�Q�A��f j1;��;B�^�� @��H�)�~F�D�S:K�ѐ1B o��Y���5� ��#g!)	��=������(	{A�P6O���>%��K�Ō	��w���mT{l�g�����!>�rƲ&� ��z��)����Ӿc�V�bI�A�ȹ8�$�f�� J��6u�ZM�� ��6wY��N�7vƀ�O��9���Y�����YT�0�����m�%�4�9y�}����?�-���ݒ�/ǉqS�k|;���
%����2���By�Ūk3
$,����Kc&�� ӟ��%[o`��SG���fT(�8/6����1A�t�*��1��w {�����F�e/ok�������x	��ƛx���Z��5�#pb9���h:G�xY��1Go#�:� ������i������&L�;B ���c�c�e~�8���ٝn|��llW/�m_�x�$]�J%f�.������W:È_v�����LwfH-3$_�Հr��RO����uZD��%
$Ǣ_��/-�rb]a����[tx�l��h�P.kie��
$����}���ڏ~ƀ���E�_�Rkn�(�5�
�h,��Џ�����|�#���?��LtC��v;c@�	�v븎)�]��(d|?��9��
���o(���:����m ��$�A7A��g;
_�;e�5킃&<�9)����8�� rj�e$6Jm�;n-�J4�o[�Y��}�
�o�ᶠ�U�����t{e�-��Jp�?���7�s[r���aZ���)��ѷhȌ9�l&��9�
$Ig�W���)��	����59C�.  ���5^���'44���~q�@�OUAi�m�
���}�K'��șA��ŉ�z����e���2aH//Ǵ{@�Ц��&�PGpJj/  Vm��*~s˃��pS�.{.���&*����-uMʉJ|� �)� �̏�y��C�\��� ��{[h�q��A��c�>gX6�mf����Q|?�pG�>��l^1�e,��ct�� @k����F~D@ �)�O8j�Dl�	8u����8��g�ˢP_'����[�4��l�?Q��ȗc�����9���v���y�S�jƀб�*��s�eyR�1<s��
��ˬ�+�̾ KOS�c_+[=+J h�k�*�eF ����G��M=" 6cnE��j �H,w<��jf�  t�fNyщW?�(�T(4�\�zTk���4�U1ͩNX��`=3@g��a��)�j�BÉ��Q�P������RZ���ݝ�N@��i�W�X�&p�gpP7�Ӗ�BW�����5�Wͯ|���f�1�tN+~��2��l2z���.�4zޝV��9k�J��sl�p�@[���6���D;��y5yc��5kzD�F;����Bz�z��r� <aJkyPB��
�"=a��e�_�h�	p�Ot�%
�ʕb�%4�7�%�&o�O1KHf,7��.Z�0И
������)��@��t:)*;����X��}�H~5r�pM�m��s#FN�9�4��3&leU�0��!bY��I�;A��5D]��T�	�u���
&2��	8U׉�.
����XK�U~�$�mܳkB5]3A w�NG����0��0��p<��X�Q�S������K��tR��j'��Ͽ�2��P�\)h�W��.'�ׄb�ff��K�����Q��3���zɾa�t7̆�
9��I�7TS`�2�(*�K�����������P�^��p��w\���7c�3t��YS!�%�U��Q����=�=畆KlI�����
�8���u��ڪ8�3*������ZH����9��}V���Lo�<G���nC~�T���sӢ+�2��~�@[Z��n��= �f��45R]e�J�ެ���S�V��j�va�r�q�ae �fiu,�;�$+�~��:�|�3B��:ꚹ�_v�06r�������FN7��韧�<+P���uף1w'�ۡ��Ӏ��%�z;�؎y�:ɽ����9�l�C��5s�d��Ͱ�����n��Z��~}=���&\=E��ƽ�fn.�~Ԑ�W�d�%��:�]�E�2uI��`��'9dl�vH]�]�G��e�z�B�ğ�r;���yѷ��_"�ϙ��
ܭ�ڲѶ�Q�|�2��&(b�j�U����g4�w�U��|^Ŝڛ�����!��M#�O1d�>63c<�?d>d���]�d���<)�77t�����p�.Ԗ��v�gm𾾡w�s祈����>k�)���Ӊ����
�}~�TYTwCU����{Ou7��"4��K�e���Y-�=$�,�͘׶�X]N�~�bHN�M�p�i�q�1jI�a���zڛ�����1o���3�� �V�ڛM>%})$w�ۛo�{2c��lA���z=&���P�7��������Y:��3����[�m�[+=/����i�	u	��9�	�+Ye�ɽ��͔���3�o+V
�:c������x*�q,�l�)�a���XI��Qd��%lљ���!��l7FJS<18�џ1cI��q]�q�]A��"����c����/��oUo�i�Da�4�]�p��c�D Y�Ʋ챎�}8�J��>�v�D&� BP[m� ͡Ҥ�&B�lU�9z�ce��pٺ���*l��6��<x�uk�&��	�Y-h	"T�o�,�c�p�ݕ��1��.�X�$(G�����y�(�6�p(� ��/U��bk�M�t����4�+`��t6����w;�W����Qń��W��G��e�����~��-�t�,�tu��\{Õ���ix9f�{����-�g�֧,�B�z-٬F�#�i���iT`k��*���d\��Z�U%�1�)1P��RaR�>�n�dK�2�Xy��	Y웵�1�T���7��u�W�_޳���,Ϡ�]x�3�|3�Xy�4i>=�C��h�H���?:��t3�|�q�Hza�L�Ci�X���X �x�ѧ�6�
p/�,�kul�U:=���#�F`
m���V�_�ҙ��� 7�px�<E��N���̐�9P�.3Gû�%%/���_[>�a�+�y�'�#c�ǀ�ē}���\Q )[,�5D�/� T���T5`��3��P�b��ar,9WT�j|�5rn���i�0�����U��l� �m*�jTY��$�U~K����K[<�5�M"�(���0�@�H><�W��؉' P���hkHP47@Pۂjv�ۚ �~SH�`��TH�u}1|<���������LD�(�޴ h��67�֨R�������Q�*�	Q��Ӱ{ͱ�^������>��;�S�tC��0 p˧�e���M�����������(P[�PB;�K+�
�f��%�.<P?0�mf�����ėq�t����b������M0k��XU�X�b�]n�n�I�5�xX�"�M7A 7��j�L
5��6    ���`���wM�������7�/�N�>�\ɪ.O��H ����_pC)4�>CV�$+�Aܾ�T�&�Z9ls�f��G2��$iu��g�{��rc/:I[�_W�ߋ�	����I�Z4��ڢ+Fa� @M���6T%-�/a���m��U��D��Q9-�@P�>���St;��	���(��g2�(�Z�?R���ç�_L��������g%���Jh��	��*���4o7(���a�>��J<�b+�>C gx0U�xd�UI&f(�@}K�H���A���d\��
 @����=&"�I�I��JdNȗU���D�ȭwl�o��p3��6��Mao�_}>$�����'kZ�b�(M���z��k�T�8�ǌW_َ����x�yn���h#�涗ᘓ�g�k�0�9���U�z��RYǯ�'����0Y���h�Y�6�bD��GK~�v��37_Y<����r'����Js��ۑ���~��F~�_ҳ�Vz[)��'&�T�4�@��1y*5|�6H�X���Gk����0�L�K#��ՒuE�D��p�8d>�����@0 `��$�hn	�G5#�5F�;b@��N"n��<�����oϑP���ҳ�SX1 &�Sk�(�tw%�䒡�!�c����O�5g jBm���x����v��ȇ8GݞA�Pqz������5������$I��*��#���4���4��^��
51��
}:Lۊ��s�;��L�ѥǵ��>�3.���%���O�wx�����!n�����|؀ăjh�x��dZ?KV�Z�SNqߩU�a>�(~����'��!p�	���<��6��S�����~����s�h����⥻d�$��0).�o���(r�%�������7\Vt���3�l��i�	5���kh1�fƀ`��3�Q�Ȫɺ�j�����8�"k����x��U��5W(8՘ܚ���!d�Α���*Je���a�(LeU��?2�@��9ş��I��@��*\����I��_N��
�+c��TpN�*��ԙ�F^��,��:�A5�Vs�C�^��Ԡ��C�F�}ZȌ������,�<�%�Q��x1���� ���h�	��j��B�*�Lp-0K���<��D}�+�gP��=�N��4 q��%�z�k&\%V5�]�@Ñ�t��|�#NjI�A�`�R��%�����@�z8�q���|�����I��H8][��xy[��t�N�e�{NS�B��+��tdL P搬���;�q�UU<�s��rb��z*S��TY��������Gy�"¬[�@�;}8��aZV��M;b@��p�J<�kU�ͺ��̸�e4�B&�X�h��3@�>ė�O9R�����w��5#�*"��4y��y�	� ȝ(e[�C�sPV!P�/��D`�hg��k�w����g�X(55��V#��S�ӂ�֕�&�h�}ȹLEqy��7��QEDq��4���d��me���7?f��ӯO�n�t㻠܍ɜYM�7M�l~ҟ��
�X�ҳ��K��T���#�l�=�u����*�c�ݾ˰�54z��0���`�@S�ڤ�@��I��@
/4ş���g��u'��U
�>B x�NX/A�����/�m�K��	 ��j�Z�+�=8#��'2����Zi� ig�m��܂�J:AXI\*
���RMi�Yu��F��]�@���=��S^� @�
}�&?�뿝1 Xz��O�X�N�v� ���4��P�	\��HvW�Y��8\�b삂Ƈ��"�����LA�^�S�`����V�i&��UH��F��=mW<����D�7���Ϙa�5z�'Z,�*��v#`\R�0�a]Va���-����@%����s6nǀR����1Q4�c��3t\�ۜk�cH�U>�˃*�d萝�nI*[�<O�S^�I� 3�� �Fs��*�f�*��浒E�R�ix�m�<b�����"w�h�Jn�� �3��p�=�a��X @Y�v;|PsI}JC��X./n�ѫP`����s�("_���c�Ӊ&���qz�U�lA�����-����k���B��v��Q���j���8��]����¤�Z''���0����U��8�!�	�$T��Xg'�7�K�!��s�]'ǵ�׭oe�y:m�w���+�^�@b���9� ˓����RU�P/��[@�AeۆbC�B��+Ǻ>.`@�)�1��#�6,?��G�C�N����lS{?���O��1 X��#k��	�D *^|D����I�0 HL��lN^���e4�S�e��J�I�P�5�w3Rv��|����}"0ũ|�k����¦J��99�G��╼��i� 7�ۊ����'U��
͏�dmp��i�T&(�)��3@�:N9�:�.�J�+��R��;ũ6�kf|��~8�cYi�G���<�&����}�`��]��h\��m_�4G�{6N��@�Y�r�U���}�f�������\W_q4��)n���]��⌊�	B(�*�0:c�8�@��X��z�+ř�^�cS�+\�f��Ϝߐ\����/��<���U��	Sv�Tn����ek����A+��=���833@�lY\}@H�*}�%+t����g˽r;rZвfG�����&�˫��ޚ�qu�і��M��9X��uH�����3#x��-��� ~�+i���N�F_���S	�n��Z��P�F���3�N�OD&Tȕ�K|m�q*y��N�KF�[h؂� Z%@}d�V��mr�X;��~�4껧�p�{Xݯ���Wn��5�Ҫ�txs''���l�a�?򰳕ꦕ^�O�QN�hh����f��s'�����]S��*uu��K�o�� �o�����)^���e�]�O�8ya����ŕt�;=��n�������W�S�K�ԑt���1���f�s#�6pXG� ���7�	��k� N��#c#���-���T|�+#����W� Q��aw���ZJW�`6�SFВ��ù�M�1<e���W�	�X�#�*���cI�k��Z�!�7d2��_��b�BK��]�٣�~ƀ�W�Y:(�\�~ƀ�܏s�i4'o��{�1�~�l��6��z"P6�/I�|7[Ի),�1���S��k�0�B�]4��Ʉ���J�A'63FF�&N�� @9jO�m\Q��#p|��,�q��Ǐp2\R�;�� o� ���/�俞�r)B��vaj���o�8Y��l	�dI(ٝ��.  �^B���-�� ��9#��+��uH6D;�
@;��]�@�D�:����#.�L P<��hX��h����������HN|��(� %�i7K
2�/A�P ���y�Ni~*P0�ʎ���5*�OѶD ̲�,�������f4h>%nM%�nD ,����)#P�C�)bb���BQ	�U��u�h�OP4oh>���/w�L�[p�D��uʸz;z-�l���ߕA�>�ppƺ��77�N� ޤ}�3=ˌp��u�iN]/���t�6Y�-���	 ��-U�,�9@���r�|�C7��4@GU�s����1q��ǭ���F����@��F,��+����e}���2k��3�y���DU/����"����c|{�"���h���mϩE�+����
��~���@^E�-d�E�#�8�}u��u�H�x �#a4�t�8𕬔!��{��O��˝�0C�w瘾���!� @U�:�$�w����3
�%cq�i����@�"�9�N�k#d>t?|� �r-AD�3�dQ^�*?{�:4�}�#밴��5�8�E�eTS���� ���bN�#|����j�T�I�"-p ����]�K���������+
�~*���	�^���-��+l��]T�zq
 ���#�^�G� ��9�Z���0 �>�a3׼����M\�$܎+Q�{q��!  �1��u���� �  �x6O�����k�N�� ���x}��j_�@�Ctr��������R����p��{M~v��XK/��!'$��ɥ%Y��[�7=��f�KH�Z�)*�z9�|3" ��k�`��i8  �{�8rw��B�6�(������ʺe�/��sJ�9�� J�k�$���H��.�Ѷ ��+c����r�����'�!� ���Up]<�� �S�e/3�0�&m�/ǒ7p���iu��5^^%ŀ`��K\�zț����@�R���A���������o�y���	�7|^Ҷ��;Q�@�
�k
FN#���������64�������W����������R3�*ޔ δ��R�^�E�&�)�z�9���'���I/ı	�Mx%��	p�����l�?�@�E^�Y���@5f{V��I�^bb)� v�rD���= o���֑��ލ:ro!>xJE�>9'��:b@�սe͍�!!�Dx�>�@�:��7E�-a�1�rT���g� �J}N��c ?�@�Sȇ�	)w�_�@�u7	o��1�'zC8]*nD ����Y'o�(`@�eDz%K(�X��������U������i��a���U{�~	m��'�z^e���@1��O
�J?z���&�Suu㔊����sw^�̳������X=C"	��E�(�ڱ>e\:W}I�g16	��gC�z���nqωWQ���U�ԁU�g�O�|�� �6��Y?@�jx=<g��앤��T3>�������	�X�L�9���;~v��:��i��E�t	�.P �??����RB&����J<~�q��TE^��p$�C'�}~9/7�o���6�,w��.�^�:�{ٶ.���n����zU��Cb�YU/{x'oD�d
��9�,�� ��<���O���e����S�Bʨ!ӿ�f
"�X%�o2����(�Tn�J�c�� 0��
M9GCΓ�^vx�]G|*ߖJ;�}�7���p�&"Q���A�A��Y��}΋D^6X?�`�.<�R��"����mu���M��ZE���
�ŏ��N�10�.�{��n�Ţ�д��N�V6ζ� ��b:�G��ׄ���Pl,��l�&��aei�S�񛯕٨�o��ʺ��1%��dM��Wv��W�%��oeob��E&�a��2��H�
.>&��2$�/8#=џh,x���YC:@ �	���VE�ګ�w���ː�[�n�L
��h�dWi�  �/���Wk���P�~��<%���J�`����QY��M����)몪F�g\�keSi�+�.�S֒�9���)�*=D�*�5�xF�>�o�V��vB��?�"����I7@r�|̉�~?��V֏��I��Cp�o�x+3Fkf�*����.G+�EkF<����9��[.Z����|6���V��H�Gχ��0>�g4d,qd�N�c�4ݪ�8A4��)�,V+�����"�z|��i�� �n��"�V��kqԡ(D��N�4���㰋@�R@[ �Rd�F�Ғ�)ǰ �w��^r�����-���­��A#�&���G��[�p�~	#O��mT�k�۴~D��?r�di�G��k���HW%[Aɳ=��aٓ�
_�#�xB{���VqS$�2�ۺ�yDݶ��A��_y��n�ߥ�����vB��:�3n%������M��o%� ĳ����`�V!.m�఍P_�>��j+)����hL���Z'j%����li�łN��:eeB����rtv�C�$�$�vՈ ڇk{���XR0�ZybǗm㴆�+��n���%(5)Bx޼9�(����|�y��[z�.+�����U��uq�S���I�I���2����O�y�NoW_A�H�����NBoW/a�Y��WZ:�Z�zƐ�Q2�%f[�rW/a���V��*.�u���A䴣k�5#���������i�d�i��D�fQ���oY��}<"     