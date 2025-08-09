create table if not exists participants (
chat_id integer not null,
user_id integer not null,
last_read_message_id integer not null,
deleted_from_chat boolean default false,
constraint chat_id_fk foreign key (chat_id) references chats(id),
constraint user_id_fk foreign key (user_id) references users(id),
constraint message_id_fk foreign key (last_read_message_id) references messages(id)
);
