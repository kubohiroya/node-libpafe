
console.log(1);
var pasoriManager = new require('./build/Release/pafe').PasoriManager();
console.log(2);

var FELICA_LITE_SYSTEM_CODE = 0x88B4;
var TIMESLOT = 0;

var STUDENT_INFO_SERVICE_CODE = 0x000B;
var STUDENT_INFO_BLOCK_NUM = 0x8004;
var STUDENT_INFO_SUBSTRING_BEGIN = 2;
var STUDENT_INFO_SUBSTRING_END = 9;

var timeslot = 0;

hex_dump = function(ary){
    var ret = '';
    for(var i = 0; i<ary.length; i++){
        //ret += String.fromCharCode(ary[i])
        ret += ary[i].toString(16)+" ";
    }
    return ret;
}

var pasori = pasoriManager.open();

pasori.set_timeout(1000);

var felica = pasori.polling(FELICA_LITE_SYSTEM_CODE, TIMESLOT);

var idm = felica.get_idm();

console.log(hex_dump(idm));

felica.close();
pasori.close();
//pasoriManager.close();
