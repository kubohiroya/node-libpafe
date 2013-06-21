{
  'targets': [
    {
      'target_name': 'pafe',

       'conditions':[
            ['OS=="linux"', {
                          'defines':['HAVE_LIBPAFE'],
                          'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc'],
                          'include_dirs':['/usr/local/include/libpafe', '/usr/local/include/libusb-1.0'],
                          'link_settings':{ 'libraries':['-lusb-1.0','-lpafe'] }
                          }
            ],
            ['OS=="mac"', {
                          'defines':['HAVE_LIBPAFE'],
                          'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc'],
                          'include_dirs':['/usr/local/include/libpafe', '/usr/local/include/libusb-1.0'],
                          'link_settings':{ 'libraries':['-lusb-1.0','-lpafe'] }
                          }
            ],
            ['OS=="win"', {
                          'defines':['HAVE_FELICALIB'],
                          'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc', 'deps/win/felicalib/felicalib.c', 'deps/win/pasori_open_multi.c'],
                          'include_dirs':['<(module_root_dir)/deps/win/felicalib', '<(module_root_dir)/deps/win/include'],
                          'cflags': ['/EHsc']
                          }
            ]
     ]
    }
  ]
}
