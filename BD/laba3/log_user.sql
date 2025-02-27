CREATE TABLE log_users (
    id BIGSERIAL PRIMARY KEY,
    user_id BIGINT REFERENCES users(id),
    change_datetime TIMESTAMP DEFAULT NOW(),
    old_value TEXT DEFAULT NULL,
    new_value TEXT DEFAULT NULL
);

CREATE OR REPLACE FUNCTION trigger_func()
RETURNS TRIGGER AS $$
DECLARE
    old_val TEXT;
BEGIN
    /* Определяем "старое" значение */
    IF (TG_OP = 'UPDATE') THEN
        old_val := OLD.name;
    ELSIF (TG_OP = 'INSERT') THEN
        old_val := NULL;
    end if;

    /* Производим запрос */
    INSERT INTO log_users
        (user_id, old_value, new_value)
        VALUES
        (NEW.id, old_val, NEW.name);

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER commit_users_change
AFTER UPDATE OR INSERT
ON users
FOR EACH ROW
EXECUTE PROCEDURE trigger_func();
