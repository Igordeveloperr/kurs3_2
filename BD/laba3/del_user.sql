CREATE OR REPLACE FUNCTION del_user(_id INTEGER) 
RETURNS VOID AS $$
DECLARE
BEGIN

DELETE FROM users WHERE id = _id;
EXCEPTION
    WHEN foreign_key_violation THEN
    RAISE EXCEPTION 'Невозможно выполнить удаление, так как есть внешние ссылки.';

END;
$$ LANGUAGE plpgsql;