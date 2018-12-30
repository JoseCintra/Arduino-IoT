/* 
  Wi-fi Remote Control with JQuery and Onsen UI
  Demo by José Cintra
  www.josecintra.com/blog
*/

$(function () {

  // Server address and pin numbers of the board (ESP8266/32 and compatibles)
  let addr = "http://192.168.0.33/command"; 
  let pins = new Map([
    [ '#led1', '05' ],
    [ '#led2', '04' ],
    [ '#led3', '00' ], 
  ]);
   
  // Click Event on switch Class
  $('.sw').on('click', function (e) { 
    let onoff = ['OFF','ON'];
    let id = "#" + $(this).attr("id"); // Get the id of the control
    let pin = pins.get(id); // Pin number
    let value = String(+$(id).prop('checked')); // On or Off
    $(id + 'v').html(onoff[value]); 
    sendAjax(addr, pin, value);
  });

  // Input event on range class
  $('.rg').on('input', function (e) {
    let id = "#" + $(this).attr("id"); // Get the id of the control
    let value = String($(id).val()); // Input range
    $(id + 'v').html(value); // Notification
  });
  $('.rg').on('change', function (e) {
    let id = "#" + $(this).attr("id"); // Get the id of the control
    let pin = pins.get(id); // Pin number
    let value = String($(id).val()); // Input range
    sendAjax(addr, pin, value);
  });


});

function sendAjax(addr, p, v) {
  $.ajax({
    method: "POST",
    url: addr,
    data: {pin: p, value: v}
  });
}
