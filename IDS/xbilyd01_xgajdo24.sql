-- VUT FIT 2018-2019
-- IDS projek 4,5 Kocici systém
-- xbilyd01 xgajdo24

DROP TABLE Race CASCADE CONSTRAINTS;
DROP TABLE Cat CASCADE CONSTRAINTS;
DROP TABLE Human CASCADE CONSTRAINTS;
DROP TABLE Serve CASCADE CONSTRAINTS;
DROP TABLE Prefer CASCADE CONSTRAINTS;
DROP TABLE Territory CASCADE CONSTRAINTS;
DROP TABLE Property CASCADE CONSTRAINTS;
DROP TABLE Borrow CASCADE CONSTRAINTS;
DROP TABLE Living CASCADE CONSTRAINTS;
DROP TABLE Owns CASCADE CONSTRAINTS;
DROP TABLE Locates CASCADE CONSTRAINTS;
DROP TABLE Life CASCADE CONSTRAINTS;

DROP SEQUENCE cat_id_seq;
DROP SEQUENCE human_id_seq;
DROP SEQUENCE territory_id_seq;
DROP SEQUENCE property_id_seq;

DROP PROCEDURE write_cats_belongings;
DROP PROCEDURE write_fang_diif_from_avg;



CREATE TABLE Race(
    race_name VARCHAR(20) NOT NULL,
    fur_type VARCHAR(20) NOT NULL,
    origin VARCHAR(20) DEFAULT 'Unknown',
    fang_avg_lenght DECIMAL(3,2) NOT NULL,
    CONSTRAINT race_pk PRIMARY KEY(race_name)
);

CREATE TABLE Cat(
   cat_id INT NOT NULL,
   race_name VARCHAR(20) NOT NULL,
   cat_name VARCHAR(20) NOT NULL,
   fur_color VARCHAR(20) NOT NULL,
   fur_scheme VARCHAR(20) NOT NULL,
   fang_lenght DECIMAL(3,2),
   eye_color VARCHAR(20),
   CONSTRAINT cat_pk PRIMARY KEY(cat_id),
   CONSTRAINT race_fk_cat FOREIGN KEY (race_name) REFERENCES Race(race_name)
);

CREATE SEQUENCE cat_id_seq;

CREATE OR REPLACE TRIGGER cat_triggerred
  BEFORE INSERT ON Cat
  FOR EACH ROW
  WHEN (new.cat_id IS NULL)
BEGIN
  SELECT cat_id_seq.nextval
  INTO :new.cat_id
  FROM dual;
END;
/


CREATE TABLE Human(
    human_id INT NOT NULL,
    human_name VARCHAR(40) NOT NULL,
    age INT NOT NULL,
    sex VARCHAR(20) NOT NULL,
    CONSTRAINT human_pk PRIMARY KEY(human_id)
);

CREATE SEQUENCE human_id_seq;

CREATE OR REPLACE TRIGGER owner_triggerred
  BEFORE INSERT ON Human
  FOR EACH ROW
  WHEN (new.human_id IS NULL)
BEGIN
  SELECT human_id_seq.nextval
  INTO :new.human_id
  FROM dual;
END;
/

CREATE TABLE Serve(
    cat_id INT NOT NULL,
    human_id INT NOT NULL,
    cat_nickname VARCHAR(20),
    CONSTRAINT serve_pk PRIMARY KEY(cat_id, human_id),
    CONSTRAINT cat_fk_serve FOREIGN KEY (cat_id) REFERENCES Cat(cat_id),
    CONSTRAINT owner_fk_serve FOREIGN KEY (human_id) REFERENCES Human(human_id)
);


CREATE TABLE Prefer(
    race_name VARCHAR(20) NOT NULL,
    human_id INT NOT NULL,
    CONSTRAINT prefer_pk PRIMARY KEY(race_name, human_id),
    CONSTRAINT race_fk_prefer FOREIGN KEY (race_name) REFERENCES Race,
    CONSTRAINT owner_fk_prefer FOREIGN KEY (human_id) REFERENCES Human(human_id)
);



CREATE TABLE Territory(
    territory_id INT NOT NULL,
    territory_name VARCHAR(20) NOT NULL,
    territory_type VARCHAR(20) NOT NULL,
    territory_capacity INT NOT NULL,
    state_of_war VARCHAR(40),
    deaths INT,
    policy VARCHAR(40),
    faith VARCHAR(40),
    CONSTRAINT territory_pk PRIMARY KEY(territory_id)
);

CREATE SEQUENCE territory_id_seq;

CREATE OR REPLACE TRIGGER territory_triggerred
  BEFORE INSERT ON Territory
  FOR EACH ROW
  WHEN (new.territory_id IS NULL)
BEGIN
  SELECT territory_id_seq.nextval
  INTO :new.territory_id
  FROM dual;
END;
/

CREATE TABLE Property(
    property_id INT NOT NULL,
    territory_id_found INT NOT NULL,
    territory_id_locates INT,
    property_name VARCHAR(40) NOT NULL,
    amount INT NOT NULL,
    CONSTRAINT property_pk PRIMARY KEY(property_id),
    CONSTRAINT found_fk_property FOREIGN KEY (territory_id_found) REFERENCES Territory(territory_id),
    CONSTRAINT locates_fk_property FOREIGN KEY (territory_id_locates) REFERENCES Territory(territory_id)
);

CREATE SEQUENCE property_id_seq;

CREATE OR REPLACE TRIGGER property_triggerred
  BEFORE INSERT ON Property
  FOR EACH ROW
  WHEN (new.property_id IS NULL)
BEGIN
  SELECT property_id_seq.nextval
  INTO :new.property_id
  FROM dual;
END;
/

CREATE TABLE Borrow(
    property_id INT NOT NULL,
    human_id INT NOT NULL,
    amount INT NOT NULL,
    CONSTRAINT borrow_pk PRIMARY KEY(property_id, human_id),
    CONSTRAINT property_fk_borrow FOREIGN KEY (property_id) REFERENCES Property,
    CONSTRAINT owner_fk_borrow FOREIGN KEY (human_id) REFERENCES Human(human_id)
);

CREATE TABLE Living(
    territory_id INT NOT NULL,
    human_id INT NOT NULL,
    address INT NOT NULL,
    CONSTRAINT living_pk PRIMARY KEY(territory_id, human_id),
    CONSTRAINT territory_fk_living FOREIGN KEY (territory_id) REFERENCES Territory,
    CONSTRAINT owner_fk_living FOREIGN KEY (human_id) REFERENCES Human(human_id)
);

CREATE TABLE Owns(
    cat_id INT NOT NULL,
    property_id INT NOT NULL,
    found DATE NOT NULL,
    discarded DATE, 
    CONSTRAINT owns_pk PRIMARY KEY(cat_id, property_id),
    CONSTRAINT cat_fk FOREIGN KEY (cat_id) REFERENCES Cat,
    CONSTRAINT property_fk_owns FOREIGN KEY (property_id) REFERENCES Property
);

CREATE TABLE Locates(
    cat_id INT NOT NULL,
    territory_id INT NOT NULL,
    entered DATE NOT NULL,
    left DATE, 
    CONSTRAINT locates_pk PRIMARY KEY(cat_id, territory_id),
    CONSTRAINT cat_fk_locates FOREIGN KEY (cat_id) REFERENCES Cat,
    CONSTRAINT territory_fk_locates FOREIGN KEY (territory_id) REFERENCES Territory
);

CREATE TABLE Life(
    cat_id INT NOT NULL,
    life_num INT NOT NULL,
    territory_id_born INT NOT NULL,
    territory_id_death,
    born DATE NOT NULL,
    died DATE, 
    death_cause VARCHAR(100),
    CONSTRAINT life_pk PRIMARY KEY(cat_id, life_num),
    CONSTRAINT cat_fk_life FOREIGN KEY (cat_id) REFERENCES Cat,
    CONSTRAINT death_fk_life FOREIGN KEY (territory_id_death) REFERENCES Territory(territory_id),
    CONSTRAINT born_fk_life FOREIGN KEY (territory_id_born) REFERENCES Territory(territory_id)
);

CREATE OR REPLACE TRIGGER life_triggered
  BEFORE INSERT ON Life
  FOR EACH ROW
BEGIN
    declare
    i INT := 0;
    BEGIN
        SELECT count(*) INTO i FROM Life WHERE Life.cat_id = :new.cat_id AND Life.life_num+1 = :new.life_num;
        IF (i = 0 AND :new.life_num != 1) THEN
            BEGIN 
                RAISE_APPLICATION_ERROR(-20000,'Nespravne ocislovani zivota');
            END;
        END IF;
    END;
END;
/






--Procedures
--**********
CREATE OR REPLACE PROCEDURE write_fang_diif_from_avg AS
    CURSOR cats IS SELECT * FROM Cat JOIN Race ON Cat.race_name = Race.race_name;
    cat cats%ROWTYPE;
    lenght_diff FLOAT;
    BEGIN
        dbms_output.put_line('Cats fang leght difference from race average:');
        OPEN cats;
        LOOP
            FETCH cats INTO cat;
            EXIT WHEN cats%NOTFOUND;
            lenght_diff := cat.fang_lenght - cat.fang_avg_lenght;
            dbms_output.put_line(cat.cat_name||'''s difference is : '||lenght_diff);
        END LOOP;
        CLOSE cats;
        dbms_output.put_line('');
    EXCEPTION
        WHEN OTHERS THEN
            RAISE_APPLICATION_ERROR(-20000,'Unexpected procudere error');
    END;
/

CREATE OR REPLACE PROCEDURE write_cats_belongings AS
    CURSOR cats IS SELECT * FROM Cat;
    cat cats%ROWTYPE;
    CURSOR properties IS SELECT * FROM Property JOIN Owns ON Property.property_id = Owns.property_id;
    prop properties%ROWTYPE; 
    BEGIN
        dbms_output.put_line('List of cats and their belongings:');
        OPEN cats;
        LOOP
            FETCH cats INTO cat;
            EXIT WHEN cats%NOTFOUND;
            dbms_output.put_line(cat.cat_name||':');
            OPEN properties;
            LOOP
                FETCH properties INTO prop;
                EXIT WHEN properties%NOTFOUND;
                IF prop.cat_id = cat.cat_id THEN
                    dbms_output.put_line('  '||prop.property_name);
                END IF;
            END LOOP;
            CLOSE properties;
        END LOOP;
        CLOSE cats;
        dbms_output.put_line('');
    EXCEPTION
        WHEN OTHERS THEN
            RAISE_APPLICATION_ERROR(-20000,'Unexpected procudere error');
    END;
/





INSERT INTO Race (race_name,fur_type,origin,fang_avg_lenght) VALUES ('Persian', 'soft', 'Japan',4.20);
INSERT INTO Race (race_name,fur_type,fang_avg_lenght) VALUES ('European', 'fluffy',3.14);
INSERT INTO Race (race_name,fur_type,origin,fang_avg_lenght) VALUES ('Scottish fold', 'straight', 'Scotland',2.71);
INSERT INTO Cat (race_name, cat_name, fur_color, fur_scheme, fang_lenght, eye_color) VALUES ('Persian', 'Sir Mewsalot', 'Ginger', 'Rough', 4.20, 'Red');
INSERT INTO Cat (race_name, cat_name, fur_color, fur_scheme, fang_lenght, eye_color) VALUES ('Scottish fold','Fuzzy Face' , 'Purple', 'Luscious', 1.5, 'Green');
INSERT INTO Cat (race_name, cat_name, fur_color, fur_scheme, fang_lenght, eye_color) VALUES ('European', 'Rainbow Princess', 'Black', 'Long', 1.69, 'Rainbow');
INSERT INTO Human(human_name, age, sex) VALUES('Boris',42,'Female');
INSERT INTO Human(human_name, age, sex) VALUES('Daniel',20,'Male');
INSERT INTO Human(human_name, age, sex) VALUES('Jack',21,'Male');
INSERT INTO Serve(cat_id, human_id, cat_nickname) VALUES(1,3,'Tom');
INSERT INTO Serve(cat_id, human_id, cat_nickname) VALUES(2,2,'Jerry');
INSERT INTO Serve(cat_id, human_id, cat_nickname) VALUES(3,1,'Bunny');
INSERT INTO Prefer(race_name,human_id)VALUES('Persian',1);
INSERT INTO Prefer(race_name,human_id)VALUES('Scottish fold',2);
INSERT INTO Prefer(race_name,human_id)VALUES('European',3);
INSERT INTO Territory(territory_name,territory_type,territory_capacity,policy,faith)VALUES('Kitchen','Peaceful',20,'Catogracy','Sphagetti monster');
INSERT INTO Territory(territory_name,territory_type,territory_capacity,policy,faith)VALUES('Living room','Peaceful',30,'Catonism','Sphagetti monster');
INSERT INTO Territory(territory_name,territory_type,territory_capacity,state_of_war,deaths)VALUES('Toilet','In War',4,'Trench Warfare',0);
INSERT INTO Property(territory_id_found, territory_id_locates, property_name, amount)VALUES(1,3,'Ball',1);
INSERT INTO Property(territory_id_found, territory_id_locates, property_name, amount)VALUES(2,3,'Sock',2);
INSERT INTO Property(territory_id_found, territory_id_locates, property_name, amount)VALUES(3,3,'Paper',3);
INSERT INTO Borrow(property_id,human_id ,amount)VALUES(1,1,1);
INSERT INTO Borrow(property_id,human_id ,amount)VALUES(2,1,1);
INSERT INTO Borrow(property_id,human_id ,amount)VALUES(3,3,2);
INSERT INTO Living(territory_id,human_id,address)VALUES(1,2,42);
INSERT INTO Living(territory_id,human_id,address)VALUES(1,1,43);
INSERT INTO Living(territory_id,human_id,address)VALUES(3,3,44);
INSERT INTO Owns(cat_id,property_id,found,discarded)VALUES(1,1,TO_DATE('11-07-1992','DD-MM-YYYY'),TO_DATE('13-09-1993','DD-MM-YYYY'));
INSERT INTO Owns(cat_id,property_id,found,discarded)VALUES(2,1,TO_DATE('01-03-1999','DD-MM-YYYY'),TO_DATE('15-02-1999','DD-MM-YYYY'));
INSERT INTO Owns(cat_id,property_id,found,discarded)VALUES(1,3,TO_DATE('20-09-1995','DD-MM-YYYY'),TO_DATE('21-09-1995','DD-MM-YYYY'));
INSERT INTO Locates(cat_id,territory_id,entered,left)VALUES(1,3,TO_DATE('26-12-1997','DD-MM-YYYY'),TO_DATE('27-12-1997','DD-MM-YYYY'));
INSERT INTO Locates(cat_id,territory_id,entered,left)VALUES(1,1,TO_DATE('27-12-1997','DD-MM-YYYY'),TO_DATE('28-12-1997','DD-MM-YYYY'));
INSERT INTO Locates(cat_id,territory_id,entered,left)VALUES(2,3,TO_DATE('18-11-1997','DD-MM-YYYY'),TO_DATE('06-04-1998','DD-MM-YYYY'));
INSERT INTO Life(cat_id,life_num,territory_id_born,territory_id_death,born,died,death_cause)VALUES(1,1,2,3,TO_DATE('12-06-1990','DD-MM-YYYY'),TO_DATE('11-07-1992','DD-MM-YYYY'),'Hit by schrapnel');
INSERT INTO Life(cat_id,life_num,territory_id_born,born)VALUES(1,2,3,TO_DATE('11-07-1992','DD-MM-YYYY'));
INSERT INTO Life(cat_id,life_num,territory_id_born,born)VALUES(2,1,1,TO_DATE('16-04-1991','DD-MM-YYYY'));
INSERT INTO Life(cat_id,life_num,territory_id_born,territory_id_death,born,died,death_cause)VALUES(3,1,1,1,TO_DATE('22-09-1989','DD-MM-YYYY'),TO_DATE('25-12-2005','DD-MM-YYYY'),'Slipped on a wet floor');
INSERT INTO Life(cat_id,life_num,territory_id_born,born)VALUES(3,2,3,TO_DATE('25-12-2005','DD-MM-YYYY'));







-- ********************
-- treti cast projektu 
-- Dotazy SELECT
-- ********************

-- dva dotazy vyuzivajici spojeni dvou tabulek       
-- *******************************************

-- zeme puvodu kocky se jmenem 'Fuzzy Face'
SELECT Race.origin 
FROM Cat NATURAL JOIN Race
WHERE Cat.cat_name = 'Fuzzy Face';

--informace o vsech zivotech kocky jmenem 'Sir Mewsalot'
SELECT LIFE_NUM , BORN, DIED,DEATH_CAUSE
FROM Cat NATURAL JOIN Life
WHERE Cat.cat_name = 'Sir Mewsalot';



-- jeden vyuzivajici spojeni tri tabulek
-- **************************************

--jmeno kocky ktere slouzi Boris
SELECT Cat.cat_name
FROM Cat, Serve, Human
WHERE Cat.cat_id = Serve.cat_id AND Serve.human_id = Human.human_id
AND Human.human_name = 'Boris'; 




-- dva dotazy s klauzuli GROUP BY a agregacni funkci
-- *************************************************


-- kolik muzu je v databazi
SELECT Human.sex, COUNT(*)
FROM Human
WHERE Human.sex = 'Male'
GROUP BY Human.sex;

-- serazene kocky podle celkoveho poctu veci co kdy nasly
SELECT Cat.cat_name, SUM(Property.amount)
FROM Cat,Owns,Property
WHERE Cat.cat_id = Owns.cat_id AND Owns.property_id = Property.property_id
GROUP BY Cat.cat_name
ORDER BY SUM(Property.amount) DESC;





-- jeden dotaz obsahujici predikat EXISTS
-- **************************************


-- Kocky ktere ziji druhy zivot
SELECT c.cat_name 
FROM Cat c 
WHERE EXISTS(SELECT * FROM Life WHERE Life.cat_id = c.cat_id AND Life.life_num = 2);




-- jeden dotaz s predikatem IN s vnorenym selectem
-- ***********************************************

-- Teritoria ktera jsou mirumilovna
SELECT Territory.territory_name
FROM Territory
WHERE Territory.territory_id IN (SELECT Territory.territory_id FROM Territory WHERE Territory.territory_type = 'Peaceful');


--TRIGGER CHECK
--*************
INSERT INTO Life(cat_id,life_num,territory_id_born,born)VALUES(3,6,3,TO_DATE('22-09-2000','DD-MM-YYYY'));

--PROCEDURES CHECK
--****************
SET SERVEROUTPUT ON;
BEGIN
    write_fang_diif_from_avg();
END;
/
BEGIN
    write_fang_diif_from_avg();
END;
/


--INDEX
--*****
EXPLAIN PLAN FOR
    SELECT Race.race_name, COUNT(*) AS Pocet_kocek
    FROM Race, Cat 
    WHERE Race.race_name = Cat.race_name
    GROUP BY Race.race_name;
SELECT plan_table_output FROM table (dbms_xplan.display());

CREATE INDEX index_cats_race ON Cat(race_name);

EXPLAIN PLAN FOR
    SELECT Race.race_name, COUNT(*) AS Pocet_kocek
    FROM Race, Cat 
    WHERE Race.race_name = Cat.race_name
    GROUP BY Race.race_name;
SELECT plan_table_output FROM table (dbms_xplan.display());

DROP INDEX index_cats_race;


GRANT ALL ON  Race TO xgajdo24;
GRANT ALL ON  Cat TO xgajdo24;
GRANT ALL ON  Human TO xgajdo24;
GRANT ALL ON  Serve TO xgajdo24;
GRANT ALL ON  Prefer TO xgajdo24;
GRANT ALL ON  Territory TO xgajdo24;
GRANT ALL ON  Property TO xgajdo24;
GRANT ALL ON  Borrow TO xgajdo24;
GRANT ALL ON  Living TO xgajdo24;
GRANT ALL ON  Owns TO xgajdo24;
GRANT ALL ON  Locates TO xgajdo24;
GRANT ALL ON  Life TO xgajdo24;

GRANT EXECUTE ON write_cats_belongings TO xgajdo24;
GRANT EXECUTE ON  write_fang_diif_from_avg TO xgajdo24;

CREATE MATERIALIZED VIEW add_a_cat
CACHE
BUILD IMMEDIATE
REFRESH ON COMMIT AS
  SELECT *
  FROM xbilyd01.Cat
  ORDER BY xbilyd01.Cat.cat_name;


select * from add_a_cat;
INSERT INTO xbilyd01.Cat (race_name, cat_name, fur_color, fur_scheme, fang_lenght, eye_color) VALUES ('Scottish fold', 'Miss Steak', 'Grey', 'Slithery', 3.25, 'Blue');
select * from add_a_cat;
COMMIT;
select * from add_a_cat;

DROP MATERIALIZED VIEW add_a_cat;