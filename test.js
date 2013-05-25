var pafe = require('./build/Release/pafe');

var FELICA_LITE_SYSTEM_CODE = 0x88B4;
var TIMESLOT = 0;
var STUDENT_INFO_SERVICE_CODE = 0x000B;
var STUDENT_INFO_BLOCK_NUM = 0x8004;
var STUDENT_INFO_SUBSTRING_BEGIN = 2;
var STUDENT_INFO_SUBSTRING_END = 9;

hex_dump = function(ary){
    var ret = '';
    for(var i = 0; i<ary.length; i++){
        ret += ary[i].toString(16)+" ";
    }
    return ret;
}

var pasoriArray = pafe.open_pasori_multi();

if(!pasoriArray){
    console.log("fail to open pasori.");
}

for(var i = 0; i < pasoriArray.length; i++){
    var pasori = pasoriArray[i];
    pasori.set_timeout(1000);
    var felica = pasori.polling(FELICA_LITE_SYSTEM_CODE, TIMESLOT);
    if(felica){
        var idm = felica.get_idm();
        console.log("["+i+"] "+ hex_dump(idm));
        felica.close();
    }else{
        console.log("polling timeout.");
    }
    pasori.close();
}

//pasoriManager.close();
