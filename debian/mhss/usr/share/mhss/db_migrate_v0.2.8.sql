-- MHSS v0.2.8 Veritabanı Migration Betiği
-- Bu betik, uygulamanın v0.2.8 ile çalışabilmesi için gerekli şema değişikliklerini yapar.
-- Çalıştırma:  psql -U postgres -d mhss_data -f db_migrate_v0.2.8.sql
-- (PostgreSQL şifresi istenirse girin)

-- ==========================================
-- 1) stokhareketleri tablosuna eksik kolonları ekle
--    (v0.2.8'den önceki sürümlerde bu kolonlar yoktu)
-- ==========================================
DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM information_schema.columns
                   WHERE table_name = 'stokhareketleri' AND column_name = 'birim_f') THEN
        ALTER TABLE stokhareketleri ADD COLUMN birim_f money;
    END IF;

    IF NOT EXISTS (SELECT 1 FROM information_schema.columns
                   WHERE table_name = 'stokhareketleri' AND column_name = 'toplam_f') THEN
        ALTER TABLE stokhareketleri ADD COLUMN toplam_f money;
    END IF;

    IF NOT EXISTS (SELECT 1 FROM information_schema.columns
                   WHERE table_name = 'stokhareketleri' AND column_name = 'kdv') THEN
        ALTER TABLE stokhareketleri ADD COLUMN kdv integer;
    END IF;
END $$;

-- ==========================================
-- 2) kasahareketleri.islem tip kontrolü
--    Eski veritabanlarında integer olabilir; yeni kod text bekliyor.
--    Eğer integer ise otomatik dönüştürülür.
-- ==========================================
DO $$
DECLARE
    col_type text;
BEGIN
    SELECT data_type INTO col_type
    FROM information_schema.columns
    WHERE table_name = 'kasahareketleri' AND column_name = 'islem';

    IF col_type = 'integer' OR col_type = 'bigint' OR col_type = 'smallint' THEN
        ALTER TABLE kasahareketleri ALTER COLUMN islem TYPE text USING (
            CASE islem
                WHEN 1 THEN 'GİRİŞ'
                WHEN 2 THEN 'ÇIKIŞ'
                WHEN 3 THEN 'SATIŞ'
                WHEN 4 THEN 'İADE'
                ELSE islem::text
            END
        );
    END IF;
END $$;
