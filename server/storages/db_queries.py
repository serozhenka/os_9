CREATE_TABLE = """
    CREATE TABLE IF NOT EXISTS weather (
        temperature decimal,
        feels_like decimal,
        weather_type text,
        city text,
        date_added timestamp
    );
"""

INSERT_WEATHER = """
    INSERT INTO weather VALUES
    (?, ?, ?, ?, ?)
"""

SELECT_LAST_WEATHER = """
    SELECT * FROM weather
    order by date_added desc
    limit 1
"""
