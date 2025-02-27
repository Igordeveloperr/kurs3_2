SELECT serv.name, rsToPos.price, user_pos.title
	FROM public.requests_services_to_position rsToPos 
	join public.requests_client_services serv on serv.id=rsToPos.service_id 
	join public.user_positions user_pos on user_pos.id=rsToPos.position_id;