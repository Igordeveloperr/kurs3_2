CREATE OR REPLACE FUNCTION sort_by_min(_minPrice INTEGER) 
RETURNS SETOF requests_services_to_position AS $$
DECLARE
BEGIN
	RETURN QUERY (SELECT * FROM requests_services_to_position WHERE price >= _minPrice);
END;
$$ LANGUAGE plpgsql;