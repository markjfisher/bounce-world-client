#ifndef BWC_CONNECTION_H
#define BWC_CONNECTION_H

void connect_service();
void disconnect_service();
void send_client_data();
void create_command(char *cmd);
void append_command(char *cmd);
void send_command();
int16_t read_response(uint8_t *buf, int16_t len);
int16_t read_response_with_error(uint8_t *buf, int16_t len);
void request_client_data();

#endif // BWC_CONNECTION_H