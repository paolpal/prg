#include "../utility.h"

// *********************************************
// Aggiunge l'utente specificato alla chatroom
// *********************************************
struct user* append_user(struct user** chatroom, char* username){
  int len;
  struct user* new_user;
  if(*chatroom==NULL){
    new_user = (struct user*)malloc(sizeof(struct user));
    len = strlen(username)+1;
    new_user->username = (char*) malloc(len*sizeof(char));
    strcpy(new_user->username, username);
    new_user->cht_sd = 0;
    new_user->next_seq_n = 0;
    new_user->next = NULL;
    *chatroom = new_user;
    return new_user;
  }
  else if(strcmp(username,(*chatroom)->username)==0) return NULL;
  else return append_user(&(*chatroom)->next, username);
}

// *********************************************
// Rimuove l'utente specificato dalla chatroom
// *********************************************
void remove_user(struct user** chatroom, char* username){
  struct user* user = *chatroom, *prev;
  if(user != NULL && strcmp(user->username,username)==0) {
    *chatroom = user->next;
    free(user);
    return;
  }
  while(user != NULL && strcmp(user->username,username)!=0) {
    prev = user;
    user = user->next;
  }
  if(user == NULL) return;
  prev->next = user->next;
  free(user);
}

// *********************************************
// Stampa l'elenco dei partecipanti alla chatroom
// *********************************************
void print_chatroom(struct user* chatroom){
  struct user* user = chatroom;
  while(user!=NULL){
    printf("-) %s\n", user->username);
    user = user->next;
  }
}

// *************************************
// Controllo che l'utente specificato
// faccia parte della chatroom
// *************************************
int chatting_with(char *user, struct user* chatroom){
  struct user * c_user = chatroom;
  while (c_user!=NULL) {
    if(strcmp(c_user->username, user)==0) return 1;
    c_user= c_user->next;
  }
  return 0;
}

// *************************************
// passo i dati necessari alla creazione
// della struttura msg.
// Il mittente è NULL per identificare
// che è l'utente stesso...
// *************************************
struct msg* create_my_msg(char* dest, char* text, int seq_n){
  struct msg* msg = (struct msg*)malloc(sizeof(struct msg));
  msg->sender = NULL;
  msg->ACK = 0;
  msg->dest = (char*) malloc((strlen(dest)+1)*sizeof(char));
  strcpy(msg->dest,dest);
  msg->text = (char*) malloc((strlen(text)+1)*sizeof(char));
  strcpy(msg->text, text);
  msg->seq_n = seq_n;
  return msg;
}

// *************************************
// la funzione riceve una chatroom e
// una pipe sulla quale scrivere
// gli username
// *************************************
void send_chatroom_mp(int p_son_sd, struct user* chatroom){
  uint32_t len;
  struct user* user = chatroom;
  while(user!=NULL){
    len = strlen(user->username)+1;
    write(p_son_sd, &len, sizeof(len));
    write(p_son_sd, user->username, len);
    user=user->next;
  }
  len = 0;
  write(p_son_sd, &len, sizeof(len));
}

void display_help_message(){
  printf("Digita un comando: \n\n");
  printf("1) signup porta username password --> crea un account sul server\n");
  printf("2) in porta username password --> accede ad un account sul server\n");
  printf("3) hanging --> ricevo una lista degli utenti che mi hanno contattato\n");
  printf("4) show username --> ricevo i messaggi pendenti mandati da *username*\n");
  printf("5) chat username --> apro una chat con *username*\n");
  printf("6) share filename --> invio il file agli utenti con cui sto chattando\n");
  printf("7) out --> eseguo una disconnessione dal server\n");
  printf("8) esc --> termino l'applicazione\n");
  printf("8) help --> mostro il messaggio di aiuto\n");
}

// *************************************
// data la lista di tutte le chat
// la funzione ritorna la chat
// corrispondente all'utente cercato
// *************************************
struct msg* find_msg_list(struct chat **l_chat_ref, char *username){
  struct chat *c_chat = *l_chat_ref;
  while(c_chat!=NULL){
    if(strcmp(c_chat->user, username)==0){
      return c_chat->l_msg;
    }
    c_chat = c_chat->next;
  }
  return NULL;
}

// *************************************
// dato lo username di destinazione e
// il numero di sequenza, setto l'ACK di
// consegna al messaggio corretto
// *************************************
void acknoledge_message(struct chat **l_chat_ref, char *username, int seq_n){
  struct msg* l_msg = find_msg_list(l_chat_ref, username);
  struct msg* c_msg = l_msg;
  while(c_msg!=NULL){
    if(c_msg->sender == NULL && c_msg->seq_n == seq_n) c_msg->ACK = 1;
    c_msg = c_msg->next;
  }
}

// *************************************
// controllo che lo username specificato
// sia nella rubrica, altrimenti non
// posso aprire la chat e ritorno 0
// *************************************
int is_in_addr_book(char* username){
  FILE *addr_book_file = NULL;
  char c_name[512];
  int found = 0;
  if((addr_book_file = fopen("rubrica.txt","r")) == NULL){
    exit(1);
  }
  while(!found && fscanf(addr_book_file,"%s", c_name) != EOF){
    if(strcmp(c_name, username)==0) found = 1;
  }
  fclose(addr_book_file);
  return found;
}

// *************************************
// per verificare che l'utente specificato
// sia online, contatto il server con
// l'apposita procedura
// *************************************
int is_online(int srv_sd, char* username){
  return online_check_protocol_client(srv_sd, username);
}

// *************************************
// la funzione conta i parametri passati
// da linea di comando.
// necessaria per non avere prblemi
// durante il parsing dei comandi
// *************************************
int parametrs_num(char* str){
  int count = 0;
  char *ptr = str;
  while((ptr = strchr(ptr, ' ')) != NULL) {
      count++;
      ptr++;
  }
  return count;
}
