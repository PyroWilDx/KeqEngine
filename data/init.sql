-- sqlite3/sqlite3.exe Keqing.db < Init.sql

DROP TABLE IF EXISTS ExploSlime;

CREATE TABLE ExploSlime (
    levelId INT PRIMARY KEY,
    bestTime NUMBER,
    CONSTRAINT levelId CHECK ( levelId >= 0 ),
    CONSTRAINT bestTime CHECK ( bestTime > 0 )
);
