create or replace view pattern_data as
(SELECT 'Минимальное значение', price, id
FROM public.requests_services_to_position order by price limit 1)
union all
(SELECT 'Максимальное значение', price, id
FROM public.requests_services_to_position order by price desc limit 1) 
union all
(SELECT 'Среднее значение', AVG(price), null 
FROM public.requests_services_to_position) 
union all
(SELECT 'Сумма значений', SUM(price), null 
FROM public.requests_services_to_position) 