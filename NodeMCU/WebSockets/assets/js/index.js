/***
Teste de Websockets com ESP8266 - Cliente
José Cintra em Outubro de 2019 - www.josecintra.com/blog/comunicacao-websockets-nodemcu-esp8266">
***/

$(document).ready(function () {

  let ledStatus = 0; // Status do LED 0 = desligado / 1 = ligado
  let buttonText = ['OFF', 'ON']; // Texto do botão do LED de acordo com o status
  let buttonStyle = ['btn btn-lg btn-secondary', 'btn btn-lg btn-success']; // Estilo do botão do LED de acordo com o status

  // Coloque aqui o IP obtino no ESP8266
  let con = new WebSocket('ws://192.168.0.31:81/', ['arduino']);

  // Evento que ocorre quando a placa envia dados
  let previous = "";
  con.onmessage = function (evt) { 
    if (evt.data != "ON" && evt.data != "OFF"){
		$('#temperature').html(evt.data);
	}
		previous = $('#log').html();
		$('#log').html(previous + '\n' + evt.data);

  };

  //$('#led-status').html(buttonText[ledStatus]);

  // Clique no botão do LED
  $('#led-status').click(function () {

    // Muda a classe e texto do botão de acordo com o status
    ledStatus = (ledStatus + 1) % 2; // Alterna entre 0 e 1
    $('#led-status').removeClass();
    $('#led-status').addClass(buttonStyle[ledStatus]);
    $('#led-status').html(buttonText[ledStatus]);

    // envia o comando para a placa
    con.send(buttonText[ledStatus]);


  });
});

