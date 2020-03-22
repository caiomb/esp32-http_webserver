# HTTP WebServer

Este projeto consiste na implemetação de um servidor HTTP usando ESP32.

O servidor HTTP é implementado usando sockets com suporte da biblioteca lwip.

A placa ESP32 está configurada para gerar um ponto de acesso wifi,
desta forma é necessário conectar o seu computador à rede Wi-Fi fornecida
pela placa para utilizar o servidor.

Após se conectar à rede Wi-Fi provisionada pelo ESP32, basta digitar o IP da
placa em algum navegador de sua escolha (Chrome, Firefox) para receber a página
HTML de onde será possível executar as ações para controlar os leds.

As ações possíveis são:
    
    - Ligar, desligar ou piscar o Led 1.
    
    - Ligar, desligar ou piscar o Led 2.
    
    - Ligar, desligar ou piscar o Led 3.