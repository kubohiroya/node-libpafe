# node-libpafe

a Node.js addon to access FeliCa with SONY PaSoRi(RC-S310,RC-S320,RC-S330).

This is wrapper library of [libpafe](https://github.com/rfujita/libpafe).

Installation
===============

You'll need to install [libpafe](https://github.com/rfujita/libpafe) beforehand.

Then, just run:

    npm install 

Usage
===

    var SYSTEM_CODE_FELICA_LITE = 0x88B4;
    var pafe = require('libpafe');
    var pasori = new pafe.Pasori();
      pasori.polling(SYSTEM_CODE_FELICA_LITE, 0,
        function(felica){
	      if(felica){
		    console.log('idm:', felica.getIDm());
            console.log('pmm:', felica.getPMm());
		    felica.close();
		    pasori.close();
		  }
        }
	  );
