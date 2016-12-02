{
  "targets": [
    {
      "target_name": "node_cctz",
      "sources": [
        "cctz/src/time_tool.cc",
        "cctz/src/time_zone_format.cc",
        "cctz/src/time_zone_if.cc",
        "cctz/src/time_zone_impl.cc",
        "cctz/src/time_zone_info.cc",
        "cctz/src/time_zone_libc.cc",
        "cctz/src/time_zone_lookup.cc",
        "cctz/src/time_zone_posix.cc",
        "src/civiltime.cc",
        "src/timezone.cc",
        "src/node_cctz.cc"
      ],
      "cflags" : [ "-std=c++11" ],
      "include_dirs" : [
        "src",
        "cctz/include",
        "<!(node -e \"require('nan')\")"
      ],
      "conditions": [
        [
          "OS==\"mac\"",
          {
            "xcode_settings": {
              "OTHER_CPLUSPLUSFLAGS": [
                "-std=c++11",
                "-stdlib=libc++"
              ],
              "OTHER_LDFLAGS": [
                "-stdlib=libc++"
              ],
              "MACOSX_DEPLOYMENT_TARGET":"10.7"
            }
          }
        ]
      ]
    }
  ]
}
