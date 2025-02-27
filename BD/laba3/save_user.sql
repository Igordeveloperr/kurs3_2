CREATE OR REPLACE FUNCTION save_user(_id INTEGER, _name TEXT, _password TEXT) 
RETURNS INTEGER AS $$
DECLARE
    user_id INTEGER;
BEGIN
    IF _id IS NULL THEN
        INSERT INTO users(name, password)
        VALUES(_name, _password)
        RETURNING id INTO user_id;
        RETURN user_id;  
    ELSE
	    UPDATE users SET
    	   name = _name,
    	   password = _password
        WHERE id = _id;
        RETURN _id;  
    END IF;
END;
$$ LANGUAGE plpgsql;