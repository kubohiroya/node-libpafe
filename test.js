var FELICA = {
    TIMEOUT: 255,
    POLLING_TIMESLOT: 0,
    SYSTEM_CODE: {
        ANY: 0xFFFF,
        FELICA_LITE: 0x88B4
    }
};

var pafe = require('./index');
var pasori = new pafe.Pasori();
pasori.setTimeout(0);

pasori.polling(FELICA.SYSTEM_CODE.FELICA_LITE,
	       FELICA.POLLING_TIMESLOT,
	       function(felica){
		   if(felica){
		       console.log('idm:',felica.getIDm());
		       console.log('pmm:',felica.getPMm());
		       felica.close();
		       pasori.close();
		   }
	       });

