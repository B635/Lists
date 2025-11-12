CREATE TABLE bookRecords
(
    id        SERIAL PRIMARY KEY,
    title     VARCHAR(255) NOT NULL,
    author    VARCHAR(255) NOT NULL,
    startDate DATE,
    endDate   DATE,
    rating    INT CHECK (rating BETWEEN 1 AND 5)
)