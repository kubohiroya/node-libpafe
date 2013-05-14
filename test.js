var pafe = require('./build/Release/pafe');
//var pafe = require('pafe');

var pasori = new pafe.PaFe();

var SYSTEM_CODE = 0x88B4;
var SERVICE_CODE = 0x000B;
var MODE = 0;
var STUDENT_ID_BLOCK_NUM = 0x04;

var timeslot = 500;

pasori.polling(SYSTEM_CODE, timeslot);

var data = pasori.readSingle(SERVICE_CODE, MODE, STUDENT_ID_BLOCK_NUM);

pasori.close();

console.log(data);
