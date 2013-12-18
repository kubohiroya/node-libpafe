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
                                        '-lusb-1.0',
                                        '-lpafe',
                                        '-L<(module_root_dir)/deps/unix/libusb-1.0.9/libusb/.libs',
                                        '-L<(module_root_dir)/deps/unix/libpafe/src/.deps'
                                        ]
                                }
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
                                        '-lusb-1.0',
                                        '-lpafe',
                                        '-L<(module_root_dir)/deps/unix/libusb-1.0.9/libusb/.libs',
                                        '-L<(module_root_dir)/deps/unix/libpafe/src/.deps'
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
