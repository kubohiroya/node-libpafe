var pafe = require('./build/Release/pafe');

var FELICA_LITE_SYSTEM_CODE = 0x88B4;
var TIMESLOT = 0;

var CARDREADER = {
    SERVICE_CODE : 0x000B,
    CHECK_ORDER_TEACHER_STUDENT: false,
    ID_INFO:{
        BLOCK_NUM : 0x8004,
        PREFIX : '01',
        BEGIN_AT : 2,
        END_AT : 9
    }
};

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


        var data = felica.read_single(CARDREADER.SERVICE_CODE,
                                      0,
                                      CARDREADER.ID_INFO.BLOCK_NUM);

        console.log(hex_dump(data));

        felica.close();
    }else{
        console.log("polling timeout.");
    }
    pasori.close();
}

//pasoriManager.close();
