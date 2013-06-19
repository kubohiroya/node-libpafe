{
  "targets": [
    {
      "target_name": "pafe"
     , "sources": [ "pafe.cc", "pasori.cc", "felica.cc"]
     , "include_dirs":["/usr/local/include/libpafe", "/usr/local/include/libusb-1.0"]

     , "link_settings":{
        "libraries":["-lusb-1.0","-lpafe"]
        }

    }
  ]
}
