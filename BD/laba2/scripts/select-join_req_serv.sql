SELECT req_app.resident_name, req_app.comments, req_cl_serv.name, 
	req_cl_serv.description, user_pos.title, rsToPos.price,
	apartment.city, apartment.street, apartment.house_number,
	use_r.name
	FROM public.requests_services req_serv 
	join public.requests_services_to_position rsToPos on rsToPos.id=req_serv.service_id
	join public.requests_client_services req_cl_serv on req_cl_serv.id=rsToPos.service_id
	join public.user_positions user_pos on user_pos.id=rsToPos.position_id
	join public.requests_client_applications req_app on req_app.id=req_serv.request_id
	join public.apartment_buildings apartment on apartment.id=req_app.object_num_id
	join public.users use_r on use_r.id=req_app.client_id;