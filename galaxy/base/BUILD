cc_library(name = "vec-type", hdrs = ["vec-type.h"], srcs = ["vec-type.cc"],
           deps =
               [
                 ":types",
                 "@com_google_absl//absl/strings:str_format",
               ])

    cc_test(name = "vec-type_test", srcs = ["vec-type_test.cc"],
            deps =
                [
                  ":vec-type",
                  "@com_google_googletest//:gtest_main",
                ])

        cc_library(name = "macros", hdrs = ["macros.h"], )

            cc_library(name = "logging", hdrs = ["logging.h"],
                       srcs = ["logging.cc"],
                       deps =
                           [
                             ":macros",
                             ":types",
                             "@com_google_absl//absl/base",
                             "@com_google_absl//absl/strings:strings",
                           ])

                cc_test(name = "logging_test", srcs = ["logging_test.cc"],
                        deps =
                            [
                              ":logging",
                              "@com_google_googletest//:gtest_main",
                            ]) cc_library(name = "memory", hdrs = ["memory.h"],
                                          srcs = ["memory.cc"], )

                    cc_library(name = "types", hdrs = ["types.h"], )