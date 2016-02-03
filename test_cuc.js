var CUC_IDCARD = {
    SERVICE_CODE: 0x000B,
    ID_INFO: {
        BLOCK_NUM: 0x8004,
        PREFIX: '01',
        BEGIN_AT: 2,
        END_AT: 9
    }
};

var read_cuc_card = function(felica){
    var data = felica.readSingle(CUC_IDCARD.SERVICE_CODE,
				 0,
				 CUC_IDCARD.ID_INFO.BLOCK_NUM);
    if (data !== undefined) {
	var userID = data.substring( CUC_IDCARD.ID_INFO.BEGIN_AT,
				     CUC_IDCARD.ID_INFO.END_AT);
	while (userID.lastIndexOf("_") === userID.length - 1) {
	    userID = userID.substring(0, userID.length - 1);
	}
	console.log("ID_CODE:"+userID);
    }
}

var FELICA = {
    TIMEOUT: 255,
    POLLING_TIMESLOT: 0,
    SYSTEM_CODE: {
        ANY: 0xFFFF,
        FELICA_LITE: 0x88B4
    }
};

var pafe = require('./build/Release/pafe');
var pasori = new pafe.Pasori();
pasori.setTimeout(100);

pasori.polling(FELICA.SYSTEM_CODE.FELICA_LITE,
	       FELICA.POLLING_TIMESLOT,
	       function(felica){
		   if(felica){
		       console.log('idm:',felica.getIDm());
		       console.log('pmm:',felica.getPMm());

		       read_cuc_card(felica);
		       
		       felica.close();
		       pasori.close();
		   }
	       });
