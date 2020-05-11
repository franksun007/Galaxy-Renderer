package(default_visibility = ["//visibility:public"])

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "sphere-drawer",
    srcs = ["sphere-drawer.cc"],
    hdrs = ["sphere-drawer.h"],
    deps = [
        "//galaxy/base:vec-type",
        "//galaxy/base:logging",
        "//galaxy/stars/base:sphere",
    ],
    linkopts = [
        "-lglut",
        "-lGL",
    ],
)

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "sphere-drawer-manual-test",
    srcs = ["sphere-drawer-manual-test.cc"],
    copts = [],
    deps = [
        ":sphere-drawer",
        "//galaxy/ui:sdl-window",
        "//galaxy/base:types",
        "//galaxy/base:vec-type",
        "//galaxy/stars/base:sphere",
    ],
)