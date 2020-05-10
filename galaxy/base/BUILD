cc_library(
    name = "vec-type",
    srcs = ["vec-type.cc"],
    hdrs = ["vec-type.h"],
    deps =
        [
            ":types",
            "@com_google_absl//absl/strings:str_format",
        ],
)

cc_test(
    name = "vec-type_test",
    srcs = ["vec-type_test.cc"],
    deps =
        [
            ":vec-type",
            "@com_google_googletest//:gtest_main",
        ],
)

cc_library(
    name = "macros",
    hdrs = ["macros.h"],
)

cc_library(
    name = "logging",
    srcs = ["logging.cc"],
    hdrs = ["logging.h"],
    deps =
        [
            ":macros",
            ":types",
            "@com_google_absl//absl/base",
            "@com_google_absl//absl/strings",
        ],
)

cc_test(
    name = "logging_test",
    srcs = ["logging_test.cc"],
    deps =
        [
            ":logging",
            "@com_google_googletest//:gtest_main",
        ],
)

cc_library(
    name = "memory",
    srcs = ["memory.cc"],
    hdrs = ["memory.h"],
    deps = [
        ":types",
    ],
)

cc_test(
    name = "memory_test",
    srcs = ["memory_test.cc"],
    deps =
        [
            ":memory",
            "@com_google_googletest//:gtest_main",
        ],
)

cc_library(
    name = "types",
    hdrs = ["types.h"],
)
