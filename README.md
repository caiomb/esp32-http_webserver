# HTTP WebServer

Este projeto consiste na implemetação de um servidor web usando ESP32,
implementado através do uso da API de sockets lwip.

A placa ESP32 está configurada para gerar um ponto de acesso wifi,
desta forma é necessário conectar o seu computador à rede Wi-Fi
fornecida pela placa para utilizar o servidor.

Após se conectar à rede Wi-Fi provisionada pelo ESP32, basta digitar o
IP da placa em algum navegador de sua escolha (Chrome, Firefox) e ela
retorná uma página Web de onde é possível executar ações para controlar
leds conectados aos pinos 26 (Led 2), 27 (Led 3) e 32 (Led 1).

As ações possíveis para cada led são:
    
    - Ligar, desligar ou piscar o Led 1.
    
    - Ligar, desligar ou piscar o Led 2.
    
    - Ligar, desligar ou piscar o Led 3.


## Download do projeto:

    https://github.com/caiomb/esp32-http_webserver