#include <server/tcp.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{

  server_t s = {
    .port = "3094"
  };

  server_init(&s);

  server_exec(&s);
}
