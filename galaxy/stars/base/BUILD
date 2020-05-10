package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sphere",
    srcs = ["sphere.cc"],
    hdrs = ["sphere.h"],
    deps = [
        "//galaxy/base:constants",
        "//galaxy/base:types",
        "//galaxy/base:vec-type",
    ]
)

cc_test(
    name = "sphere_test",
    srcs = ["sphere_test.cc"],
    deps = [
        ":sphere",
        "//galaxy/base:vec-type",
        "@com_google_googletest//:gtest_main",
    ],
)