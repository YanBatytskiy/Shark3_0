CREATE OR REPLACE FUNCTION public.delete_user()
 RETURNS trigger
 LANGUAGE plpgsql
AS $function$
begin
	delete from "users_passhash" where user_id = old.id;
	return old;
end;	
$function$;

CREATE OR REPLACE FUNCTION public.new_user_create()
 RETURNS trigger
 LANGUAGE plpgsql
AS $function$
begin
	insert into "users_passhash" ("user_id", "password_hash") values
	(New.id, null);
	return new;
end;	
$function$;

create trigger trg_delete_user before
delete
    on
    public.users for each row execute function delete_user();


create trigger trg_new_user_create after
insert
    on
    public.users for each row execute function new_user_create();
