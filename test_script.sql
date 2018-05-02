CREATE TABLE abc FIELDS (Field1=int, Field2=str)
INSERT INTO abc (Field1=19,Field2='Hello World')
INSERT INTO abc (Field1=20,Field2='Hello World')
INSERT INTO abc (Field1=21,Field2='Hello World')
SELECT * FROM abc WHERE Field1=19
UPDATE abc SET Field1=20 WHERE Field2='Hello World'
SELECT * FROM abc WHERE Field1=20
DELETE FROM abc where Field1=19