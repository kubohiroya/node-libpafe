{
  'targets': [
    {
      'target_name': 'pafe',

       'conditions':[
            ['OS=="linux"', {
                          'defines':['HAVE_LIBPAFE'],
                          'sources': [ 'pafe_addon.cc', 'pafe.cc'],
                          'include_dirs':["<!(node -e \"require('nan')\")", "/usr/local/include/libpafe"],
                          'link_settings':{
                                'libraries':[
 				        "<!(libusb-config --libs)", '-L/usr/local/lib -lpafe'
                                        ]
                                        }
                          }
            ],
            ['OS=="mac"', {
                          'defines':['HAVE_LIBPAFE'],
                          'sources': [ 'pafe_addon.cc', 'pafe.cc'],
			  'include_dirs':["<!(node -e \"require('nan')\")", "/usr/local/include/libpafe"],
                          'link_settings':{
                                'libraries':[
 				        "<!(libusb-config --libs)", '-L/usr/local/lib -lpafe'
                                        ]
                                        }
                          }
            ],
            ['OS=="win"', {
                          'defines':['HAVE_FELICALIB'],
			  'sources': [ 'pafe_addon.cc', 'pafe.cc',
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
