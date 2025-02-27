CREATE OR REPLACE FUNCTION get_value_by_id (
    tableName VARCHAR,
    columnName VARCHAR,
    id BIGINT
)
RETURNS TEXT AS $$
DECLARE
    res TEXT;
BEGIN
    EXECUTE 'SELECT '|| columnName ||' FROM '|| tableName ||' WHERE id = $1' USING id INTO res;
    RETURN res;
END;
$$ LANGUAGE plpgsql;
