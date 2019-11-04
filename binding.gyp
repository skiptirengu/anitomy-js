{
    "targets": [
        {
            "target_name": "anitomy-js",
            "sources": [
                "lib/anitomy/anitomy/anitomy.cpp",
                "lib/anitomy/anitomy/anitomy.h",
                "lib/anitomy/anitomy/element.cpp",
                "lib/anitomy/anitomy/element.h",
                "lib/anitomy/anitomy/keyword.cpp",
                "lib/anitomy/anitomy/keyword.h",
                "lib/anitomy/anitomy/options.h",
                "lib/anitomy/anitomy/parser.cpp",
                "lib/anitomy/anitomy/parser.h",
                "lib/anitomy/anitomy/parser_helper.cpp",
                "lib/anitomy/anitomy/parser_number.cpp",
                "lib/anitomy/anitomy/string.cpp",
                "lib/anitomy/anitomy/string.h",
                "lib/anitomy/anitomy/token.cpp",
                "lib/anitomy/anitomy/token.h",
                "lib/anitomy/anitomy/tokenizer.cpp",
                "lib/anitomy/anitomy/tokenizer.h",
                "src/addon.cc",
                "src/file_parser.hpp",
                "src/file_parser.cc",
                "src/parser_worker.hpp",
                "src/parser_worker.cc",
                "src/util.hpp",
                "src/util.cc",
                "src/validate.hpp",
                "src/validate.cc"
            ],
            "xcode_settings": {
                "CLANG_CXX_LANGUAGE_STANDARD": "c++14",
                "CLANG_CXX_LIBRARY": "libc++",
                "MACOSX_DEPLOYMENT_TARGET": "10.9",
                "GCC_ENABLE_CPP_EXCEPTIONS": "NO",
                "GCC_ENABLE_CPP_RTTI": "NO",
                "OTHER_CPLUSPLUSFLAGS": [
                    "-Wall",
                    "-O3"
                ]
            },
            "cflags_cc": [
                "-fno-exceptions",
                "-fno-rtti",
                "-std=c++14",
                "-Wall",
                "-O3"
            ],
            "cflags_cc!": [
                "-std=gnu++0x",
                "-std=gnu++1y"
            ],
            "include_dirs": [
                "<!(node -e \"require('nan')\")",
                "lib/anitomy"
            ]
        }
    ]
}
