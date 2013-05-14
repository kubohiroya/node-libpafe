{
  "targets": [
    {
      "target_name": "pafe"
     , "sources": [ "pafe.cc"]
     , "include_dirs":["/usr/local/include/libpafe"]

     , "link_settings":{
        "libraries":["-lusb-1.0","-lpafe"]
        }

    }
  ]
}
