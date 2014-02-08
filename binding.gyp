{
  'targets': [
    {
      'target_name': 'pafe',

       'conditions':[
            ['OS=="linux"', {
                          'defines':['HAVE_LIBPAFE'],
                          'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc'],
                          'include_dirs':[
                                       '<(module_root_dir)/deps/unix/libusb-1.0.18/libusb',
                                       '<(module_root_dir)/deps/unix/libusb-compat-0.1.5/libusb',
                                       '<(module_root_dir)/deps/unix/libnfc-1.7.0/include'
                                       ],
                          'link_settings':{
                                'libraries':[
                                        '<(module_root_dir)/deps/unix/libusb-1.0.18/libusb/.libs/libusb-1.0.a',
                                        '<(module_root_dir)/deps/unix/libusb-compat-0.1.5/libusb/.libs/libusb.a',
                                        '<(module_root_dir)/deps/unix/libnfc-1.7.0/libnfc/.libs/libnfc.a'
                                        ]
                                }
                          }
            ],
            ['OS=="mac"', {
                          'defines':['HAVE_LIBNFC'],
                          'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc'],
                          'include_dirs':[
                                        '<(module_root_dir)/deps/unix/libusb-1.0.18/libusb',
                                       '<(module_root_dir)/deps/unix/libusb-compat-0.1.5/libusb',
                                        '<(module_root_dir)/deps/unix/libnfc-1.7.0/include'
                                        ],
                          'link_settings':{
                                'libraries':[
                                        '<(module_root_dir)/deps/unix/libusb-1.0.18/libusb/.libs/libusb-1.0.a',
                                        '<(module_root_dir)/deps/unix/libusb-compat-0.1.5/libusb/.libs/libusb.a',
                                        '<(module_root_dir)/deps/unix/libnfc-1.7.0/libnfc/.libs/libnfc.a'
                                        ]
                                }

                          }
            ],
            ['OS=="win"', {
                          'defines':['HAVE_FELICALIB'],
                          'sources': [
                                       'pafe.cc', 'pasori.cc', 'felica.cc',
                                       'deps/win/felicalib/felicalib.c',
                                       'deps/win/pasori_open_multi.c'
                                       ],
                          'include_dirs':[
                                        '<(module_root_dir)/deps/win/felicalib',
                                        '<(module_root_dir)/deps/win/include'
                                        ],
                          'cflags': ['/EHsc']
                          }
            ]
     ]
    }
  ]
}
