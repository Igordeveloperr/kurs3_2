WITH filtered_requests AS (
    SELECT filter_array_of_requests(
        ARRAY(
            SELECT (id, resident_name, comments, client_id, object_type_id)::t_request
            FROM requests_client_applications
        ),
        3
    ) AS requests
)
SELECT 
    (request).id,
    (request).resident_name,
    (request).comments,
    (request).client_id,
    (request).object_type_id
FROM filtered_requests, unnest(requests) AS request;