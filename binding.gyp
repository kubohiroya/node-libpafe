{
  'targets': [
    {
      'target_name': 'pafe',

       'conditions':[
            ['OS=="linux"', {
                          'defines':['HAVE_LIBPAFE'],
                          'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc'],
                          'include_dirs':[
                                       '<(module_root_dir)/deps/unix/libusb-1.0.9/libusb',
                                       '<(module_root_dir)/deps/unix/libpafe/src'
                                       ],
                          'link_settings':{
                                'libraries':[
                                        '<(module_root_dir)/deps/unix/libusb-1.0.9/libusb/.libs/libusb-1.0.a',
                                        '<(module_root_dir)/deps/unix/libpafe/src/.libs/libpafe.a'
                                        ]
                                },
                          'ldflags': [ 
                                        '-lusb-1.0',
                                        '-lpafe',
                                        '-L<(module_root_dir)/deps/unix/libusb-1.0.9/libusb/.libs',
                                        '-L<(module_root_dir)/deps/unix/libpafe/src/.deps'
                                ]
                          }
            ],
            ['OS=="mac"', {
                          'defines':['HAVE_LIBPAFE'],
                          'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc'],
                          'include_dirs':[
                                        '<(module_root_dir)/deps/unix/libusb-1.0.9/libusb',
                                        '<(module_root_dir)/deps/unix/libpafe/src'
                                        ],
                          'link_settings':{
                                'libraries':[
                                        '<(module_root_dir)/deps/unix/libusb-1.0.9/libusb/.libs/libusb-1.0.a',
                                        '<(module_root_dir)/deps/unix/libpafe/src/.libs/libpafe.a'
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
