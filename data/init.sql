DROP TABLE GoalGame;

CREATE TABLE GoalGame (
    levelId INT PRIMARY KEY,
    bestTime NUMBER,
    CONSTRAINT levelId CHECK ( levelId >= 0 ),
    CONSTRAINT bestTime CHECK ( bestTime > 0 )
);
