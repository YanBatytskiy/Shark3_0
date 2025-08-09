create table if not exists message_status (
message_id integer not  null,
chat_id integer not null,
user_id integer not null,
status varchar(20) not null,
status_deleted boolean default false,
constraint status_check check (status in ('NOT DELIVERED','DELIVERED', 'READ')),
constraint message_id_fk foreign key (message_id) references messages(id),
constraint user_id_fk foreign key (user_id) references users(id),
constraint chat_id_fk foreign key (chat_id) references chats(id)
);
