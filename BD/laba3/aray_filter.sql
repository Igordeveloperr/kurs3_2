CREATE TYPE t_request AS (
    id INTEGER,
    resident_name TEXT,
	comments TEXT,
    client_id INTEGER,
	object_type_id INTEGER
);

CREATE OR REPLACE FUNCTION filter_array_of_requests (
    arr t_request[],
    filter_var INTEGER
)
RETURNS t_request[]
AS $$
BEGIN

    RETURN ARRAY( /* Преобразуем выборку в массив */
        SELECT (id, resident_name, comments, client_id, object_type_id) /* Создаем таблицу из элементов массива */
        FROM unnest(arr)
        WHERE object_type_id >= filter_var
    );
	
END;
$$ LANGUAGE plpgsql;
