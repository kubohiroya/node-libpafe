# node-libpafe

a Node.js addon to access FeliCa with SONY PaSoRi(RC-S310,RC-S320,RC-S330).

This is a wrapper library selectively using one of multiple implementations : [libpafe](https://github.com/rfujita/libpafe) for Linux/OS X plathome or [felicalib](http://felicalib.tmurakam.org/) for Windows plathome.

Installation
===============

You'll need to install [libpafe](https://github.com/rfujita/libpafe) or [felicalib](http://felicalib.tmurakam.org/) beforehand.

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

License
===
GPL-2.0

Author
===
* Hiroya Kubo <hiroya@cuc.ac.jp>, Faculty of Global Studies, Chiba University of Commerce, Japan.
