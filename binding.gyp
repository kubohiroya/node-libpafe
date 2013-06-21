{
  'targets': [
    {
      'target_name': 'pafe',
       'sources': [ 'pafe.cc', 'pasori.cc', 'felica.cc'],

       'conditions':[
            ['OS=="linux"', {
                          'defines':['HAVE_LIBPAFE'],
                          'include_dirs':['/usr/local/include/libpafe', '/usr/local/include/libusb-1.0'],
                          'link_settings':{ 'libraries':['-lusb-1.0','-lpafe'] }
                          }
            ],
            ['OS=="mac"', {
                          'defines':['HAVE_LIBPAFE'],
                          'include_dirs':['/usr/local/include/libpafe', '/usr/local/include/libusb-1.0'],
                          'link_settings':{ 'libraries':['-lusb-1.0','-lpafe'] }
                          }
            ],
            ['OS=="win"', {
                          'defines':['HAVE_FELICALIB'],
                          'include_dirs':['<(module_root_dir)/deps/felicalib/include'],
                          'ldflags':['-L<(module_root_dir)/deps/felicalib'],
                          'libraries':['<(module_root_dir)/deps/felicalib/lib/felicalib.lib']
                          }
            ]
     ]
    }
  ]
}
